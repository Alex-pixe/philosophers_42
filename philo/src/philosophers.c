/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbridget <cbridget@student.21-school.ru    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/09 17:56:07 by cbridget          #+#    #+#             */
/*   Updated: 2022/04/09 18:26:55 by cbridget         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void*	testing(void *str)
{
	printf("str= %s\n", (char *)str);
	usleep(5000000);
	printf("after sleep\n");
	return NULL;
}

int	main(void)
{
	pthread_t	test1, test2;
	if (pthread_create(&test1, NULL, &testing, "Hello world"))
		return (1);
	if (pthread_create(&test2, NULL, &testing, "this is the second thread"))
		return (3);
	if (pthread_join(test1, NULL))
		return (2);
	if (pthread_join(test2, NULL))
		return (4);
	return (0);
}
