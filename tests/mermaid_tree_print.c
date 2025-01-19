#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct s_token
{
    char *content;
    int type;
    int quotes; // 0 - no quotes, 1 - ', 2 - "
    struct s_token *next;
} t_token;

typedef struct s_node
{
    t_token *token;
    struct s_node *left;
    struct s_node *right;
} t_node;

// Helper function to generate a unique identifier for a node
void generate_node_id(t_node *node, char *buffer, size_t buffer_size)
{
    snprintf(buffer, buffer_size, "%s", node->token ? node->token->content : "null");
}

// Recursive function to traverse the binary tree and generate Mermaid graph syntax
void generate_mermaid(t_node *node, FILE *output)
{
    if (!node)
        return;

    char parent_id[256];
    generate_node_id(node, parent_id, sizeof(parent_id));

    if (node->left)
    {
        char left_id[256];
        generate_node_id(node->left, left_id, sizeof(left_id));
        fprintf(output, "    %s((%s))-->%s((%s))\n", parent_id, parent_id, left_id, left_id);
        generate_mermaid(node->left, output);
    }

    if (node->right)
    {
        char right_id[256];
        generate_node_id(node->right, right_id, sizeof(right_id));
        fprintf(output, "    %s((%s))-->%s((%s))\n", parent_id, parent_id, right_id, right_id);
        generate_mermaid(node->right, output);
    }
}

// Helper function to create a new node
t_node *create_node(const char *content)
{
    t_token *token = (t_token *)malloc(sizeof(t_token));
    token->content = strdup(content);
    token->type = 0;
    token->quotes = 0;
    token->next = NULL;

    t_node *node = (t_node *)malloc(sizeof(t_node));
    node->token = token;
    node->left = NULL;
    node->right = NULL;
    return node;
}

// Free the allocated memory for the tree
void free_tree(t_node *node)
{
    if (!node)
        return;
    free_tree(node->left);
    free_tree(node->right);
    free(node->token->content);
    free(node->token);
    free(node);
}

int main()
{
    // Create a sample binary tree
    t_node *root = create_node("1");
    root->left = create_node("2");
    root->right = create_node("3");
    root->left->left = create_node("4");
    root->left->right = create_node("5");
    root->right->left = create_node("6");
    root->right->right = create_node("7");
    root->left->left->left = create_node("8");
    root->left->left->right = create_node("9");
    root->left->right->left = create_node("10");

    // Open output file for the Mermaid diagram
    FILE *output = fopen("tree_diagram.md", "w");
    if (!output)
    {
        perror("Failed to open output file");
        free_tree(root);
        return 1;
    }

    // Write the Mermaid graph header
    fprintf(output, "graph TB\n");

    // Generate the Mermaid graph
    generate_mermaid(root, output);

    // Close the file
    fclose(output);

    // Free the allocated memory
    free_tree(root);

    printf("Mermaid diagram generated in 'tree_diagram.mmd'\n");
    return 0;
}
