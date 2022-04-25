/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ph_threads.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbridget <cbridget@student.21-school.ru    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/25 11:55:38 by cbridget          #+#    #+#             */
/*   Updated: 2022/04/25 11:56:47 by cbridget         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	*th_phil(void *phil)
{
	t_th_phil	*tmp_phil;

	tmp_phil = phil;
	if (tmp_phil->number % 2)
		ft_usleep(tmp_phil->envph->time_eat, tmp_phil);
	while (1)
	{
		if (!choose_forks(tmp_phil))
			return (NULL);
		if (tmp_phil->envph->num_time_eat != -555)
		{
			pthread_mutex_lock(&tmp_phil->envph->mt_eatnum);
			tmp_phil->eat_num++;
			pthread_mutex_unlock(&tmp_phil->envph->mt_eatnum);
		}
		th_phil2(tmp_phil);
		if (tmp_phil->envph->exit)
		{
			pthread_mutex_unlock(&tmp_phil->envph->mt_exit);
			return (NULL);
		}
		pthread_mutex_unlock(&tmp_phil->envph->mt_exit);
		print_m(phil, "is thinking", 1);
	}
	return (NULL);
}

void	th_phil2(t_th_phil	*tmp_phil)
{
	pthread_mutex_lock(&tmp_phil->mt_whendie);
	tmp_phil->when_die += tmp_phil->envph->time_die;
	pthread_mutex_unlock(&tmp_phil->mt_whendie);
	print_m(tmp_phil, "is eating", 1);
	ft_usleep(tmp_phil->envph->time_eat, tmp_phil);
	sleeping_phil(tmp_phil);
	pthread_mutex_lock(&tmp_phil->envph->mt_exit);
}

void	*choose_forks(t_th_phil	*tmp_phil)
{
	if (tmp_phil->number == 1)
	{
		try_take_fork(&tmp_phil->mut_frk, tmp_phil);
		if (tmp_phil->envph->num_phils == 1)
		{
			pthread_mutex_unlock(&tmp_phil->mut_frk);
			return (NULL);
		}
		try_take_fork(&tmp_phil[tmp_phil->envph->num_phils - 1].mut_frk, \
		tmp_phil);
	}
	else
	{
		try_take_fork(&(tmp_phil - 1)->mut_frk, tmp_phil);
		try_take_fork(&tmp_phil->mut_frk, tmp_phil);
	}
	pthread_mutex_lock(&tmp_phil->envph->mt_exit);
	if (tmp_phil->envph->exit)
	{
		pthread_mutex_unlock(&tmp_phil->envph->mt_exit);
		sleeping_phil(tmp_phil);
		return (NULL);
	}
	pthread_mutex_unlock(&tmp_phil->envph->mt_exit);
	return (tmp_phil);
}

void	sleeping_phil(t_th_phil	*phil)
{
	if (phil->number == 1)
	{
		pthread_mutex_unlock(&phil[phil->envph->num_phils - 1].mut_frk);
		pthread_mutex_unlock(&phil->mut_frk);
	}
	else
	{
		pthread_mutex_unlock(&phil->mut_frk);
		pthread_mutex_unlock(&(phil - 1)->mut_frk);
	}
	pthread_mutex_lock(&phil->envph->mt_exit);
	if (phil->envph->exit)
	{
		pthread_mutex_unlock(&phil->envph->mt_exit);
		return ;
	}
	pthread_mutex_unlock(&phil->envph->mt_exit);
	print_m(phil, "is sleeping", 1);
	ft_usleep(phil->envph->time_sleep, phil);
}

void	try_take_fork(pthread_mutex_t *mut_frk, t_th_phil *phils)
{
	pthread_mutex_lock(mut_frk);
	pthread_mutex_lock(&phils->envph->mt_exit);
	if (phils->envph->exit)
	{
		pthread_mutex_unlock(&phils->envph->mt_exit);
		return ;
	}
	pthread_mutex_unlock(&phils->envph->mt_exit);
	print_m(phils, "has taken a fork", 1);
}
