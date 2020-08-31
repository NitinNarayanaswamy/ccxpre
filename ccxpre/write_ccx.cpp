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
// 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

#include <iostream>
#include <string>
#include <fstream>
#include <chrono>
#include <ctime>

#include <write_ccx.hpp>
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

#define INPUT_FILE_NAME input_file.substr(0, input_file.find_last_of('.'))

namespace write_ccx {
    void input_file(const std::string input_file, const bool write_ccx_boilerplate, const bool overwrite_flag) {
        if(utilities::is_file(input_file) == true && overwrite_flag == false) {
            PRINT_ERROR(input_file << " file exists, to force overwrite use -f option");
            return;
        }

        std::ofstream ccx_input_file(input_file);
        auto time = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
        ccx_input_file << "**Author: " << "Email: " << "\n";
        ccx_input_file << "\n";
        ccx_input_file << "**model information {\n";
        ccx_input_file << "**Name: " << INPUT_FILE_NAME << "    Units: mm N C" << "\n";
        if(write_ccx_boilerplate == true) {ccx_input_file << "**Input file: " << input_file << "    Mesh file: " << "\n";}
        else {ccx_input_file << "**Input file: " << input_file << "    Mesh file: " << INPUT_FILE_NAME+"_cmesh.inp" << "\n";}
        ccx_input_file << "**Description: test model" << "\n";
        ccx_input_file << "**Time: " << ctime(&time);
        ccx_input_file << "**}\n";
        ccx_input_file << "\n";
        ccx_input_file << "*HEADING\n";
        ccx_input_file << input_file << "\n";
        ccx_input_file << "\n";
        ccx_input_file << "**include files {\n";
        if(write_ccx_boilerplate == true) {ccx_input_file << "**INCLUDE, INPUT = " << "\n";}
        else {ccx_input_file << "*INCLUDE, INPUT = " << INPUT_FILE_NAME+"_cmesh.inp" << "\n";}
        ccx_input_file << "**INCLUDE, INPUT = " << "Materials.inp" << "\n";
        ccx_input_file << "**}\n";
        ccx_input_file << "\n";
        ccx_input_file << "**materials {\n";
        ccx_input_file << "*MATERIAL, NAME = Steel_mm\n";
        ccx_input_file << "*DENSITY\n";
        ccx_input_file << "7.8e-9\n";
        ccx_input_file << "*ELASTIC\n";
        ccx_input_file << "2.1e5, 0.3\n";
        ccx_input_file << "**}\n";
        ccx_input_file << "\n";
        ccx_input_file << "**boundary conditions {\n";
        ccx_input_file << "*BOUNDARY\n";
        ccx_input_file << "fix, 1, 3\n";
        ccx_input_file << "**}\n";
        ccx_input_file << "\n";
        ccx_input_file << "**sections {\n";
        ccx_input_file << "*SOLID SECTION, ELSET = Eall, MATERIAL = Steel_mm\n";
        ccx_input_file << "**}\n";
        ccx_input_file << "\n";
        ccx_input_file << "**step 1 {\n";
        ccx_input_file << "*STEP\n";
        ccx_input_file << "*STATIC\n";
        ccx_input_file << "\n";
        ccx_input_file << "*CLOAD\n";
        ccx_input_file << "load, 1, 1.0\n";
        ccx_input_file << "\n";
        ccx_input_file << "*NODE FILE\n";
        ccx_input_file << "U\n";
        ccx_input_file << "*EL FILE\n";
        ccx_input_file << "E, S\n";
        ccx_input_file << "*END STEP\n";
        ccx_input_file << "**}\n";
        ccx_input_file.close();
        PRINT_INFO(input_file << " created");
    }
}