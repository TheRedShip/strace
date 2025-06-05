/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   strace.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: TheRed <TheRed@students.42.fr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/05 15:42:21 by TheRed            #+#    #+#             */
/*   Updated: 2025/06/05 15:42:21 by TheRed           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STRACE_H
# define STRACE_H

# include <stdio.h>
# include <stdbool.h>
# include <stdarg.h>
# include <stdlib.h>
# include <string.h> 
# include <unistd.h>
# include <signal.h>
# include <sys/user.h>
# include <sys/wait.h>
# include <sys/ptrace.h>

typedef struct s_options
{
	bool	calls;
}	t_options;


// PARSING

t_options	parse_argv(int argc, char **argv);

// UTILS

void		ft_exit_message(char *message, ...);

#endif