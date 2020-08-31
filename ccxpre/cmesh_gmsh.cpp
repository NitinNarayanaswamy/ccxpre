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
#define DEBUG false
#if DEBUG
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
#define KEY_VALUE_DELIMITER '='
#define DELIMITER ','
#define ELEMENT_SET_KEY "ELSET"
#define LINE_ELEMENT_SET "LIN"
#define SURFACE_ELEMENT_SET "SUR"
#define SOLID_ELEMENT_SET "SOL"
#define NEW_ELEMENT_LINE(type, elset) "*ELEMENT,TYPE="+type+",ELSET="+elset

namespace cmesh_gmsh {
    std::string is_element_header_broken(const std::string element_line, const std::string element_config) {
        std::string elset_value = utilities::get_key_value_pair(element_line, ELEMENT_SET_KEY, KEY_VALUE_DELIMITER, DELIMITER);
        if(elset_value == "") {PRINT_ERROR(ELEMENT_SET_KEY << " not found"); return "";}
        else {PRINT_DEBUG("*" << elset_value << "*");}
        if(elset_value.substr(0, 4) == "LINE") {
            std::string element_type = utilities::get_key_value_pair(element_config, LINE_ELEMENT_SET, KEY_VALUE_DELIMITER, DELIMITER);
            PRINT_DEBUG("*" << element_type << "*");
            if(element_type == "") {return "";}
            else {return NEW_ELEMENT_LINE(element_type, elset_value);}
        }
        else if(elset_value.substr(0, 7) == "SURFACE") {
            std::string element_type = utilities::get_key_value_pair(element_config, SURFACE_ELEMENT_SET, KEY_VALUE_DELIMITER, DELIMITER);
            PRINT_DEBUG("*" << element_type << "*");
            if(element_type == "") {return "";}
            else {return NEW_ELEMENT_LINE(element_type, elset_value);}
        }
        else {PRINT_WARNING(elset_value << " unknown or not supported");}
        return "";
    }

    void write(const std::string input_file, const std::string element_config, const bool overwrite_flag) {
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
                    else {clean_mesh_file << current_line << "\n"; continue;}
                }
                else if(skip_line_flag != true) {clean_mesh_file << current_line << "\n"; continue;}
                else {continue;}
            }
            else {skip_line_count--;}
        }
        mesh_file.close();
        clean_mesh_file.close();
        PRINT_INFO(INPUT_FILE_NAME+"_cmesh.inp" << " created");
    }
}