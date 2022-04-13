/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbridget <cbridget@student.21-school.ru    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/09 17:56:07 by cbridget          #+#    #+#             */
/*   Updated: 2022/04/13 19:14:28 by cbridget         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

/*typedef struct s_thread {
	pthread_t	test1[5];
	pthread_mutex_t mutex1;
	int x;
} t_thread;*/

void*	testing()
{
/*	printf("str= %s\n", (char *)str);
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
	return NULL;*/
	printf("thread has created\n");
	return NULL;
}

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

	i = 0;
	if (parser(argc, argv, &args))
		return (3);
	phils = (t_th_phil *)malloc(sizeof(t_th_phil) * args.num_phils);
	if (!phils)
		return (1);
	while (i < args.num_phils)
	{
		if (pthread_create(&phils[i].th_p, NULL, testing, NULL))
			return (1);
		i++;
	}
	i = 0;
	while (i < args.num_phils)
	{
		if (pthread_join(phils[i].th_p, NULL))
			return (2);
		i++;
	}
	return (0);
}
