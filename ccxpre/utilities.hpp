#pragma once

#include <string>

namespace utilities {
    void print_help();

    bool is_file(const std::string file);

    void str_to_upper(std::string &str);

    std::string get_key_value_pair(const std::string str, const std::string keyword, const char key_value_delimiter, const char delimiter);

    std::string which_element_type(const std::string element_config, const char code);
}