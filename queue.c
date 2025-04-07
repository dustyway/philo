/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   queue.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pschneid <pschneid@student.42berl...>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 14:00:04 by pschneid          #+#    #+#             */
/*   Updated: 2025/04/07 21:43:51 by pschneid         ###   ########.fr       */
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

	new_node = ft_lstnew(value);
	pthread_mutex_lock(&queue->mtx);
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
	if (queue->size < n + 1)
		return (NULL);
	pthread_mutex_lock(&queue->mtx);
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
