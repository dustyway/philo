/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   queue.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pschneid <pschneid@student.42berl...>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 14:00:04 by pschneid          #+#    #+#             */
/*   Updated: 2025/04/11 19:56:30 by pschneid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "queue.h"

void	init_queue(t_queue **q, void (*del)(void *))
{
	*q = malloc(sizeof(t_queue));
	(*q)->front = NULL;
	(*q)->back = NULL;
	(*q)->size = 0;
	(*q)->del = del;
	pthread_mutex_init(&(*q)->mtx, NULL);
}

void	clear_queue(t_queue **q)
{
	ft_lstclear(&((*q)->front), (*q)->del);
	pthread_mutex_destroy(&(*q)->mtx);
	free(*q);
	*q = NULL;
}

void	enqueue(t_queue *queue, void *value)
{
	t_list	*new_node;

	pthread_mutex_lock(&queue->mtx);
	new_node = ft_lstnew(value);
	if (queue->back == NULL)
	{
		queue->front = new_node;
		queue->back = new_node;
	}
	else
	{
		queue->back->next = new_node;
		queue->back = new_node;
	}
	queue->size++;
	pthread_mutex_unlock(&queue->mtx);
}

void	*dequeue(t_queue *queue)
{
	void	*value;
	t_list	*head;

	if (queue->front == NULL)
		return (NULL);
	pthread_mutex_lock(&queue->mtx);
	head = queue->front;
	value = head->content;
	queue->front = queue->front->next;
	ft_lstdelone(head, NULL);
	if (queue->front == NULL)
		queue->back = NULL;
	queue->size--;
	pthread_mutex_unlock(&queue->mtx);
	return (value);
}

void	*dequeue_nth(t_queue *queue, size_t n)
{
	void	*value;
	t_list	*prev;
	t_list	*nth;

	if (n == 0) 
		return (dequeue(queue));
	pthread_mutex_lock(&queue->mtx);		
	if (queue->size < n + 1){
	    pthread_mutex_unlock(&queue->mtx);
	    return (NULL);
	}
	prev = lst_drop(queue->front, n - 1);
	nth = prev->next;
	value = nth->content;
	prev->next = nth->next;
	if (nth == queue->back)
		queue->back = prev;
	ft_lstdelone(nth, NULL);
	queue->size--;
	pthread_mutex_unlock(&queue->mtx);
	return (value);
}

void queue_iter(t_queue *queue, void (*f)(void *)) {
	pthread_mutex_lock(&queue->mtx);
	ft_lstiter(queue->front, f);
	pthread_mutex_unlock(&queue->mtx);
}

void	*queue_peek_n(t_queue *queue, size_t n)
{
    void *value;
	if (queue == NULL)
		return (NULL);
	pthread_mutex_lock(&queue->mtx);
	value = lst_nth(queue->front, n);
	pthread_mutex_unlock(&queue->mtx);
	return value;
}

size_t	queue_size(t_queue *queue)
{
    size_t size;
    pthread_mutex_lock(&queue->mtx);
    size = queue->size;
    pthread_mutex_unlock(&queue->mtx);
    return (size);
	
}
