/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mvasquez <mvasquez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/01 16:38:49 by mvasquez          #+#    #+#             */
/*   Updated: 2026/02/07 14:39:55 by mvasquez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

static char	*ft_get_line(char *save)
{
	size_t	i;
	size_t	j;
	char	*line;

	i = 0;
	j = 0;
	if (!save || !save[0])
		return (NULL);
	while (save[i] && save[i] != '\n')
		i++;
	if (save[i] == '\n')
		i++;
	line = malloc(sizeof(char) * (i + 1));
	if (!line)
		return (NULL);
	while (j < i)
	{
		line[j] = save[j];
		j++;
	}
	line[j] = '\0';
	return (line);
}

static char	*ft_get_rest(char *save)
{
	size_t	i;
	size_t	j;
	char	*rest;

	i = 0;
	while (save[i] && save[i] != '\n')
		i++;
	if (!save[i])
	{
		free(save);
		return (NULL);
	}
	rest = malloc(sizeof(char) * (ft_strlen(save) - i + 1));
	if (!rest)
	{
		free(save);
		return (NULL);
	}
	i++;
	j = 0;
	while (save[i])
		rest[j++] = save[i++];
	rest[j] = '\0';
	free(save);
	return (rest);
}

static char	*ft_read_buffer(char *save, int fd)
{
	char	*buffer;
	int		nbytes;

	buffer = malloc(sizeof(char) * (BUFFER_SIZE + 1));
	if (!buffer)
		return (NULL);
	nbytes = 1;
	while (nbytes > 0 && !ft_strchr(save))
	{
		nbytes = read(fd, buffer, BUFFER_SIZE);
		if (nbytes == -1)
		{
			free (buffer);
			free (save);
			return (NULL);
		}
		buffer[nbytes] = '\0';
		save = join_string(save, buffer);
	}
	free (buffer);
	return (save);
}

char	*get_next_line(int fd)
{
	static char	*save;
	char		*line;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	save = ft_read_buffer(save, fd);
	if (!save)
		return (NULL);
	line = ft_get_line(save);
	if (!line)
	{
		free(save);
		save = NULL;
		return (NULL);
	}
	save = ft_get_rest(save);
	return (line);
}
