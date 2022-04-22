/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbridget <cbridget@student.21-school.ru    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/09 17:56:07 by cbridget          #+#    #+#             */
/*   Updated: 2022/04/22 18:21:57 by cbridget         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	main(int argc, char **argv)
{
	t_envph		envph;
	t_th_phil	*phils;

	phils = ph_init(argc, argv, &envph);
	if (!phils)
		return (3);
	if (create_ths(phils, &envph))
		return (3);
	return (ph_destroy(phils));
}

int	create_ths(t_th_phil *phils, t_envph *envph)
{
	int	i;

	i = 0;
	while (i < envph->num_phils)
	{
		phils[i].envph = envph;
		phils[i].number = i + 1;
		if (envph->num_time_eat != -555)
			phils[i].eat_num = 0;
		phils[i].when_die = get_time(envph) + envph->time_die;
		if (pthread_create(&phils[i].th_phil, NULL, th_phil, &phils[i]))
		{
			free(phils);
			return (1);
		}
		i++;
	}
	if (pthread_create(&envph->th_die, NULL, when_die, phils))
	{
		free(phils);
		return (1);
	}
	return (0);
}

int	ph_destroy(t_th_phil *phils)
{
	int	i;

	i = 0;
	while (i < phils->envph->num_phils)
	{
		if (pthread_join(phils[i].th_phil, NULL))
		{
			free(phils);
			return (5);
		}
		pthread_mutex_destroy(&phils[i].mut_frk);
		pthread_mutex_destroy(&phils[i].mt_whendie);
		i++;
	}
	i = 0;
	if (pthread_join(phils->envph->th_die, NULL))
		i = 5;
	pthread_mutex_destroy(&phils->envph->mt_mess);
	pthread_mutex_destroy(&phils->envph->mt_eatnum);
	pthread_mutex_destroy(&phils->envph->mt_exit);
	free(phils);
	return (i);
}

t_th_phil	*ph_init(int argc, char **argv, t_envph *envph)
{
	t_th_phil	*phils;
	int			i;

	i = -1;
	if (parser(argc, argv, envph))
		return (NULL);
	if (pthread_mutex_init(&envph->mt_mess, NULL) \
	|| pthread_mutex_init(&envph->mt_eatnum, NULL) \
	|| pthread_mutex_init(&envph->mt_exit, NULL))
		return (NULL);
	phils = (t_th_phil *)malloc(sizeof(t_th_phil) * envph->num_phils);
	if (!phils)
		return (NULL);
	gettimeofday(&envph->tv, NULL);
	envph->start_t = (envph->tv.tv_sec * 1000) + (envph->tv.tv_usec / 1000);
	while (++i < envph->num_phils)
	{
		if (pthread_mutex_init(&phils[i].mut_frk, NULL) \
		|| pthread_mutex_init(&phils[i].mt_whendie, NULL))
		{
			free(phils);
			return (NULL);
		}
	}
	return (phils);
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

void	print_m(t_th_phil *phils, char *str, int flag)
{
	pthread_mutex_lock(&phils->envph->mt_mess);
	pthread_mutex_lock(&phils->envph->mt_exit);
	if (phils->envph->exit && flag)
	{
		pthread_mutex_unlock(&phils->envph->mt_exit);
		pthread_mutex_unlock(&phils->envph->mt_mess);
		return ;
	}
	pthread_mutex_unlock(&phils->envph->mt_exit);
	printf("%llu %d %s\n", get_time(phils->envph), phils->number, str);
	pthread_mutex_unlock(&phils->envph->mt_mess);
}
