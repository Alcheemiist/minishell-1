/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins1.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malaoui <malaoui@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/27 15:02:51 by youkhart          #+#    #+#             */
/*   Updated: 2021/01/15 12:52:10 by malaoui          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	init_builtins(void)
{
	int		i;
	char	**tmp;

	i = 0;
	if (!(tmp = ft_split("echo,cd,pwd,export,unset,env,exit", ',')))
		return ;
	while (i < 7)
	{
		g_builtins_str[i] = ft_strdup(tmp[i]);
		free(tmp[i++]);
	}
	free(tmp);
	g_builtins_str[i] = NULL;
	g_builtins[0] = &builtin_echo;
	g_builtins[1] = &builtin_cd;
	g_builtins[2] = &builtin_pwd;
	g_builtins[3] = &builtin_export;
	g_builtins[4] = &builtin_unset;
	g_builtins[5] = &builtin_env;
	g_builtins[6] = &builtin_exit;
}

int		builtin_echo(int in, int out, char **argv)
{
	int endl;

	endl = 1;
	redirect_in_out(in, out);
	if (tab_len(argv) == 1)
		write(1, "\n", 1);
	else
	{
		argv++;
		while (*argv && ft_strlen(*argv) > 0 && checker(*argv))
		{
			argv++;
			endl = 0;
		}
		while (*argv)
		{
			write(1, *argv, ft_strlen(*argv));
			if (*(argv + 1))
				write(1, " ", 1);
			argv++;
		}
		write(1, "\n", endl);
	}
	return (0);
}

int		builtin_pwd(int in, int out, char **argv)
{
	char buf[1024];

	(void)argv;
	redirect_in_out(in, out);
	ft_bzero(buf, 1024);
	getcwd(buf, 1024);
	write(1, buf, ft_strlen(buf));
	write(1, "\n", 1);
	return (0);
}

int		builtin_cd(int in, int out, char **argv)
{
	int		ret;
	char	*dir;
	char	cwd[1024];
	t_env	*tmp;

	(void)in;
	(void)out;
	if (tab_len(argv) == 1 && !(tmp = env_with_key("HOME")))
	{
		ft_printf("minishell: cd: HOME not set\n");
		return (1);
	}
	else
		dir = tab_len(argv) == 1 ? tmp->value : argv[1];
	dir = ft_strlen(dir) ? dir : ".";
	if ((ret = chdir(dir)) < 0)
	{
		ft_printf("minishell: cd: %s: %s\n", dir, strerror(errno));
		return (1);
	}
	set_env("OLDPWD",
	((tmp = env_with_key("PWD")) ? tmp->value : ft_strdup("")));
	get_cwd(cwd, 1000);
	set_env("PWD", cwd);
	return (0);
}

int		builtin_exit(int in, int out, char **argv)
{
	char	*tmp;

	(void)in;
	(void)out;
	if (tab_len(argv) == 2 && (tmp = skip_blank(argv[1])) && ft_isdigit(tmp[0]))
		g_return = ft_atoi(skip_blank(argv[1]));
	else if (tab_len(argv) == 2 && !isallnum(skip_blank(argv[1])))
	{
		g_return = 255;
		ft_printf("minishell: exit: %s: numeric argument required\n", argv[1]);
	}
	else if (tab_len(argv) == 2)
		g_return = (is_number(skip_blank(argv[1])) ?
		ft_atoi(skip_blank(argv[1])) : 0);
	else if (tab_len(argv) > 2)
		g_return = 1;
	g_return %= 256;
	return (-1);
}
