/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lomont <lomont@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/08 02:29:19 by lomont            #+#    #+#             */
/*   Updated: 2025/01/09 04:59:06 by lomont           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

char	*fill_line_buffer(int fd, char *left_c, char *buffer)
{
	char *tmp;
	size_t b_read;
	
	if (!left_c)
		left_c = ft_strdup("");

	b_read = 1;
	while (b_read > 0 && ft_strchr(left_c, '\n') == 0)
	{
		read(fd, buffer, BUFFER_SIZE);
		dprintf(STDERR_FILENO, "on a read");
		if (!left_c)
			left_c = ft_strdup(buffer);
		else
		{
			tmp = left_c;
			left_c = ft_strjoin(tmp, buffer);
		}
	}
	free(tmp);
	return (left_c);
}

char	*set_line(char *line_buffer)
{
	size_t		i;
	char		*left_c;

	if (!line_buffer)
		return (NULL);
	i = 0;
	left_c = NULL;
	while (line_buffer[i] != '\n' && line_buffer[i] != '\0')
		i++;
	left_c = ft_substr(line_buffer, i, ft_strlen(line_buffer) - i);
	if (!left_c)
		return (NULL);
	line_buffer[i] = '\0';
	return (left_c);
}

char	*get_next_line(int fd)
{
	static char	*left_c;
	char		*line;
	char		*buffer;

	buffer = calloc((BUFFER_SIZE + 1), sizeof(char));
	if (!buffer)
		return (NULL);
	// buffer[0] = '\0' ;
	left_c = NULL;
	if (fd > 0)
		line = fill_line_buffer(fd, left_c, buffer);
	else
	{
		free(buffer);
		return (NULL);
	}
	left_c = set_line(line);
	return (line);
}

void	*ft_memmove(void *dest, const void *src, size_t n)
{
	size_t		i;
	char		*tmp_dst;
	const char	*tmp_src;

	if (!dest && !src)
		return (NULL);
	i = 0;
	tmp_dst = (char *)dest;
	tmp_src = (char *)src;
	if (tmp_dst > tmp_src)
		while (n-- > 0)
			tmp_dst[n] = tmp_src[n];
	else
	{
		while (i < n)
		{
			tmp_dst[i] = tmp_src[i];
			i++;
		}
	}
	return (dest);
}

void	*ft_memcpy(void *dest, const void *src, size_t n)
{
	size_t				i;
	unsigned char		*tmp_dest;
	const unsigned char	*tmp_src;

	if (!dest && !src)
		return (dest);
	i = 0;
	tmp_dest = (unsigned char *)dest;
	tmp_src = (const unsigned char *)src;
	while (i < n)
	{
		tmp_dest[i] = tmp_src[i];
		i++;
	}
	return (dest);
}
