/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chon <chon@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/10 11:26:17 by chon              #+#    #+#             */
/*   Updated: 2024/07/03 18:42:56 by chon             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

void	find_paths(t_var *p, char **av)
{
	char	*filepath;

	p->i = 0;
	p->j = -1;
	while (p->i < p->cmd_ct)
	{
		while (p->cmd_filepaths[++p->j])
		{
			p->filepath_0 = ft_strjoin(p->cmd_filepaths[p->j], "/");
			filepath = ft_strjoin(p->filepath_0, p->cmd_args[p->i][0]);
			free(p->filepath_0);
			if (access(filepath, X_OK) > -1)
			{
				p->exec_cmd_path[p->i] = ft_strdup(filepath);
				free(filepath);
				break ;
			}
			free(filepath);
		}
		if (!p->exec_cmd_path[p->i])
			p->exec_cmd_path[p->i] = ft_strdup("invalid");
		check_filepath(p, av);
		p->j = -1;
	}
	p->exec_cmd_path[p->i] = NULL;
}

void	execute(t_var *p, char *infile, int fd_in, int fd_out)
{
	if ((access(infile, R_OK) < 0 && !p->i)
		|| (p->i > 0 && !p->cmd_args[p->i - 1][0]))
	{
		if (dup2(p->empty_fd, STDIN_FILENO) < 0)
			ft_error(errno, ft_strdup("dup"), p, 1);
		close(p->empty_fd);
	}
	else
		if (dup2(fd_in, STDIN_FILENO) < 0)
			ft_error(errno, ft_strdup("dup"), p, 1);
	if (dup2(fd_out, STDOUT_FILENO) < 0)
		ft_error(errno, ft_strdup("dup"), p, 1);
	close_pipes(p);
	if (access(infile, R_OK) > 0 || p->i)
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
				execute(p, infile, p->in_fd, p->fd[p->i][1]);
			else if (p->i < p->cmd_ct - 1)
				execute(p, infile, p->fd[p->i - 1][0], p->fd[p->i][1]);
			else
				execute(p, infile, p->fd[p->i - 1][0], p->out_fd);
		}
		waitpid(p->pid[p->i], NULL, WNOHANG);
	}
	close_pipes(p);
}

void	init(char **av, t_var *p, int ac)
{
	p->cmd_args = NULL;
	p->cmd_filepaths = NULL;
	p->exec_cmd_path = NULL;
	p->cmd_ct = ac - 3;
	setup(p);
	p->in_fd = open(av[1], O_RDONLY);
	if (p->in_fd < 0)
		ft_error(errno, ft_strdup(av[1]), p, 0);
	p->out_fd = open(av[ac - 1], O_WRONLY | O_TRUNC | O_CREAT, 0644);
	if (p->out_fd < 0)
		ft_error(errno, ft_strdup(av[ac - 1]), p, 1);
	p->cmd_filepaths = ft_split(p->filepaths, ':');
	if (!p->cmd_filepaths)
		ft_error(errno, ft_strdup("cmd_filepaths"), p, 1);
	p->cmd_args = malloc(sizeof(char **) * (p->cmd_ct + 1));
	if (!p->cmd_args)
		ft_error(errno, ft_strdup("malloc cmd_args"), p, 1);
	p->i = -1;
	while (++p->i < p->cmd_ct)
		p->cmd_args[p->i] = ft_split(av[p->i + 2], ' ');
	p->cmd_args[p->i] = NULL;
	p->exec_cmd_path = calloc(p->cmd_ct + 1, sizeof(char *));
	if (!p->exec_cmd_path)
		ft_error(errno, ft_strdup("malloc exec_cmd_path"), p, 1);
	find_paths(p, av);
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
		init(av, &p, ac);
		p.empty_fd = open("empty.txt", O_TRUNC | O_CREAT, 0777);
		pipex(&p, av[1]);
		free_all(&p);
	}
	else
	{
		if (p.filepaths)
			free(p.filepaths);
		ft_printf("Input an infile, at least two cmd args, and an outfile\n");
	}
	return (0);
}
