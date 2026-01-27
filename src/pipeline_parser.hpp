
#pragma once
#include <string>
#include <vector>
bool try_split_pipeline(std::vector<std::string> const& tokens,
                        std::vector<std::vector<std::string>>& commands);
void print_split_tokens(std::vector<std::vector<std::string>> const& commands);
bool found_pipeline_operator(std::vector<std::string> const& tokens);