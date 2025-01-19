/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   testcase.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ipetrov <ipetrov@student.42bangkok.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/15 16:22:11 by ipetrov           #+#    #+#             */
/*   Updated: 2025/01/15 17:20:28 by ipetrov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell/include/minishell.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// Function to allocate a node
t_node *alloc_node(t_token *token) {
    t_node *node = malloc(sizeof(t_node));
    if (!node) {
        perror("Failed to allocate node");
        return NULL;
    }

    node->token = malloc(sizeof(t_token));
    if (!node->token) {
        perror("Failed to allocate token");
        free(node);
        return NULL;
    }

    memcpy(node->token, token, sizeof(t_token));
    node->left = NULL;
    node->right = NULL;

    return node;
}

// Function to create a binary tree from the token list
t_node *build_tree(t_token *list, int size) {
    if (size == 0 || list == NULL) {
        return NULL;
    }

    t_node *root = alloc_node(&list[0]);
    t_node *current = root;

    for (int i = 1; i < size; i++) {
        if (list[i].content == NULL) { // NULL indicates moving up the tree
            if (current->right == NULL && current != root) {
                current = current->right; // Move to parent's right
            }
        } else {
            t_node *new_node = alloc_node(&list[i]);
            if (current->left == NULL) {
                current->left = new_node;
            } else if (current->right == NULL) {
                current->right = new_node;
            }
            current = new_node; // Move down the tree
        }
    }

    return root;
}

// Function to generate Mermaid code
void generate_mermaid(t_node *node, FILE *file) {
    if (!node) return;

    if (node->left) {
        fprintf(file, "    \"%s\" --> \"%s\";\n", node->token->content, node->left->token->content);
        generate_mermaid(node->left, file);
    }

    if (node->right) {
        fprintf(file, "    \"%s\" --> \"%s\";\n", node->token->content, node->right->token->content);
        generate_mermaid(node->right, file);
    }
}

// Free the tree recursively
void free_tree(t_node *node) {
    if (!node) return;
    free_tree(node->left);
    free_tree(node->right);
    free(node->token);
    free(node);
}

// Test function
int init_testtree1() {
    t_token list[] = {
        {"|", PIPE, 0},
        {"cat", ARG, 0},
        {"<<", REDIR_HEREDOC, 0},
        {"EOF", ARG, 0},
        {"|", PIPE, 0},
        {"ls", ARG, 0},
        {"$VAR1", ARG, 0},
        {"/", ARG, 0},
        {"|", PIPE, 0},
        {"echo", ARG, 0},
        {"$ENV2 | ls | cat", ARG, 1},
        {"|", PIPE, 0},
        {"cat", ARG, 0},
        {NULL, 0, 0} // End of list
    };

    int size = sizeof(list) / sizeof(list[0]) - 1; // Exclude NULL terminator
    t_node *tree = build_tree(list, size);

    // Open file for writing Mermaid diagram
    FILE *file = fopen("tree.mermaid", "w");
    if (!file) {
        perror("Failed to open file");
        free_tree(tree);
        return 1;
    }

    fprintf(file, "graph TD;\n");
    generate_mermaid(tree, file);
    fclose(file);

    printf("Mermaid diagram saved to tree.mermaid\n");

    free_tree(tree);
    return 0;
}

int main() {
    init_testtree1();
    return 0;
}

