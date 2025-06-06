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

void strace(char **argv_exec, t_options options)
{
	(void)	options;
	int		i;
	pid_t	pid;
	int		status;

	pid = fork();
	if (pid == 0)
	{
		ptrace(PTRACE_TRACEME, 0, NULL, NULL);
		kill(getpid(), SIGSTOP);
		execvp(argv_exec[0], argv_exec);

		ft_exit_message("Failed to execute command: %s", argv_exec[0]);
	}

	status = 0;
	waitpid(pid, &status, 0);
	ptrace(PTRACE_SETOPTIONS, pid, 0, PTRACE_O_TRACESYSGOOD);

	ptrace(PTRACE_SYSCALL, pid, NULL, NULL);
	
	printf("Parent process %d is now tracing child process %d.\n", getpid(), pid);
	
	i = 0;
	while (1)
	{
		waitpid(-1, &status, __WALL);

		if (WIFEXITED(status) || WIFSIGNALED(status))
			break;

		int sig = WSTOPSIG(status);

		if (sig == SIGTRAP)
		{
			ptrace(PTRACE_SYSCALL, pid, 0, 0);
			continue;
		}
		if ((sig & 0x80) == 0)
		{
			ptrace(PTRACE_SYSCALL, pid, 0, sig);
			continue;
		}

		struct user_regs_struct regs;
		ptrace(PTRACE_GETREGS, pid, NULL, &regs);

		if (i % 2 == 0)
		{
			long scno = regs.orig_rax;
			printf("%s() = ", syscall_name(scno));
			// printf(">> Entering syscall #%ld ()\n", scno);
		}
		else
		{
			long ret = regs.rax;

			printf("%ld\n", ret);
		}


		ptrace(PTRACE_SYSCALL, pid, 0, 0);
		
		++i;
	}

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