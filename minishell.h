/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: root <root@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/20 18:40:49 by viktortr          #+#    #+#             */
/*   Updated: 2023/10/24 17:25:25 by root             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

#include <stdbool.h>
# include "./libft/libft.h"
# include <dirent.h>
# include <fcntl.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdio.h>
# include <stdlib.h>
/* # include <sys/ioctl.h> */
# include <sys/stat.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <termcap.h>
/* # include <termios.h> */
# include <unistd.h>

# define MAXARGS 10

/* 
All commands have at least a type. Have looked at the type, the code
typically casts the *cmd to some specific cmd type.
*/
struct		cmd
{
	int	type; /* ' ' (exec), | (pipe), '<' or '>' for redirection */
};

struct		execcmd
{
	int type;            /* ' ' */
	char *argv[MAXARGS]; /* arguments to the command to be exec-ed */
};

struct		redircmd
{
	int type;        /* < or > */
	struct cmd *cmd; /* the command to be run (e.g., an execcmd) */
	char *file;      /* the input/output file */
	int mode;        /* the mode to open the file with */
	int fd;          /* the file descriptor number to use for the file */
};

struct		pipecmd
{
	int type;          /* | */
	struct cmd *left;  /* left side of pipe */
	struct cmd *right; /* right side of pipe */
};

int			runcmd(struct cmd *cmd);
int	fork1(void); /* Fork but exits on failure. */
int			getcmd(char *buf, int nbuf);
int			gettoken(char **ps, char *es, char **q, char **eq);
int			peek(char **ps, char *es, char *toks);
char		*mkcopy(char *s, char *es);
struct cmd	*execcmd(void);
struct cmd	*parsecmd(char *s);
struct cmd	*redircmd(struct cmd *subcmd, char *file, int type);
struct cmd	*pipecmd(struct cmd *left, struct cmd *right);
struct cmd	*parseredirs(struct cmd *cmd, char **ps, char *es);
struct cmd	*parseline(char **ps, char *es);
struct cmd	*parsepipe(char **ps, char *es);
struct cmd	*parseexec(char **ps, char *es);
int			ft_fileno(FILE *stream);
int		builtins(char *buf);

#endif
