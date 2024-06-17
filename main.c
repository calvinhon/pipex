/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chon <chon@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/10 11:26:17 by chon              #+#    #+#             */
/*   Updated: 2024/06/17 16:49:05 by chon             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

char	*find_path(char **cmd_filepaths, char *cmd)
{
	int		i;
	char	*filepath;
	
	i = -1;
	while (cmd_filepaths[++i])
	{
		filepath = ft_strjoin(cmd_filepaths[i], cmd);
		if (access(filepath, X_OK) > -1)
			return (filepath);
		free(filepath);
	}
	return (NULL);
}

void	execute(t_var *p, int cmd_idx, int *fd)
{
	if (!cmd_idx)
	{
		if (dup2(fd[1], STDOUT_FILENO) < 0)
			ft_error(errno, "dup", p);
	}
	else
		if (dup2(fd[0], STDIN_FILENO) < 0)
			ft_error(errno, "dup", p);
	close(fd[0]);
	close(fd[1]);
	execve(p->execute_cmds[cmd_idx], p->cmd_args[cmd_idx], NULL);
}

void	pipex(t_var *p)
{
	int		fd[2];
	int		pid1;

	if (pipe(fd) < 0)
		ft_error(errno, "pipe", p);
	pid1 = fork();
	if (pid1 < 0)
		ft_error(errno, "pid1", p);
	if (!pid1)
		execute(p, 0, fd);
	waitpid(pid1, NULL, 0);
	execute(p, 1, fd);
}

void	init(char **av, t_var *p, int ac)
{
	char	*path1;
	char	*path2;
	int		i;

	p->cmd_args = NULL;
	p->cmd_filepaths = NULL;
	p->in_fd = open(av[1], O_RDONLY);
	p->out_fd = open(av[4], O_WRONLY | O_CREAT, 0644);
	if (p->in_fd == -1)
		ft_error(errno, av[1], p);
	if (p->out_fd == -1)
		ft_error(errno, av[4], p);
	path1 = "/Users/chon/.docker/bin/:/usr/local/bin/:/usr/bin/:/bin/:/usr/sbin/:";
	path2 = "/sbin/:/usr/local/munki/:/Users/chon/.docker/bin/:";
	p->cmd_filepaths = ft_split(ft_strjoin(path1, path2), ':');
	if (!p->cmd_filepaths)
		ft_error(errno, "cmd_filepaths", p);
	p->cmd_args = malloc(sizeof(char **) * 3);
	p->cmd_args[0] = ft_split(av[2], ' ');
	p->cmd_args[1] = ft_split(av[3], ' ');
	p->cmd_args[2] = NULL;
	i = 0;
	p->execute_cmds = malloc(sizeof(char *) * (ac - 2));
	while (--ac - 2 > 0)
	{
		p->execute_cmds[i] = find_path(p->cmd_filepaths, p->cmd_args[i][0]);
		if (!p->execute_cmds[i])
			ft_error(errno, "cmd not found", p);
		i++;
	}
	p->execute_cmds[i] = NULL;
}

int	main(int ac, char **av)
{
	t_var	p;

	if (ac == 5)
	{
		init(av, &p, ac);
		pipex(&p);
		free_char_arr(p.cmd_filepaths, p.cmd_args);
	}
	return (0);
}