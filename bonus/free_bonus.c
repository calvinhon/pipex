/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chon <chon@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/01 14:18:37 by chon              #+#    #+#             */
/*   Updated: 2024/07/09 16:43:08 by chon             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

void	free_int_array(int **twoD, int cmd_ct)
{
	int	i;

	i = -1;
	if (twoD)
	{
		while (++i < cmd_ct - 1)
			free(twoD[i]);
		free(twoD);
	}
}

void	free_char_arr(char **twoD, char ***threeD)
{
	int	i;
	int	j;

	i = -1;
	j = -1;
	if (twoD)
	{
		while (twoD[++i])
			free(twoD[i]);
		free(twoD);
	}
	if (threeD)
	{
		while (threeD[++j])
			free_char_arr(threeD[j], NULL);
		free(threeD);
	}
}

void	free_all(t_var *p)
{
	free(p->filepaths);
	free(p->pid);
	free_int_array(p->fd, p->cmd_ct);
	free_char_arr(p->cmd_filepaths, p->cmd_args);
	free_char_arr(p->exec_cmd_path, NULL);
}
