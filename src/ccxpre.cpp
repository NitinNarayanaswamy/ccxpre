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

#include <../ccxpre/utilities.hpp>
#include <../ccxpre/write_ccx.hpp>
#include <../ccxpre/input_env.hpp>

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

#define OPT_STR "hfmb:i:e:"
#define USAGE_FMT "[-h] [-fm] [-b b_arg | -i i_arg [-e e_arg]]"

#define WRITE_CCX_BOILERPLATE true

typedef struct {
    bool write_clean_mesh_file_only;
    bool force_overwrite;
    std::string input_file_name;
    std::string element_config;
} options_t;

int main(int argc, char *argv[]) {
    if(argc == 1) {utilities::print_help(); return EXIT_SUCCESS;}

    int opt;
    options_t options = {false, false, "", "All"};

    while((opt = getopt(argc, argv, OPT_STR)) != EOF) {
        switch(opt) {
            case 'h':
                utilities::print_help();
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
                write_ccx::input_file(optarg, WRITE_CCX_BOILERPLATE, options.force_overwrite);
                return EXIT_SUCCESS;
            case 'i':
                options.input_file_name = optarg;
                break;
            case 'e':
                options.element_config = optarg;
                break;
            case '?':
                PRINT_INFO("See help using -h option");
                return EXIT_SUCCESS;
        }
    }

    if(options.element_config == "All") {PRINT_WARNING("By default all elements sets are written");}
    input_env::check_n_run(options.input_file_name, options.element_config, options.force_overwrite);
    return EXIT_SUCCESS;
}

// #include <iostream>
// #include <string>
// #include <fstream>
// #include <chrono>
// #include <ctime>

// void print_ccxpre_help() {
//     // limit each line to 82 chars
//     // std::cout << "" << std::endl;
//     std::cout << "ccxpre, Copyright (C) 2020 Nitin Narayanaswamy" << std::endl;
//     std::cout << "ccxpre comes with ABSOLUTELY NO WARRANTY." << std::endl;
//     std::cout << "This is free software, and you are welcome to redistribute it" << std::endl;
//     std::cout << "under certain conditions." << std::endl;
//     std::cout << std::endl;
//     std::cout << "Usage syntax: ccxpre [options] <your_file_name> [features]" << std::endl;
//     std::cout << std::endl;
//     std::cout << "Common usage:" << std::endl;
//     std::cout << "    ccxpre <your_file_name>_mesh.inp    If the input file is a Abaqus mesh file generated using Gmsh. Clean mesh file <your_file_name>_cmesh.inp and Calculix input file <your_file_name>.inp are created." << std::endl;
//     std::cout << "    ccxpre <your_file_name>_mesh.inp P CAX4    Only surface element sets are included in the clean mesh file and CAX4 is set as element type." << std::endl;
//     std::cout << std::endl;
//     std::cout << "options" << std::endl;
//     std::cout << "    -b    Used to create a CalculiX project with boilerplate code. An input file <your_file_name>.inp is created which contains some common CalculiX commands to get started." << std::endl;
//     std::cout << "    -f    Force overwrite of clean mesh and Calculix input files." << std::endl;
//     std::cout << "    -i    Write only Calculix input file." << std::endl;
//     std::cout << "    -m    Write only clean mesh file." << std::endl;
// }

// bool element_type_code_check(const std::string element_set_include, const char code) {
//     if(element_set_include == "A") {return true;}
//     for(char element_set_code : element_set_include) {
//         if(element_set_code == code) {return true;}
//     }
//     return false;
// }

// struct broken_element_output {
//     bool broken_element_flag = false;
//     bool is_element_definition_broken_flag = false;
//     std::string new_element_line = "";
// };

