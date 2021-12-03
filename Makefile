# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: tayamamo <tayamamo@student.42tokyo.jp>     +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2021/12/04 00:36:10 by tayamamo          #+#    #+#              #
#    Updated: 2021/12/04 00:36:12 by tayamamo         ###   ########.fr        #
# **************************************************************************** #

NAME      := a.out

CXX       := clang++
CFLAGS    := -Wall -Wextra -Werror -std=c++98 --pedantic
RM        := rm -fr

SRC       := main.cpp
SRC_DIR   := .
OBJ_DIR   := ./obj
SRCS      := $(addprefix $(SRC_DIR)/, $(SRC))
OBJS      := $(addprefix $(OBJ_DIR)/, $(SRC:.cpp=.o))
DEPS      := $(addprefix $(OBJ_DIR)/, $(SRC:.cpp=.d))
HEADERS   := $(shell find . -not -path "./.ccls-cache/*" -type f -name '*.hpp' -print)
CPPLINT_FILTERS := --filter=-legal/copyright
DFLAGS	   = -MMD -MF $(@:.o=.d)

.PHONY: all
all: $(NAME)

# Linking
$(NAME): $(OBJS)
	$(CXX) $(CFLAGS) -o $@ $^

# Compiling
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CFLAGS) -o $@ -c $< $(DFLAGS)

.PHONY: lint
lint:
	cpplint --recursive $(CPPLINT_FILTERS) $(SRC) $(HEADERS)

.PHONY: clean
clean:
	$(RM) $(NAME).dSYM $(OBJ_DIR)
	$(RM) *.so *.gcno *.gcda *.gcov *.info $(COVERAGE)

.PHONY: fclean
fclean: clean
	$(RM) $(NAME)

.PHONY: re
re: fclean all

.PHONY: test
test: re
	./$(NAME)

.PHONY: leak
leak: CFLAGS += -g -fsanitize=leak
leak: test

.PHONY: address
address: CFLAGS += -g -fsanitize=address
address: test

.PHONY: thread
thread: CFLAGS += -g -fsanitize=thread
thread: test

.PHONY: memory
memory: CFLAGS += -g -fsanitize=memory
memory: test

.PHONY: valgrind
valgrind: re
	valgrind --leak-check=full --show-leak-kinds=all --track-fds=yes ./$(NAME)
