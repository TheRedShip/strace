/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: TheRed <TheRed@students.42.fr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/06 16:00:10 by TheRed            #+#    #+#             */
/*   Updated: 2025/06/06 16:00:10 by TheRed           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdio.h>

int main(int argc, char **argv)
{
	getpid();
	getpid();
	getpid();
	write(1, "Hello, World!\n", 14);
	getpid();

	return (0);
}