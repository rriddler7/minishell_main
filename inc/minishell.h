/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/05 20:08:31 by pveeta            #+#    #+#             */
/*   Updated: 2022/03/14 01:39:48 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdlib.h>
# include <unistd.h>
# include <stdio.h>
# include <unistd.h>
# include <fcntl.h>
# include <string.h> 
# include <sys/errno.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <signal.h>
# include <sys/wait.h>
# include <sys/types.h>
# include <dirent.h>

# define U_INT	unsigned int

typedef enum e_status
{
	success,
	fail,
	end,
}	t_status;

typedef struct s_direct
{
	char			*name;
	char			*stop_word;
	int				value;
	int				incoming;
	int				twin;
	struct s_direct	*next;
}	t_direct;

typedef struct s_env
{
	char			*key;
	char			*value;
	int				equal;
	struct s_env	*next;
}	t_env;

typedef struct s_templ
{
	char			*value;
	U_INT			status;
	struct s_templ	*next;
}	t_templ;

typedef struct s_comm
{
	char			**words;
	t_direct		*direct_in;
	t_direct		*direct_out;
	int				build_number;
	struct s_comm	*next;
}	t_comm;

typedef struct s_input
{
	int			num_of_command;
	char		**arg_env;
	int			**fd;
	U_INT		num_error;
	char		*token;
	t_comm		*command;
	t_direct	*direct;
	t_env		*envp;
	t_env		*star;
	t_status	have_star;
}	t_input;

/*----Все билд-ины---*/
U_INT		launch_pwd(t_input *input);
U_INT		launch_env(t_input *input);
U_INT		launch_unset(t_input *input, t_comm *command);
U_INT		launch_export(t_input *input, t_comm *command, U_INT i);
U_INT		launch_exit(t_input *input, t_comm *command);
U_INT		launch_echo(t_input *input);
U_INT		launch_cd(t_input *input, t_comm *command);
U_INT		launch_exit(t_input *input, t_comm *command);
void		cd_print_error(t_input *input, U_INT error_num, \
			char *msg, char *descrip);
char		*get_path_cd(t_input *input, t_comm *command, t_env *copy);
t_env		*create_sort_env(t_env *old, t_input *input);
t_env		*env_sort(t_env *list, t_env *new, t_input *input);
t_env		*sort_export(t_input *input);

/*----child_and_dups - работа с дочками, поиск пути---*/
void		it_is_child(t_input *input, U_INT i, U_INT counter);

/*----init-инициализация структур---*/
void		init_input(t_input *input);
void		direct_init(t_direct *new, U_INT *i, U_INT j, char *str);
void		choose_build(t_comm *command);
void		mark_direct(t_input *input);

/*----get_path-получение пути---*/
char		*get_path(char *bin, t_input *input, U_INT i);

/*----dollar - обработка доллара---*/
char		*find_dollar(char *tmp, U_INT *i, t_input *input);
char		*once_more_find_dollar(char *mid, t_input *input, U_INT i);
char		*find_envp(char *mid, U_INT *i, t_input *input);
char		*find_question(char *mid, U_INT *i, t_input *input, char c);

/*----heredoc---*/
t_status	launch_heredoc(t_direct *copy, t_input *input, int fd);

/*----free_struct-очистка структур---*/
void		free_env(t_input *input);
void		free_all(t_input *input);
void		free_t_comm(t_input *input);
void		free_direct(t_input *input);
void		free_str_command(char *str_command, t_input *input, U_INT i);

/*----main----*/
int			main(int argc, char **argv, char **envp);

/*----envp - заполнение t_env *envp из argv[3]----*/
t_status	put_envp(char **envp, t_input *input);
void		make_env_array(t_input *input, char ***full_envp);
t_env		*create_new_list(char *str, t_input *input);
void		add_list_back(t_env **new, t_env **envp);

/*----finder- препарсинг, поиск спецсимвлов----*/
t_status	finder(char *str_command, t_input *input, U_INT i);

/*----one_build_in - запуск билд-ина, если у нас всего 1 команда----*/
int			reverse_redir(t_input *input);
void		reverse_redir2(t_input *input, int fd);
U_INT		launcher(t_input *input, t_comm	*command);

/*----parser - начало парсинга----*/
t_status	parser(char *str_command, t_input *input, U_INT i, U_INT j);
void		go_through_word(char *str, U_INT *i, t_input *input);

/*----pre_open - начало работы команд----*/
void		try_open(t_input *input);

/*----put_t_comm - заполнение пустой структуры t_comm----*/
t_status	create_empty_t_comm(char *str_command, t_input *input);

