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

#include "philosophers.h"

/*typedef struct s_thread {
	pthread_t	test1[5];
	pthread_mutex_t mutex1;
	int x;
} t_thread;

void*	testing()
{
	printf("str= %s\n", (char *)str);
	printf("procces is %d\n", getpid());
	usleep(500000);
	printf("after sleep\n");
	int i = -1;
	while (++i < 10000000)
	{
		pthread_mutex_lock(&((t_thread*)envm)->mutex1);
		((t_thread*)envm)->x++;
		pthread_mutex_unlock(&((t_thread*)envm)->mutex1);
	}
	return NULL;
	printf("thread has created\n");
	return NULL;
}*/

int	main(int argc, char **argv)
{
/*	t_thread	envm;
	envm.x = 0;
	int i = 0;
	pthread_mutex_init(&(envm.mutex1), NULL);
	while (i < 5)
	{
		if (pthread_create(&(envm.test1[i]), NULL, testing, &envm))
			return (1);
		i++;
	}
	i = 0;
	while (i < 5)
	{
		if (pthread_join(envm.test1[i], NULL))
			return (2);
		i++;
	}
	pthread_mutex_destroy(&(envm.mutex1));
	printf("x = %d\n", envm.x);*/
	t_args		args;
	t_th_phil	*phils;
	int			i;

	i = -1;
	if (parser(argc, argv, &args))
		return (3);
	phils = (t_th_phil *)malloc(sizeof(t_th_phil) * args.num_phils);
	if (!phils)
		return (1);
	gettimeofday(&args.tv, NULL);
	args.start_t = (args.tv.tv_sec * 1000) + (args.tv.tv_usec / 1000);
	while (++i < args.num_phils)
		pthread_mutex_init(&phils[i].mut_frk, NULL);
	i = 0;
	while (i < args.num_phils)
	{
		phils[i].args = &args;
		phils[i].number = i + 1;
		printf("init mutes: %ld, num: %d\n", phils[i].mut_frk.__sig, phils[i].number);
//		pthread_mutex_init(&phils[i].mut_frk, NULL);
		if (pthread_create(&phils[i].th_phil, NULL, th_phil, &phils[i]))
			return (1);
//		pthread_mutex_init(&phils[i].mut_frk, NULL);
		i++
	}
	i = 0;
	while (i < args.num_phils)
	{
		if (pthread_join(phils[i].th_phil, NULL))
			return (2);
		pthread_mutex_destroy(&phils[i].mut_frk);
		i++;
	}
	return (0);
}

void	*th_phil(void *phil)
{
	t_th_phil	*tmp_phil;

	tmp_phil = phil;
	while (1)
	{
		if (tmp_phil->number == 1)
		{
			try_take_fork(&tmp_phil->mut_frk, tmp_phil->number, tmp_phil->args);
//			printf("lock num: %d\n", tmp_phil->number);
			try_take_fork(&tmp_phil[tmp_phil->args->num_phils - 1].mut_frk, tmp_phil->number, tmp_phil->args);
//			printf("lock num: %d\n", tmp_phil[tmp_phil->args->num_phils - 1].number);
		}
		else
		{
			try_take_fork(&(tmp_phil - 1)->mut_frk, tmp_phil->number, tmp_phil->args);
//			printf("lock num: %d\n", (tmp_phil - 1)->number);
			try_take_fork(&tmp_phil->mut_frk, tmp_phil->number, tmp_phil->args);
//			printf("lock num: %d\n", tmp_phil->number);
		}
		printf("%llu %d is eating\n", get_time(tmp_phil->args), tmp_phil->number);
		usleep(tmp_phil->args->time_eat * 1000);
		sleeping_phil(tmp_phil);
		printf("%llu %d is thinking\n", get_time(tmp_phil->args), tmp_phil->number);
	}
}

void	sleeping_phil(t_th_phil	*phil)
{
	if (phil->number == 1)
	{
		pthread_mutex_unlock(&phil[phil->args->num_phils - 1].mut_frk);
//		printf("unlock num: %d\n", phil[phil->args->num_phils - 1].number);
		pthread_mutex_unlock(&phil->mut_frk);
//		printf("unlock num: %d\n", phil->number);
	}
	else
	{
		pthread_mutex_unlock(&phil->mut_frk);
//		printf("unlock num: %d\n", phil->number);
		pthread_mutex_unlock(&(phil - 1)->mut_frk);
//		printf("unlock num: %d\n", (phil - 1)->number);
	}
	printf("%llu %d is sleeping\n", get_time(phil->args), phil->number);
	usleep(phil->args->time_sleep * 1000);
}

void	try_take_fork(pthread_mutex_t *mut_frk, int num, t_args *args)
{
//	printf("befor lock num: %d\n", num);
	pthread_mutex_lock(mut_frk);
//	printf("lock num: %d\n", num);
	printf("%llu %d has taken a fork\n", get_time(args), num);
}

unsigned long long	get_time(t_args *args)
{
	gettimeofday(&args->tv, NULL);
	return ((args->tv.tv_sec * 1000) + (args->tv.tv_usec / 1000) - args->start_t);
}
