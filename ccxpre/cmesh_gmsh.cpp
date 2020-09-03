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
#include <boost/algorithm/string.hpp>

#include <cmesh_gmsh.hpp>
#include <utilities.hpp>

#define PRINT(message) std::cout << message << std::endl
#ifndef NDEBUG
    #define PRINT_DEBUG(message) std::cout << "DEBUG    " << message << std::endl
#else
    #define PRINT_DEBUG(message) 
#endif
#define PRINT_INFO(message) std::cout << "INFO    " << message << std::endl
#define PRINT_WARNING(message) std::cout << "WARNING    " << message << std::endl
#define PRINT_ERROR(message) std::cout << "ERROR    " << message << std::endl
#define PRINT_NEW_LINE std::cout << std::end

#define INPUT_FILE_EXT input_file.substr(input_file.find_last_of('.')+1)
#define INPUT_FILE_NAME input_file.substr(0, input_file.find_last_of('.'))

#define CCXPRE_KEY_VALUE_DELIMITER '='
#define CCXPRE_DELIMITER ','
#define CCXPRE_LINE_ELEMENT_SET "LIN"
#define CCXPRE_SURFACE_ELEMENT_SET "SUR"
#define CCXPRE_SOLID_ELEMENT_SET "SOL"
#define CCXPRE_RCAL_ELEMENT_SET "RCES"

#define ELEMENT_SET_KEY "ELSET"
#define NEW_ELEMENT_LINE(type, elset) "*ELEMENT,TYPE="+type+",ELSET="+elset

namespace cmesh_gmsh {
    void write(const std::string input_file, const std::string element_config,
               const std::string recalculate_input, const bool overwrite_flag) {
        if(utilities::is_file(input_file) == false) {
            PRINT_ERROR(input_file << " file does not exists");
            return;
        }
        if(utilities::is_file(INPUT_FILE_NAME+"_cmesh.inp") == true && overwrite_flag == false) {
            PRINT_ERROR(INPUT_FILE_NAME+"_cmesh.inp" << " file exists, to force overwrite use -f option");
            return;
        }

        std::ifstream mesh_file(input_file);
        std::ofstream clean_mesh_file(INPUT_FILE_NAME+"_cmesh.inp");
        std::string current_line;
        unsigned int skip_line_count = 0;
        bool skip_line_flag = false;
        while(std::getline(mesh_file, current_line)) {
            if(skip_line_count == 0) {
                if(current_line.at(0) == '*' && current_line.at(1) != '*') {
                    skip_line_flag = false;
                    utilities::str_to_upper(current_line);
                    if(current_line == "*HEADING") {
                        skip_line_count = 1;
                        continue;
                    }
                    else if(current_line == "*NODE") {
                        clean_mesh_file << "*NODE,NSET=Nall\n";
                        continue;
                    }
                    else if(current_line.substr(0, 8) == "*ELEMENT" && element_config != "All") {
                        std::string new_element_line = is_element_header_broken(current_line.substr(8), element_config);
                        if(new_element_line != "") {
                            PRINT_INFO("Writing elements under header " << current_line << " and replacing the header with " << new_element_line);
                            clean_mesh_file << new_element_line << "\n";
                            continue;
                        }
                        else {
                            PRINT_INFO("Skipping elements under header " << current_line);
                            skip_line_flag = true;
                            continue;
                        }
                    }
                    else if(current_line.substr(0, 6) == "*ELSET" && recalculate_input != "None") {
                        bool broken_element_set = is_element_set_broken(current_line.substr(6), recalculate_input);
                        if(broken_element_set == false) {
                            PRINT_INFO("Writing elements under set " << current_line);
                            clean_mesh_file << current_line << "\n";
                            continue;
                        }
                        else {
                            PRINT_INFO("Skipping elements under set " << current_line);
                            skip_line_flag = true;
                            continue;
                        }
                    }
                    else {clean_mesh_file << current_line << "\n"; continue;}
                }
                else if(skip_line_flag != true) {clean_mesh_file << current_line << "\n"; continue;}
                else {continue;}
            }
            else {skip_line_count--;}
        }
        // add recal broken elset
        mesh_file.close();
        clean_mesh_file.close();
        PRINT_INFO(INPUT_FILE_NAME+"_cmesh.inp" << " created");
    }

    std::string is_element_header_broken(const std::string element_line, const std::string element_config) {
        std::string elset_in_line = utilities::get_key_value_pair(element_line, ELEMENT_SET_KEY, CCXPRE_KEY_VALUE_DELIMITER, CCXPRE_DELIMITER);
        if(elset_in_line == "") {PRINT_ERROR(ELEMENT_SET_KEY << " not found"); return "";}
        if(elset_in_line.substr(0, 4) == "LINE") {
            std::string element_type = utilities::get_key_value_pair(element_config, CCXPRE_LINE_ELEMENT_SET, CCXPRE_KEY_VALUE_DELIMITER, CCXPRE_DELIMITER);
            PRINT_DEBUG(element_type);
            if(element_type == "") {return "";}
            else {return NEW_ELEMENT_LINE(element_type, elset_in_line);}
        }
        else if(elset_in_line.substr(0, 7) == "SURFACE") {
            std::string element_type = utilities::get_key_value_pair(element_config, CCXPRE_SURFACE_ELEMENT_SET, CCXPRE_KEY_VALUE_DELIMITER, CCXPRE_DELIMITER);
            PRINT_DEBUG(element_type);
            if(element_type == "") {return "";}
            else {return NEW_ELEMENT_LINE(element_type, elset_in_line);}
        }
        else {PRINT_WARNING(elset_in_line << " unknown or not supported");}
        return "";
    }

    bool is_element_set_broken(const std::string element_set_line, const std::string recalculate_input) {
        std::string elset_in_line = utilities::get_key_value_pair(element_set_line, ELEMENT_SET_KEY, CCXPRE_KEY_VALUE_DELIMITER, CCXPRE_DELIMITER);
        PRINT_DEBUG(elset_in_line);
        if(elset_in_line == "") {PRINT_ERROR(ELEMENT_SET_KEY << " not found"); return "";}
        std::string elset_in_input = utilities::get_key_value_pair(recalculate_input, CCXPRE_RCAL_ELEMENT_SET, CCXPRE_KEY_VALUE_DELIMITER, CCXPRE_DELIMITER);
        PRINT_DEBUG(elset_in_input);
        if(elset_in_line == elset_in_input) {return true;}
        return false;
    }
}