/*----redirect - поиск редиректов---*/
void		find_direct_in_comm(t_input *input, char *str, U_INT j, U_INT *i);
void		add_heredoc(t_input *input);

/*----signals - обработчики сигналов---*/
void		main_handler(int signal);
void		in_signal(t_input *input, U_INT i, t_comm *command, U_INT counter);
void		handler_child(int sig_num);
void		handler_pipex(int sig_num);

/*----shlvl - считаем уровень истории---*/
void		put_shlvl(t_input *input);

/*----star - обработка звезды---*/
void		find_star(t_comm *tmp, t_input *input);
char		**easy(char **old, t_input *input, U_INT k, t_env *star);
char		**realloc_words(char **old, t_input *input, U_INT size, \
			t_templ *templ);
t_status	check_template(t_input *input, char *str, t_templ *temple, \
			U_INT i);
void		add_list_in_templ(t_templ *temp, char *str, t_input *input, \
			U_INT i);
U_INT		check_tmp_in_dir(t_templ *templ, t_input *input);
t_templ		*create_template_list(char *str, t_input *input, U_INT num, \
			U_INT i);
char		*change_star(t_comm **tmp, t_input *input, U_INT *k);
void		find_star_in_comm(t_input *input);
t_status	n_star(char *str);
void		free_temple(t_templ *temple);
char		**big_circle(t_templ *temple, t_input *input, U_INT size_good_temp);

/*----quotes - работа с кавычками---*/
t_status	go_to_end_quote(char *str, U_INT *i, char c, t_input *input);
void		clean_command(t_input *input);
void		clean_direct(t_input *input);

/*----utils_cut - полезные функции для обзрезки строк---*/
char		*ft_strjoin(char *s1, char *s2, t_input *input);
char		*ft_strjoin_for_3(char *s1, char *s2, char *s3, t_input *input);
char		*modif_strdup(char *src, t_input *input);
char		*modif_substr(char *s, U_INT start, U_INT len, t_input *input);
/*----utils_print - полезные функции для печати---*/
void		print_error(t_input *input, U_INT error_num, char *msg, \
			char *descrip);
t_status	print_token(t_input *input, char *str);

/*----utils_other - полезные функции для строк---*/
U_INT		ft_strlen(char *s);
char		*modif_itoa(int n, t_input *input);
t_status	ft_strcmp(char *s1, char *s2);
long long	modif_atoi(char *s, int *flag, int i);
int			ft_isalpha(int c);

/*----utils_other2 - ПРОДОЛЖЕНИЕ: полезные функции для строк---*/
int			ft_strncmp(const char *str1, const char *str2, size_t n);
int			modif_strncmp(char *str1, char *str2, int n);
int			words_number(char const *str, char c);
char		*next_word(char const *str, char c);
char		**ft_split(char const *s, char c);

/*----utils_other3 - ПРОДОЛЖЕНИЕ: полезные функции для строк---*/
char		*ft_strchr(char *str, char c);
void		iter_str(char *str, U_INT *i);
void		my_gnl(char **new_line, t_input *input);

/*----utils_list - ПРОДОЛЖЕНИЕ: полезные функции для строк---*/
void		ft_lstadd_middle(t_env *new, t_env *tmp);
void		ft_lstadd_back(t_env **lst, t_env *new);
void		ft_lstadd_front(t_env **lst, t_env *new);
U_INT		ft_lstsize(t_env *lst);
void		lstadd_back(t_templ **lst, t_templ *new);

/*----pipes ---*/
void		make_fork(t_input *input, t_comm *command, U_INT i);
void		open_pipes(t_input *input);
void		clean_path(char **path, U_INT i);
void		close_fd(t_input *input, int *fd_file, U_INT counter, U_INT i);

char		*path_home(t_input *input, t_comm *command);

//mac:     gcc file.c -L/Users/$USER/.brew/Cellar/readline/8.1.1/lib/ -I/Users/$USER/.brew/Cellar/readline/8.1.1/include -lreadline -o filename
//linux:   gcc -L/usr/local/lib -I/usr/local/include *.c -lreadline

#endif

/* В обработку ошибок: 
bash-3.2$ ytjy >>> gh
bash: syntax error near unexpected token `>'
bash-3.2$ ytjy <<< gh
bash: ytjy: command not found
bash-3.2$ ytjy ||| gh
bash: syntax error near unexpected token `|'

Проверить:
- токены в кавычках
- как выдает ls * | wc
- как выдает ls i* * *i
*/