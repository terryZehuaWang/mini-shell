
#include <string>
#include <vector>

#include "job.hpp"
bool try_external_command_with_redirections(
    std::vector<std::string> const& tokens, bool const& is_foreground,
    std::vector<Job>& jobs, std::string const& line);

void foreground_wait(pid_t const& pid);
void reap_background(std::vector<Job>& jobs);
std::vector<char*> convert_std_strings_to_c_strings(
    std::vector<std::string> const& std_strings);
void exit_on_open_error(int fd);
void exit_on_dup2_error(int dup2_return);
void try_external_command(std::vector<std::string> const& tokens,
                          bool const& is_foreground, std::vector<Job>& jobs,
                          std::string const& line);
