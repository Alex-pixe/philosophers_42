/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbridget <cbridget@student.21-school.ru    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/09 17:56:07 by cbridget          #+#    #+#             */
/*   Updated: 2022/04/15 15:23:27by cbridget         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers_bonus.h"

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
	sem_unlink("forks");
	sem_unlink("mess");
	envph.forks = sem_open("forks", O_CREAT, S_IRWXG, envph.num_phils);
	envph.mess = sem_open("mess", O_CREAT, S_IRWXG, 1);
	if (envph.forks == SEM_FAILED || envph.mess == SEM_FAILED)
		return (5);
	while (++i < envph.num_phils)
	{
		phils[i].envph = &envph;
		phils[i].number = i + 1;
		if (envph.num_time_eat != -555)
			phils[i].eat_num = 0;
		phils[i].when_die = get_time(&envph) + envph.time_die;
		phils[i].ph_p = fork();
		if (phils[i].ph_p == -1)
			return (7);
		if (phils[i].ph_p == 0)
			ph_proc(&phils[i]);
	}
	i = -1;
	int status;
	int err = 0;
	waitpid(-1, &status, 0);
//	if (WIFEXITED(status))
//	{
	err = WEXITSTATUS(status);
	kill(0, 2);
	sem_unlink("forks");
	sem_unlink("mess");
	free(phils);
//	}
//	sem_unlink("forks");
//	sem_unlink("mess");
	return (err);
}

void	ph_proc(t_th_phil *phils)
{
	printf("here num: %d\n", phils->number);
	if (sem_open("forks", 0) == SEM_FAILED || sem_open("mess", 0) == SEM_FAILED)
	{
		printf("fail num: %d\n", phils->number);
		ft_exit(phils, 5);
	}
	printf("here num: %d\n", phils->number);
	if (phils->number % 2)
		usleep(phils->envph->time_eat * 1000);
	while (1)
	{
		if (ph_dead(phils))
			ft_exit(phils, 0);
		if (sem_wait(phils->envph->forks))
			ft_exit(phils, 5);
		print_m(phils, "has taken a fork");
		if (ph_dead(phils))
			ft_exit(phils, 0);
		if (sem_wait(phils->envph->forks))
			ft_exit(phils, 5);
		print_m(phils, "has taken a fork");
		if (ph_dead(phils))
			ft_exit(phils, 0);
		phils->when_die += phils->envph->time_die;
		print_m(phils, "is eating");
		usleep(phils->envph->time_eat * 1000);
		if (ph_dead(phils))
			ft_exit(phils, 0);
		if (sem_post(phils->envph->forks) || sem_post(phils->envph->forks))
			ft_exit(phils, 0);
		if (ph_dead(phils))
			ft_exit(phils, 0);
		print_m(phils, "is sleeping");
		usleep(phils->envph->time_sleep * 1000);
		if (ph_dead(phils))
			ft_exit(phils, 0);
		print_m(phils, "is thinking");
	}
//	if (sem_close(phils->envph->forks) || sem_close(phils->envph->mess))
//		ft_exit(phils, 5);
	ft_exit(phils, 0);
}

void	ft_exit(t_th_phil *phils, int err)
{
	sem_close(phils->envph->forks);
	sem_close(phils->envph->mess);
//	free(phils);
	exit(err);
}

int	ph_dead(t_th_phil *phils)
{
	if (get_time(phils->envph) >= phils->when_die)
	{
		print_m(phils, "died");
		return (1);
	}
	return (0);
}

void	print_m(t_th_phil *phils, char *str)
{
	if (sem_wait(phils->envph->mess))
		exit(7);
	printf("%llu %d %s\n", get_time(phils->envph), phils->number, str);
	if (sem_post(phils->envph->mess))
		exit(7);
}

big_num	get_time(t_envph *envph)
{
	gettimeofday(&envph->tv, NULL);
	return ((envph->tv.tv_sec * 1000) + (envph->tv.tv_usec / 1000) - envph->start_t);
}
