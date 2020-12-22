#include "minishell.h"

int		cparser(char *s, char q)
{
	int i;
	int in;
	int c;

	i = 0;
	in = 0;
	c = 0;
	while (s[i] != '\0')
	{
		if (QUOTE(s[i]))
		{
			if (have_end(s + i + 1, s[i], &i))
				in = 1;
			else
				in = 0;
		}
		if (in == 0 && s[i] == q)
			c++;
		i++;
	}
	return (c);
}

char	*append(char *s, char c)
{
	char	*str;
	int		i;
	int		len;

	i = 0;
	len = ft_strlen(s);
	if (!(str = malloc(sizeof(char ) * (len + 2))))
		return (NULL);
	while (i < len)
	{
		str[i] = s[i];
		i++;
	}
	if (s)
		free(s);
	str[i] = c;
	str[i + 1] = '\0';
	return (str);
}

char	*get_arg(char *line, char c, int *pos)
{
	int 	i;
	int 	end;
	int 	in;
	char	*s;
	char 	*tmp;

	i = 0;
	end = 0;
	in = 0;
	s = NULL;
	tmp = NULL;
	while (line[i] != '\0')
	{
		if (in == 0 && QUOTE(line[i]))
		{
			end = i + 1;
			if (!(tmp = inside_quotes(line + i + 1, &end, line[i])))
			{
				in = 1;
				free(tmp);
			}
		}
		if (i == end)
			in = 0;
		if (line[i] == c && in == 0)
		{
			*pos += i;
			break ;
		}
		s = append(s, line[i]);
		i++;
	}
	if (line[i] == '\0')
        *pos += i - 1;
	return (s);
}

char	**parser_split(char *line, char c)
{
	int	 	i;
	int		nb_c;
	char 	**split;
	int		j;

	i = 0;
	j = 0;
	nb_c = cparser(line, c) + 1;
	if (!(split = (char **)malloc(sizeof(char *) * (nb_c + 1))))
		return (NULL);
	i = 0;
	while (i < nb_c)
	{
		split[i] = get_arg(line + j, c, &j);
		j++;
		i++;
	}
	split[i] = NULL;
	return (split);
}
 