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

// char *handle_odd_quotes(char *arg, int quote_count, char quote_type)
// {
//     char *new_arg;
//     new_arg = arg;
//     char extra_input[256];

//     while (quote_count % 2 != 0)
//     {
//         extra_input[0] = '\0';
//         ft_printf("quote> \n");
//         read(0, extra_input, 256);
//         extra_input[strcspn(extra_input, "\n")] = 0;
//         quote_count += count_quotes(extra_input, quote_type); 

//         char *temp_arg = malloc(ft_strlen(new_arg) + ft_strlen(extra_input) + 1);
//         strcpy(temp_arg, new_arg);
//         strcat(temp_arg, extra_input);
//         if (new_arg != arg)
//             free(new_arg);
//         new_arg = temp_arg;
//     }

//     return new_arg;
// }

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
	// int idx = -1;
	// while(ps[++idx])
	// 	ft_printf("Str: %s\n", ps[idx]);

	if (q)
		*q = s;

	ret = *s;

	// if (*s == '\'' || *s == '\"')
	// {
	// 	char quote = *s;
	// 	s++;
	// 	while (s < es && *s != quote)
	// 		s++;
	// 	if (s < es)
	// 	{
	// 		*s = '\0';   // Replace closing quote with null terminator
	// 		ret = quote; // Return the quote type as the token
	// 	}
	// 	else
	// 	{
	// 		ft_printf("Missing closing quote\n");
	// 		exit(-1);
	// 	}
	// }
	// else
	 if (*s == 0)
		(void)s;
	else if (ft_strncmp(s, ">>", 2) == 0)
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
