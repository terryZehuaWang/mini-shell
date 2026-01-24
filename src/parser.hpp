#pragma once
#include <string>
#include <vector>
std::vector<std::string> tokenize(const std::string& line);
void push_current_to_tokens_logic(
    std::string& current,
    std::vector<std::string>& tokens);  // tokenize helper
void print_tokens(std::vector<std::string> const& tokens);
