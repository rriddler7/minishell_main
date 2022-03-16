NAME_O 				= minishell
NAME_B 				= minishell_bonus
NAME				= minishell

FLAGS 				= -Wall -Wextra -Werror

R_FLAGS				= -L ~/.brew/opt/readline/lib -I ~/.brew/opt/readline/include		

SRC_O_DIR			= src/mandatory/
SRC_B_DIR			= src/bonus/
SRC_O 				= envp.c \
					free_struct.c \
					free_struct2.c \
					init.c \
					main.c \
					signals.c \
					utils_cut.c \
					utils_other.c \
					utils_other2.c \
					utils_other3.c \
					utils_print.c \
					utils_lists.c \
					shlvl.c \
					parser.c \
					put_t_comm.c \
					finder.c \
					redirect.c \
					quotes.c \
					dollar.c \
					pre_open.c \
					one_build_in.c \
					build_echo.c \
					build_cd.c \
					build_cd2.c \
					build_pwd.c \
					build_unset.c \
					build_export.c \
					build_export2.c \
					build_export3.c \
					build_env.c \
					build_exit.c \
					star.c \
					star2.c \
					star3.c \
					star4.c \
					star5.c \
					star6.c \
					pipe.c \
					child_and_dups.c \
					heredoc.c \
					get_path.c

SRC_B 				= envp_bonus.c \
					free_struct_bonus.c \
					free_struct2_bonus.c \
					init_bonus.c \
					main_bonus.c \
					signals_bonus.c \
					utils_cut_bonus.c \
					utils_other_bonus.c \
					utils_other2_bonus.c \
					utils_other3_bonus.c \
					utils_print_bonus.c \
					utils_lists_bonus.c \
					shlvl_bonus.c \
					parser_bonus.c \
					put_t_comm_bonus.c \
					finder_bonus.c \
					redirect_bonus.c \
					quotes_bonus.c \
					dollar_bonus.c \
					pre_open_bonus.c \
					one_build_in_bonus.c \
					build_echo_bonus.c \
					build_cd_bonus.c \
					build_cd2_bonus.c \
					build_pwd_bonus.c \
					build_unset_bonus.c \
					build_export_bonus.c \
					build_export2_bonus.c \
					build_export3_bonus.c \
					build_env_bonus.c \
					build_exit_bonus.c \
					star_bonus.c \
					star2_bonus.c \
					star3_bonus.c \
					star4_bonus.c \
					star5_bonus.c \
					star6_bonus.c \
					pipe_bonus.c \
					child_and_dups_bonus.c \
					heredoc_bonus.c \
					get_path_bonus.c

HEAD_DIR 			= inc/
HEAD_O 				= minishell.h
HEAD_B 				= minishell_bonus.h

OBJ_O				= $(patsubst %.c,%.o,$(SRC_O))
OBJ_B				= $(patsubst %.c,%.o,$(SRC_B))
OBJ_DIR				= obj/
OBJ_O_DIR			= obj/mandatory/
OBJ_B_DIR			= obj/bonus/

OBJS_1 				= $(addprefix $(OBJ_O_DIR), $(OBJ_O))
OBJS_2 				= $(addprefix $(OBJ_B_DIR), $(OBJ_B))

CC 					= gcc
RM 					= rm -Rf

all: 				$(NAME_O)

bonus:				$(NAME_B)

$(OBJ_O_DIR)%.o: 	$(SRC_O_DIR)%.c $(HEAD_DIR)$(HEAD_O)
					@mkdir -p $(OBJ_O_DIR)
					@gcc -c $< -o $@ -I $(HEAD_DIR) -I ~/.brew/opt/readline/include

$(OBJ_B_DIR)%.o: 	$(SRC_B_DIR)%.c $(HEAD_DIR)$(HEAD_B)
					@mkdir -p $(OBJ_B_DIR)
					@gcc -c $< -o $@ -I $(HEAD_DIR) -I ~/.brew/opt/readline/include

$(NAME_O):  		$(OBJS_1)
					$(CC) $(FLAGS) $(OBJS_1) -o $(NAME_O) -lreadline $(R_FLAGS)


$(NAME_B):  		$(OBJS_2)
					$(CC) $(FLAGS) $(OBJS_2) -o $(NAME_B) -lreadline $(R_FLAGS)

clean:
					$(RM) $(OBJ_DIR)


fclean: 			clean
					$(RM) $(NAME_O) $(NAME_B)

re: 				fclean all

.PHONY: 			all bonus clean fclean re