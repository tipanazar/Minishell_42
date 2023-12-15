#ifndef MINISHELL_H
# define MINISHELL_H

# include "./libft/libft.h"
# include <dirent.h>
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

# define INITIAL_MAXARGS 10

struct				s_cmd
{
	int				type;
};

struct				s_execcmd
{
	int				type;
	char			**argv;
	int				argc;
	int				max_args;
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

int					runcmd(struct s_cmd *cmd, char **env);
int					fork1(void);
int					getcmd(char *buf, int nbuf);
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
void				export(char *buf, char ***custom_environ);
char				*custom_getenv(char *name, char **custom_environ,
						bool full_str);
void				ft_cd(char *buf, char **custom_environ);
void				unset(char *buf, char ***custom_environ);
char				*find_command_in_path(char *command);
void				free_cmd(struct s_cmd *command);
int					redirect_cmd(struct s_redircmd *rcmd,
						char **custom_environ);
void				pipe_command(struct s_pipecmd *pcmd, char **env);
char				*read_and_trim_line(void);
void				ctrl_c_handler(int sig);
bool				is_blank(const char *buf);
void 				echo(char *buf, char **custom_environ);
char				*export_validator(char *buf);
char 				**create_unset_arr(char *buf, char **custom_environ);
void				export(char *buf, char ***custom_environ);

#endif
