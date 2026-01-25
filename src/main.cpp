#include <iostream>
#include <string>
#include <vector>

#include "builtins.hpp"
#include "executor.hpp"
#include "redirections_parser.hpp"
#include "tokenizer.hpp"

int main() {
  std::string line;
  bool is_foreground;
  while (true) {
    std::cout << "minishell$ " << std::flush;
    if (!std::getline(std::cin, line)) {
      std::cout << "\nEOF\n";
      break;
    }
    std::vector<std::string> tokens = tokenize(line);
    if (try_builtin(tokens)) {
      continue;
    }

    if (tokens[tokens.size() - 1] == "&") {
      tokens.pop_back();
      is_foreground = false;
    } else {
      is_foreground = true;
    }
    if (try_external_command_with_redirections(tokens, is_foreground)) continue;
    try_external_command(tokens, is_foreground);
  }
  return 0;
}
