/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mvasquez <mvasquez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/01 16:43:04 by mvasquez          #+#    #+#             */
/*   Updated: 2026/02/07 14:40:00 by mvasquez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

unsigned int	ft_strchr(const char *str)
{
	size_t	i;

	i = 0;
	if (!str)
		return (0);
	while (str[i])
	{
		if (str[i] == '\n')
			return (1);
		i++;
	}
	return (0);
}

unsigned int	ft_strlen(const char *str)
{
	int	i;

	if (!str)
		return (0);
	i = 0;
	while (str[i])
		i++;
	return (i);
}

char	*join_string(char *s1, char *s2)
{
	char	*ret;
	char	*temp;
	int		i;
	int		j;
	int		size;

	size = ft_strlen(s2);
	if (s1)
		size += ft_strlen(s1);
	temp = malloc(sizeof(char) * (size + 1));
	if (!temp)
		return (NULL);
	ret = temp;
	i = 0;
	j = 0;
	while (s1 && s1[i])
	{
		temp[i] = s1[i];
		i++;
	}
	while (s2 && s2[j])
		temp[i++] = s2[j++];
	temp[i] = 0;
	free(s1);
	return (ret);
}
