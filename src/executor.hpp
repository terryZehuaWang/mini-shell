
#include <string>
#include <vector>
bool try_external_command_with_redirections(
    std::vector<std::string> const& tokens, bool const& is_foreground);
void try_external_command(std::vector<std::string> const& tokens,
                          bool const& is_foreground);
void foreground_wait(int const& pid);
std::vector<char*> convert_std_strings_to_c_strings(
    std::vector<std::string> const& std_strings);

void exit_on_open_error(int fd);
void exit_on_dup2_error(int dup2_return);