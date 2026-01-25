#include "tokenizer.hpp"

#include <iostream>
#include <vector>

std::vector<std::string> tokenize(const std::string& line) {
  std::vector<std::string> tokens;
  std::string current;
  for (size_t i = 0; i < line.size(); i++) {
    char c = line[i];
    if (c == ' ') {
      push_current_to_tokens_logic(current, tokens);
    } else if (c == '>' && i + 1 < line.size() && line[i + 1] == '>') {
      push_current_to_tokens_logic(current, tokens);
      tokens.push_back(">>");
      i++;
    } else if (c == '|' || c == '<' || c == '>' || c == '&') {
      push_current_to_tokens_logic(current, tokens);
      tokens.push_back(std::string(1, c));
    } else {
      current += c;
    }
  }
  if (!current.empty()) {
    tokens.push_back(current);
  }
  return tokens;
}
void push_current_to_tokens_logic(std::string& current,
                                  std::vector<std::string>& tokens) {
  if (!current.empty()) {
    tokens.push_back(current);
    current.clear();
  }
}

void print_tokens(std::vector<std::string> const& tokens) {
  std::cout << "tokens:" << std::endl;
  for (size_t i = 0; i < tokens.size(); ++i)
    std::cout << tokens[i] << std::endl;
}
