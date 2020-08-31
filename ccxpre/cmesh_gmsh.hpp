#pragma once

#include <string>

namespace cmesh_gmsh {
    void write(const std::string input_file, const std::string element_config, const bool overwrite_flag);

    std::string is_element_header_broken(const std::string element_line, const std::string element_config);
}