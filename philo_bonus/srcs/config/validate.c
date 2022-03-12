/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validate.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: flda-sil <flda-sil@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/09 11:12:38 by flda-sil          #+#    #+#             */
/*   Updated: 2022/03/12 11:49:08 by flda-sil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <philo.h>

int	validate(char *argv[])
{
	int	index;
	int	x;

	index = 0;
	while (argv[index])
	{
		if (ft_strlen(argv[index]) > 10)
			return (1);
		x = 0;
		while (argv[index][x])
		{
			if (!is_number(argv[index][x]))
				return (1);
			x++;
		}
		index++;
	}
	return (0);
}

void	*unlock(pthread_mutex_t	*m)
{
	pthread_mutex_unlock(m);
	return (NULL);
}
