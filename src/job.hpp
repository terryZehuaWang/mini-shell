#pragma once
#include <sys/types.h>

#include <string>
enum class JobState { Running, Done };
struct Job {
  int jid;
  pid_t pid;
  std::string command_str;
  JobState state;
};