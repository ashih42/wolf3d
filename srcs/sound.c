/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sound.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ashih <ashih@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/07/26 06:04:02 by ashih             #+#    #+#             */
/*   Updated: 2018/08/08 19:19:40 by ashih            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"

void			play_sound(char *file, char *path)
{
	char		*str;
	char		*temp;

	str = ft_strjoin(path, file);
	temp = str;
	str = ft_strjoin("afplay ", str);
	ft_strdel(&temp);
	temp = str;
	str = ft_strjoin(str, " &");
	ft_strdel(&temp);
	system(str);
	ft_strdel(&str);
}

void			init_bgm(char *path)
{
	static char	*bgm[16] = {
		"bgm/ANewMorning.mp3",
		"bgm/Crystals.mp3",
		"bgm/Daisuke.mp3",
		"bgm/DeepCover.mp3",
		"bgm/ElectricDreams.mp3",
		"bgm/Flatline.mp3",
		"bgm/Hotline.mp3",
		"bgm/Hydrogen.mp3",
		"bgm/InnerAnimal.mp3",
		"bgm/ItsSafeNow.mp3",
		"bgm/Knock.mp3",
		"bgm/Miami2.mp3",
		"bgm/Musikk2.mp3",
		"bgm/Paris2.mp3",
		"bgm/Perturbator.mp3",
		"bgm/TurfMain.mp3"
	};

	play_sound(bgm[rand() % 16], path);
}
