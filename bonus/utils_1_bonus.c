/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_1_bonus.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chon <chon@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/10 16:09:22 by chon              #+#    #+#             */
/*   Updated: 2024/07/02 16:57:33 by chon             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

void	check_filepath(t_var *p, char **av)
{
	int	exit_switch;

	exit_switch = 0;
	if (p->i++ == p->cmd_ct - 1)
		exit_switch = 1;
	if (ft_strlen(av[p->i]) != 0)
	{
		if (!p->exec_cmd_path[p->i - 1]
			|| ft_strlen(p->cmd_args[p->i - 1][0]) == 0)
		{
			free(p->exec_cmd_path[p->i - 1]);
			p->exec_cmd_path[p->i - 1] = ft_strdup("invalid");
			ft_error(errno, ft_strjoin("command not found: ", p->cmd_args[p->i - 1][0]), p, exit_switch);
		}
	}
}

void	setup(t_var *p)
{
	int	i;

	i = -1;
	p->fd = ft_calloc(p->cmd_ct - 1, sizeof(int *));
	if (!p->fd)
		ft_error(errno, "fd calloc", p, 1);
	while (++i < p->cmd_ct - 1)
	{
		p->fd[i] = ft_calloc(2, sizeof(int));
		if (!p->fd)
			ft_error(errno, "fd calloc", p, 1);
	}
	p->pid = ft_calloc(p->cmd_ct, sizeof(int));
	if (!p->pid)
		ft_error(errno, "pid calloc", p, 1);
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
	// ft_printf("%s: %s\n", strerror(error), str);
	(void)error;
	ft_printf("zsh: %s\n", str);
	free(str);
	if (exit_switch)
	{
		free_all(p);
		exit(EXIT_FAILURE);
	}
}
