#pragma once

#include <string>

namespace ccxpre::cmesh_gmsh {
    void write(const std::string input_file, const std::string element_config,
               const std::string research_config, const bool overwrite_flag);

    std::string is_element_header_broken(const std::string element_line, const std::string element_config);

    bool is_set_broken(const std::string set_line, const std::string research_config, const bool is_node_set);
}