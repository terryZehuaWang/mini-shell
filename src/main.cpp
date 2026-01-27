#include <iostream>
#include <string>
#include <vector>

#include "builtins.hpp"
#include "executor.hpp"
#include "job.hpp"
#include "redirections_parser.hpp"
#include "split_pipeline.hpp"
#include "tokenizer.hpp"
int main() {
  std::string line;
  bool is_foreground;

  std::vector<Job> jobs;

  while (true) {
    reap_background(jobs);
    std::cout << "minishell$ " << std::flush;
    if (!std::getline(std::cin, line)) {
      std::cout << "\nEOF\n";
      break;
    }

    std::vector<std::string> tokens = tokenize(line);

    if (tokens.empty()) continue;
    if (try_builtin(tokens, jobs)) continue;

    if (tokens[tokens.size() - 1] == "&") {  // builtins does not support &
      tokens.pop_back();
      is_foreground = false;
    } else {
      is_foreground = true;
    }
    if (try_external_command_with_pipeline(tokens)) continue;
    if (try_external_command_with_redirections(tokens, is_foreground, jobs,
                                               line))
      continue;
    try_external_command(tokens, is_foreground, jobs, line);
  }
  return 0;
}
