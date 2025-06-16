/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: TheRed <TheRed@students.42.fr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/05 15:43:03 by TheRed            #+#    #+#             */
/*   Updated: 2025/06/05 15:43:03 by TheRed           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "strace.h"

void	trace(pid_t pid, t_options options)
{
	(void) 	options;
	int			i;
	int			status;
	sigset_t	empty;
	sigset_t	blocked;

	status = 0;

	sigemptyset(&empty);
    sigemptyset(&blocked);

	sigaddset(&blocked, SIGHUP);
    sigaddset(&blocked, SIGINT);
    sigaddset(&blocked, SIGQUIT);
    sigaddset(&blocked, SIGPIPE);
    sigaddset(&blocked, SIGTERM);

	sigprocmask(SIG_SETMASK, &empty, NULL);
	waitpid(pid, &status, 0);
	sigprocmask(SIG_BLOCK, &blocked, NULL);

	ptrace(PTRACE_SETOPTIONS, pid, 0, PTRACE_O_TRACESYSGOOD);
	ptrace(PTRACE_SYSCALL, pid, NULL, NULL);
	
	printf("Parent process %d is now tracing child process %d.\n", getpid(), pid);
	
	i = 0;
	while (1)
	{
		sigprocmask(SIG_SETMASK, &empty, NULL);
		waitpid(-1, &status, __WALL);
		sigprocmask(SIG_BLOCK, &blocked, NULL);

		if (WIFEXITED(status) || WIFSIGNALED(status))
			break ;

		int sig = WSTOPSIG(status);

		if (sig == SIGTRAP)
		{
			ptrace(PTRACE_SYSCALL, pid, 0, 0);
			continue ;
		}
		if ((sig & 0x80) == 0)
		{
			ptrace(PTRACE_SYSCALL, pid, 0, sig);
			continue ;
		}
		print_single_syscall(pid, (bool)(i % 2) == false);

		ptrace(PTRACE_SYSCALL, pid, 0, 0);
		
		++i;
	}
}

void	strace(char **argv_exec, t_options options)
{
	pid_t	pid;

	pid = fork();
	if (pid < 0)
		ft_exit_message("Failed to fork process");

	if (pid == 0)
	{
		ptrace(PTRACE_TRACEME, 0, NULL, NULL);
		kill(getpid(), SIGSTOP);
		execvp(argv_exec[0], argv_exec);

		ft_exit_message("Failed to execute command: %s", argv_exec[0]);
	}

	trace(pid, options);
}

int main(int argc, char **argv)
{
	t_options	options;
	char		**argv_exec;
	int			i;

	options = parse_argv(argc, argv);

	i = 1;
	while (i < argc)
	{
		if (argv[i][0] != '-')
			break;
		i++;
	}

	argv_exec = argv + i;

	printf("Executing strace with options:\n");
	if (options.calls)
		printf(" - Calls tracing enabled\n");
	else
		printf(" - Calls tracing disabled\n");
	printf(" - Executing command: ");
	for (int i = 0; argv_exec[i] != NULL; i++)
		printf("%s ", argv_exec[i]);
	printf("\n\n");

	strace(argv_exec, options);

	printf("Strace finished.\n");
}