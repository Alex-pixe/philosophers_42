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
		envph.ph_p = fork();
		if (envph.ph_p == -1)
			return (7);
		if (envph.ph_p == 0)
			ph_proc();
	}
	return (0);
}
