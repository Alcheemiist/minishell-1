#include "minishell.h"

int			    ft_prompt(char **line)
{
	char	cwd[1024];

	ft_bzero(cwd, 1000);
	getcwd(cwd, 1000);
	ft_putstr_fd("\033[0;32m", STDOUT);
	if (g_signal_c == 1)
	{
		ft_putchar_fd('\r', STDOUT);
		g_signal_c = 0;
	}
	if (g_signal_d == 1)
	{
		ft_putstr_fd(cwd, STDOUT);
		ft_putstr_fd(" $ ", STDOUT);
	}
	ft_putstr_fd("\033[0m", STDOUT);
	return (get_next_line(STDIN, line));
}

void		    init_shell(char **env)
{
	init_environment(env);
	init_builtins();
	g_status = 0;
	g_signal_c = 0;
	g_signal_d = 1;
	signal(SIGINT, signal_handler);
	signal(SIGQUIT, signal_handler);
}

bool		    handle_return(int rt, char *line)
{
	if (rt == 0)
	{
		if (ft_strlen(line))
		{
			free(line);
			line = NULL;
			g_signal_d = 0;
			return (true);
		}
		else
		{
			ft_putchar_fd('\n', STDOUT);
			exit(1);
		}
	}
	g_signal_d = 1;
    return (false);
}

t_pipeline		**parse_data(char *line)
{
	char		*checked_line;
	t_pipeline	**parsed_line;

	checked_line = NULL;
	if (!(checked_line = check_line(line)))
	{
		free(line);
		return (NULL);
	}
	parsed_line = parser(checked_line);
	print_parsed_line(parsed_line);
	return (parsed_line);
}

void		    exec_parsed(t_pipeline **parsed_line)
{
	int i;

	i = 0;
	while (parsed_line[i] != NULL)
	{
		g_status = execute_pipeline(parsed_line[i]);
		i++;
	}
}