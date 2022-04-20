/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers_bonus.h                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbridget <cbridget@student.21-school.ru    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/09 17:47:24 by cbridget          #+#    #+#             */
/*   Updated: 2022/04/20 16:22:36 by cbridget         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_BONUS_H
# define PHILOSOPHERS_BONUS_H

# include <stdio.h>
# include <stdlib.h>
# include <pthread.h>
# include <unistd.h>
# include <sys/time.h>
# include <sys/stat.h>
# include <fcntl.h>
# include <semaphore.h>
# include <signal.h>

typedef unsigned long long	t_big_num;

typedef struct s_envph {
	int					num_phils;
	int					time_die;
	int					time_eat;
	int					time_sleep;
	int					num_time_eat;
	t_big_num			start_t;
	struct timeval		tv;
	pthread_t			th_ate;
	pthread_mutex_t		mt_ate;
	sem_t				*forks;
	sem_t				*mess;
	sem_t				*ev_ate;
}	t_envph;

typedef struct s_th_phil {
	int					number;
	pid_t				ph_p;
	t_big_num			when_die;
	pthread_t			th_die;
	pthread_mutex_t		mt_die;
	t_envph				*envph;
}	t_th_phil;

int			parser(int argc, char **argv, t_envph *envph);
int			ft_error(int err);
int			verify_argv2(char **nums, int *i, int *j);
int			verify_argv(int argc, char **argv);
int			ft_isdigit(int c);
int			ft_atoi(const char *str, int *overflow);
int			skipp(const char *str, int *sign, int *overflow);
void		ph_proc(t_th_phil *phils);
t_big_num	get_time(t_envph *envph);
void		*when_die(void *tmp_phil);
int			ph_dead(t_th_phil *phils);
void		print_m(t_th_phil *phils, char *str, int flag);
void		ft_usleep(t_big_num ms, t_th_phil *phils);
void		ft_close(t_th_phil *phils, int err);
void		*check_ev(void	*tmp_phils);
int			ft_init(int argc, char **argv, t_envph *envph, t_th_phil **phils);
void		th_ate(t_th_phil *phils);
void		phil_is_eating(t_th_phil *phils, int *ph_ate);

#endif
