#include <philo.h>

t_log			*logs = 0;
struct	timeval start_simulation;

long int	get_miliseconds(struct timeval t)
{
	return ((t.tv_usec / 1000) + (t.tv_sec * 1000));
}

void	add_log(char *name, t_status status, long int log_time)
{
	t_log	*begin;
	t_log	*new_log;

	new_log = calloc(1, sizeof(t_log));
	new_log->name = name;
	new_log->status = status;
	new_log->log_time = log_time;
	begin = logs;
	if (begin == 0)
	{
		logs = new_log;
		return ;
	}
	while (begin->next)
		begin = begin->next;
	begin->next = new_log;
}

void	*lifespan(void *p)
{
	struct	timeval start_eating;
	struct	timeval finish_eating;
	t_philo	*philo;

	philo = (t_philo *) p;
	while (1)
	{
		gettimeofday(&start_eating, NULL);
		if (get_miliseconds(start_eating) - get_miliseconds(philo->last_meal) >= philo->time_to_die)
		{
			printf("%s ficous %ld milisegundos sem comer\n", philo->name, get_miliseconds(start_eating) - get_miliseconds(philo->last_meal));
			add_log(philo->name, DEAD, get_miliseconds(start_eating) - get_miliseconds(start_simulation));
			pthread_mutex_lock(philo->fork);
			return (NULL);
		}
		if (philo->fork->__align == 0)
			pthread_mutex_lock(philo->fork);
		else
			continue ;
		if (philo->left->fork->__align == 0)
			pthread_mutex_lock(philo->left->fork);
		else
		{
			pthread_mutex_unlock(philo->fork);
			continue ;
		}
		gettimeofday(&start_eating, NULL);
		gettimeofday(&finish_eating, NULL);
		//printf("%s ficous %ld milisegundos sem comer\n", philo->name, get_miliseconds(start_eating) - get_miliseconds(philo->last_meal));
		add_log(philo->name, EATING, get_miliseconds(start_eating) - get_miliseconds(start_simulation));
		//printf("%s comeu em %ld\n", philo->name, get_miliseconds(start_eating) - get_miliseconds(start_simulation));
		while (get_miliseconds(finish_eating) - get_miliseconds(start_eating) <= philo->time_to_eat)
		{
			gettimeofday(&finish_eating, NULL);
		}
		gettimeofday(&philo->last_meal, NULL);
		pthread_mutex_unlock(philo->fork);
		pthread_mutex_unlock(philo->left->fork);
		usleep(1000);
}
}
void	*show_logs()
{
	t_log	*tmp;
	while (1)
	{
		while (logs)
		{
			if (EATING == logs->status)
				printf("%ld %s is eating\n", logs->log_time, logs->name);
			if (DEAD == logs->status)
				printf("%ld %s died\n", logs->log_time, logs->name);
			tmp = logs;
			logs = logs->next;
			free(tmp);
		}
	}
}

int	main(int argc, char *argv[])
{
	t_philo			philo_one;
	t_philo			philo_two;
	t_philo			philo_three;

	philo_one.name = "Kant";
	philo_two.name = "João";
	philo_three.name = "Pedro";
	philo_one.fork = calloc(1, sizeof(pthread_mutex_t));
	philo_two.fork = calloc(1, sizeof(pthread_mutex_t));
	philo_three.fork = calloc(1, sizeof(pthread_mutex_t));


	philo_one.left = &philo_two;
	philo_one.right = &philo_three;

	philo_two.left = &philo_three;
	philo_two.right = &philo_one;

	philo_three.left = &philo_one;
	philo_three.right = &philo_two;


	philo_one.time_to_die = 2000;
	philo_one.time_to_eat = 500;
	
	philo_two.time_to_die = 2000;
	philo_two.time_to_eat = 500;

	philo_three.time_to_die = 2000;
	philo_three.time_to_eat = 500;

	gettimeofday(&start_simulation, NULL);
	gettimeofday(&philo_one.last_meal, NULL);
	gettimeofday(&philo_two.last_meal, NULL);
	gettimeofday(&philo_three.last_meal, NULL);

	pthread_t	log;

	pthread_create(&philo_three.thread, NULL, &lifespan, &philo_three);
	pthread_create(&philo_one.thread, NULL, &lifespan, &philo_one);
	pthread_create(&philo_two.thread, NULL, &lifespan, &philo_two);


	pthread_create(&log, NULL, &show_logs, NULL);
	pthread_join(log, NULL);
	pthread_join(philo_one.thread, NULL);
	pthread_join(philo_two.thread, NULL);
	pthread_join(philo_three.thread, NULL);
	return (0);
}

