/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chon <chon@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/10 11:26:34 by chon              #+#    #+#             */
/*   Updated: 2024/07/08 11:14:09 by chon             ###   ########.fr       */
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
	char	*filepath_0;
	char	*filepath;
	char	**cmd_filepaths;
	char	***cmd_args;
	char	**exec_cmd_path;
	int		**fd;
	int		*pid;
	int		cmd_ct;
	int		in_fd;
	int		out_fd;
	int		i;
	int		j;
	int		k;
	int		empty_fd;
	int		hd_shift;
}	t_var;

void	ft_error(int error, char *str, t_var *p, int exit_switch);
void	free_char_arr(char **twoD, char ***threeD);
void	free_int_array(int **twoD, int cmd_ct);
void	free_all(t_var *p);
void	setup(t_var *p);
void	close_pipes(t_var *p);
void	check_filepaths(t_var *p, char **av);
void	pipex(t_var *p, char *infile);

#endif