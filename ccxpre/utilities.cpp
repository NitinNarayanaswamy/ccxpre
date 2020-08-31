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
#define PRINT_NEW_LINE std::cout << std::endl

#define VERSION 0.1
#define USAGE_FMT "[-h] [-fm] [-b b_arg | -i i_arg [-e e_arg]]"

namespace utilities {
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
                    continue;}
                else if(str_char == key_value_delimiter) {is_value = true; continue;}
                else if(is_value == false) {key.push_back(str_char); continue;}
                else {value.push_back(str_char);}
            }
        }
        if(key == keyword) {return value;}
        return "";
    }
}