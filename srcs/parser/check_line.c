/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_line.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malaoui <malaoui@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/27 15:57:12 by malaoui           #+#    #+#             */
/*   Updated: 2021/01/21 10:12:59 by malaoui          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char		*get_inside(char *s, int *start, char quote)
{
	char	*t;
	int		i;

	i = 1;
	t = ft_strdup("");
	while (s[i] != '\0' && s[i] != quote)
		t = append(t, s[i++]);
	if (s[i] == '\0')
		i--;
	if (s[i] == quote)
		i++;
	*start += i;
	return (t);
}

t_exp		inp(char *s)
{
	int		i;
	t_exp	in;
	int		end;

	i = 0;
	end = 0;
	in.sgl = 0;
	in.dbl = 0;
	while (s[i] != '\0')
	{
		while ((size_t)(i + 1) < ft_strlen(s) && is_q(s[i]))
		{
			end = i;
			if (have_end(s + i, s[i], &end) && s[i] == '\'')
				in.sgl += 1;
			if (have_end(s + i, s[i], &end) && s[i] == '\"')
				in.dbl += 1;
			i++;
		}
		if (s[i] == '$')
			return (in);
		if (s[i] != '\0')
			i++;
	}
	return (in);
}

char		*env_normed(char *s, char *line, int *i)
{
	char	*tmp;
	char	*t;

	t = NULL;
	tmp = ft_strdup(s);
	if (s)
		free(s);
	t = handle_env(line + *i, i);
	s = ft_strjoin(tmp, t);
	free(tmp);
	if (t)
		free(t);
	return (s);
}

char		*ft_env(char *line)
{
	int		i;
	char	*s;
	t_exp	in;

	i = 0;
	s = NULL;
	ft_bzero(&in, sizeof(t_exp));
	while (i < (int)ft_strlen(line) && line[i] != '\0')
	{
		while ((is_q(line[i]) && i == 0)
			|| (i > 0 && line[i - 1] != '\\' && (is_q(line[i]))))
		{
			in.dbl += (line[i] == '\"' ? 1 : 0);
			in.sgl += (line[i] == '\'' ? 1 : 0);
			s = append(s, line[i++]);
		}
		i += (line[i] == '\\' && line[i + 1] == '$' && in.sgl == 0 ? 1 : 0);
		if ((line[i] == '$' && i == 0) || (i > 1 && line[i - 1] != '\\'
		&& line[i] == '$' && (line[i + 1] != '\0' && line[i + 1] != '\\')
		&& in.sgl % 2 != 1))
			s = env_normed(s, line, &i);
		else if (i < (int)ft_strlen(line) && line[i] != '\0')
			s = append(s, line[i++]);
	}
	return (s);
}

char		*check_line(char *line)
{
	char	*rt;

	rt = NULL;
	if (line == NULL)
		return (NULL);
	if (!(check_syntax(line)))
	{
		g_status = 2;
		return (0);
	}
	return (line);
}
