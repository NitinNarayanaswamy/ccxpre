#pragma once

#include <iostream>

namespace ccxpre::input_env {
    void check_n_run(const std::string input_file, const std::string element_config,
                     const bool overwrite_flag, const bool write_clean_mesh_file_only,
                     const std::string recalculate_input);
}