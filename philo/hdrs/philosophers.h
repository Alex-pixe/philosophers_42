/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbridget <cbridget@student.21-school.ru    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/09 17:47:24 by cbridget          #+#    #+#             */
/*   Updated: 2022/04/20 18:22:33 by cbridget         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H

# include <stdio.h>
# include <stdlib.h>
# include <pthread.h>
# include <unistd.h>
# include <sys/time.h>

typedef unsigned long long t_big_num;

typedef struct s_envph {
	int	num_phils;
	int	time_die;
	int	time_eat;
	int	time_sleep;
	int	num_time_eat;
	int	exit;
	t_big_num	start_t;
	struct timeval	tv;
	pthread_t	th_die;
	pthread_mutex_t	mt_mess;
} t_envph;

typedef struct s_th_phil {
	pthread_t		th_phil;
	pthread_mutex_t	mut_frk;
	int				number;
	int				eat_num;
	t_big_num			when_die;
	t_envph			*envph;
} t_th_phil;

int	parser(int argc, char **argv, t_envph *envph);
int	ft_error(int err);
int	verify_argv2(char **nums, int *i, int *j);
int	verify_argv(int argc, char **argv);
int	ft_isdigit(int c);
int	ft_atoi(const char *str, int *overflow);
int	skipp(const char *str, int *sign, int *overflow);

void	*th_phil(void *phil);
void	sleeping_phil(t_th_phil	*phil);
void	try_take_fork(pthread_mutex_t *mut_frk, int num, t_envph *envph);
t_big_num	get_time(t_envph *envph);
void	*when_die(void *phils);
void	ft_usleep(t_big_num ms, t_th_phil *phils);

# endif
