NAME        = minishell

CC          = cc
CFLAGS      = -Wall -Wextra -Werror -g3

SRC_DIR     = src
LIBFT_DIR   = libft
INC_DIR     = includes
OBJ_DIR     = objs

BUILTINS_DIR    = $(SRC_DIR)/builtins
CD_DIR         = $(BUILTINS_DIR)/cd
EXEC_DIR       = $(SRC_DIR)/exec
HD_DIR         = $(EXEC_DIR)/here_doc
PIPE_DIR       = $(EXEC_DIR)/pipe
PARSE_DIR      = $(SRC_DIR)/parsing
WILDCARD_DIR   = $(PARSE_DIR)/wildcards
REDIRECT_DIR   = $(SRC_DIR)/redirect

SRCS = $(SRC_DIR)/main.c \
       $(SRC_DIR)/get_readline.c \
       $(SRC_DIR)/bonus.c \
       $(SRC_DIR)/shell_loop.c \
       $(BUILTINS_DIR)/ft_env.c \
       $(BUILTINS_DIR)/ft_pwd.c \
       $(BUILTINS_DIR)/ft_echo.c \
       $(BUILTINS_DIR)/ft_export.c \
       $(BUILTINS_DIR)/ft_unset.c \
       $(BUILTINS_DIR)/ft_exit.c \
       $(BUILTINS_DIR)/ft_cd.c \
       $(SRC_DIR)/signal.c \
       $(PARSE_DIR)/handle_quote.c \
       $(WILDCARD_DIR)/handle_wildcards.c \
       $(WILDCARD_DIR)/handle_wildcards2.c \
       $(WILDCARD_DIR)/handle_wildcards3.c \
       $(PARSE_DIR)/handle_quote2.c \
       $(PARSE_DIR)/handle_dollar.c \
       $(PARSE_DIR)/split_quote.c \
       $(REDIRECT_DIR)/cmd_redirect.c \
       $(REDIRECT_DIR)/find_env_path.c \
       $(REDIRECT_DIR)/parse_cmd.c \
       $(REDIRECT_DIR)/redirect_input.c \
       $(REDIRECT_DIR)/redirect_output.c

OBJS    = $(SRCS:%.c=$(OBJ_DIR)/%.o)
INCLUDES = -I $(INC_DIR) -I $(LIBFT_DIR)

all: $(NAME)

$(NAME): $(OBJS) $(LIBFT_DIR)/libft.a
	@echo "\033[32mLinking objects and creating $(NAME)...\033[0m"
	@$(CC) $(CFLAGS) $(OBJS) $(LIBFT_DIR)/libft.a -o $(NAME) -lreadline
	@echo "\033[32m$(NAME) successfully created!\033[0m"

$(OBJ_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(LIBFT_DIR)/libft.a:
	@echo "\033[34mBuilding libft...\033[0m"
	@make -C $(LIBFT_DIR) > /dev/null 2>&1

clean:
	@echo "\033[33mCleaning object files...\033[0m"
	@rm -rf $(OBJ_DIR)
	@make -C $(LIBFT_DIR) clean > /dev/null 2>&1
	@echo "\033[32mClean completed successfully!\033[0m"

fclean: clean
	@echo "\033[33mRemoving $(NAME)...\033[0m"
	@rm -f $(NAME)
	@make -C $(LIBFT_DIR) fclean > /dev/null 2>&1
	@echo "\033[32mFull clean completed successfully!\033[0m"

re: fclean all

.PHONY: all clean fclean re
