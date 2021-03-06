/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   norm_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malaoui <malaoui@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/28 12:16:09 by malaoui           #+#    #+#             */
/*   Updated: 2021/01/21 10:13:05 by malaoui          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char		*get_norm_outside(int *i, char *m, char *s, int *start)
{
	char	*tmp;
	char	*rt;

	*i += 1;
	tmp = NULL;
	rt = ft_strdup("");
	tmp = outside_quotes(s + *i, i);
	rt = ft_strdup(m);
	if (m)
		free(m);
	if (tmp == NULL)
		tmp = ft_strdup("");
	m = ft_strjoin(rt, tmp);
	free(tmp);
	free(rt);
	*start += *i;
	return (m);
}

char		*get_norm_inside(int *i, char *s, char *rt)
{
	char	*tmp;
	char	*t;

	t = NULL;
	tmp = NULL;
	*i += 1;
	tmp = ft_strdup(rt);
	if (rt != NULL)
		free(rt);
	t = ft_new_inside(s + *i, i, s[*i - 1]);
	if (t == NULL)
		t = ft_strdup("");
	rt = ft_strjoin(tmp, t);
	if (t)
		free(t);
	if (tmp)
		free(tmp);
	return (rt);
}

char		*outside_quotes_escape_helper(char *s, char *rt, int *i)
{
	char	*tmp;

	tmp = NULL;
	if (s[*i] == '\\' && s[*i + 1] == '\0')
	{
		*i += 1;
		tmp = ft_strdup(rt);
		if (rt)
			free(rt);
		rt = ft_strjoin(tmp, " ");
		if (tmp)
			free(tmp);
	}
	else if (s[*i] == '\\')
	{
		*i += 1;
		rt = append(rt, s[*i]);
	}
	else
		rt = append(rt, s[*i]);
	return (rt);
}

char		*outside_quotes(char *s, int *start)
{
	int		i;
	char	*rt;
	int		end;
	char	*tmp;

	i = 0;
	rt = NULL;
	end = 0;
	tmp = NULL;
	while (i < (int)ft_strlen(s) && s[i])
	{
		if (is_q(s[i]) && have_end(s + i + 1, s[i], &end) && !escape(s, i - 1))
			rt = get_norm_inside(&i, s, rt);
		else if (!ft_strchr(" <>\t", s[i]))
			rt = outside_quotes_escape_helper(s, rt, &i);
		if (i < (int)ft_strlen(s) &&
		ft_strchr(" <>\t\r\f", s[i]) && !escape(s, i - 1))
			break ;
		else
			i++;
	}
	*start += i;
	if (i >= (int)ft_strlen(s) || s[i] == '\0')
		*start += i - 1;
	return (rt);
}
