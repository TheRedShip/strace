/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: TheRed <TheRed@students.42.fr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/09 19:41:14 by TheRed            #+#    #+#             */
/*   Updated: 2025/06/09 19:41:14 by TheRed           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "strace.h"

ssize_t read_string_procfs(pid_t child, unsigned long addr, char *buf, size_t buflen)
{
	char path[64];

	snprintf(path, sizeof(path), "/proc/%d/mem", (int)child);
	int fd = open(path, O_RDONLY);
	if (fd < 0)
	{
		perror("open /proc/<pid>/mem");
		return (-1);
	}

	ssize_t nread = pread(fd, buf, buflen, (long int)addr);
	close(fd);

	if (nread <= 0)
		return (nread);

	buf[buflen - 1] = '\0';
	buf[strcspn(buf, "\0")] = '\0';
	return (nread);
}

void	print_syscall_args(pid_t pid, struct user_regs_struct regs)
{
	long			scno;
	t_syscall_info	syscall_info;
	long long		regs_values[6] = {regs.rdi, regs.rsi, regs.rdx, regs.r10, regs.r8, regs.r9};

	scno = regs.orig_rax;
	syscall_info = get_syscall_info(scno);

	printf("%s(", syscall_info.name);

	for (int i = 0; i < syscall_info.argc; i++)
	{
		if (i > 0)
			printf(", ");
		
		switch (syscall_info.arg_types[i])
		{
			case INT:
				printf("%lld", regs_values[i]);
				break;
			case UINT:

				printf("%llu", regs_values[i]);
				break;
			case STR:
				if (regs_values[i] == 0)
					printf("NULL");
				else
				{
					char str_buf[256];
					if (read_string_procfs(pid, regs_values[i], str_buf, sizeof(str_buf)) < 0)
					{
						perror("read_string_procfs");
						printf("ERROR");
					}
					else
						printf("\"%s\"", str_buf);
				}
				break;
			case PTR:
				if (regs_values[i] == 0)
					printf("NULL");
				else
					printf("%p", (void *)regs_values[i]);
				break;
			case STRUCT:
				printf("{STRUCT at %p}", (void *)regs_values[i]);
				break;
			default:
				printf("unknown");
				break;
		}
	}

	printf(") ");
}

void	print_single_syscall(pid_t pid, bool is_enter)
{
	struct user_regs_struct regs;
	struct iovec iov = {
		.iov_base = &regs,
		.iov_len  = sizeof(regs),
	};

	if (ptrace(PTRACE_GETREGSET, pid, (void*)NT_PRSTATUS, &iov) < 0)
	{
		perror("ptrace(GETREGSET)");
		ft_exit_message("Failed to get registers for PID %d", pid);
	}

	if (is_enter)
		print_syscall_args(pid, regs);
	else
		printf("= %llu\n", regs.rax);
}