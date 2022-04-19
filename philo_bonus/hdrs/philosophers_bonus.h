/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers_bonus.h                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbridget <cbridget@student.21-school.ru    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/09 17:47:24 by cbridget          #+#    #+#             */
/*   Updated: 2022/04/19 20:38:51 by cbridget         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H

# include <stdio.h>
# include <stdlib.h>
# include <pthread.h>
# include <unistd.h>
# include <sys/time.h>
# include <sys/stat.h>
# include <fcntl.h>
# include <semaphore.h>
# include <signal.h>

typedef unsigned long long big_num;

typedef struct s_envph {
	int	num_phils;
	int	time_die;
	int	time_eat;
	int	time_sleep;
	int	num_time_eat;
	int	exit;
	big_num	start_t;
	struct timeval	tv;
//	pthread_t	th_die;
//	pid_t	tmp_proc;
	sem_t *forks;
	sem_t *mess;
	sem_t *all_eat;
//	pid_t	ph_p;
} t_envph;

typedef struct s_th_phil {
	int				number;
	int				eat_num;
	pid_t			ph_p;
	big_num			when_die;
	pthread_t		th_die;
	t_envph			*envph;
} t_th_phil;

int	parser(int argc, char **argv, t_envph *envph);
int	ft_error(int err);
int	verify_argv2(char **nums, int *i, int *j);
int	verify_argv(int argc, char **argv);
int	ft_isdigit(int c);
int	ft_atoi(const char *str, int *overflow);
int	skipp(const char *str, int *sign, int *overflow);

void	ph_proc(t_th_phil *phils);
big_num	get_time(t_envph *envph);
void	*when_die(void *tmp_phil);
void	ft_exit(t_th_phil *phils, int err);
int	ph_dead(t_th_phil *phils);
void	print_m(t_th_phil *phils, char *str, int flag);
void	ft_usleep(big_num ms, t_th_phil *phils);

# endif
