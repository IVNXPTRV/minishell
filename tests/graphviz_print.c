#include <stdio.h>
#include <stdlib.h>

// Define the structures
typedef enum e_token_type {
    TOKEN_UNKNOWN,
    TOKEN_WORD,
    TOKEN_OPERATOR
} t_token_type;

typedef enum e_quotes_type {
    QUOTE_NONE,
    QUOTE_SINGLE,
    QUOTE_DOUBLE
} t_quotes_type;

typedef struct s_token {
    char *content;
    t_token_type type;
    t_quotes_type quotes; // 0 - no quotes, 1 - ', 2 - "
    struct s_token *next;
} t_token;

typedef struct s_node {
    t_token *token;
    struct s_node *left;
    struct s_node *right;
} t_node;

// Function to escape quotes for Graphviz output
void escape_quotes(char *str, char *buffer) {
    while (*str) {
        if (*str == '"' || *str == '\\') {
            *buffer++ = '\\';
        }
        *buffer++ = *str++;
    }
    *buffer = '\0';
}

// Function to generate Graphviz DOT representation
void generate_graphviz(FILE *file, t_node *root, int *node_counter) {
    if (!root) {
        return;
    }

    int current_node = (*node_counter)++;

    // Write the current node
    char escaped_content[256] = "";
    if (root->token && root->token->content) {
        escape_quotes(root->token->content, escaped_content);
        fprintf(file, "    node%d [label=\"%s\"]\n", current_node, escaped_content);
    } else {
        fprintf(file, "    node%d [label=\"(null)\"]\n", current_node);
    }

    // Process left child
    if (root->left) {
        int left_node = *node_counter;
        generate_graphviz(file, root->left, node_counter);
        fprintf(file, "    node%d -> node%d\n", current_node, left_node);
    }

    // Process right child
    if (root->right) {
        int right_node = *node_counter;
        generate_graphviz(file, root->right, node_counter);
        fprintf(file, "    node%d -> node%d\n", current_node, right_node);
    }
}

// Wrapper function to create the DOT file
void export_to_graphviz(t_node *root, const char *filename) {
    FILE *file = fopen(filename, "w");
    if (!file) {
        perror("Failed to open file");
        return;
    }

    fprintf(file, "digraph BinaryTree {\n");
    fprintf(file, "    node [shape=circle]\n");

    int node_counter = 0;
    generate_graphviz(file, root, &node_counter);

    fprintf(file, "}\n");
    fclose(file);
}

// Example usage
int main() {
    // Create a sample tree manually for testing
    t_token token1 = {"root", TOKEN_WORD, QUOTE_NONE, NULL};
    t_token token2 = {"left", TOKEN_WORD, QUOTE_NONE, NULL};
    t_token token3 = {"right", TOKEN_WORD, QUOTE_NONE, NULL};

    t_node left_node = {&token2, NULL, NULL};
    t_node right_node = {&token3, NULL, NULL};
    t_node root_node = {&token1, &left_node, &right_node};

    export_to_graphviz(&root_node, "tree.dot");

    printf("DOT file 'tree.dot' has been created. Use Graphviz to visualize it.\n");
    return 0;
}
