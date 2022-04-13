/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbridget <cbridget@student.21-school.ru    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/09 17:47:24 by cbridget          #+#    #+#             */
/*   Updated: 2022/04/13 19:13:29 by cbridget         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H

# include <stdio.h>
# include <stdlib.h>
# include <pthread.h>
# include <unistd.h>

typedef struct s_args {
	int	num_phils;
	int	time_die;
	int	time_eat;
	int	time_sleep;
	int	num_time_eat;
} t_args;

typedef struct s_th_phil {
	pthread_t	th_p;
} t_th_phil;

int	parser(int argc, char **argv, t_args *args);
int	ft_error(int err);
int	verify_argv2(char **nums, int *i, int *j);
int	verify_argv(int argc, char **argv);
int	ft_isdigit(int c);
int	ft_atoi(const char *str, int *overflow);
int	skipp(const char *str, int *sign, int *overflow);

# endif
