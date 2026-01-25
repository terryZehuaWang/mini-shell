#pragma once
#include <string>
#include <vector>
bool parse_redirections(std::vector<std::string> const& tokens,
                        std::vector<std::string>& token_arguments,
                        std::string& direction_str, std::string& target_file);