/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ph_process_bonus.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbridget <cbridget@student.21-school.ru    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/20 15:11:31 by cbridget          #+#    #+#             */
/*   Updated: 2022/04/25 13:03:37 by cbridget         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers_bonus.h"

void	ph_proc(t_th_phil *phils)
{
	int	ph_ate;

	ph_ate = 0;
	if (pthread_create(&phils->th_die, NULL, when_die, phils))
		exit(5);
	if (pthread_detach(phils->th_die))
		exit(5);
	if (pthread_mutex_init(&phils->mt_die, NULL))
		exit(5);
	if (phils->number % 2)
		ft_usleep(phils->envph->time_eat, phils);
	while (1)
	{
		phil_is_eating(phils, &ph_ate);
		if (sem_post(phils->envph->forks) || sem_post(phils->envph->forks))
			exit(5);
		print_m(phils, "is sleeping", 1);
		ft_usleep(phils->envph->time_sleep, phils);
		print_m(phils, "is thinking", 1);
	}
	exit(0);
}

void	phil_is_eating(t_th_phil *phils, int *ph_ate)
{
	if (sem_wait(phils->envph->forks))
		exit(5);
	print_m(phils, "has taken a fork", 1);
	if (sem_wait(phils->envph->forks))
		exit(5);
	print_m(phils, "has taken a fork", 1);
	pthread_mutex_lock(&phils->mt_die);
	phils->when_die += phils->envph->time_die;
	pthread_mutex_unlock(&phils->mt_die);
	print_m(phils, "is eating", 1);
	if (phils->envph->num_time_eat != -555 \
		&& *ph_ate < phils->envph->num_time_eat)
		(*ph_ate)++;
	if (phils->envph->num_time_eat != -555 \
		&& *ph_ate == phils->envph->num_time_eat)
	{
		if (sem_post(phils->envph->ev_ate))
			exit(5);
		(*ph_ate)++;
	}
	ft_usleep(phils->envph->time_eat, phils);
}

void	ft_usleep(t_big_num ms, t_th_phil *phils)
{
	t_big_num	ms_end;
	t_big_num	ms_now;

	gettimeofday(&phils->envph->tv, NULL);
	ms_now = (phils->envph->tv.tv_sec * 1000) + \
	(phils->envph->tv.tv_usec / 1000);
	ms_end = ms_now + ms;
	while (ms_now < ms_end)
	{
		gettimeofday(&phils->envph->tv, NULL);
		ms_now = (phils->envph->tv.tv_sec * 1000) + \
		(phils->envph->tv.tv_usec / 1000);
		usleep(100);
	}
}

void	*when_die(void *tmp_phil)
{
	t_th_phil	*phil;

	phil = tmp_phil;
	while (1)
	{
		pthread_mutex_lock(&phil->mt_die);
		if (phil->when_die <= get_time(phil->envph))
		{
			print_m(phil, "died", 0);
			exit(0);
			break ;
		}
		pthread_mutex_unlock(&phil->mt_die);
		usleep(100);
	}
	return (NULL);
}

void	print_m(t_th_phil *phils, char *str, int flag)
{
	if (sem_wait(phils->envph->mess))
		exit(7);
	printf("%llu %d %s\n", get_time(phils->envph), phils->number, str);
	if (flag)
	{
		if (sem_post(phils->envph->mess))
			exit(7);
	}
}
