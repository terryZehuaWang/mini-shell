#include "pipeline_parser.hpp"

#include <iostream>
#include <string>
#include <vector>
bool try_split_pipeline(std::vector<std::string> const& tokens,
                        std::vector<std::vector<std::string>>& commands) {
  commands.clear();
  bool found_redirection_operator = false;

  std::vector<std::string> current;
  for (size_t i = 0; i < tokens.size(); i++) {
    if (tokens[i] != "|") {
      current.push_back(tokens[i]);
      if (tokens[i] == "<" || tokens[i] == ">" || tokens[i] == ">>")
        found_redirection_operator = true;
    } else {  // tokens[i] == "|"
      if (current.empty()) {
        std::cout << "missing command before pipe operator" << std::endl;
        return false;
      }
      commands.push_back(current);
      current.clear();
    }
  }

  if (tokens[tokens.size() - 1] == "&") {
    std::cout << "pipe operator does not support background operator"
              << std::endl;
    return false;
  }
  if (found_redirection_operator) {
    std::cout << "pipe operator does not support redirection operator"
              << std::endl;
    return false;
  }
  if (current.empty()) {
    std::cout << "missing command after pipe operator" << std::endl;
    return false;
  }

  commands.push_back(current);
  return true;
}

bool found_pipeline_operator(std::vector<std::string> const& tokens) {
  for (size_t i = 0; i < tokens.size(); i++)
    if (tokens[i] == "|") return true;
  return false;
}

void print_split_tokens(std::vector<std::vector<std::string>> const& commands) {
  for (size_t i = 0; i < commands.size(); i++) {
    std::cout << "command " << i << ": ";
    for (size_t j = 0; j < commands[i].size(); j++) {
      std::cout << commands[i][j];
      std::cout << " ";
    }
    std::cout << std::endl;
  }
}