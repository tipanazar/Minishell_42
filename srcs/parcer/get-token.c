#include "../../minishell.h"

char *move_past_whitespace(char *s, char *es, const char *whitespace)
{
	while (s < es && ft_strchr(whitespace, *s))
		s++;
	return (s);
}

char get_symbol_token(char **s)
{
	char ret;

	ret = **s;
	(*s)++;
	return (ret);
}

char get_alphanumeric_token(char **s, char *es, const char *whitespace,
							const char *symbols)
{
	char ret;

	ret = 'a';
	while (*s < es && !ft_strchr(whitespace, **s) && !ft_strchr(symbols, **s))
		(*s)++;
	return (ret);
}

int get_token(char **ps, char *es, char **q, char **eq)
{
	char *s;
	int ret;
	const char *whitespace;
	const char *symbols;

	s = *ps;
	whitespace = " \t\r\n\v";
	symbols = "<|>";
	s = move_past_whitespace(s, es, whitespace);

	if (q)
		*q = s;
	ret = *s;

	if (*s == '\'' || *s == '\"')
	{
		char quote = *s;
		s++;
		while (s < es && *s != quote)
			s++;
		if (s < es)
		{
			*s = '\0';   // Replace closing quote with null terminator
			ret = quote; // Return the quote type as the token
		}
		else
		{
			ft_printf("Missing closing quote\n");
			exit(-1);
		}
	}
	else
	 if (*s == 0)
		(void)s;
	else 
	if (ft_strncmp(s, ">>", 2) == 0)
	{
		ret = '+';
		s += 2;
	}
	else if (ft_strncmp(s, "<<", 2) == 0)
	{
		ret = '-';
		s += 2;
	}
	else if (*s == '|' || *s == '<' || *s == '>')
		ret = get_symbol_token(&s);
	else
		ret = get_alphanumeric_token(&s, es, whitespace, symbols);

	if (eq)
		*eq = s;

	s = move_past_whitespace(s, es, whitespace);
	*ps = s;

	return ret;
}
