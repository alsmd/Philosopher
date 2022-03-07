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

int	start_eating(t_philo *philo)
{
	long int		start_eating;
	long int		 finish_eating;

	message(EATING, philo);
	set_time(&start_eating);
	set_time(&finish_eating);
	while (finish_eating - start_eating <= philo->data->time_to_eat)
		set_time(&finish_eating);
	set_time(&philo->last_meal);
	pthread_mutex_unlock(philo->fork);
	pthread_mutex_unlock(philo->left->fork);
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
	pthread_mutex_lock(philo->last_meal_locker);
	if ((get_miliseconds(current_time) - philo->last_meal >= philo->data->time_to_die) && philo->last_meal != -1)
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
	while (1)
	{
		if (!philo->data->start_simulation)
			continue ;
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
	t_philo	*begin;

	begin = (t_philo *)p;
	while (begin)
	{
		if (!begin->data->start_simulation)
			continue ;
		check_is_dead(begin);
		if (begin->data->end_simulation)
			return (NULL);
		begin = begin->right;
	}
}
void	start_simulation(t_philo *philos)
{
	t_philo			*begin;
	pthread_t		info;
	int		first;

	begin = philos;
	first = 1;
	begin = philos;
	while (begin && (begin != philos || first))
	{
		pthread_create(&begin->thread, NULL, &lifespan, begin);
		begin = begin->right;
		first = 0;
	}
	pthread_create(&info, NULL, &death_checker, philos);
	set_time(&philos->data->start_simulation);
	first = 1;
	begin = philos;
	while (begin && (begin != philos || first))
	{
		pthread_join(begin->thread, NULL);
		begin = begin->right;
		first = 0;
	}
	pthread_join(info, NULL);
}
