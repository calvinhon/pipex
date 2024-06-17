/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chon <chon@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/10 11:26:34 by chon              #+#    #+#             */
/*   Updated: 2024/06/17 16:43:00 by chon             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# include "libft/libft.h"
# include <sys/wait.h>
# include <errno.h>

typedef struct s_var
{
	char	*filepath;
	char	**cmd_filepaths;
	char	**execute_cmds;
	char	***cmd_args;
	int		in_fd;
	int		out_fd;
} t_var;

void	ft_error(int error, char *str, t_var *p);
void	free_char_arr(char **twoD_arr, char ***threeD_arr);

#endif