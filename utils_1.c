/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_1.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chon <chon@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/10 16:09:22 by chon              #+#    #+#             */
/*   Updated: 2024/06/11 16:08:34 by chon             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	free_char_array(char **array)
{
	int	i;

	i = -1;
	while (array[++i])
		free(array[i]);
	free(array);
}

void	ft_error(int error, char *str, t_var *p)
{
	int	i;

	i = -1;
	ft_printf("%s: %s\n", strerror(error), str);
	free_char_array(p->cmd_filepaths);
	while (p->cmd_args[++i])
		free_char_array(p->cmd_args[i]);
	free(p->cmd_args);
	exit(EXIT_FAILURE);
}
