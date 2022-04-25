/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ph_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbridget <cbridget@student.21-school.ru    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/25 12:06:49 by cbridget          #+#    #+#             */
/*   Updated: 2022/04/25 12:08:06 by cbridget         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

t_big_num	get_time(t_envph *envph)
{
	gettimeofday(&envph->tv, NULL);
	return ((envph->tv.tv_sec * 1000) + \
	(envph->tv.tv_usec / 1000) - envph->start_t);
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

void	*when_die(void *phils)
{
	t_th_phil	*tmp_phils;
	t_big_num	ntime;
	int			i;
	int			neat;

	i = 0;
	neat = 0;
	tmp_phils = phils;
	while (1)
	{
		ntime = get_time(tmp_phils[i].envph);
		while (i < tmp_phils->envph->num_phils)
		{
			if (when_die2(tmp_phils, ntime, &i, &neat))
				return (NULL);
		}
		if (i == neat)
			return (when_die3(tmp_phils));
		usleep(100);
		i = 0;
		neat = 0;
	}
}

void	*when_die3(t_th_phil *tmp_phils)
{
	pthread_mutex_lock(&tmp_phils->envph->mt_exit);
	tmp_phils->envph->exit = 1;
	pthread_mutex_unlock(&tmp_phils->envph->mt_exit);
	return (NULL);
}

int	when_die2(t_th_phil *tmp_phils, t_big_num ntime, int *i, int *neat)
{
	pthread_mutex_lock(&tmp_phils[*i].mt_whendie);
	if (tmp_phils[*i].when_die <= ntime)
	{
		pthread_mutex_unlock(&tmp_phils[*i].mt_whendie);
		pthread_mutex_lock(&tmp_phils->envph->mt_exit);
		tmp_phils->envph->exit = 1;
		pthread_mutex_unlock(&tmp_phils->envph->mt_exit);
		print_m(&tmp_phils[*i], "died", 0);
		return (1);
	}
	pthread_mutex_unlock(&tmp_phils[*i].mt_whendie);
	pthread_mutex_lock(&tmp_phils->envph->mt_eatnum);
	if (tmp_phils[*i].envph->num_time_eat != -555 \
	&& tmp_phils[*i].eat_num >= tmp_phils[*i].envph->num_time_eat)
		(*neat)++;
	pthread_mutex_unlock(&tmp_phils->envph->mt_eatnum);
	(*i)++;
	return (0);
}
