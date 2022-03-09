/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: flda-sil <flda-sil@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/09 11:12:13 by flda-sil          #+#    #+#             */
/*   Updated: 2022/03/09 11:20:54 by flda-sil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <philo.h>

long int	get_miliseconds(struct timeval t)
{
	return ((t.tv_usec / 1000) + (t.tv_sec * 1000));
}

void	set_time(long int *i)
{
	struct timeval	tmp;

	gettimeofday(&tmp, NULL);
	*i = get_miliseconds(tmp);
}
