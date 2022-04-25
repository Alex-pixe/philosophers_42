/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbridget <cbridget@student.21-school.ru    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/09 17:56:07 by cbridget          #+#    #+#             */
/*   Updated: 2022/04/25 12:13:28 by cbridget         ###   ########.fr       */
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
