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

void	print_arg_type(t_arg_type arg_type, pid_t pid, long long addr)
{
	char str_buf[256];

	switch (arg_type)
	{
		case INT:
			printf("%lld", addr);
			break;
		case UINT:
			printf("%llu", addr);
			break;
		case STR:
			if (addr == 0)
				printf("NULL");
			else
			{
				if (read_string_procfs(pid, addr, str_buf, sizeof(str_buf)) < 0)
					perror("read_string_procfs");
				else
					printf("\"%s\"", str_buf);
			}
			break;
		case PTR:
			if (addr == 0)
				printf("NULL");
			else
				printf("%p", (void *)addr);
			break;
		case STRUCT:
			printf("{STRUCT at %p}", (void *)addr);
			break;
		default:
			printf("unknown");
			break;
	}
}

void	print_syscall_args(t_syscall_info syscall_info, pid_t pid, struct user_regs_struct regs)
{
	long long		regs_values[6] = {regs.rdi, regs.rsi, regs.rdx, regs.r10, regs.r8, regs.r9};

	printf("%s(", syscall_info.name);

	for (int i = 0; i < syscall_info.argc; i++)
	{
		if (i > 0)
			printf(", ");
		
		print_arg_type(syscall_info.arg_types[i], pid, regs_values[i]);
	}

	printf(") ");
}

void	print_single_syscall(pid_t pid, bool is_enter)
{
	t_syscall_info			syscall_info;
	struct user_regs_struct regs;
	struct iovec			iov = {
		.iov_base = &regs,
		.iov_len  = sizeof(regs),
	};

	if (ptrace(PTRACE_GETREGSET, pid, (void*)NT_PRSTATUS, &iov) < 0)
	{
		perror("ptrace(GETREGSET)");
		ft_exit_message("Failed to get registers for PID %d", pid);
	}

	syscall_info = get_syscall_info(regs.orig_rax);

	if (is_enter)
		print_syscall_args(syscall_info, pid, regs);
	else
	{
		printf("= ");
		print_arg_type(syscall_info.ret_type, pid, regs.rax);
		printf("\n");
	}
}