#pragma once
#include <string>
#include <vector>

#include "job.hpp"
bool try_builtin(std::vector<std::string> const& tokens,
                 std::vector<Job>& jobs);
                 
void print_help_message();
