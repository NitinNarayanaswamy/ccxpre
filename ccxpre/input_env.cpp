#include <iostream>

#include <utilities.hpp>

#define PRINT(message) std::cout << message << std::endl
#define PRINT_NEW_LINE std::cout << std::endl

namespace input_env {
    void check_n_run(const std::string input_file, const std::string element_config) {
        const auto input_file_ext = input_file.substr(input_file.find_last_of('.')+1);
        if(input_file_ext == "inp") {
            PRINT("INFO    Abaqus input file detected");
            if(utilities::is_file(input_file)) {
                // do mesh clean
            }
            else {PRINT("ERROR    " << input_file << " file not found");}
        }
        else {
            PRINT("ERROR    ." << input_file_ext << " files not supported");
        }
    }
}