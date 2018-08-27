/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_player.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ashih <ashih@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/25 18:01:56 by ashih             #+#    #+#             */
/*   Updated: 2018/08/25 18:04:43 by ashih            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"
#include "netplay.h"

int				parse_player(int fd, t_master *m)
{
	int			error;
	char		*line;
	char		**arr;

	error = 0;
	line = NULL;
	arr = NULL;
	m->map_x = 0;
	m->map_y = 0;
	get_next_line(fd, &line);
	if (!(line && (arr = ft_strsplit(line, ' ')) &&
		ft_char_array_length(arr) == 2 &&
		!ft_atoi_check(arr[0], &(m->map_x)) &&
		!ft_atoi_check(arr[1], &(m->map_y))))
		error = 1;
	m->players[m->pid].pos.x = (float)(m->map_x) + 0.5f;
	m->players[m->pid].pos.y = (float)(m->map_y) + 0.5f;
	m->players[m->pid].default_dir.x = -1.0f;
	m->players[m->pid].default_dir.y = 0.0f;
	m->players[m->pid].default_plane.x = 0.0f;
	m->players[m->pid].default_plane.y = -0.66f;
	m->players[m->pid].angle = 0.0f;
	ft_strdel(&line);
	ft_char_array_del(arr);
	return (error ? ft_puterror(ERROR_PLAYER, 0, 1) : 0);
}

int				validate_player(t_master *m)
{
	if (is_valid_move((int)m->players[m->pid].pos.x,
		(int)m->players[m->pid].pos.y, &m->map))
		return (0);
	else
		return (ft_puterror(ERROR_PLAYER_IL, 0, 1));
}
