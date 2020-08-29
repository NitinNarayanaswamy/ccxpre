#pragma once

#include <string>

namespace utilities {
    void print_help();

    bool is_file(const std::string file_name);

    bool element_type_code_check(const std::string element_set_include, const char code);
}