// void search_for_broken_element_definition(const std::string temp_line, const std::string element_set_include, const std::string element_set_include_type, broken_element_output &out) {
//     if(temp_line[0] != '*') {
//         out.broken_element_flag = false;
//         return;
//     }
//     else if(temp_line.substr(0, 8) == "*ELEMENT") {
//         unsigned int equal_count = 0;
//         std::string element_type;
//         for(char line_char : temp_line) {
//             if(line_char == '=') {
//                 equal_count++;
//                 continue;
//             }
//             if(equal_count == 2) {
//                 element_type.push_back(line_char);
//             }
//         }
//         // Gmsh element set codes A, L, P, S for All sets, Line, Surface, Solid
//         if(element_type.substr(0, 4) == "Line") {
//             bool include = element_type_code_check(element_set_include, 'L');
//             if(include) {
//                 std::cout << "INFO\tWriting Gmsh Line element definition " << temp_line << std::endl;
//                 out.broken_element_flag = false;
//                 out.is_element_definition_broken_flag = false;
//                 out.new_element_line = "*ELEMENT,TYPE="+element_set_include_type+",ELSET="+element_type;
//                 return;
//             }
//             else {
//                 std::cout << "INFO\tSkipping Gmsh Line element definition " << temp_line << std::endl;
//                 out.broken_element_flag = true;
//                 out.is_element_definition_broken_flag = true;
//                 return;
//             }
//         }
//         else if(element_type.substr(0, 7) == "Surface") {
//             bool include = element_type_code_check(element_set_include, 'P');
//             if(include) {
//                 std::cout << "INFO\tWriting Gmsh Surface element definition " << temp_line << std::endl;
//                 out.broken_element_flag = false;
//                 out.is_element_definition_broken_flag = false;
//                 out.new_element_line = "*ELEMENT,TYPE="+element_set_include_type+",ELSET="+element_type;
//                 return;
//             }
//             else {
//                 std::cout << "INFO\tSkipping Gmsh Surface element definition " << temp_line << std::endl;
//                 out.broken_element_flag = true;
//                 out.is_element_definition_broken_flag = true;
//                 return;
//             }
//         }
//     }
//     out.broken_element_flag = false;
//     out.is_element_definition_broken_flag = false;
//     return;
// }

// void ccx_input_file_write(const std::string input_file_name, const std::string mesh_input_file_name, const bool dummy_mesh_input_file) {
//     std::string file_name;
//     if(dummy_mesh_input_file == true) {file_name = input_file_name;}
//     else {file_name = input_file_name+".inp";}
//     std::ofstream ccx_input_file(file_name);
//     auto time = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
//     ccx_input_file << "*HEADING\n";
//     if(dummy_mesh_input_file == true) {ccx_input_file << input_file_name << "\n";}
//     else {ccx_input_file << input_file_name+".inp" << "\n";}
//     ccx_input_file << "\n";
//     ccx_input_file << "**{ model info\n";
//     ccx_input_file << "**Name: test\tUnits: mm N C" << "\n";
//     if(dummy_mesh_input_file == true) {ccx_input_file << "**Input file: " << input_file_name << "\tMesh file: " << mesh_input_file_name << "\n";}
//     else {ccx_input_file << "**Input file: " << input_file_name+".inp" << "\tMesh file: " << input_file_name+"_cmesh.inp" << "\n";}
//     ccx_input_file << "**Discription: " << "\n";
//     ccx_input_file << "**Time: " << ctime(&time);
//     ccx_input_file << "**}\n";
//     ccx_input_file << "\n";
//     ccx_input_file << "**{ include files\n";
//     if(dummy_mesh_input_file == true) {ccx_input_file << "*INCLUDE, INPUT = " << mesh_input_file_name << "\n";}
//     else {ccx_input_file << "*INCLUDE, INPUT = " << input_file_name+"_cmesh.inp" << "\n";}
//     ccx_input_file << "**INCLUDE, INPUT = " << "Materials.inp" << "\n";
//     ccx_input_file << "**}\n";
//     ccx_input_file << "\n";
//     ccx_input_file << "**{ materials\n";
//     ccx_input_file << "\n";
//     ccx_input_file << "**}\n";
//     ccx_input_file << "\n";
//     ccx_input_file << "**{ boundary conditions\n";
//     ccx_input_file << "*BOUNDARY\n";
//     ccx_input_file << "fix, 1, 3\n";
//     ccx_input_file << "**}\n";
//     ccx_input_file << "\n";
//     ccx_input_file << "**{ sections\n";
//     ccx_input_file << "*SOLID SECTION, ELSET = Eall, MATERIAL = Steel_A36_mm\n";
//     ccx_input_file << "**}\n";
//     ccx_input_file << "\n";
//     ccx_input_file << "**{ step 1\n";
//     ccx_input_file << "*STEP\n";
//     ccx_input_file << "*STATIC\n";
//     ccx_input_file << "\n";
//     ccx_input_file << "*CLOAD\n";
//     ccx_input_file << "load, 1, 1.0\n";
//     ccx_input_file << "\n";
//     ccx_input_file << "*NODE FILE\n";
//     ccx_input_file << "U\n";
//     ccx_input_file << "*EL FILE\n";
//     ccx_input_file << "E, S\n";
//     ccx_input_file << "*END STEP\n";
//     ccx_input_file << "**}\n";
//     ccx_input_file.close();
// }

