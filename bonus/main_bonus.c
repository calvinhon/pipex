/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chon <chon@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/10 11:26:17 by chon              #+#    #+#             */
/*   Updated: 2024/06/25 14:02:08 by chon             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

void	find_paths(t_var *p, int ac)
{
	char	*filepath_0;
	char	*filepath;

	p->i = 0;
	p->j = -1;
	while (--ac - 2 > 0)
	{
		while (p->cmd_filepaths[++p->j])
		{
			filepath_0 = ft_strjoin(p->cmd_filepaths[p->j], "/");
			filepath = ft_strjoin(filepath_0, p->cmd_args[p->i][0]);
			free(filepath_0);
			if (access(filepath, X_OK) > -1)
			{
				p->execute_cmds[p->i] = ft_strdup(filepath);
				break ;
			}
			free(filepath);
		}
		if (!p->execute_cmds[p->i++])
			ft_error(errno, "cmd not found", p);
		p->j = -1;
		free(filepath);
	}
	p->execute_cmds[p->i] = NULL;
}

void	execute(t_var *p, int **fd, int fd_in, int fd_out)
{
	p->j = -1;
	p->k = -1;

	(void)fd;
	// printf("in:%d out:%d\n", fd_in, fd_out);
	if (dup2(fd_in, STDIN_FILENO) < 0 || dup2(fd_out, STDOUT_FILENO) < 0)
		ft_error(errno, "dup", p);
	// close(fd[0][0]);
	// while (++p->j < p->cmd_ct - 1)
	// {
	// 	while (++p->k < 2)
	// 		close(fd[p->j][p->k]);
	// 	p->k = -1;
	// }
	if (execve(p->execute_cmds[p->i], p->cmd_args[p->i], NULL) < 0)
		ft_error(errno, "execve", p);
}

void	pipex(t_var *p)
{
	int	fd[p->cmd_ct - 1][2];
	int	pid[p->cmd_ct];

	p->i = -1;
	while (++p->i < p->cmd_ct - 1)
		if (pipe(fd[p->i]) < 0)
			ft_error(errno, "pipe", p);
	p->i = -1;
	while (++p->i < p->cmd_ct)
	{
		pid[p->i] = fork();
		if (pid[p->i] < 0)
			ft_error(errno, "fork", p);
		if (!pid[p->i])
		{
			if (!p->i)
				execute(p, (int **)fd, p->in_fd, fd[p->i][1]);
			else if (p->i < p->cmd_ct - 1)
				execute(p, (int **)fd, fd[p->i - 1][0], fd[p->i][1]);
			else
				execute(p, (int **)fd, fd[p->i - 1][0], p->out_fd);
		}
	}
	p->j = -1;
	p->k = -1;
	while (++p->j < p->cmd_ct - 1)
	{
		while (++p->k < 2)
			// if ((p->j == p->i && !p->k) || (p->j == p->i + 1 && p->k))
			close(fd[p->j][p->k]);
		p->k = -1;
	}
	// p->i = -1;
	// while (++p->i < p->cmd_ct)
	// 	waitpid(pid[p->i], NULL, 0);
}

void	init(char **av, t_var *p, int ac, char *env)
{
	p->cmd_args = NULL;
	p->cmd_filepaths = NULL;
	p->execute_cmds = NULL;
	p->in_fd = open(av[1], O_RDONLY);
	p->out_fd = open(av[ac - 1], O_WRONLY | O_TRUNC | O_CREAT, 0644);
	if (p->in_fd == -1)
		ft_error(errno, av[1], p);
	if (p->out_fd == -1)
		ft_error(errno, av[ac - 1], p);
	p->cmd_filepaths = ft_split(env, ':');
	if (!p->cmd_filepaths)
		ft_error(errno, "cmd_filepaths", p);
	p->cmd_args = malloc(sizeof(char **) * (ac - 2));
	if (!p->cmd_args)
		ft_error(errno, "malloc cmd_args", p);
	p->i = -1;
	while (++p->i + 2 < ac - 1)
		p->cmd_args[p->i] = ft_split(av[p->i + 2], ' ');
	p->cmd_args[p->i] = NULL;
	p->execute_cmds = calloc((ac - 2), sizeof(char *));
	if (!p->cmd_args)
		ft_error(errno, "malloc cmd_args", p);
	find_paths(p, ac);
}

int	main(int ac, char **av, char **env)
{
	t_var	p;
	int		i;

	i = 0;
	p.filepaths = NULL;
	while (!ft_strnstr(env[i], "PATH", 4))
		i++;
	if (env[i])
		p.filepaths = ft_substr(env[i], 5, ft_strlen(env[i]) - 5);
	if (ac > 4 && p.filepaths)
	{
		p.cmd_ct = ac - 3;
		init(av, &p, ac, p.filepaths);
		printf("%s\n", p.cmd_filepaths[1][0]);
		pipex(&p);
		free(p.filepaths);
		free_char_arr(p.cmd_filepaths, p.cmd_args);
		free_char_arr(p.execute_cmds, NULL);
	}
	else
		ft_printf("Input an infile, two cmd args, and an outfile\n");
	return (0);
}
