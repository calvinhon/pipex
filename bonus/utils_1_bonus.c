/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_1_bonus.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chon <chon@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/10 16:09:22 by chon              #+#    #+#             */
/*   Updated: 2024/06/19 15:36:15 by chon             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

void	free_char_arr(char **twoD_arr, char ***threeD_arr)
{
	int	i;
	int	j;

	i = -1;
	j = -1;
	if (twoD_arr)
	{
		while (twoD_arr[++i])
			free(twoD_arr[i]);
		free(twoD_arr);
	}
	if (threeD_arr)
	{
		while (threeD_arr[++j])
			free_char_arr(threeD_arr[j], NULL);
		free(threeD_arr);
	}
}

void	ft_error(int error, char *str, t_var *p)
{
	ft_printf("%s: %s\n", strerror(error), str);
	free_char_arr(p->cmd_filepaths, p->cmd_args);
	free_char_arr(p->execute_cmds, NULL);
	exit(EXIT_FAILURE);
}
