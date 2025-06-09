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
# include <errno.h>
# include <fcntl.h>
# include <sys/uio.h>
# include <linux/elf.h>

#define MAX_SYSCALL_NR 467

typedef struct s_options
{
	bool	calls;
}	t_options;


typedef enum	s_arg_type
{
    INT,
    UINT,
    STR,
    PTR,
    STRUCT,
}	t_arg_type;

typedef struct	s_syscall_info
{
	char	*name;
	short	argc;
    t_arg_type	arg_types[6];
}	t_syscall_info;


// PARSING

t_options				parse_argv(int argc, char **argv);

// UTILS

void					ft_exit_message(char *message, ...);

// SYSCALLS

t_syscall_info          get_syscall_info(long scno);

// PRINT

void					print_single_syscall(pid_t pid, bool is_enter);

#endif