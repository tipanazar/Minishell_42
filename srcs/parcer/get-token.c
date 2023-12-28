#include "../../minishell.h"

char	*move_past_whitespace(char *s, char *es, const char *whitespace)
{
	while (s < es && ft_strchr(whitespace, *s))
		s++;
	return (s);
}

int	get_alphanumeric_token(char **s, char *es, const char *whitespace,
		const char *symbols)
{
	while (*s < es && !ft_strchr(whitespace, **s) && !ft_strchr(symbols, **s))
		(*s)++;
	if (*s < es)
		**s = '\0';
	return ('a');
}

int	process_symbol_or_alpha(char **s, char *es, const char *whitespace,
		const char *symbols)
{
	int	ret;

	if (ft_strncmp(*s, ">>", 2) == 0 || ft_strncmp(*s, "<<", 2) == 0)
	{
		if (**s == '>')
			ret = '+';
		else
			ret = '-';
		*s += 2;
	}
	else if (**s == '|' || **s == '<' || **s == '>')
	{
		ret = **s;
		(*s)++;
	}
	else
		ret = get_alphanumeric_token(s, es, whitespace, symbols);
	return (ret);
}

int	get_token(char **ps, char *es, char **q, char **eq)
{
	char		*s;
	const char	*whitespace;
	const char	*symbols;
	int			ret;

	s = *ps;
	whitespace = " \t\r\n\v";
	symbols = "<|>";
	s = move_past_whitespace(s, es, whitespace);
	if (q)
		*q = s;
	ret = *s;
	if (*s == 0)
		(void)s;
	else
		ret = process_symbol_or_alpha(&s, es, whitespace, symbols);
	if (eq)
		*eq = s;
	*ps = move_past_whitespace(s, es, whitespace);
	return (ret);
}
