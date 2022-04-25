/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser2_bonus.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbridget <cbridget@student.21-school.ru    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/13 14:43:25 by cbridget          #+#    #+#             */
/*   Updated: 2022/04/25 13:03:48 by cbridget         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers_bonus.h"

int	ft_atoi(const char *str, int *overflow)
{
	size_t			i;
	int				sign;
	unsigned long	result;

	result = 0;
	sign = 1;
	i = skipp(str, &sign, overflow);
	while (str[i] >= '0' && str[i] <= '9')
	{
		result = result * 10 + (str[i] - '0');
		i ++;
		if (result > 2147483650)
			break ;
	}
	if (result > 2147483647 && sign == 1)
		*overflow = 1;
	if (result > 2147483648 && sign == -1)
		*overflow = 1;
	return ((int)result * sign);
}

int	skipp(const char *str, int *sign, int *overflow)
{
	int	i;

	i = 0;
	while (str[i] == ' ' || str[i] == '\t' || str[i] == '\n'
		|| str[i] == '\v' || str[i] == '\f' || str[i] == '\r')
		i ++;
	if (str[i] == '+' || str[i] == '-')
	{
		if (str[i] == '-')
			*sign = -1;
		i ++;
		*overflow = 2;
	}
	return (i);
}

int	ft_init(int argc, char **argv, t_envph *envph, t_th_phil **phils)
{
	if (parser(argc, argv, envph))
		return (3);
	*phils = (t_th_phil *)malloc(sizeof(t_th_phil) * envph->num_phils);
	if (!*phils)
		return (1);
	gettimeofday(&envph->tv, NULL);
	envph->start_t = (envph->tv.tv_sec * 1000) + (envph->tv.tv_usec / 1000);
	sem_unlink("forks");
	sem_unlink("mess");
	if (envph->num_time_eat != -555)
	{
		sem_unlink("everyone_ate");
		envph->ev_ate = sem_open("everyone_ate", O_CREAT, S_IRWXG, 0);
		if (envph->ev_ate == SEM_FAILED)
			ft_close(*phils, 5);
	}
	envph->forks = sem_open("forks", O_CREAT, S_IRWXG, envph->num_phils);
	envph->mess = sem_open("mess", O_CREAT, S_IRWXG, 1);
	if (envph->forks == SEM_FAILED || envph->mess == SEM_FAILED)
		ft_close(*phils, 5);
	return (0);
}
