/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lomont <lomont@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/08 02:29:19 by lomont            #+#    #+#             */
/*   Updated: 2025/01/08 04:26:52 by lomont           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

char *fill_line_buffer(int fd, char *left_c, char *buffer)
{	
	while (*buffer != '\n' || *buffer != '\0')
	{
		read(fd, buffer, BUFFER_SIZE);
		if (left_c)
			left_c = ft_strjoin(left_c, buffer);
		else
			left_c = ft_strdup(buffer);
	}
	return (left_c);
}
char *set_line(char *line_buffer)
{
	size_t	i;
	char *left_c;
	i = 0;
	while (line_buffer[i] != '\n' || line_buffer[i] != '\0')
		i++;
	line_buffer[i] = '\0';
	left_c = ft_substr(line_buffer, 0, i);
	return (left_c);
}

char *get_next_line(int fd)
{
	static char* chars;
	char *buffer;
	char *left_c;
	
	buffer = malloc(BUFFER_SIZE * sizeof(char));
	if (!buffer)
		return ("NULL");
	if (fd >= 0)
	{
		left_c = fill_line_buffer(fd, left_c, buffer);
		//printf("Bravo, tu as reussi a lire un fichier : %s", buffer);
	}
	free(buffer);
	chars = set_line(left_c);
	return (left_c);
}

int main(void)
{
	int fd;

	fd = open("feur", O_RDONLY);
	//get_next_line(fd);
	printf("%s\n", get_next_line(fd));
	//printf("%s\n", get_next_line(fd));
	//printf("%s\n", get_next_line(fd));
}