// int main(int argc, char *argv[]) {
//     if(argc == 1) {print_ccxpre_help(); return 0;}
//     if(argc >= 2) {
//         std::string ccxpre_options = argv[1];
//         unsigned int argv_mesh_input_file_name = 1;
//         unsigned int argv_element_set_include = 1;
//         unsigned int argv_element_set_include_type = 1;
//         bool write_cmesh_file = true; // clean mesh
//         bool write_input_file = true;
//         bool force_overwrite = false;
//         bool dummy_mesh_input_file = false;

//         if(ccxpre_options.at(0) == '-') {
//             if(ccxpre_options == "-h") {
//                 print_ccxpre_help();
//                 return 0;
//             }
//             if(argc < 3) {
//                 std::cout << "ERROR\tRequires mesh file as input" << std::endl;
//                 std::cout << "INFO\tRun syntax, ccxpre [options] <your_file_name>_mesh.inp" << std::endl;
//                 return 0;
//             }
//             for(char option_code : ccxpre_options) {
//                 if(option_code == 'm') {write_cmesh_file = true; write_input_file = false;}
//                 if(option_code == 'i') {write_cmesh_file = false; write_input_file = true;}
//                 if(option_code == 'f') {force_overwrite = true;}
//                 if(option_code == 'b') {dummy_mesh_input_file = true;}
//             }
//             argv_mesh_input_file_name = 2;
//             argv_element_set_include = 3;
//             argv_element_set_include_type = 4;
//         }
//         else {
//             argv_mesh_input_file_name = 1;
//             argv_element_set_include = 2;
//             argv_element_set_include_type = 3;
//         }

//         if(dummy_mesh_input_file == true) {
//             ccx_input_file_write(argv[argv_mesh_input_file_name], argv[argv_mesh_input_file_name], dummy_mesh_input_file);
//             std::cout << "INFO\t" << argv[argv_mesh_input_file_name] << " created" << std::endl;
//             return 0;
//         }

//         std::string mesh_input_file_name = argv[argv_mesh_input_file_name];
//         if(mesh_input_file_name.size() >= 10) {
//             int count = mesh_input_file_name.size()-9;
//             std::string extension;
//             std::string input_file_name;
//             for(auto it = mesh_input_file_name.begin(); it != mesh_input_file_name.end(); it++) {
//                 if(count > 0) {input_file_name.push_back(*it);} else {extension.push_back(*it);}
//                 count--;
//             }
//             if(extension == "_mesh.inp") {
//                 // turn into function temp part
//                 if(dummy_mesh_input_file == false) {
//                     std::ifstream temp_input_file(mesh_input_file_name);
//                     if(!temp_input_file.is_open()) {
//                         std::cout << "ERROR\t" << mesh_input_file_name << " does not exist" << std::endl;
//                         return 0;
//                     }
//                     temp_input_file.close();
//                 }

