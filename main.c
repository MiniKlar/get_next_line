/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lomont <lomont@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/09 02:45:30 by lomont            #+#    #+#             */
/*   Updated: 2025/01/09 04:13:58 by lomont           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

int main(void)
{
	int fd;
	fd = open("feur", O_RDONLY);
	char *line = get_next_line(fd);
	printf("%s", line);
	free(line);
}