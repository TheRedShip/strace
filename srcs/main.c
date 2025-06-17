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

pid_t	g_traced_pid = 0;

int	trace(pid_t pid, t_options options)
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
	return (status);
}

int	strace(char **argv_exec, t_options options)
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

	g_traced_pid = pid;

	return (trace(pid, options));
}

int main(int argc, char **argv)
{
	t_options	options;
	char		**argv_exec;
	int			i;

	ft_setup_signal();

	options = parse_argv(argc, argv);

	i = 1;
	while (i < argc)
	{
		if (argv[i][0] != '-')
			break;
		i++;
	}

	argv_exec = argv + i;

	int exit_status = strace(argv_exec, options);

	if (WIFEXITED(exit_status))
	{
		int exit_code = WEXITSTATUS(exit_status);
		printf("\n+++ exited with %d +++\n", exit_code);
		return (exit_code);
	}
	else if (WIFSIGNALED(exit_status))
	{
		int signal = WTERMSIG(exit_status);
		printf("\n+++ killed by signal %d +++\n", signal);
		return (128 + signal);
	}
	return (0);
}