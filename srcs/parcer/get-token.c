#include "../../minishell.h"

char	*move_past_whitespace(char *s, char *es, const char *whitespace)
{
	while (s < es && ft_strchr(whitespace, *s))
		s++;
	return (s);
}

void	skip_non_special_tokens(char **s, char *es)
{
	while (*s < es && !ft_strchr(" \t\r\n\v", **s) && !ft_strchr("<|>", **s))
	{
		if (**s == '\"')
		{
			(*s)++;
			while (*s < es && **s != '\"')
				(*s)++;
		}
		else if (**s == '\'')
		{
			(*s)++;
			while (*s < es && **s != '\'')
				(*s)++;
		}
		if (*s < es)
			(*s)++;
	}
}

void	process_special_tokens(char **s, int *token)
{
	if (*token == '|')
		(*s)++;
	else if (*token == ';')
		(*s)++;
	else if (*token == '>')
	{
		(*s)++;
		if (**s == '>')
		{
			*token = '+';
			(*s)++;
		}
	}
	else if (*token == '<')
	{
		(*s)++;
		if (**s == '<')
		{
			*token = '-';
			(*s)++;
		}
	}
	else if (*token != '\0')
		*token = 'a';
}

int	get_token(char **ps, char *es, char **q, char **eq)
{
	char		*s;
	const char	*whitespace;
	int			ret;

	s = *ps;
	whitespace = " \t\r\n\v";
	s = move_past_whitespace(s, es, whitespace);
	if (q)
		*q = s;
	ret = *s;
	process_special_tokens(&s, &ret);
	if (ret == 'a')
		skip_non_special_tokens(&s, es);
	if (eq)
		*eq = s;
	*ps = move_past_whitespace(s, es, whitespace);
	return (ret);
}
