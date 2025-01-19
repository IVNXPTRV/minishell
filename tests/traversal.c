/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   traversal.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ipetrov <ipetrov@student.42bangkok.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/15 19:28:51 by ipetrov           #+#    #+#             */
/*   Updated: 2025/01/15 20:05:07 by ipetrov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell/include/minishell.h"

// Post-order traversal (left, right, root) with applying a function when coming back
void traverse(t_node *root)
{
    if (root == NULL)//base case
	{
		return ;
	}
	else//recursicve case
	{
		// Apply the function to the node when step into
        applyFunction(root);

        // Traverse left subtree
        traverse(root->left);
		// Apply the function to the node when left leaf visited
		applyFunction(root);

        // Traverse right subtree
       	traverse(root->right);
        // Apply the function to the node when coming back and right visited
        applyFunction(root); //cleaning when coming back use redir node to restore FD
    }
}