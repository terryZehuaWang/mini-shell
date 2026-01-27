#include "split_pipeline.hpp"

#include <iostream>
#include <string>
#include <vector>
bool try_split_pipeline(std::vector<std::string> const& tokens,
                        std::vector<std::vector<std::string>>& commands) {
  commands.clear();
  bool found_pipeline_operator = false;

  std::vector<std::string> current;
  for (int i = 0; i < (int)tokens.size(); i++) {
    if (tokens[i] != "|") {
      current.push_back(tokens[i]);
      if (i + 1 < (int)tokens.size() && tokens[i + 1] == "|" &&
          tokens[i] == "&") {
        std::cout
            << "minishell: pipe operator does not support background operator"
            << std::endl;
      }

    } else {  // tokens[i] == "|"
      if (current.empty()) {
        std::cout << "pipe operator must have at least 1 argument before it"
                  << std::endl;
        return false;
      }
      commands.push_back(current);
      current.clear();
      found_pipeline_operator = true;
    }
  }
  if (found_pipeline_operator && current.empty()) {
    std::cout << "pipe operator must have at least 1 argument after it"
              << std::endl;
    return false;
  }
  commands.push_back(current);
  // print_split_tokens(commands);
  return found_pipeline_operator;
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