/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chon <chon@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/10 11:26:34 by chon              #+#    #+#             */
/*   Updated: 2024/06/25 13:57:35 by chon             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_BONUS_H
# define PIPEX_BONUS_H

# include "../libft/libft.h"
# include <sys/wait.h>
# include <errno.h>

typedef struct s_var
{
	char	*filepaths;
	char	**cmd_filepaths;
	char	***cmd_args;
	char	**execute_cmds;
	int		cmd_ct;
	int		in_fd;
	int		out_fd;
	int		i;
	int		j;
	int		k;
}	t_var;

void	ft_error(int error, char *str, t_var *p);
void	free_char_arr(char **twoD_arr, char ***threeD_arr);

#endif