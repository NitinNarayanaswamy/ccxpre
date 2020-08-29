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

#define PRINT(message) std::cout << message << std::endl
#define PRINT_NEW_LINE std::cout << std::endl

namespace utilities {
    void print_help() {
        // limit each line to 82 chars
        PRINT("ccxpre, Copyright (C) 2020 Nitin Narayanaswamy");
        PRINT("ccxpre comes with ABSOLUTELY NO WARRANTY.");
        PRINT("This is free software, and you are welcome to redistribute it");
        PRINT("under certain conditions.");
        PRINT_NEW_LINE;
        PRINT("Usage syntax: ccxpre [options] <your_file_name> [features]");
        PRINT_NEW_LINE;
        PRINT("Common usage:");
        PRINT("    ccxpre <your_file_name>_mesh.inp    If the input file is a Abaqus mesh file generated using Gmsh. Clean mesh file <your_file_name>_cmesh.inp and Calculix input file <your_file_name>.inp are created.");
        PRINT("    ccxpre <your_file_name>_mesh.inp P CAX4    Only surface element sets are included in the clean mesh file and CAX4 is set as element type.");
        PRINT_NEW_LINE;
        PRINT("options");
        PRINT("    -b    Used to create a CalculiX project with boilerplate code. An input file <your_file_name>.inp is created which contains some common CalculiX commands to get started.");
        PRINT("    -f    Force overwrite of clean mesh and Calculix input files.");
        PRINT("    -i    Write only Calculix input file.");
        PRINT("    -m    Write only clean mesh file.");
    }

    bool is_file(const std::string file_name) {
        std::ifstream file(file_name);
        if(file.is_open()) {return true;}
        file.close();
        return false;
    }

    bool element_type_code_check(const std::string element_set_include, const char code) {
        if(element_set_include == "A") {return true;}
        for(char element_set_code : element_set_include) {
            if(element_set_code == code) {return true;}
        }
        return false;
    }
}