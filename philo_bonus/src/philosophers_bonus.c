/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers_bonus.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbridget <cbridget@student.21-school.ru    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/09 17:56:07 by cbridget          #+#    #+#             */
/*   Updated: 2022/04/19 20:54:58 by cbridget         ###   ########.fr       */
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
	if (envph.num_time_eat != -555)
	{
		sem_unlink("all_eat");
		envph.all_eat = sem_open("all_eat", O_CREAT, S_IRWXG, 0);
		if (envph.all_eat == SEM_FAILED)
		{
			printf("fail\n");
			return (5);
		}
		while (++i < envph.num_phils)
			sem_wait(envph.all_eat);
		while (++i < envph.num_phils)
			sem_post(envph.all_eat);
		sem_wait(envph.all_eat);
		printf("succes\n");
		return (0);
	}
	envph.exit = 0;
	envph.forks = sem_open("forks", O_CREAT, S_IRWXG, envph.num_phils);
	envph.mess = sem_open("mess", O_CREAT, S_IRWXG, 1);
	envph.all_eat = sem_open("all_eat", O_CREAT, S_IRWXG, -envph.num_phils);
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
	if (envph.num_time_eat != 555)
	{
		while (++i < envph.num_phils)
			sem_wait(envph.all_eat);
		i = -1;
	}
	else
	{
		waitpid(-1, &status, 0);
		err = WEXITSTATUS(status);
	}
	while (++i < envph.num_phils)
		kill(phils[i].ph_p, 2);
	sem_close(phils->envph->forks);
	sem_close(phils->envph->forks);
	sem_unlink("forks");
	sem_unlink("mess");
	free(phils);
	return (0);
}

void	ph_proc(t_th_phil *phils)
{
	if (pthread_create(&phils->th_die, NULL, when_die, phils))
		ft_exit(phils, 5);
	if (phils->number % 2)
		ft_usleep(phils->envph->time_eat, phils);
	while (1)
	{
		if (sem_wait(phils->envph->forks))
			ft_exit(phils, 5);
		print_m(phils, "has taken a fork", 1);
		if (sem_wait(phils->envph->forks))
			ft_exit(phils, 5);
		print_m(phils, "has taken a fork", 1);
		phils->when_die += phils->envph->time_die;
		print_m(phils, "is eating", 1);
		ft_usleep(phils->envph->time_eat, phils);
		if (sem_post(phils->envph->forks) || sem_post(phils->envph->forks))
			ft_exit(phils, 5);
		print_m(phils, "is sleeping", 1);
		ft_usleep(phils->envph->time_sleep, phils);
		print_m(phils, "is thinking", 1);
	}
	ft_exit(phils, 0);
}

void	ft_usleep(big_num ms, t_th_phil *phils)
{
	big_num	ms_end;
	big_num	ms_now;

	gettimeofday(&phils->envph->tv, NULL);
	ms_end = (phils->envph->tv.tv_sec * 1000) + (phils->envph->tv.tv_usec / 1000) + ms;
	ms_now = (phils->envph->tv.tv_sec * 1000) + (phils->envph->tv.tv_usec / 1000);
	while (ms_now < ms_end)
	{
		gettimeofday(&phils->envph->tv, NULL);
		ms_now = (phils->envph->tv.tv_sec * 1000) + (phils->envph->tv.tv_usec / 1000);
		usleep(10);
	}
}

void	ft_exit(t_th_phil *phils, int err)
{
	pthread_join(phils->th_die, NULL);
	exit(err);
}

void	*when_die(void *tmp_phil)
{
	t_th_phil	*phil;

	phil = tmp_phil;
	while (1)
	{
		if (phil->when_die <= get_time(phil->envph))
		{
			phil->envph->exit = 1;
			print_m(phil, "dead", 0);
			ft_exit(phil, 0);
			break ;
		}
		usleep(10);
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

big_num	get_time(t_envph *envph)
{
	gettimeofday(&envph->tv, NULL);
	return ((envph->tv.tv_sec * 1000) + (envph->tv.tv_usec / 1000) - envph->start_t);
}
