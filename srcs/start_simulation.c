#include <philo.h>


int	try_get_fork(t_philo *philo)
{
	if (philo->fork->__align == 0)
		pthread_mutex_lock(philo->fork);
	else
		return (0);
	if (philo->fork_right->__align == 0)
	{
		pthread_mutex_lock(philo->fork_right);
		message(TAKEN_FORK, philo);
	}
	else
	{
		pthread_mutex_unlock(philo->fork);
		return (0);
	}
	return (1);
}

int	start_eating(t_philo *philo)
{
	message(EATING, philo);
	usleep(philo->data->time_to_eat * 1000);
	pthread_mutex_lock(philo->last_meal_locker);
	set_time(&philo->last_meal);
	pthread_mutex_unlock(philo->last_meal_locker);
	pthread_mutex_unlock(philo->fork);
	pthread_mutex_unlock(philo->fork_right);
	philo->n_meals++;
	if (philo->n_meals == philo->data->meals_must_eat)
		return (1);
	message(SLEEPING, philo);
	usleep(philo->data->time_to_sleep * 1000);
	return (0);
}

void	check_is_dead(t_philo *philo)
{
	struct	timeval current_time;

	gettimeofday(&current_time, NULL);
	//pthread_mutex_lock(philo->last_meal_locker);
	if ((get_miliseconds(current_time) - philo->last_meal >= philo->data->time_to_die))
	{
//		pthread_mutex_unlock(philo->last_meal_locker);
		message(DIED, philo);
		pthread_mutex_lock(philo->data->end_simulation_lock);
		philo->data->end_simulation = 1;
		pthread_mutex_unlock(philo->data->end_simulation_lock);
	}
//	else
//		pthread_mutex_unlock(philo->last_meal_locker);
}

void	*lifespan(void *p)
{
	t_philo	*philo;
	int		n_meals;
	int		first;

	first = 1;
	philo = (t_philo *)p;
	while (1)
	{
		if (!philo->data->start_simulation)
			continue ;
		if (first)
		{
			pthread_mutex_lock(philo->last_meal_locker);
			set_time(&philo->last_meal);
			pthread_mutex_unlock(philo->last_meal_locker);
		}
		/* if (philo->id % 2 == 0)
			usleep(5000); */
		check_is_dead(philo);
		if (philo->data->end_simulation)
			break;
		first = 0;
		if (!try_get_fork(philo))
			continue ;
		if (start_eating(philo))
			break ;
		message(THINKING, philo);
	}
}

void	*death_checker(void *p)
{
	t_philo	**philo;
	int		index;

	index = 0;
	philo = (t_philo **)p;
	while (1)
	{
		if (!philo[index]->data->start_simulation)
			continue ;
		if (philo[index]->last_meal);
			check_is_dead(philo[index]);
		if (philo[index]->data->end_simulation)
			return (NULL);
		index++;
		if (philo[index] == NULL)
			index = 0;
	}
}
void	start_simulation(t_philo **philos)
{
	int				index;
	pthread_t		info;

	index = 0;
	while (philos[index])
	{
		pthread_create(&philos[index]->thread, NULL, &lifespan, philos[index]);
		index++;
	}
	set_time(&philos[0]->data->start_simulation);
	//pthread_create(&info, NULL, &death_checker, philos);
	index = 0;
	while (philos[index])
	{
		pthread_join(philos[index]->thread, NULL);
		index++;
	}
	//pthread_join(info, NULL);
}
