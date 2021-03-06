/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malaoui <malaoui@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/27 15:49:37 by youkhart          #+#    #+#             */
/*   Updated: 2021/01/21 10:12:59 by malaoui          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*jump_redirection_sign(char *out)
{
	char	*s;
	int		i;
	char	quote;

	i = 0;
	s = NULL;
	while (*out == '>' || *out == '<' || is_blank(*out))
		out++;
	if (is_q(*out))
	{
		quote = *out;
		out++;
		s = ft_new_inside(out, &i, quote);
	}
	else
		s = ft_strdup(out);
	return (s);
}

int		redirect_inputs(char **tokens, int out, int pipe_in, char **input_files)
{
	int		i;
	int		fd;
	char	*parsed;

	i = 0;
	if (!(tab_len(input_files)))
		g_status = execute_command(pipe_in, out, tokens);
	while (i < tab_len(input_files))
	{
		parsed = jump_redirection_sign(input_files[i]);
		fd = open(parsed, O_RDONLY);
		if (fd < 0)
		{
			ft_printf("minishell: %s: No such file or directory\n", parsed);
			free(parsed);
			return (1);
		}
		if (i == tab_len(input_files) - 1)
			g_status = execute_command(fd, out, tokens);
		free(parsed);
		i++;
	}
	return (g_status);
}

int		truncate_file(char *out)
{
	int		i;

	i = 0;
	while (out[i] == '>')
		i++;
	return (i == 2 ?
		O_APPEND | O_CREAT | O_WRONLY : O_TRUNC | O_CREAT | O_WRONLY);
}

int		redirect_outputs(t_command *cmd, int pipe_in, int pipe_out)
{
	int		i;
	int		fd;
	char	*parsed;

	i = 0;
	while (cmd && i < tab_len(cmd->output_files))
	{
		parsed = jump_redirection_sign(cmd->output_files[i]);
		if ((fd = open(parsed, truncate_file(cmd->output_files[i]), 0644)) < 0)
		{
			i++;
			free(parsed);
			continue;
		}
		free(parsed);
		if (i == tab_len(cmd->output_files) - 1)
			g_status = redirect_inputs(cmd->tokens, fd,
				pipe_in, cmd->input_files);
		close(fd);
		i++;
	}
	if (cmd && (!(tab_len(cmd->output_files))))
		g_status = redirect_inputs(cmd->tokens,
			pipe_out, pipe_in, cmd->input_files);
	return (g_status);
}
