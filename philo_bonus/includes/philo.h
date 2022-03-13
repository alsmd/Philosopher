/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: flda-sil <flda-sil@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/09 11:13:54 by flda-sil          #+#    #+#             */
/*   Updated: 2022/03/12 22:35:14 by flda-sil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H
# include <unistd.h>
# include <stdlib.h>
# include <pthread.h>
# include <sys/time.h>
# include <stdio.h>
# include <limits.h>
# include <stdio.h>

//Kill
# include <signal.h>
//Wait
# include <sys/types.h>
# include <sys/wait.h>
//Sema
# include <fcntl.h>           /* For O_* constants */
# include <sys/stat.h>        /* For mode constants */
# include <semaphore.h>

# define FALSE 0
# define TRUE 1
# define TAKEN_FORK "has taken a fork"
# define EATING "is eating"
# define SLEEPING "is sleeping"
# define THINKING "is thinking"
# define DIED "died"
# define SEM_FORK_AVAIABLE "/forks_avaiables"
# define SEM_PHILO_DEAD "/philo_dead"
# define SEM_PHILO_DEAD_LOG "/philo_dead_log"
# define SEM_PHILO_SATISFIED "/philo_satisfied"

typedef struct s_data
{
	sem_t			*forks;
	sem_t			*philo_is_dead;
	sem_t			*philo_log;
	sem_t			*philo_is_satisfied;
	long int		time_to_die;
	long int		time_to_eat;
	long int		time_to_sleep;
	long int		meals_must_eat;
	long int		start_simulation;
	int				n_philo;
}	t_data;

typedef struct s_philo
{
	int				id;
	long int		last_meal;
	sem_t			*last_meal_sem;
	sem_t			*died_sem;
	int				n_meals;
	int				is_eating;
	int				process_id;
	t_data			*data;
}	t_philo;

t_philo		**create_simulation(char *argv[]);
void		start_simulation(t_philo **philos);
void		message(char *action, t_philo *philo);
void		free_simulation(t_philo **philos);

//Routines
void		one_philo_routine(t_philo **philos);
void		*lifespan(void *p);

//Checker
void		*check_is_dead(void *p);
int			check_dissatisfaction(t_philo **philo);
void		check_simulation_end(t_philo **philos);

//Validate
int			validate(char *argv[]);

//utils
int			is_number(char c);
int			ft_strlen(char *str);
int			ft_atoi(const char *nptr);
void		*ft_calloc(size_t nmemb, size_t size);
void		set_time(long int *i);
long int	get_miliseconds(struct timeval t);
void		*unlock(pthread_mutex_t	*m);
char		*ft_itoa(int n);
int			ft_strncmp(const char *s1, const char *s2, size_t n);

#endif
