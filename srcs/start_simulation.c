#include <philo.h>


int	try_get_fork(t_philo *philo)
{
	if (philo->fork->__align == 0)
		pthread_mutex_lock(philo->fork);
	else
		return (0);
	if (philo->left && philo->left->fork->__align == 0)
	{
		pthread_mutex_lock(philo->left->fork);
		message(TAKEN_FORK, philo);
	}
	else
	{
		pthread_mutex_unlock(philo->fork);
		return (0);
	}
	return (1);
}

void	start_eating(t_philo *philo)
{
	struct	timeval start_eating;
	struct	timeval finish_eating;

	gettimeofday(&start_eating, NULL);
	gettimeofday(&finish_eating, NULL);
	message(EATING, philo);
	while (get_miliseconds(finish_eating) - get_miliseconds(start_eating) <= philo->data->time_to_eat)
		gettimeofday(&finish_eating, NULL);
	gettimeofday(&philo->last_meal, NULL);
	pthread_mutex_unlock(philo->fork);
	pthread_mutex_unlock(philo->left->fork);
}

void	check_is_dead(t_philo *philo)
{
	struct	timeval current_time;

	gettimeofday(&current_time, NULL);
	if ((get_miliseconds(current_time) - get_miliseconds(philo->last_meal) >= philo->data->time_to_die))
	{
		message(DIED, philo);
		philo->data->end_simulation = 1;
	}
}

void	*lifespan(void *p)
{
	t_philo	*philo;
	int		n_meals;
	int		first;

	first = 1;
	n_meals = 0;
	philo = (t_philo *)p;
	while (1)
	{
		if (!philo->data->start_simulation)
			continue ;
		if (first)
			gettimeofday(&philo->last_meal, NULL);
		first = 0;
		check_is_dead(philo);
		if (philo->data->end_simulation)
			break;
		if (!try_get_fork(philo))
			continue ;
		start_eating(philo);
		n_meals++;
		if (n_meals == philo->data->meals_must_eat)
			break;
		message(SLEEPING, philo);
		usleep(philo->data->time_to_sleep * 1000);
		message(THINKING, philo);
	}
}

void	start_simulation(t_philo *philos)
{
	t_philo	*begin;
	int		first;

	first = 1;
	begin = philos;
	while (begin && (begin != philos || first))
	{
		pthread_create(&begin->thread, NULL, &lifespan, begin);
		begin = begin->right;
		first = 0;
	}
	set_time(&philos->data->start_simulation);
	first = 1;
	begin = philos;
	while (begin && (begin != philos || first))
	{
		pthread_join(begin->thread, NULL);
		begin = begin->right;
		first = 0;
	}
}
