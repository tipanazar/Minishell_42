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

// typedef struct s_cmd
// {
// 	int type;
// } t_cmd;

struct				s_execcmd
{
	int				type;
	char **argv;  // Dynamically allocated array of arguments
	int argc;     // Current count of arguments
	int max_args; // Current capacity of the argv array
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
int					ft_fileno(FILE *stream);
char 				*concat_args(char **args);
int					builtins(char *buf, char **env);
void				env(char **env);
void 				export(char *buf, char ***custom_environ);
char 				*custom_getenv(char *name, char **custom_environ);
void 				ft_cd(char *buf, char **custom_environ);
void 				unset(char *buf, char ***custom_environ);

#endif
