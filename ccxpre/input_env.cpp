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

#include <input_env.hpp>
#include <utilities.hpp>
#include <cmesh_gmsh.hpp>
#include <write_ccx.hpp>

#define PRINT(message) std::cout << message << std::endl
#define DEBUG true
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

namespace input_env {
    void check_n_run(const std::string input_file, const std::string element_config, const bool overwrite_flag, const bool write_clean_mesh_file_only) {
        if(INPUT_FILE_EXT == "inp") {
            if(utilities::is_file(input_file)) {
                PRINT_INFO("Abaqus input file detected");
                cmesh_gmsh::write(input_file, element_config, overwrite_flag);
                if(write_clean_mesh_file_only == false) {write_ccx::input_file(INPUT_FILE_NAME+"_ccx.inp", false, overwrite_flag);}
            }
            else {PRINT_ERROR(input_file << " file not found");}
        }
        else {
            PRINT_ERROR(INPUT_FILE_EXT << " files not supported");
        }
    }
}