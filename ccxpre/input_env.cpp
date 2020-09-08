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

#include <ccxpre.hpp>
#include <log.hpp>

#define INPUT_FILE_EXT input_file.substr(input_file.find_last_of('.')+1)
#define INPUT_FILE_NAME input_file.substr(0, input_file.find_last_of('.'))

namespace ccxpre::input_env {
    void check_n_run(const std::string input_file, const std::string element_config,
                     const bool overwrite_flag, const bool write_clean_mesh_file_only,
                     const std::string recalculate_input) {
        if(INPUT_FILE_EXT == "inp") {
            if(utilities::is_file(input_file)) {
                PRINT_INFO("Abaqus input file detected");
                cmesh_gmsh::write(input_file, element_config, recalculate_input, overwrite_flag);
                if(write_clean_mesh_file_only == false) {write_ccx::input_file(INPUT_FILE_NAME+"_ccx.inp", false, overwrite_flag);}
            }
            else {PRINT_ERROR(input_file << " file not found");}
        }
        else {
            PRINT_ERROR(INPUT_FILE_EXT << " files not supported");
        }
    }
}