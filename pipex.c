/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chon <chon@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/08 09:17:25 by chon              #+#    #+#             */
/*   Updated: 2024/07/08 09:17:25 by chon             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	execute(t_var *p, char *infile, int fd_in, int fd_out)
{
	if ((!p->i && access(infile, R_OK) < 0)
		|| (p->i > 0 && !p->cmd_args[p->i - 1][0]))
	{
		if (dup2(p->empty_fd, STDIN_FILENO) < 0)
			ft_error(errno, ft_strdup("dup"), p, 1);
		close(p->empty_fd);
	}
	else
		if (dup2(fd_in, STDIN_FILENO) < 0)
			ft_error(errno, ft_strdup("dup"), p, 1);
	if (!(p->i == p->cmd_ct - 1 && !p->cmd_args[p->i][0]))
		if (dup2(fd_out, STDOUT_FILENO) < 0)
			ft_error(errno, ft_strdup("dup"), p, 1);
	close_fds(p);
	if (!access(infile, R_OK) || p->i)
		if (execve(p->exec_cmd_path[p->i], p->cmd_args[p->i], NULL) < 0)
			ft_error(errno, ft_strdup("execve"), p, 1);
}

void	pipex(t_var *p, char *infile)
{
	p->i = -1;
	while (++p->i < p->cmd_ct - 1)
		if (pipe(p->fd[p->i]) < 0)
			ft_error(errno, ft_strdup("pipe"), p, 1);
	p->i = -1;
	while (++p->i < p->cmd_ct)
	{
		p->pid[p->i] = fork();
		if (p->pid[p->i] < 0)
			ft_error(errno, ft_strdup("fork"), p, 1);
		if (!p->pid[p->i] && p->exec_cmd_path[p->i])
		{
			if (!p->i)
				execute(p, infile, p->infile, p->fd[p->i][1]);
			else if (p->i < p->cmd_ct - 1)
				execute(p, infile, p->fd[p->i - 1][0], p->fd[p->i][1]);
			else
				execute(p, infile, p->fd[p->i - 1][0], p->outfile);
		}
		waitpid(p->pid[p->i], NULL, WNOHANG);
	}
	close_fds(p);
}
