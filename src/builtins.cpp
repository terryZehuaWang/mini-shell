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
    for (size_t i = 0; i < tokens.size(); i++) {
      if (tokens[i] == "|") {
        std::cout << "minishell: pipeline with builtins not supported"
                  << std::endl;
        return true;
      }
      if (tokens[i] == "&") {
        std::cout << "minishell: background with builtins not supported"
                  << std::endl;
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
  if (tokens[0] == "help") {
    if (tokens.size() == 1) {
      print_help_message();
    } else {
      std::cout << "help: too many arguments" << std::endl;
    }
  }
  if (tokens[0] == "exit") std::exit(0);
  return false;
}

void print_help_message() {
  std::cout << "minishell help" << std::endl
            << "Builtins:" << std::endl
            << "  cd <dir>" << std::endl
            << "  jobs" << std::endl
            << "  exit" << std::endl
            << "  help" << std::endl
            << std::endl
            << "Operators:" << std::endl
            << "  <    input redirection" << std::endl
            << "  >    output redirection" << std::endl
            << "  >>   output append" << std::endl
            << "  &    background execution" << std::endl
            << "  |    pipeline (foreground only)" << std::endl
            << std::endl
            << "Notes:" << std::endl
            << "  - pipelines are foreground only" << std::endl
            << "  - builtins cannot be used with | or &" << std::endl
            << "  - redirection within pipelines is not supported" << std::endl;
}