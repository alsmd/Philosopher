#include <philo.h>

void	add_to_table(t_philo **list, t_data *data)
{
	static int	id;
	t_philo		*new_philo;
	t_philo		*begin;

	new_philo = ft_calloc(1, sizeof(t_philo));
	new_philo->fork = ft_calloc(1, sizeof(pthread_mutex_t));
	new_philo->last_meal_locker = ft_calloc(1, sizeof(pthread_mutex_t));
	pthread_mutex_init(new_philo->fork, NULL);
	pthread_mutex_init(new_philo->last_meal_locker, NULL);
	new_philo->data = data;
	new_philo->last_meal = -1;
	id++;
	new_philo->id = id;
	begin = *list;
	if (!begin)
		*list = new_philo;
	else
	{
		begin->left = new_philo;
		while (begin->right && begin->right != *list)
			begin = begin->right;
		begin->right = new_philo;
		new_philo->left = begin;
		new_philo->right = *list;
	}
}

t_philo	*create_simulation(char *argv[])
{
	t_philo	*philos;
	int		n;
	t_data	*data;

	philos = 0;
	n = ft_atoi(argv[0]);
	data = ft_calloc(1, sizeof(t_data));
	data->time_to_die = ft_atoi(argv[1]);
	data->time_to_eat = ft_atoi(argv[2]);
	data->time_to_sleep = ft_atoi(argv[3]);
	data->meals_must_eat = -1;
	data->message_lock = ft_calloc(1, sizeof(pthread_mutex_t));
	data->end_simulation_lock = ft_calloc(1, sizeof(pthread_mutex_t));
	pthread_mutex_init(data->message_lock, NULL);
	pthread_mutex_init(data->end_simulation_lock, NULL);
	if (argv[4])
		data->meals_must_eat = ft_atoi(argv[4]);
	while (n)
	{
		add_to_table(&philos, data);
		n--;
	}
	return (philos);
}
