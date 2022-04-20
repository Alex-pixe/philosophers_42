/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbridget <cbridget@student.21-school.ru    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/09 17:56:07 by cbridget          #+#    #+#             */
/*   Updated: 2022/04/20 18:26:21 by cbridget         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	main(int argc, char **argv)
{
	t_envph		envph;
	t_th_phil	*phils;
	int			i;

	i = -1;
	if (parser(argc, argv, &envph))
		return (3);
	phils = (t_th_phil *)malloc(sizeof(t_th_phil) * envph.num_phils);
	if (!phils)
		return (1);
	gettimeofday(&envph.tv, NULL);
	envph.start_t = (envph.tv.tv_sec * 1000) + (envph.tv.tv_usec / 1000);
	while (++i < envph.num_phils)
	{
		if (pthread_mutex_init(&phils[i].mut_frk, NULL))
			return (ft_close());
	}
	if (pthread_mutex_init(&envph.mt_mess, NULL))
		return (ft_close());
	i = 0;
	while (i < envph.num_phils)
	{
		phils[i].envph = &envph;
		phils[i].number = i + 1;
		if (envph.num_time_eat != -555)
			phils[i].eat_num = 0;
		phils[i].when_die = get_time(&envph) + envph.time_die;
		if (pthread_create(&phils[i].th_phil, NULL, th_phil, &phils[i]))
			return (1);
		i++;
	}
	if (pthread_create(&envph.th_die, NULL, when_die, phils))
		return (1);
	i = 0;
	while (i < envph.num_phils)
	{
		if (pthread_join(phils[i].th_phil, NULL))
			return (2);
		pthread_mutex_destroy(&phils[i].mut_frk);
		i++;
	}
	pthread_join(envph.th_die, NULL);
	return (0);
}

void	*when_die(void *phils)
{
	t_th_phil	*tmp_phils;
	t_big_num		ntime;
	int i;
	int	neat;

	i = 0;
	neat = 0;
	tmp_phils = phils;
	while (1)
	{
		while (i < tmp_phils->envph->num_phils)
		{
			ntime = get_time(tmp_phils[i].envph);
			if (tmp_phils[i].when_die <= ntime)
			{
				tmp_phils->envph->exit = 1;
				printf("%llu %d died\n", get_time(tmp_phils->envph), tmp_phils[i].number);
				return (NULL);
			}
			if (tmp_phils[i].envph->num_time_eat != -555 && tmp_phils[i].eat_num >= tmp_phils[i].envph->num_time_eat)
				neat++;
			i++;
		}
		if (i == neat)
		{
			tmp_phils->envph->exit = 1;
			return (NULL);
		}
		usleep(10);
		i = 0;
		neat = 0;
	}
}

void	*th_phil(void *phil)
{
	t_th_phil	*tmp_phil;

	tmp_phil = phil;
	if (tmp_phil->number % 2)
//		usleep(1500);
		ft_usleep(tmp_phil->envph->time_eat, tmp_phil);
//	else if (tmp_phil->number % 2)
//		ft_usleep(17000, tmp_phil);
	while (1)
	{
		if (tmp_phil->number == 1)
		{
			try_take_fork(&tmp_phil->mut_frk, tmp_phil->number, tmp_phil->envph);
			if (tmp_phil->envph->num_phils == 1)
			{
				pthread_mutex_unlock(&tmp_phil->mut_frk);
				return (NULL);
			}
			try_take_fork(&tmp_phil[tmp_phil->envph->num_phils - 1].mut_frk, tmp_phil->number, tmp_phil->envph);
		}
		else
		{
			try_take_fork(&(tmp_phil - 1)->mut_frk, tmp_phil->number, tmp_phil->envph);
			try_take_fork(&tmp_phil->mut_frk, tmp_phil->number, tmp_phil->envph);
		}
		if (tmp_phil->envph->exit)
		{
			sleeping_phil(tmp_phil);
			return (NULL);
		}
		if (tmp_phil->envph->num_time_eat != -555)
			tmp_phil->eat_num++;
		tmp_phil->when_die += tmp_phil->envph->time_die;
		printf("%llu %d is eating\n", get_time(tmp_phil->envph), tmp_phil->number);
		ft_usleep(tmp_phil->envph->time_eat, tmp_phil);
		sleeping_phil(tmp_phil);
		if (tmp_phil->envph->exit)
			return (NULL);
		printf("%llu %d is thinking\n", get_time(tmp_phil->envph), tmp_phil->number);
	}
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
	if (phil->envph->exit)
			return ;
	printf("%llu %d is sleeping\n", get_time(phil->envph), phil->number);
	ft_usleep(phil->envph->time_sleep, phil);
}

void	try_take_fork(pthread_mutex_t *mut_frk, int num, t_envph *envph)
{
	pthread_mutex_lock(mut_frk);
	if (envph->exit)
			return ;
	printf("%llu %d has taken a fork\n", get_time(envph), num);
}

t_big_num	get_time(t_envph *envph)
{
	gettimeofday(&envph->tv, NULL);
	return ((envph->tv.tv_sec * 1000) + (envph->tv.tv_usec / 1000) - envph->start_t);
}

void	ft_usleep(t_big_num ms, t_th_phil *phils)
{
	t_big_num	ms_end;
	t_big_num	ms_now;

	gettimeofday(&phils->envph->tv, NULL);
	ms_end = (phils->envph->tv.tv_sec * 1000) + \
	(phils->envph->tv.tv_usec / 1000) + ms;
	ms_now = (phils->envph->tv.tv_sec * 1000) + \
	(phils->envph->tv.tv_usec / 1000);
	while (ms_now < ms_end)
	{
		gettimeofday(&phils->envph->tv, NULL);
		ms_now = (phils->envph->tv.tv_sec * 1000) + \
		(phils->envph->tv.tv_usec / 1000);
		usleep(10);
	}
}

void	print_m(t_th_phil *phils, char *str, int flag)
{
	
}
