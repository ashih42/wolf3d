/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ashih <ashih@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/03 09:50:23 by ashih             #+#    #+#             */
/*   Updated: 2018/08/25 17:54:50 by ashih            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"

void			print_map(t_map *map)
{
	int			i;

	ft_printf("map width =\t%d\n", map->width);
	ft_printf("map height =\t%d\n", map->height);
	ft_printf("\n");
	i = 0;
	while (i < map->height)
		ft_printf("%s\n", map->cell[i++]);
}

int				move_entity_westward(t_entity *e, t_master *m)
{
	t_vect2		target;

	target.x = e->pos.x - MOV_INC;
	if (is_valid_move(target.x - MOV_LOOKAHEAD, e->pos.y, &m->map))
	{
		e->pos.x = target.x;
		return (1);
	}
	return (0);
}

int				move_entity_eastward(t_entity *e, t_master *m)
{
	t_vect2		target;

	target.x = e->pos.x + MOV_INC;
	if (is_valid_move(target.x + MOV_LOOKAHEAD, e->pos.y, &m->map))
	{
		e->pos.x = target.x;
		return (1);
	}
	return (0);
}

int				move_entity_northward(t_entity *e, t_master *m)
{
	t_vect2		target;

	target.y = e->pos.y - MOV_INC;
	if (is_valid_move(e->pos.x, target.y - MOV_LOOKAHEAD, &m->map))
	{
		e->pos.y = target.y;
		return (1);
	}
	return (0);
}

int				move_entity_southward(t_entity *e, t_master *m)
{
	t_vect2		target;

	target.y = e->pos.y + MOV_INC;
	if (is_valid_move(e->pos.x, target.y + MOV_LOOKAHEAD, &m->map))
	{
		e->pos.y = target.y;
		return (1);
	}
	return (0);
}
