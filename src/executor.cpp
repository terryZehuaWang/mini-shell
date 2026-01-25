
#include "executor.hpp"

#include <fcntl.h>
#include <sys/stat.h>
#include <sys/wait.h>

#include <cerrno>
#include <cstring>
#include <iostream>
#include <string>
#include <vector>

#include "redirections_parser.hpp"

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
    int fd;
    if (direction_str == "<") {
      fd = open(target_file.c_str(), O_RDONLY);
    } else {
      int open_flag = O_WRONLY | O_CREAT;
      if (direction_str == ">") {
        open_flag |= O_TRUNC;
      } else if (direction_str == ">>") {
        open_flag |= O_APPEND;
      }
      fd =
          open(target_file.c_str(), open_flag, 0644);  // 0644 means rw- r-- r--
    };
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

void foreground_wait(int const& pid) {
  int child_status;
  while (true) {
    pid_t waited_pid = waitpid(pid, &child_status, 0);
    if (waited_pid == -1 && errno == EINTR) continue;
    std::cerr << "waitpid failed: " << std::strerror(errno) << "\n";
    break;
  }
}

void reap_background() {
  int status;
  while (true) {
    pid_t waited_pid = waitpid(-1, &status, WNOHANG);
    if (waited_pid > 0)
      continue;
    else if (waited_pid == 0)
      break;
    else {  //(waited_pid == -1)
      if (errno == EINTR) continue;
      if (errno == ECHILD) break;
    }
  }
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