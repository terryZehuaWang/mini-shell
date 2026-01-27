#include "builtins.hpp"

#include <unistd.h>

#include <iostream>
#include <string>
#include <vector>

#include "executor.hpp"
#include "job.hpp"

bool try_builtin(std::vector<std::string> const& tokens,
                 std::vector<Job>& jobs) {
  if (tokens[0] == "cd" || tokens[0] == "jobs" || tokens[0] == "exit") {
    for (int i = 0; i < (int)tokens.size(); i++) {
      if (tokens[i] == "|") {
        std::cout << "minishell: pipeline with builtins not supported"
                  << std::endl;
        return true;
      }
      if (tokens[i] == "&") {
        std::cout << "minishell: background with builtins" << std::endl;
        return true;
      }
    }
  }
  if (tokens[0] == "cd") {
    if (tokens.size() < 2)
      std::cout << "cd: too few arguments" << std::endl;
    else if (chdir(tokens[1].c_str()) != 0)
      perror("cd");
    return true;
  }
  if (tokens[0] == "jobs") {
    if (tokens.size() == 1) {
      if (jobs.empty()) {
        std::cout << "No jobs exist" << std::endl;
        return true;
      }
      reap_background(jobs);
      for (size_t i = 0; i < jobs.size(); i++)
        std::cout << jobs[i].jid << " " << jobs[i].pid << " "
                  << jobs[i].command_str << " "
                  << (jobs[i].state == JobState::Running ? "Running" : "Done")
                  << std::endl;
    } else {
      std::cout << "jobs: too many arguments" << std::endl;
    }

    return true;
  }
  if (tokens[0] == "exit") std::exit(0);
  return false;
}