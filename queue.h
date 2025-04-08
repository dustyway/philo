/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   queue.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pschneid <pschneid@student.42berl...>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 14:03:09 by pschneid          #+#    #+#             */
/*   Updated: 2025/04/08 22:23:50 by pschneid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <pthread.h>
#include <stdlib.h>

typedef struct s_list
{
	void			*content;
	struct s_list	*next;
}					t_list;

typedef struct s_queue
{
	t_list			*front;
	t_list			*back;
	size_t			size;
	int				updated;
	void			(*del)(void *);
	pthread_mutex_t	mtx;
}					t_queue;

t_list				*ft_lstnew(void *content);
void				ft_lstdelone(t_list *lst, void (*del)(void *));
void				ft_lstclear(t_list **lst, void (*del)(void *));
t_list				*lst_drop(t_list *lst, size_t n);
void				*lst_nth(t_list *lst, size_t n);
void	ft_lstiter(t_list *lst, void (*f)(void *));
void				init_queue(t_queue **q, void (*del)(void *));
void	clear_queue(t_queue **q);
int					queue_is_empty(t_queue *queue);
void				enqueue(t_queue *queue, void *value);
void				*dequeue(t_queue *queue);
void				*dequeue_nth(t_queue *queue, size_t n);
void				*queue_peek_n(t_queue *q, size_t n);
void				queue_set_updated(t_queue *queue, int value);
int					queue_check_updated(t_queue *queue);
void queue_iter(t_queue *queue, void (*f)(void *));
