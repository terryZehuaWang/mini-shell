#include <sys/wait.h>
#include <unistd.h>

#include <cerrno>
#include <cstring>
#include <iostream>
#include <string>
#include <vector>
std::vector<std::string> tokenize(const std::string& line);
void push_current_to_tokens_logic(
    std::string& current,
    std::vector<std::string>& tokens);  // tokenize helper

void print_tokens(std::vector<std::string> const& tokens);
void run_external_command(std::vector<std::string> const& tokens);
std::vector<char*> convert_std_strings_to_c_strings(
    std::vector<std::string> const& std_strings);

bool handle_builtin(std::vector<std::string> const& tokens);
int main() {
  std::string line;
  while (true) {
    std::cout << "minishell$ " << std::flush;
    if (!std::getline(std::cin, line)) {
      std::cout << "\nEOF\n";
      break;
    }
    std::vector<std::string> tokens = tokenize(line);
    if (handle_builtin(tokens)) {
      continue;
    }
    run_external_command(tokens);
  }
  return 0;
}

std::vector<std::string> tokenize(const std::string& line) {
  std::vector<std::string> tokens;
  std::string current;
  for (size_t i = 0; i < line.size(); i++) {
    char c = line[i];
    if (c == ' ') {
      push_current_to_tokens_logic(current, tokens);
    } else if (c == '>' && i + 1 < line.size() && line[i + 1] == '>') {
      push_current_to_tokens_logic(current, tokens);
      tokens.push_back(">>");
      i++;
    } else if (c == '|' || c == '<' || c == '>' || c == '&') {
      push_current_to_tokens_logic(current, tokens);
      tokens.push_back(std::string(1, c));
    } else {
      current += c;
    }
  }
  if (!current.empty()) {
    tokens.push_back(current);
  }
  return tokens;
}

void push_current_to_tokens_logic(std::string& current,
                                  std::vector<std::string>& tokens) {
  if (!current.empty()) {
    tokens.push_back(current);
    current.clear();
  }
}

void print_tokens(std::vector<std::string> const& tokens) {
  std::cout << "tokens:" << std::endl;
  for (size_t i = 0; i < tokens.size(); ++i)
    std::cout << tokens[i] << std::endl;
}

void run_external_command(std::vector<std::string> const& tokens) {
  char* command = const_cast<char*>(tokens[0].c_str());
  std::vector<char*> arguments = convert_std_strings_to_c_strings(tokens);
  arguments.push_back(nullptr);
  pid_t pid = fork();
  if (pid == 0) {
    execvp(command, arguments.data());
    std::cerr << "execvp failed. Error message: " << std::strerror(errno) << std::endl;
    _exit(127);
  } else if (pid > 0) {
    int child_status;
    while (true) {
      pid_t waited_pid = waitpid(pid, &child_status, 0);
      if (waited_pid == -1 && errno == EINTR)  // interrupted by signal
        continue;
      break;
    }
  } else if (pid == -1) {
    std::cerr << "fork failed. Error message: "  << std::strerror(errno) << std::endl;
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

bool handle_builtin(std::vector<std::string> const& tokens) {
  if (tokens[0] == "cd") {
    if (chdir(tokens[1].c_str()) != 0) perror("cd");
    return true;
  }
  if (tokens[0] == "exit") std::exit(0);
  return false;
}
