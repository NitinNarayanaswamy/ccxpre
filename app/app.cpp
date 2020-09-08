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
#include <getopt.h>

#include <ccxpre/ccxpre.hpp>
#include <ccxpre/log.hpp>

#define OPT_STR "hfmb:i:e:r:"
#define USAGE_FMT "[-h] [-fm] [-b b_arg | -i i_arg [-e e_arg -r r_arg]]"

#define WRITE_CCX_BOILERPLATE true

typedef struct {
    bool write_clean_mesh_file_only;
    bool force_overwrite;
    std::string input_file_name;
    std::string element_config;
    std::string recalculate_input;
} options_t;

int main(int argc, char *argv[]) {
    if(argc == 1) {ccxpre::utilities::print_help(); return EXIT_SUCCESS;}

    int opt;
    options_t options = {false, false, "", "All", "None"};

    while((opt = getopt(argc, argv, OPT_STR)) != EOF) {
        switch(opt) {
            case 'h':
                ccxpre::utilities::print_help();
                return EXIT_SUCCESS;
            case 'f':
                options.force_overwrite = true;
                PRINT_WARNING("Files will be overwritten");
                break;
            case 'm':
                options.write_clean_mesh_file_only = true;
                PRINT_INFO("Writing only clean mesh file");
                break;
            case 'b':
                ccxpre::write_ccx::input_file(optarg, WRITE_CCX_BOILERPLATE, options.force_overwrite);
                return EXIT_SUCCESS;
            case 'i':
                options.input_file_name = optarg;
                break;
            case 'e':
                options.element_config = optarg;
                break;
            case 'r':
                options.recalculate_input = optarg;
                break;
            case '?':
                PRINT_INFO("For help use -h option");
                return EXIT_SUCCESS;
        }
    }

    if(options.element_config == "All") {PRINT_WARNING("By default all elements sets are written");}
    ccxpre::input_env::check_n_run(options.input_file_name, options.element_config, options.force_overwrite,
                           options.write_clean_mesh_file_only, options.recalculate_input);
    return EXIT_SUCCESS;
}
