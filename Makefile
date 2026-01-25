CXX = g++
CXXFLAGS = -Wall -Wextra -Werror -g -O0 -std=c++17

SRC = src/main.cpp \
      src/tokenizer.cpp \
      src/builtins.cpp \
      src/executor.cpp \
      src/redirections_parser.cpp \
	   

OBJ = $(SRC:.cpp=.o)

minishell: $(OBJ)
	$(CXX) $(CXXFLAGS) $(OBJ) -o $@

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f minishell $(OBJ)

