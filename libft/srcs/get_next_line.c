/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apuel <apuel@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/12/03 15:31:18 by ashih             #+#    #+#             */
/*   Updated: 2018/08/25 15:40:58 by apuel            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#define GNL_BUFF_SIZE 4096

static int		remove_from_recollection(t_list **list, int fd)
{
	t_list *prev;
	t_list *element;

	prev = (t_list *)0;
	element = *list;
	while (element)
	{
		if (((t_fd_ctx *)element->content)->fd == fd)
			break ;
		prev = element;
		element = element->next;
	}
	if (element)
	{
		if (!prev)
			*list = element->next;
		else
			prev->next = element->next;
		free(((t_fd_ctx *)element->content)->str);
		ft_lstdelone(&element, (void (*)(void *, size_t))free);
	}
	return (0);
}

static t_fd_ctx	*attempt_recall(t_list **list, int fd)
{
	t_list		*element;
	t_fd_ctx	context;

	element = *list;
	while (element)
	{
		if (((t_fd_ctx *)element->content)->fd == fd)
			return (element->content);
		element = element->next;
	}
	context.fd = fd;
	context.pos = GNL_BUFF_SIZE + 1;
	if (!(context.str = malloc(GNL_BUFF_SIZE + 1)))
		return ((t_fd_ctx *)0);
	element = ft_lstnew(&context, sizeof(t_fd_ctx));
	if (element)
	{
		context.str[0] = '\0';
		ft_lstadd(list, element);
		return (element->content);
	}
	free(context.str);
	return ((t_fd_ctx *)0);
}

static size_t	line_copy(t_list **list, t_fd_ctx *info, int *error, char *out)
{
	size_t	i;
	char	*str;

	str = info->str;
	i = info->pos;
	while (str[i] && str[i] != '\n' && i < GNL_BUFF_SIZE)
		i++;
	if (str[i] == '\n')
		*error = 2;
	else if (i != GNL_BUFF_SIZE && !str[i])
		*error = 0;
	else
		*error = 1;
	i -= info->pos;
	ft_memcpy(out, &str[info->pos], i);
	out[i] = '\0';
	info->pos += i;
	if (*error == 0 && i != 0)
		remove_from_recollection(list, info->fd);
	else if (*error != 0)
		info->pos++;
	return (i);
}

static size_t	read_buffer(t_list **list, int fd, int *error, char *out)
{
	t_fd_ctx	*info;

	if (*error != 1)
		return (0);
	info = attempt_recall(list, fd);
	if (info && info->pos > GNL_BUFF_SIZE)
	{
		*error = read(fd, info->str, GNL_BUFF_SIZE);
		if (*error <= 0)
		{
			out[0] = '\0';
			remove_from_recollection(list, fd);
			return (0);
		}
		info->str[*error] = '\0';
		info->pos = 0;
		return (line_copy(list, info, error, out));
	}
	else if (info)
		return (line_copy(list, info, error, out));
	*error = -1;
	return (0);
}

int				get_next_line(const int fd, char **line)
{
	static t_list	*list;
	char			*output;
	size_t			size;
	int				error;

	if (!line)
		return (remove_from_recollection(&list, fd));
	size = 0;
	error = 1;
	*line = (char *)0;
	while (error == 1)
	{
		if ((output = malloc(size + GNL_BUFF_SIZE + 1)) && size != 0)
			ft_memcpy(output, *line, (size_t)size);
		if (*line)
			ft_strdel(line);
		if (!(*line = output))
			error = -1;
		size += read_buffer(&list, fd, &error, (*line) + size);
	}
	if (error > 0 || (error == 0 && size != 0))
		return (1);
	if (*line)
		ft_strdel(line);
	return (error);
}
