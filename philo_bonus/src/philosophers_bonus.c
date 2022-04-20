/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers_bonus.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbridget <cbridget@student.21-school.ru    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/09 17:56:07 by cbridget          #+#    #+#             */
/*   Updated: 2022/04/20 16:26:48 by cbridget         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers_bonus.h"

int	main(int argc, char **argv)
{
	t_envph		envph;
	t_th_phil	*phils;
	int			i;
	int			status;

	i = -1;
	if (ft_init(argc, argv, &envph, &phils))
		return (1);
	while (++i < envph.num_phils)
	{
		phils[i].envph = &envph;
		phils[i].number = i + 1;
		phils[i].when_die = get_time(&envph) + envph.time_die;
		phils[i].ph_p = fork();
		if (phils[i].ph_p == -1)
			ft_close(phils, 9);
		if (phils[i].ph_p == 0)
			ph_proc(&phils[i]);
	}
	if (envph.num_time_eat != -555)
		th_ate(phils);
	waitpid(-1, &status, 0);
	i = WEXITSTATUS(status);
	ft_close(phils, i);
	return (0);
}

void	th_ate(t_th_phil *phils)
{
	if (pthread_create(&phils->envph->th_ate, NULL, check_ev, phils))
		ft_close(phils, 7);
	if (pthread_detach(phils->envph->th_ate))
		ft_close(phils, 7);
}

void	ft_close(t_th_phil *phils, int err)
{
	int	i;

	i = -1;
	while (++i < phils->envph->num_phils)
		kill(phils[i].ph_p, 2);
	sem_close(phils->envph->forks);
	sem_close(phils->envph->forks);
	sem_unlink("forks");
	sem_unlink("mess");
	if (phils)
	{
		free(phils);
		phils = NULL;
	}
	exit(err);
}

void	*check_ev(void	*tmp_phils)
{
	t_th_phil	*phils;
	int			i;

	i = 0;
	phils = tmp_phils;
	while (i < phils->envph->num_phils)
	{
		sem_wait(phils->envph->ev_ate);
		i++;
	}
	sem_close(phils->envph->ev_ate);
	sem_unlink("everyone_ate");
	ft_close(phils, 0);
	return ((void *)0);
}

t_big_num	get_time(t_envph *envph)
{
	gettimeofday(&envph->tv, NULL);
	return ((envph->tv.tv_sec * 1000) + \
	(envph->tv.tv_usec / 1000) - envph->start_t);
}
