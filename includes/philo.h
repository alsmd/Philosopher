/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: flda-sil <flda-sil@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/09 11:13:54 by flda-sil          #+#    #+#             */
/*   Updated: 2022/03/09 11:13:55 by flda-sil         ###   ########.fr       */
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
# define FALSE 0
# define TRUE 1
# define TAKEN_FORK "has taken a fork"
# define EATING "is eating"
# define SLEEPING "is sleeping"
# define THINKING "is thinking"
# define DIED "died"

typedef struct s_data
{
	pthread_mutex_t	*message_lock;
	long int		time_to_die;
	long int		time_to_eat;
	long int		time_to_sleep;
	long int		meals_must_eat;
	long int		start_simulation;
	int				end_simulation;
	pthread_mutex_t	*end_simulation_lock;
}	t_data;

typedef struct s_philo
{
	int				id;
	char			*name;
	pthread_mutex_t	*last_meal_locker;
	pthread_mutex_t	*is_eating_locker;
	long int		last_meal;
	int				n_meals;
	int				is_eating;
	pthread_t		thread;
	pthread_mutex_t	*fork;
	pthread_mutex_t	*fork_right;
	t_data			*data;
}	t_philo;

t_philo		**create_simulation(char *argv[]);
void		start_simulation(t_philo **philos);
void		message(char *action, t_philo *philo);
void		free_simulation(t_philo **philos);

//Routines
void		one_philo_routine(t_philo **philos);
void		*lifespan(void *p);
void		*death_checker(void *p);

//Checker
int			check_is_dead(t_philo *philo);
int			check_dissatisfaction(t_philo **philo);

//Validate
int			validate(char *argv[]);

//utils
int			is_number(char c);
int			ft_strlen(char *str);
int			ft_atoi(const char *nptr);
void		*ft_calloc(size_t nmemb, size_t size);
void		set_time(long int *i);
long int	get_miliseconds(struct timeval t);

#endif
