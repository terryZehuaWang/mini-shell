#include "redirections_parser.hpp"

#include <iostream>
#include <string>
#include <vector>
bool parse_redirections(std::vector<std::string> const& tokens,
                        std::vector<std::string>& token_arguments,
                        std::string& direction_str, std::string& target_file) {
  token_arguments.clear();

  for (size_t i = 0; i < tokens.size(); i++) {
    if (tokens[i] == "<" || tokens[i] == ">" || tokens[i] == ">>") {
      if (i + 1 >= tokens.size()) {
        std::cerr << "syntax error: expected file after " << tokens[i]
                  << std::endl;
        return false;
      }
      direction_str = tokens[i];
      target_file = tokens[i + 1];

      i++;
    } else
      token_arguments.push_back(tokens[i]);
  }

  return true;
}
bool redirection_operator_exist(std::vector<std::string> const& tokens) {
  for (size_t i = 0; i < tokens.size(); i++)
    if (tokens[i] == "<" || tokens[i] == ">" || tokens[i] == ">>") return true;
  return false;
}