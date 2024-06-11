/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chon <chon@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/10 11:26:17 by chon              #+#    #+#             */
/*   Updated: 2024/06/11 15:02:03 by chon             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

// void	pipex(t_var p)
// {
// }

void	init(char **av, t_var *p)
{
	p->in_fd = open(av[1], O_RDONLY | O_CREAT);
	if (p->in_fd == -1)
		ft_error(errno, av[1], p);
	p->out_fd = open(av[4], O_WRONLY | O_CREAT);
	if (p->out_fd == -1)
		ft_error(errno, av[4], p);
}

int	main(int ac, char **av)
{
	t_var	p;

	(void)ac;
	init(av, &p);
	// pipex(p);
	return (0);
}