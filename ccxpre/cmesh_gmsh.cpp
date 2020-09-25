// ccxpre helps configure Calculix finite element projects
// Copyright (C) 2020  Nitin Narayanaswamy
// 
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License along
// with this program; if not, write to the Free Software Foundation, Inc.,
// 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA

#include <iostream>
#include <string>
#include <fstream>
#include <vector>

#include <ccxpre.hpp>
#include <log.hpp>

#define INPUT_FILE_EXT input_file.substr(input_file.find_last_of('.')+1)
#define INPUT_FILE_NAME input_file.substr(0, input_file.find_last_of('.'))
#define CLEAN_MESH_FILE INPUT_FILE_NAME+"_cmesh.inp"

#define CCXPRE_KEY_VALUE_DELIMITER '='
#define CCXPRE_DELIMITER ','
#define CCXPRE_LINE_ELEMENT_SET "LIN"
#define CCXPRE_SURFACE_ELEMENT_SET "SUR"
#define CCXPRE_SOLID_ELEMENT_SET "SOL"
#define CCXPRE_RCAL_ELEMENT_SET "RCES"
#define CCXPRE_TEMP_DATA_FILE "ccxpre_temp.dat"

#define HEADING_HEADER "*HEADING"
#define NODE_HEADER "*NODE"
#define ELEMENT_HEADER "*ELEMENT"
#define NODE_SET_HEADER "*NSET"
#define ELEMENT_SET_HEADER "*ELSET"
#define NODE_SET_KEY "NSET"
#define ELEMENT_SET_KEY "ELSET"
#define NEW_ELEMENT_LINE(type, elset) "*ELEMENT,TYPE="+type+",ELSET="+elset
#define NEW_ELEMENT_SET_LINE(elset) "*ELSET,ELSET="+elset

namespace ccxpre::cmesh_gmsh {
    void write(const std::string input_file, const std::string element_config,
               const std::string recalculate_input, const bool overwrite_flag) {
        if(ccxpre::utilities::is_file(input_file) == false) {
            PRINT_ERROR(input_file << " file does not exists");
            return;
        }
        if(ccxpre::utilities::is_file(CLEAN_MESH_FILE) == true && overwrite_flag == false) {
            PRINT_ERROR(CLEAN_MESH_FILE << " file exists, to force overwrite use -f option");
            return;
        }

        std::ifstream mesh_file(input_file);
        std::ofstream clean_mesh_file(CLEAN_MESH_FILE);
        std::string current_line;
        unsigned int skip_line_count = 0;
        bool skip_line_flag = false;
        while(std::getline(mesh_file, current_line)) {
            if(skip_line_count == 0) {
                if(current_line.at(0) == '*' && current_line.at(1) != '*') {
                    skip_line_flag = false;
                    if(ccxpre::utilities::is_str_equal(current_line.substr(0, 8), HEADING_HEADER)) {
                        skip_line_count = 1;
                        continue;
                    }
                    else if(ccxpre::utilities::is_str_equal(current_line.substr(0, 5), NODE_HEADER)) {
                        clean_mesh_file << "*NODE,NSET=Nall\n";
                        continue;
                    }
                    else if(ccxpre::utilities::is_str_equal(current_line.substr(0, 8), ELEMENT_HEADER) && element_config != "All") {
                        std::string new_element_line = is_element_header_broken(current_line.substr(8), element_config);
                        if(new_element_line != "") {
                            PRINT_INFO("Writing elements under header " << current_line);
                            PRINT_INFO("and replacing the header with " << new_element_line);
                            clean_mesh_file << new_element_line << '\n';
                            continue;
                        }
                        else {
                            PRINT_INFO("Skipping elements under header " << current_line);
                            skip_line_flag = true;
                            continue;
                        }
                    }
                    else if(ccxpre::utilities::is_str_equal(current_line.substr(0, 6), ELEMENT_SET_HEADER) && recalculate_input != "None") {
                        if(!is_set_broken(current_line.substr(6), recalculate_input, false)) {
                            PRINT_INFO("Writing elements under header " << current_line << " without changes");
                            clean_mesh_file << current_line << '\n';
                            continue;
                        }
                        else {
                            PRINT_INFO("Elements under header " << current_line << " are marked for recalculation");
                            skip_line_flag = true;
                            continue;
                        }
                    }
                    else {clean_mesh_file << current_line << '\n'; continue;}
                }
                else if(!skip_line_flag) {clean_mesh_file << current_line << '\n'; continue;}
                else {continue;}
            }
            else {skip_line_count--;}
        }
        clean_mesh_file.close();

        if(recalculate_input != "None") {
            mesh_file.clear();
            mesh_file.seekg(0);
            bool is_nodes = false;
            std::vector<unsigned int> nodes;
            std::ofstream ccxpre_temp_file(CCXPRE_TEMP_DATA_FILE);
            ccxpre_temp_file << "**ccxpre recalculation {\n";
            while(std::getline(mesh_file, current_line)) {
                if(current_line.at(0) == '*' && current_line.at(1) != '*') {
                    if(is_nodes) {
                        std::string new_elements = ccxpre::utilities::find_elements_of_nodes(CLEAN_MESH_FILE, nodes, ELEMENT_HEADER, CCXPRE_KEY_VALUE_DELIMITER, CCXPRE_DELIMITER);
                        ccxpre_temp_file << new_elements << '\n';
                        nodes.clear();
                    }
                    is_nodes = false;
                    if(ccxpre::utilities::is_str_equal(current_line.substr(0, 5), NODE_SET_HEADER)) {
                        if(is_set_broken(current_line.substr(5), recalculate_input, true)) {
                            PRINT_INFO("Recalculating and writing elements using node set " << current_line);
                            std::string new_set = ccxpre::utilities::get_key_value_pair(current_line, NODE_SET_KEY, CCXPRE_KEY_VALUE_DELIMITER, CCXPRE_DELIMITER);
                            ccxpre_temp_file << NEW_ELEMENT_SET_LINE(new_set) << '\n';
                            is_nodes = true;
                            continue;
                        }
                    }
                }
                else if(is_nodes) {
                    std::vector<unsigned int> temp_numbers =  ccxpre::utilities::csv_str_to_numbers(current_line);
                    nodes.insert(nodes.end(), temp_numbers.begin(), temp_numbers.end());
                }
            }
            if(is_nodes) {
                std::string new_elements = ccxpre::utilities::find_elements_of_nodes(CLEAN_MESH_FILE, nodes, ELEMENT_HEADER, CCXPRE_KEY_VALUE_DELIMITER, CCXPRE_DELIMITER);
                ccxpre_temp_file << new_elements << '\n';
                nodes.clear();
                is_nodes = false;
            }
            ccxpre_temp_file << "**}\n";
            ccxpre_temp_file.close();
            ccxpre::utilities::write_ccxpre_temp_to_cmesh(CCXPRE_TEMP_DATA_FILE, CLEAN_MESH_FILE);
            if(std::remove(CCXPRE_TEMP_DATA_FILE) != 0) {PRINT_ERROR("Failed to delete ccxpre temporary file");}
        }
        mesh_file.close();
        PRINT_INFO(CLEAN_MESH_FILE << " created");
    }

