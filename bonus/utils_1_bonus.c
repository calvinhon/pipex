/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_1_bonus.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chon <chon@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/10 16:09:22 by chon              #+#    #+#             */
/*   Updated: 2024/07/01 14:19:31 by chon             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

void	setup(t_var *p)
{
	int	i;

	i = -1;
	p->fd = ft_calloc(p->cmd_ct - 1, sizeof(int *));
	if (!p->fd)
		ft_error(errno, "fd calloc", p);
	while (++i < p->cmd_ct - 1)
	{
		p->fd[i] = calloc(2, sizeof(int));
		if (!p->fd)
			ft_error(errno, "fd calloc", p);
	}
	p->pid = ft_calloc(p->cmd_ct, sizeof(int));
	if (!p->pid)
		ft_error(errno, "pid calloc", p);
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

void	ft_error(int error, char *str, t_var *p)
{
	ft_printf("%s: %s\n", strerror(error), str);
	free_all(p);
	exit(EXIT_FAILURE);
}
