/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_list.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pschneid <pschneid@student.42berl...>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 20:24:58 by pschneid          #+#    #+#             */
/*   Updated: 2025/03/21 20:25:00 by pschneid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "queue.h"

t_list	*ft_lstnew(void *content)
{
	t_list	*new_node;

	new_node = (t_list *)malloc(sizeof(t_list));
	if (!new_node)
		return (NULL);
	new_node->content = content;
	new_node->next = NULL;
	return (new_node);
}

void	ft_lstdelone(t_list *lst, void (*del)(void *))
{
	if (!lst)
		return ;
	if (del)
		del(lst->content);
	free(lst);
}

void	ft_lstclear(t_list **lst, void (*del)(void *))
{
	t_list	*tmp;

	if (*lst == NULL)
		return ;
	while (*lst)
	{
		tmp = (*lst)->next;
		ft_lstdelone(*lst, del);
		*lst = tmp;
	}
	*lst = NULL;
}

t_list	*lst_drop(t_list *lst, size_t n)
{
	if (!lst)
		return (NULL);
	while (n > 0)
	{
		lst = lst->next;
		if (!lst)
			return (NULL);
		n--;
	}
	return (lst);
}
//
// void	*lst_peek_n(t_list *lst, size_t n)
// {
// 	t_list	*tail;
//
// 	if (lst == NULL)
// 		return (NULL);
// 	tail = lst_drop(lst, n);
// 	if (tail == NULL)
// 		return (NULL);
// 	return (tail->content);
// }

void	*lst_nth(t_list *lst, size_t n)
{
	if (!lst)
		return (NULL);
	while (n > 0)
	{
		lst = lst->next;
		if (!lst)
			return (NULL);
		n--;
	}
	return (lst->content);
}