    std::string is_element_header_broken(const std::string element_line, const std::string element_config) {
        std::string elset_in_line = ccxpre::utilities::get_key_value_pair(element_line, ELEMENT_SET_KEY, CCXPRE_KEY_VALUE_DELIMITER, CCXPRE_DELIMITER);
        if(elset_in_line == "") {PRINT_ERROR(ELEMENT_SET_KEY << " not found"); return "";}
        if(ccxpre::utilities::is_str_equal(elset_in_line.substr(0, 4), "LINE")) {
            std::string element_type = ccxpre::utilities::get_key_value_pair(element_config, CCXPRE_LINE_ELEMENT_SET, CCXPRE_KEY_VALUE_DELIMITER, CCXPRE_DELIMITER);
            if(element_type == "") {return "";}
            else {return NEW_ELEMENT_LINE(element_type, elset_in_line);}
        }
        else if(ccxpre::utilities::is_str_equal(elset_in_line.substr(0, 7), "SURFACE")) {
            std::string element_type = ccxpre::utilities::get_key_value_pair(element_config, CCXPRE_SURFACE_ELEMENT_SET, CCXPRE_KEY_VALUE_DELIMITER, CCXPRE_DELIMITER);
            if(element_type == "") {return "";}
            else {return NEW_ELEMENT_LINE(element_type, elset_in_line);}
        }
        else {PRINT_WARNING(elset_in_line << " unknown or not supported");}
        return "";
    }

    bool is_set_broken(const std::string set_line, const std::string recalculate_input, const bool is_node_set) {
        std::string set_in_line;
        if(is_node_set) {set_in_line = ccxpre::utilities::get_key_value_pair(set_line, NODE_SET_KEY, CCXPRE_KEY_VALUE_DELIMITER, CCXPRE_DELIMITER);}
        else {set_in_line = ccxpre::utilities::get_key_value_pair(set_line, ELEMENT_SET_KEY, CCXPRE_KEY_VALUE_DELIMITER, CCXPRE_DELIMITER);}
        PRINT_DEBUG("Set in line "+set_in_line);
        if(is_node_set && set_in_line == "") {PRINT_ERROR(NODE_SET_KEY << " not defined"); return false;}
        else if(!is_node_set && set_in_line == "") {PRINT_ERROR(ELEMENT_SET_KEY << " not defined"); return false;}
        std::string temp_input = recalculate_input.substr(5);
        std::string temp_set;
        for(const char str_char : temp_input) {
            if(str_char == ',') {
                std::string set_in_input = ccxpre::utilities::get_key_value_pair("RCES="+temp_set, CCXPRE_RCAL_ELEMENT_SET, CCXPRE_KEY_VALUE_DELIMITER, CCXPRE_DELIMITER);
                PRINT_DEBUG("Set in input "+set_in_input);
                if(set_in_line == set_in_input) {return true;}
                temp_set.clear();
            }
            else {temp_set.push_back(str_char);}
        }
        std::string set_in_input = ccxpre::utilities::get_key_value_pair("RCES="+temp_set, CCXPRE_RCAL_ELEMENT_SET, CCXPRE_KEY_VALUE_DELIMITER, CCXPRE_DELIMITER);
        PRINT_DEBUG("Set in input "+set_in_input);
        if(set_in_line == set_in_input) {return true;}
        return false;
    }
}