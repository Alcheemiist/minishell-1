/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_line_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malaoui <malaoui@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/28 12:14:38 by malaoui           #+#    #+#             */
/*   Updated: 2020/12/28 12:16:29 by malaoui          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char		*ft_get_value(char *line, int *pos)
{
	int		i;
	char	*s;
	char	*key;
	t_env	*env;

	i = 0;
	s = NULL;
	key = NULL;
	while (line[i] != '\0' && ft_isalnum(line[i]))
		i++;
	key = ft_substr(line, 0, i);
	env = env_with_key(key);
	if (env != NULL)
		s = ft_strdup(env->value);
	else
		s = ft_strdup("");
	if (key)
		free(key);
	*pos += i;
	return (s);
}

char		*dollar_norm(char *s, int *i, int value)
{
	char	*tmp;
	char	*p;

	tmp = NULL;
	p = NULL;
	tmp = ft_strdup(s);
	if (s)
		free(s);
	p = ft_itoa(value);
	s = ft_strjoin(tmp, p);
	if (tmp)
		free(tmp);
	if (p)
		free(p);
	*i += 2;
	return (s);
}

char		*dollar_norm_dollar(char *s, int *i, char *line)
{
	char	*tmp;
	char	*p;
	int		env;

	env = 0;
	p = NULL;
	tmp = NULL;
	*i += 1;
	tmp = ft_strdup(s);
	if (s)
		free(s);
	if (ft_isalnum(line[*i]))
		env = 1;
	p = ft_get_value(line + *i, i);
	s = ft_strjoin(tmp, ((ft_strlen(p) == 0 && env == 0) ? "$" : p));
	if (tmp)
		free(tmp);
	if (p)
		free(p);
	return (s);
}

char		*handle_env(char *line, int *pos)
{
	char	*s;
	int		i;

	s = NULL;
	i = 0;
	while (line[i] == '$' && line[i + 1] == '$')
		s = dollar_norm(s, &i, g_child);
	if (line[i] == '$' && line[i + 1] == '?')
		s = dollar_norm(s, &i, g_status);
	else if (line[i] == '$')
		s = dollar_norm_dollar(s, &i, line);
	*pos += i;
	return (s);
}