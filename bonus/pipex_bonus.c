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

#include "pipex_bonus.h"

void	execute(t_var *p, int fd_in, int fd_out)
{
	// printf("%s\n", p->cmd_args[p->i][0]);
	// printf("in:%d out:%d\n", fd_in, fd_out);
	if (dup2(fd_in, STDIN_FILENO) < 0)
		ft_error(errno, ft_strdup("dup"), p, 1);
	if (dup2(fd_out, STDOUT_FILENO) < 0)
		ft_error(errno, ft_strdup("dup"), p, 1);
	close_fds(p);
	if (execve(p->exec_cmd_path[p->i], p->cmd_args[p->i], NULL) < 0)
		ft_error(errno, ft_strdup("execve"), p, 1);
}

void	pipex(t_var *p)
{
	int	valid_cmds_start;
	int	orig_v_c_s;

	valid_cmds_start = p->cmd_ct - 1;
	p->i = 0;
	while (valid_cmds_start > -1 
		&& ft_strncmp(p->exec_cmd_path[valid_cmds_start], "invalid", 7))
		valid_cmds_start--;
	valid_cmds_start++;
	if (valid_cmds_start != p->cmd_ct)
	{
		orig_v_c_s = valid_cmds_start;
		if (orig_v_c_s == p->cmd_ct - 1)
			execute(p, p->empty_fd, p->outfile);
		else
		{
			p->i = -1;
			while (++valid_cmds_start != p->cmd_ct)
				if (pipe(p->fd[++p->i]) < 0)
					ft_error(errno, ft_strdup("pipe"), p, 1);
			p->i = -1;
			orig_v_c_s--;
			while (++orig_v_c_s != p->cmd_ct)
			{
				p->pid[++p->i] = fork();
				if (p->pid[p->i] < 0)
					ft_error(errno, ft_strdup("fork"), p, 1);
			// printf("%d\n", p->i);
			// printf("%s\n", p->exec_cmd_path[p->i]);
				if (!p->pid[p->i])
				{
					if (!p->i && !orig_v_c_s)
						execute(p, p->infile, p->fd[p->i][1]);
					else if (!p->i)
						execute(p, p->empty_fd, p->fd[p->i][1]);
					else if (orig_v_c_s < p->cmd_ct - 1)
						execute(p, p->fd[p->i - 1][0], p->fd[p->i][1]);
					else
						execute(p, p->fd[p->i - 1][0], p->outfile);
				}
				waitpid(p->pid[p->i], NULL, WNOHANG);
			}
		}
		close_fds(p);
	}
}
