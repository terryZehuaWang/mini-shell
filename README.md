# MiniShell — A Minimal UNIX Shell in C++

MiniShell is a small UNIX-like shell implemented in C++ using POSIX system calls.
It supports command execution, I/O redirection, background jobs, and foreground pipelines.

Demo video (<2 minutes): https://www.youtube.com/watch?v=flBpD5-LZeY 
---

## Features
### Built-in Commands
- `cd <dir>` — change working directory
- `jobs` — list background jobs and their states
- `exit` — exit the shell
- `help` — display supported commands and operators

### Command Execution
-  Execute external programs using `fork()` and `execvp()`
-  Basic error handling for common syscalls (fork/execvp/open/dup2/waitpid)
  
### I/O Redirection
- Input redirection: `< file`
- Output redirection: `> file`
- Output append: `>> file`

### Background Jobs
- Run commands in the background using `&`
- Maintain a job table (job ID, PID, command, state)
- Reap finished background jobs using `waitpid(WNOHANG)`

### Pipelines
- Foreground pipelines using `|`
- Multiple processes executing commands connected with pipes
  
---

## Supported Syntax (Examples)

```sh
help
ls
echo hello > out.txt
cat < out.txt
echo hi >> out.txt
sleep 5 &
jobs
ls | wc -l
ls | sort | wc -l
