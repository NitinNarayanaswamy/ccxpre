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
#include <fstream>
#include <algorithm>
#include <vector>

#include <log.hpp>

#define VERSION 0.1
#define USAGE_FMT "[-h] [-fm] [-b b_arg | -i i_arg [-e e_arg -r r_arg]]"

namespace ccxpre::utilities {
    void print_help() {
        // limit each line to 82 chars
        PRINT("ccxpre " << VERSION << ", Copyright (C) 2020 Nitin Narayanaswamy");
        PRINT("ccxpre " << VERSION << " comes with ABSOLUTELY NO WARRANTY.");
        PRINT("This is free software, and you are welcome to redistribute it");
        PRINT("under certain conditions.");
        PRINT_NEW_LINE;
        PRINT("Usage: ccxpre " << USAGE_FMT);
        PRINT_NEW_LINE;
        PRINT("Common usage:");
        PRINT("    ccxpre -b newfile.inp");
        PRINT_NEW_LINE;
        PRINT("    ccxpre -i meshfile.inp -e SUR=CAX4");
        // PRINT("    ccxpre yourfile.inp    If the input file is a Abaqus mesh file ge");
        // PRINT("                                        nerated using Gmsh. Clean mesh file <your_file_name>_cmesh.inp and Calculix input file <your_file_name>.inp are created.");
        // PRINT("    ccxpre <your_file_name>_mesh.inp P CAX4    Only surface element sets are included in the clean mesh file and CAX4 is set as element type.");
        PRINT_NEW_LINE;
        PRINT("Options:");
        PRINT("    -h    Prints help.");
        PRINT_NEW_LINE;
        PRINT("    -f    Force overwrite for all files.");
        PRINT_NEW_LINE;
        PRINT("    -m    Write only clean mesh file.");
        PRINT_NEW_LINE;
        PRINT("    -b    Writes a CalculiX input file with boilerplate code. An input file is ");
        PRINT("          created which contains some common CalculiX commands to get started.");
        PRINT_NEW_LINE;
        PRINT("    -i    Writes a clean mesh file and a Calculix input file. Requires a mesh file");
        PRINT("          as an argument.");
        PRINT_NEW_LINE;
        PRINT("    -e    Writes only selected element groups. Requires element set code and type ");
        PRINT("          pairs as an argument. By default all element groups are written.");
        PRINT("          For example -e SUR=CAX4 options will replace element type to CAX4 for ");
        PRINT("          all Surface definitions in Element header. LIN for Line, SUR for Surface");
        PRINT("          are currently supported. For multiple input use comma as a separator.");
        PRINT_NEW_LINE;
        PRINT("    -r    Recalculates element sets of edges. Requires edge node group name.");
        PRINT("          For example -r RCES=Edge1Nodes, use comma to separate multiple groups ");
        PRINT("          like, -r RCES=Edge1Nodes,EDGE2NODES.");
    }

    bool is_file(const std::string file) {
        std::ifstream this_file(file);
        if(this_file.is_open()) {return true;}
        this_file.close();
        return false;
    }

    void str_to_upper(std::string &str) {
        std::transform(str.begin(), str.end(), str.begin(), ::toupper);
    }

    bool is_str_equal(std::string str1, const std::string str2) {
        str_to_upper(str1);
        if(str1 == str2) {return true;}
        else {return false;}
        return false;
    }

    unsigned int get_first_number_in_csv_str(const std::string str) {
        std::string number_str;
        for(const char str_char : str) {
            if(str_char == ',') {
                return std::stoi(number_str);
            }
            else {number_str.push_back(str_char);}
        }
        return 0;
    }

    std::vector<unsigned int> csv_str_to_numbers(const std::string str) {
        std::vector<unsigned int> numbers;
        unsigned int number = 0;
        std::string number_str;
        for(const char str_char : str) {
            if(str_char != ' ') {
                if(str_char == ',') {
                    numbers.emplace_back(std::stoi(number_str));
                    number_str.clear();
                    continue;
                }
                else {number_str.push_back(str_char);}
            }
        }
        if(number_str != "") {numbers.emplace_back(std::stoi(number_str));}
        return numbers;
    }

    std::string get_key_value_pair(const std::string str, const std::string keyword, const char key_value_delimiter, const char delimiter) {
        std::string key;
        std::string value;
        bool is_value = false;
        for(const char str_char : str) {
            if(str_char != ' ') {
                if(str_char == ',') {
                    if(key == keyword) {return value;}
                    is_value = false;
                    key.clear();
                    value.clear();
                    continue;
                }
                else if(str_char == key_value_delimiter) {is_value = true; continue;}
                else if(is_value == false) {key.push_back(str_char); continue;}
                else {value.push_back(str_char);}
            }
        }
        if(key == keyword) {return value;}
        return "";
    }

    std::string find_elements_of_nodes(const std::string input_file, std::vector<unsigned int> &nodes,
                                            const std::string element_header, const char key_value_delimiter,
                                            const char delimiter) {
        bool is_element = false;
        unsigned int element_count = 0;
        std::string current_line;
        std::string new_elements;
        std::ifstream mesh_file(input_file, std::ios::in);
        while(std::getline(mesh_file, current_line)) {
            if(current_line.at(0) == '*' && current_line.at(1) != '*') {
                is_element = false;
                if(current_line.substr(0, 8) == element_header) {
                    is_element = true;
                }
            }
            else if(is_element) {
                unsigned int node_match_count = 0;
                std::vector<unsigned int> nodes_in_line = csv_str_to_numbers(current_line);
                for(unsigned int node : nodes) {
                    for(auto node_in_line = nodes_in_line.begin()+1; node_in_line < nodes_in_line.end(); node_in_line++) {
                        if(*node_in_line == node) {
                            node_match_count++;
                            break;
                        }
                    }
                }
                if(node_match_count == 2) {
                    if(element_count == 10) {new_elements.push_back('\n'); element_count = 0;}
                    std::string element = std::to_string(get_first_number_in_csv_str(current_line))+", ";
                    new_elements.append(element);
                    element_count++;
                }
                else if(node_match_count > 2) {PRINT_WARNING("More than two nodes found, see line " << current_line);}
            }
        }
        return new_elements;
    }

    void write_ccxpre_temp_to_cmesh(const std::string temp_file, const std::string mesh_file) {
        std::string current_line;
        std::ofstream mesh_file_stream(mesh_file, std::ios::app);
        std::ifstream temp_file_stream(temp_file);
        while(std::getline(temp_file_stream, current_line)) {
            mesh_file_stream << current_line << '\n';
        }
        temp_file_stream.close();
        mesh_file_stream.close();
    }
}
