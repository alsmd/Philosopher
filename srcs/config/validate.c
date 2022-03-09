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
