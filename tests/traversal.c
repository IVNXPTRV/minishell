/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   traversal.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ipetrov <ipetrov@student.42bangkok.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/15 19:28:51 by ipetrov           #+#    #+#             */
/*   Updated: 2025/01/20 13:14:09 by ipetrov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell/include/minishell.h"

// Post-order traversal (left, right, root) with applying a function when coming back
void traverse(t_node *root, void **args)
{
	void *param;

    if (root == NULL)//base case
	{
		return ;
	}
	else//recursicve case
	{
		// Apply the function to the node when step into
        applyFunction(root, &param);

        // Traverse left subtree
        traverse(root->left, args);
		// Apply the function to the node when left leaf visited
		applyFunction(root, &param);

        // Traverse right subtree
       	traverse(root->right, args);
        // Apply the function to the node when coming back and right visited
        applyFunction(root, &param); //cleaning when coming back use redir node to restore FD
    }
}