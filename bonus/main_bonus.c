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

void	init2(char **av, t_var *p, int ac)
{
	p->cmd_ct = ac - 3 - p->hd_shift;
	setup(p);
	if (p->hd_shift)
	{
		p->in_fd = open(av[1], O_RDONLY);
		if (p->in_fd < 0)
			ft_error(errno, ft_strdup(av[1]), p, 0);
	}
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
		p->cmd_args[p->i] = ft_split(av[p->i + 2 + p->hd_shift], ' ');
	p->cmd_args[p->i] = NULL;
	p->exec_cmd_path = calloc(p->cmd_ct + 1, sizeof(char *));
	if (!p->exec_cmd_path)
		ft_error(errno, ft_strdup("malloc exec_cmd_path"), p, 1);
	find_paths(p, av);
}

void	init1(char **av, t_var *p, char **env)
{
	char	*line;

	p->cmd_args = NULL;
	p->cmd_filepaths = NULL;
	p->exec_cmd_path = NULL;
	p->filepaths = NULL;
	p->i = 0;
	while (!ft_strnstr(env[p->i], "PATH", 4))
		p->i++;
	if (env[p->i])
		p->filepaths = ft_substr(env[p->i], 5, ft_strlen(env[p->i]) - 5);
	if (!ft_strncmp(av[1], "here_doc", 8))
	{
		p->hd_shift = 1;
		p->in_fd = open("here_doc.txt", O_TRUNC | O_CREAT, 0777);
		if (p->in_fd < 0)
			ft_error(errno, ft_strdup("here_doc.txt"), p, 1);
		line = get_next_line(0);
		while (ft_strncmp(av[2], line, ft_strlen(line) - 1))
		{
			write(p->in_fd, line, ft_strlen(line));
			free(line);
			line = get_next_line(0);
		}
		free(line);
	}
}

int	main(int ac, char **av, char **env)
{
	t_var	p;

	p.hd_shift = 0;
	init1(av, &p, env);
	if (((!ft_strncmp(av[1], "here_doc", 8) && ac > 5)
		|| (ft_strncmp(av[1], "here_doc", 8) && ac > 4)) && p.filepaths)
	{
		init2(av, &p, ac);
		p.empty_fd = open("empty.txt", O_TRUNC | O_CREAT, 0777);
		if (p.empty_fd < 0)
			ft_error(errno, ft_strdup("empty.txt"), &p, 1);
		pipex(&p, av[1]);
		free_all(&p);
	}
	else
	{
		if (p.filepaths)
			free(p.filepaths);
		ft_printf("Ensure ENV exists and ");
		ft_printf("input either (i) an infile or (ii) here_doc and LIMITER, ");
		ft_printf("at least two cmd args, and an outfile\n");
	}
	return (0);
}
