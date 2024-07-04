/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_1_bonus.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chon <chon@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/10 16:09:22 by chon              #+#    #+#             */
/*   Updated: 2024/07/04 11:44:35 by chon             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

int	is_empty(t_var p, char **av)
{
	size_t	sp_ct;

	if (!ft_strlen(av[p.i + 1]))
		return (0);
	sp_ct = 0;
	p.j = -1;
	while (av[p.i + 1][++p.j])
	{
		if (av[p.i + 1][p.j] == ' ')
			sp_ct++;
	}
	if (sp_ct == ft_strlen(av[p.i + 1]))
		return (0);
	else
		return (1);
}

void	check_filepath(t_var *p, char **av)
{
	int		exit_switch;
	char	*err_msg;

	exit_switch = 0;
	if (++p->i == p->cmd_ct)
		exit_switch = 1;
	if ((p->i == 1 && !access(av[1], X_OK)
			&& (!ft_strncmp("invalid", p->exec_cmd_path[p->i - 1], 7)
			|| !is_empty(*p, av)))
		|| (p->i > 1 && !ft_strncmp("invalid", p->exec_cmd_path[p->i - 1], 7)))
	{
		if (ft_strlen(av[p->i + 1]))
		{
			err_msg = ft_strjoin("command not found: ",
					p->cmd_args[p->i - 1][0]);
			ft_error(errno, err_msg, p, exit_switch);
		}
		else
			ft_error(errno, ft_strdup("permission denied:"), p, exit_switch);
	}
}

void	setup(t_var *p)
{
	int	i;

	i = -1;
	p->fd = ft_calloc(p->cmd_ct - 1, sizeof(int *));
	if (!p->fd)
		ft_error(errno, ft_strdup("fd calloc"), p, 1);
	while (++i < p->cmd_ct - 1)
	{
		p->fd[i] = ft_calloc(2, sizeof(int));
		if (!p->fd)
			ft_error(errno, ft_strdup("fd calloc"), p, 1);
	}
	p->pid = ft_calloc(p->cmd_ct, sizeof(int));
	if (!p->pid)
		ft_error(errno, ft_strdup("pid calloc"), p, 1);
}

void	close_pipes(t_var *p)
{
	p->j = -1;
	p->k = -1;
	while (++p->j < p->cmd_ct - 1)
	{
		while (++p->k < 2)
			close(p->fd[p->j][p->k]);
		p->k = -1;
	}
}

void	ft_error(int error, char *str, t_var *p, int exit_switch)
{
	if (!error)
		ft_printf("No such file or directory: %s\n", str);
	else
		ft_printf("%s: %s\n", strerror(error), str);
	free(str);
	if (exit_switch)
	{
		free_all(p);
		exit(EXIT_FAILURE);
	}
}
