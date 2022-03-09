#include <philo.h>

void	one_philo_routine(t_philo **philos)
{
	pthread_t		info;

	set_time(&philos[0]->data->start_simulation);
	set_time(&philos[0]->last_meal);
	message(TAKEN_FORK, philos[0]);
	pthread_create(&info, NULL, &death_checker, philos);
	pthread_join(info, NULL);
}

static void	lunche(t_philo *philo)
{
	pthread_mutex_lock(philo->fork);
	pthread_mutex_lock(philo->fork_right);
	message(TAKEN_FORK, philo);
	message(TAKEN_FORK, philo);
	pthread_mutex_lock(philo->last_meal_locker);
	set_time(&philo->last_meal);
	philo->n_meals += 1;
	pthread_mutex_unlock(philo->last_meal_locker);
	philo->is_eating = TRUE;
	message(EATING, philo);
	usleep(philo->data->time_to_eat * 1000);
	philo->is_eating = FALSE;
	pthread_mutex_unlock(philo->fork);
	pthread_mutex_unlock(philo->fork_right);
}

void	*lifespan(void *p)
{
	t_philo	*philo;
	int		n_meals;
	int		first;

	first = 1;
	philo = (t_philo *)p;
	while (!philo->data->end_simulation)
	{
		if (!philo->data->start_simulation)
			continue ;
		if (first)
		{
			if (philo->id % 2 == 0)
				usleep(5000);
			pthread_mutex_lock(philo->last_meal_locker);
			set_time(&philo->last_meal);
			pthread_mutex_unlock(philo->last_meal_locker);
		}
		first = 0;
		lunche(philo);
		if (philo->n_meals == philo->data->meals_must_eat)
			return (NULL);
		message(SLEEPING, philo);
		usleep(philo->data->time_to_sleep * 1000);
		message(THINKING, philo);
	}
}

void	*death_checker(void *p)
{
	t_philo	**philo;
	int		index;

	philo = (t_philo **)p;
	while (check_dissatisfaction(philo))
	{
		if (!philo[0]->data->start_simulation)
			continue;
		index = 0;
		while (philo[index])
		{
			if (philo[index]->is_eating == FALSE)
				check_is_dead(philo[index]);
			if (philo[index]->data->end_simulation)
				return (NULL);
			usleep(1000);
			index++;
		}
	}
}