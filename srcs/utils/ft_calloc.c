#include <philo.h>

void	*ft_calloc(size_t nmemb, size_t size)
{
	void	*r;
	int		index;

	r = malloc(nmemb * size);
	if (r)
	{
		index = 0;
		while (index < nmemb * size)
		{
			((char *)r)[index] = 0;
			index++;
		}
		return (r);
	}
	return (0);
}