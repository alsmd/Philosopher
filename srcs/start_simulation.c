#include <philo.h>


int	try_get_fork(t_philo *philo)
{
	pthread_mutex_lock(philo->fork);
	message(TAKEN_FORK, philo);
	pthread_mutex_lock(philo->fork_right);
	message(TAKEN_FORK, philo);

	philo->is_eating = TRUE;
	pthread_mutex_lock(philo->last_meal_locker);
	set_time(&philo->last_meal);
	message(EATING, philo);
	philo->is_eating = FALSE;
	philo->n_meals += 1;
	pthread_mutex_unlock(philo->last_meal_locker);

	usleep(philo->data->time_to_eat * 1000);
	pthread_mutex_unlock(philo->fork);
	pthread_mutex_unlock(philo->fork_right);
	message(SLEEPING, philo);
	usleep(philo->data->time_to_sleep * 1000);
	return (1);
}

int	start_eating(t_philo *philo)
{
	
	return (0);
}

void	check_is_dead(t_philo *philo)
{
	struct	timeval current_time;

	gettimeofday(&current_time, NULL);
	pthread_mutex_lock(philo->last_meal_locker);
	if (philo->last_meal && (get_miliseconds(current_time) - philo->last_meal > philo->data->time_to_die) && philo->n_meals != philo->data->meals_must_eat)
	{
		pthread_mutex_unlock(philo->last_meal_locker);
		message(DIED, philo);
		pthread_mutex_lock(philo->data->end_simulation_lock);
		philo->data->end_simulation = 1;
		pthread_mutex_unlock(philo->data->end_simulation_lock);
	}
	else
		pthread_mutex_unlock(philo->last_meal_locker);
}

void	*lifespan(void *p)
{
	t_philo	*philo;
	int		n_meals;
	int		first;

	first = 1;
	philo = (t_philo *)p;
	if (philo->id % 2 == 0)
		usleep(5000);
	while (!philo->data->end_simulation)
	{
		if (first)
		{
			pthread_mutex_lock(philo->last_meal_locker);
			set_time(&philo->last_meal);
			pthread_mutex_unlock(philo->last_meal_locker);
		}
		first = 0;
		if (!try_get_fork(philo))
			continue ;
		message(THINKING, philo);
	}
}

int	check_dissatisfaction(t_philo **philo)
{
	int	index;

	if (philo[0]->data->meals_must_eat == -1)
		return (1);
	index = 0;
	while (philo[index])
	{
		pthread_mutex_lock(philo[index]->last_meal_locker);
		if (philo[index]->n_meals < philo[index]->data->meals_must_eat)
		{
			pthread_mutex_unlock(philo[index]->last_meal_locker);
			return (1);
		}
		pthread_mutex_unlock(philo[index]->last_meal_locker);
		index++;
	}
	pthread_mutex_lock(philo[0]->data->end_simulation_lock);
	philo[0]->data->end_simulation = 1;
	pthread_mutex_unlock(philo[0]->data->end_simulation_lock);
	return (0);
}

void	*death_checker(void *p)
{
	t_philo	**philo;
	int		index;

	philo = (t_philo **)p;
	while (check_dissatisfaction(philo))
	{
		index = 0;
		while (philo[index])
		{
			if (philo[index]->is_eating == FALSE)
				check_is_dead(philo[index]);
			if (philo[index]->data->end_simulation)
				return (NULL);
			index++;
		}
	}

}
void	start_simulation(t_philo **philos)
{
	int				index;
	pthread_t		info;

	index = 0;
	if (philos[1] == 0)
	{
		set_time(&philos[0]->data->start_simulation);
		pthread_mutex_lock(philos[0]->last_meal_locker);
		set_time(&philos[0]->last_meal);
		pthread_mutex_unlock(philos[0]->last_meal_locker);
		message(TAKEN_FORK, philos[0]);
		pthread_create(&info, NULL, &death_checker, philos);
		pthread_join(info, NULL);
	}
	else
	{
		set_time(&philos[0]->data->start_simulation);
		while (philos[index])
		{
			pthread_create(&philos[index]->thread, NULL, &lifespan, philos[index]);
			index++;
		}
		pthread_create(&info, NULL, &death_checker, philos);
		index = 0;
		while (philos[index])
		{
			pthread_join(philos[index]->thread, NULL);
			index++;
		}
		pthread_join(info, NULL);
	}
	
}