//                 // element set and type
//                 std::string element_set_include;
//                 std::string element_set_include_type;
//                 if(write_cmesh_file == true) {
//                     if(argv[argv_element_set_include] != NULL) {
//                         element_set_include = argv[argv_element_set_include];
//                     }
//                     else {
//                         element_set_include = "A";
//                         std::cout << "WARNING\tAll Gmsh elements sets are included by default. To write only a specific Gmsh element set use syntax, ccxpre [options] <your_file_name>_mesh.inp <element_set_codes>. Element set codes are L, P, S for Line, Surface, Solid respectively" << std::endl;
//                     }
//                     if(argv[argv_element_set_include_type] != NULL && argv[argv_element_set_include] != NULL) {
//                         element_set_include_type = argv[argv_element_set_include_type];
//                     }
//                     else {
//                         element_set_include_type = "";
//                         std::cout << "WARNING\tNo ccx element type input for Gmsh elements sets, hence unchanged by default. To write only a specific ccx element type for a Gmsh element set use syntax, ccxpre [options] <your_file_name>_mesh.inp <element_set_codes> <element_set_type> for example a axissymmetric model with quad elements, use, ccxpre test_mesh.inp P CAX4" << std::endl;
//                     }
//                 }

//                 // clean mesh write
//                 if(write_cmesh_file == true) {
//                     if(force_overwrite == false) {
//                         std::ifstream temp_input_file(input_file_name+"_cmesh.inp");
//                         if(temp_input_file.is_open()) {
//                             std::cout << "ERROR\t" << input_file_name+"_cmesh.inp" << " exists" << std::endl;
//                             return 0;
//                         }
//                         temp_input_file.close();
//                     }
//                     std::ifstream mesh_input_file(mesh_input_file_name);
//                     std::ofstream mesh_cleaned_input_file(input_file_name+"_cmesh.inp");
//                     std::string temp_line;
//                     unsigned int line_count = 1;
//                     broken_element_output out;
//                     while(std::getline(mesh_input_file, temp_line)) {
//                         // skip first two lines that contains heading
//                         if(line_count >= 3) {
//                             if(temp_line == "*NODE") {
//                                 mesh_cleaned_input_file << "*NODE,NSET=Nall\n";
//                                 continue;
//                             }
//                             search_for_broken_element_definition(temp_line, element_set_include, element_set_include_type, out);
//                             if(out.is_element_definition_broken_flag | out.broken_element_flag) {
//                                 continue;
//                             }
//                             else {
//                                 if(temp_line.substr(0, 8) == "*ELEMENT") {
//                                     mesh_cleaned_input_file << out.new_element_line << "\n";
//                                     continue;
//                                 }
//                                 mesh_cleaned_input_file << temp_line << "\n";
//                             }
//                         }
//                         line_count++;
//                     }
//                     mesh_cleaned_input_file.close();
//                     std::cout << "INFO\t" << input_file_name+"_cmesh.inp" << " created" << std::endl;
//                 }
//                 else {
//                     std::cout << "INFO\tClean mesh file not created" << std::endl;
//                 }

//                 // ccx input file write
//                 if(write_input_file == true) {
//                     if(force_overwrite == false) {
//                         std::ifstream temp_input_file(input_file_name+".inp");
//                         if(temp_input_file.is_open()) {
//                             std::cout << "ERROR\t" << input_file_name+".inp" << " exists" << std::endl;
//                             return 0;
//                         }
//                         temp_input_file.close();
//                     }
//                     ccx_input_file_write(input_file_name, mesh_input_file_name, dummy_mesh_input_file);
//                     std::cout << "INFO\t" << input_file_name+".inp" << " created" << std::endl;
//                 }
//                 else {
//                     std::cout << "INFO\tInput file not created" << std::endl;
//                 }
//             }
//             else {
//                 std::cout << "ERROR\tInvalid mesh file name" << std::endl;
//                 std::cout << "INFO\tMesh file name syntax, <your_file_name>_mesh.inp" << std::endl;
//             }
//         }
//         else {
//             std::cout << "ERROR\tInvalid mesh file name" << std::endl;
//             std::cout << "INFO\tMesh file name syntax, <your_file_name>_mesh.inp" << std::endl;
//         }
//     }
//     else {
//         std::cout << "ERROR\tRequires mesh file as input" << std::endl;
//         std::cout << "INFO\tRun syntax, ccxpre [options] <your_file_name>_mesh.inp" << std::endl;
//     }
//     return 0;
// }