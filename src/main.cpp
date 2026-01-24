#include <fcntl.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>

#include <algorithm>
#include <cerrno>
#include <cstring>
#include <iostream>
#include <string>
#include <vector>

#include "parser.hpp"

void try_external_command(std::vector<std::string> const& tokens,
                          bool const& is_foreground);

bool try_builtin(std::vector<std::string> const& tokens);
bool try_external_command_with_redirections(
    std::vector<std::string> const& tokens, bool const& is_foreground);
std::vector<char*> convert_std_strings_to_c_strings(
    std::vector<std::string> const& std_strings);
void foreground_wait(int const& pid);
bool parse_redirections(std::vector<std::string> const& tokens,
                        std::vector<std::string>& arguments,
                        std::string& target_file);
void exit_on_open_error(int fd);
void exit_on_dup2_error(int dup2_return);
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

    if (tokens[tokens.size()] == "&") {
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

void try_external_command(std::vector<std::string> const& tokens,
                          bool const& is_foreground) {
  char* command = const_cast<char*>(tokens[0].c_str());
  std::vector<char*> arguments = convert_std_strings_to_c_strings(tokens);
  arguments.push_back(nullptr);
  pid_t pid = fork();
  if (pid == 0) {
    execvp(command, arguments.data());
    std::cerr << "execvp failed. Error message: " << std::strerror(errno)
              << std::endl;
    _exit(127);
  } else if (pid > 0 && is_foreground) {
    foreground_wait(pid);
  } else if (pid == -1) {
    std::cerr << "fork failed. Error message: " << std::strerror(errno)
              << std::endl;
  }
}

std::vector<char*> convert_std_strings_to_c_strings(
    std::vector<std::string> const& std_strings) {
  std::vector<char*> c_strings;
  for (std::string const& std_string : std_strings) {
    c_strings.push_back(const_cast<char*>(std_string.c_str()));
  }
  return c_strings;
}

void foreground_wait(int const& pid) {
  int child_status;
  while (true) {
    pid_t waited_pid = waitpid(pid, &child_status, 0);
    if (waited_pid == -1 && errno == EINTR)  // interrupted by signal
      continue;
    break;
  }
}

bool try_builtin(std::vector<std::string> const& tokens) {
  if (tokens[0] == "cd") {
    if (chdir(tokens[1].c_str()) != 0) perror("cd");
    return true;
  }
  if (tokens[0] == "exit") std::exit(0);
  return false;
}

bool parse_redirections(std::vector<std::string> const& tokens,
                        std::vector<std::string>& token_arguments,
                        std::string& direction_str, std::string& target_file) {
  token_arguments.clear();
  bool target_file_exist = false;
  for (size_t i = 0; i < tokens.size(); i++) {
    if (tokens[i] == "<" || tokens[i] == ">" || tokens[i] == ">>") {
      if (i + 1 >= tokens.size()) {
        std::cerr << "syntax error: expected file after <<tokens[i]"
                  << std::endl;
        return false;
      }
      direction_str = tokens[i];
      target_file = tokens[i + 1];
      target_file_exist = true;
      i++;
    } else
      token_arguments.push_back(tokens[i]);
  }

  return (target_file_exist);
}
bool try_external_command_with_redirections(
    std::vector<std::string> const& tokens, bool const& is_foreground) {
  std::vector<std::string> token_arguments;
  std::string direction_str;
  std::string target_file;
  if (!parse_redirections(tokens, token_arguments, direction_str, target_file))
    return false;

  std::vector<char*> arguments =
      convert_std_strings_to_c_strings(token_arguments);
  arguments.push_back(nullptr);

  int pid = fork();
  if (pid == 0) {
    //< in
    //> out
    //>> out append
    int open_flag;
    if (direction_str == "<")
      open_flag = O_RDONLY;
    else if (direction_str == ">")
      open_flag = O_WRONLY | O_TRUNC | O_CREAT;
    else if (direction_str == ">>")
      open_flag = O_WRONLY | O_APPEND | O_CREAT;

    int fd = open(target_file.c_str(), open_flag);
    exit_on_open_error(fd);
    exit_on_dup2_error(
        dup2(fd, (direction_str == "<") ? STDIN_FILENO : STDOUT_FILENO));
    close(fd);
    execvp(arguments[0], arguments.data());
    std::cerr << "execvp failed. Error message: " << std::strerror(errno)
              << std::endl;

  } else if (pid == -1) {
    std::cerr << "fork failed. Error message: " << std::strerror(errno)
              << std::endl;
    return true;  // to avoid printing fork error message again in try external
                  // command
  }
  if (is_foreground) {
    foreground_wait(pid);
  }
  return true;
}
void exit_on_open_error(int fd) {
  if (fd < 0) {
    std::cerr << "open failed: " << std::strerror(errno) << "\n";
    _exit(1);
  }
}
void exit_on_dup2_error(int dup2_return) {
  if (dup2_return < 0) {
    std::cerr << "dup2 failed: " << std::strerror(errno) << "\n";
    _exit(1);
  }
}
