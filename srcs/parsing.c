/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: TheRed <TheRed@students.42.fr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/05 15:51:53 by TheRed            #+#    #+#             */
/*   Updated: 2025/06/05 15:51:53 by TheRed           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "strace.h"

t_options	parse_argv(int argc, char **argv)
{
	int			i;
	t_options	options;

	options.calls = false;

	if (argc == 1)
		ft_exit_message("Usage: %s <program> [args...]\n", argv[0]);

	i = 1;
	while (i < argc)
	{
		if (strncmp(argv[i], "-", 1) != 0)
			break ;

		if (strncmp(argv[i], "-c", 2) == 0)
			options.calls = true;
		i++;
	}

	return (options);
}