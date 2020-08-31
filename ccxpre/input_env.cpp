#include <iostream>

#include <input_env.hpp>
#include <utilities.hpp>
#include <cmesh_gmsh.hpp>

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

namespace input_env {
    void check_n_run(const std::string input_file, const std::string element_config, const bool overwrite_flag) {
        if(INPUT_FILE_EXT == "inp") {
            if(utilities::is_file(input_file)) {
                PRINT_INFO("Abaqus input file detected");
                cmesh_gmsh::write(input_file, element_config, overwrite_flag);
            }
            else {PRINT_ERROR(input_file << " file not found");}
        }
        else {
            PRINT_ERROR(INPUT_FILE_EXT << " files not supported");
        }
    }
}