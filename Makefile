NAME = irc
FLAGS = -Wall -Wextra -Werror -std=c++98
SRCS =	src/Connections.cpp \
		src/Server.cpp \
		src/Socket.cpp \
		src/main.cpp
		
OBJ_PATH = ./objs/
OBJS = $(patsubst %.cpp,$(OBJ_PATH)%.o,$(SRCS))
CPP = c++

all:$(NAME)

$(NAME): $(OBJ_PATH) $(OBJS)
	@$(CPP) $(FLAGS) $(OBJS) -o $(NAME)
	@echo "                                                         "
	@echo " \033[1;32m  ___|   _ \    \  |   _ \ _ _|  |      ____|  __ \   | "
	@echo "  |      |   |  |\/ |  |   |  |   |      __|    |   |  | "
	@echo "  |      |   |  |   |  ___/   |   |      |      |   | _| "
	@echo " \____| \___/  _|  _| _|    ___| _____| _____| ____/  _) \033[0m"
	@echo "                                                         "

$(OBJ_PATH)%.o: %.cpp
	@$(CPP) $(FLAGS) -c $< -o $@

$(OBJ_PATH):
	@mkdir -p $(OBJ_PATH)

clean:
	@rm -rf $(OBJ_PATH)
	@echo "\033[1;32m.o files removed!\033[0m"

fclean: clean
	@rm -rf $(NAME)
	@echo "\033[1;32mbinary files removed!\033[0m"

re: fclean all