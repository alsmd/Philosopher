#include <philo.h>

long int	get_miliseconds(struct timeval t)
{
	return ((t.tv_usec / 1000) + (t.tv_sec * 1000));
}

void	set_time(long int *i)
{
	struct	timeval tmp;

	gettimeofday(&tmp, NULL);
	*i = get_miliseconds(tmp);
}
