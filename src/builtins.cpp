#include "builtins.hpp"

#include <unistd.h>

#include <string>
#include <vector>

bool try_builtin(std::vector<std::string> const& tokens) {
  if (tokens[0] == "cd") {
    if (chdir(tokens[1].c_str()) != 0) perror("cd");
    return true;
  }
  if (tokens[0] == "exit") std::exit(0);
  return false;
}