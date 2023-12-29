#ifndef MINISHELL_H
# define MINISHELL_H

# include "./libft/libft.h"
# include <dirent.h>
# include <errno.h>
# include <fcntl.h>
# include <limits.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/stat.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <termcap.h>
# include <unistd.h>

extern int			g_exit_code;

typedef struct s_parseexec
{
	char			*q;
	char			*eq;
	int				tok;
}					t_parseexec;

struct				s_cmd
{
	int				type;
	int				flag;
};

typedef struct s_export
{
	bool			has_equal_sign;
	char			inside_quotes;
	char			*quote_type;
}					t_ValidationArgs;

struct				s_execcmd
{
	int				type;
	char			**argv;
	int				argc;
	int				max_args;
};

struct				s_echo_args
{
	char			*buf;
	char			**custom_environ;
	int				*idx;
	int				*inside_sing_quotes;
	int				newline;
};

struct				s_redircmd
{
	int				type;
	struct s_cmd	*cmd;
	char			*file;
	int				mode;
	int				fd;
};

struct				s_pipecmd
{
	int				type;
	struct s_cmd	*left;
	struct s_cmd	*right;
};

char				*check_for_quotes(char *buffer);
int					runcmd(struct s_cmd *cmd, char **env);
int					fork1(void);
int					exec_cmd(struct s_cmd *cmd, char **custom_environ);
int					get_token(char **ps, char *es, char **q, char **eq);
int					peek(char **ps, char *es, char *toks);
char				*mkcopy(char *s, char *es);
struct s_cmd		*execcmd(void);
struct s_cmd		*parsecmd(char *s);
struct s_cmd		*redircmd(struct s_cmd *subcmd, char *file, int type);
struct s_cmd		*pipecmd(struct s_cmd *left, struct s_cmd *right);
struct s_cmd		*parseredirs(struct s_cmd *cmd, char **ps, char *es);
struct s_cmd		*parsepipe(char **ps, char *es);
struct s_cmd		*parseexec(char **ps, char *es);
char				*concat_args(char **args);
int					builtins(char *buf, char **env);
void				env(char **env);
char				*custom_getenv(char *name, char **custom_environ,
						bool full_str);
void				ft_cd(char *buf, char **custom_environ);
void				unset(char *buf, char ***custom_environ);
char				*find_command_in_path(char *command, char **custom_environ);
void				free_cmd(struct s_cmd *command);
void				redirect_cmd(struct s_redircmd *rcmd,
					char **custom_environ);
void				pipe_command(struct s_pipecmd *pcmd, char **env);
char				*read_and_trim_line(char *buf);
void				ctrl_c_handler(int sig);
bool				is_blank(const char *buf);
void				echo(char *buf, char **custom_environ);
void				echo_n_handler(char *buf, int *idx, int *newline);
int					process_quotes(char *buf, int *idx,
						int *inside_sing_quotes);
void				handle_variable_expansion(char *buf,
					char **custom_environ, int *idx);
void				process_variables(char *buf, char **custom_environ,
						int *idx, int inside_sing_quotes);
void				process_echo_command(struct s_echo_args *args);
char				*export_validator(char *buf);
char				**create_unset_arr(char *buf, char **custom_environ);
bool				handle_space_and_equal(char *buf, int idx,
						bool *has_equal_sign, char inside_quotes);
bool				check_quote_status(char *buf, int idx, char inside_quotes,
						char *quote_type);
char				*validate_buffer(char *buf, t_ValidationArgs *args);
int					update_env_var(char ***custom_environ, char *new_buf,
						int idx);
void				export(char *buf, char ***custom_environ);
void				add_new_env_var(char ***custom_environ, char *new_buf,
						int idx);
void				pwd(void);
char				*check_for_pipes(char *buffer);
bool				handle_command(char *new_buf, char ***custom_env);
int					builtin_exit(char *buf);

#endif
