/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chon <chon@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/10 11:26:17 by chon              #+#    #+#             */
/*   Updated: 2024/06/19 15:36:11 by chon             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

void	find_paths(t_var *p, int ac)
{
	int		i;
	int		j;
	char	*filepath;

	i = 0;
	j = -1;
	while (--ac - 2 > 0)
	{
		while (p->cmd_filepaths[++j])
		{
			filepath = ft_strjoin(p->cmd_filepaths[j], p->cmd_args[i][0]);
			if (access(filepath, X_OK) > -1)
			{
				p->execute_cmds[i] = ft_strdup(filepath);
				break ;
			}
			free(filepath);
		}
		if (!p->execute_cmds[i])
			ft_error(errno, "cmd not found", p);
		i++;
		j = -1;
		free(filepath);
	}
	p->execute_cmds[i] = NULL;
}

void	execute(t_var *p, int cmd_idx, int *fd)
{
	if (!cmd_idx)
	{
		if (dup2(p->in_fd, STDIN_FILENO) < 0 || dup2(fd[1], STDOUT_FILENO) < 0)
			ft_error(errno, "dup", p);
	}
	else
		if (dup2(fd[0], STDIN_FILENO) < 0 || dup2(p->out_fd, STDOUT_FILENO) < 0)
			ft_error(errno, "dup", p);
	close(fd[0]);
	close(fd[1]);
	if (execve(p->execute_cmds[cmd_idx], p->cmd_args[cmd_idx], NULL) < 0)
		ft_error(errno, "execve", p);
}

void	pipex(t_var *p)
{
	int	fd[2];
	int	pid1;
	int	pid2;

	if (pipe(fd) < 0)
		ft_error(errno, "pipe", p);
	pid1 = fork();
	if (pid1 < 0)
		ft_error(errno, "pid1", p);
	if (!pid1)
		execute(p, 0, fd);
	pid2 = fork();
	if (pid2 < 0)
		ft_error(errno, "pid2", p);
	if (!pid2)
		execute(p, 1, fd);
	close(fd[0]);
	close(fd[1]);
	waitpid(pid1, NULL, 0);
	waitpid(pid2, NULL, 0);
}

void	init(char **av, t_var *p, int ac)
{
	char	*paths;

	p->cmd_args = NULL;
	p->cmd_filepaths = NULL;
	p->execute_cmds = NULL;
	p->in_fd = open(av[1], O_RDONLY);
	p->out_fd = open(av[4], O_WRONLY | O_TRUNC | O_CREAT, 0644);
	if (p->in_fd == -1)
		ft_error(errno, av[1], p);
	if (p->out_fd == -1)
		ft_error(errno, av[4], p);
	paths = "/usr/local/bin/:/usr/bin/:/bin/:/usr/sbin/:/sbin/";
	p->cmd_filepaths = ft_split(paths, ':');
	if (!p->cmd_filepaths)
		ft_error(errno, "cmd_filepaths", p);
	p->cmd_args = malloc(sizeof(char **) * 3);
	if (!p->cmd_args)
		ft_error(errno, "malloc cmd_args", p);
	p->cmd_args[0] = ft_split(av[2], ' ');
	p->cmd_args[1] = ft_split(av[3], ' ');
	p->cmd_args[2] = NULL;
	p->execute_cmds = calloc((ac - 2), sizeof(char *));
	if (!p->cmd_args)
		ft_error(errno, "malloc cmd_args", p);
	find_paths(p, ac);
}

int	main(int ac, char **av)
{
	t_var	p;

	if (ac == 5)
	{
		init(av, &p, ac);
		pipex(&p);
		free_char_arr(p.cmd_filepaths, p.cmd_args);
		free_char_arr(p.execute_cmds, NULL);
	}
	else
		ft_printf("Input an infile, two cmd args, and an outfile\n");
	return (0);
}
