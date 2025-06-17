/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: TheRed <TheRed@students.42.fr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/17 14:57:23 by TheRed            #+#    #+#             */
/*   Updated: 2025/06/17 14:57:23 by TheRed           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "strace.h"

static void handle_sigint(int signo)
{
	(void)signo;

	printf("ft_strace: Process %d detached\n", g_traced_pid);
	
	if (ptrace(PTRACE_DETACH, g_traced_pid, NULL, NULL) < 0)
		perror("ptrace(DETACH)");
	
	if (signo == SIGQUIT)
		printf("Quit (core dumped)\n");
	exit(EXIT_SUCCESS);
}

void	ft_setup_signal(void)
{
	struct sigaction sa;

	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sa.sa_handler = handle_sigint;

	sigaction(SIGINT, &sa, NULL);
	sigaction(SIGQUIT, &sa, NULL);
}