CXX = g++
CXXFLAGS = -Wall -Wextra -Werror -g -O0 -std=c++17

minishell: src/main.cpp src/parse.cpp
	$(CXX) $(CXXFLAGS) $^ -o $@
