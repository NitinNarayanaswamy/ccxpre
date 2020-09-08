#pragma once

#include <string>
#include <vector>

namespace ccxpre::utilities {
    void print_help();

    bool is_file(const std::string file);

    void str_to_upper(std::string &str);

    std::vector<unsigned int> csv_str_to_numbers(const std::string str);

    std::string get_key_value_pair(const std::string str, const std::string keyword, const char key_value_delimiter, const char delimiter);

    std::string which_element_type(const std::string element_config, const char code);

    std::string find_elements_of_nodes(const std::string input_file, std::vector<unsigned int> &nodes,
                                            const std::string element_header, const char key_value_delimiter,
                                            const char delimiter);
    
    void write_ccxpre_temp_to_cmesh(const std::string temp_file, const std::string mesh_file);
}