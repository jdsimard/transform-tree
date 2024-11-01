#include <stdio.h>
#include <stdlib.h>

#include "transform_tree\transform_tree.h"

// gcc .\example1.c -o example1.exe .\transform_tree\transform_tree.c


double myAbs(double val);
double myDouble(double val);
double myTriple(double val);
double mySquare(double val);
double pass(double val);

int main(void) {
    // will be used for setting each node's unique id
    int node_count = 0;

    

    transform_tree_node *root = create_transform_tree_node(node_count++,
                                                            NULL,
                                                            &pass,
                                                            &callback_print_io,
                                                            NULL);
    if (root == NULL) return -1;

    transform_tree_node *child_node = create_transform_tree_node(node_count++,
                                                                    root,
                                                                    &myAbs,
                                                                    &callback_print_io,
                                                                    NULL);
    if (child_node == NULL) return -1;

    child_node = create_transform_tree_node(node_count++,
                                                root,
                                                &myDouble,
                                                &callback_print_io,
                                                NULL);
    if (child_node == NULL) return -1;

    child_node = create_transform_tree_node(node_count++,
                                                root,
                                                &myTriple,
                                                &callback_print_io,
                                                NULL);
    if (child_node == NULL) return -1;
    
    child_node = create_transform_tree_node(node_count++,
                                                root->children[1],
                                                &myAbs,
                                                &callback_print_io,
                                                NULL);
    if (child_node == NULL) return -1;

    child_node = create_transform_tree_node(node_count++,
                                                root->children[1],
                                                &mySquare,
                                                &callback_print_io,
                                                NULL);
    if (child_node == NULL) return -1;

    double final_node_output = 0;
    child_node = create_transform_tree_node(node_count++,
                                                child_node,
                                                &myTriple,
                                                &callback_print_io_and_write_output_in_flag,
                                                &final_node_output);
    if (child_node == NULL) return -1;

    print_transform_tree(root);
    
    
   
    

    printf("\nWe have initialized the variable final_node_output = %.2f\n\n", final_node_output);
    double input_value = -9.5;
    compute_transform_tree(root, input_value);
    printf("\nAfter computing the transform tree, we have final_node_output = %.2f due to the assigned callback function\n", final_node_output);

    delete_transform_tree(&root);
    printf("\nThe tree has been deleted, and allocated memory freed\n\n");
    print_transform_tree(root);
}

double myAbs(double val) {
    return val >= 0 ? val : -1*val;
}

double myDouble(double val) {
    return 2 * val;
}

double myTriple(double val) {
    return 3 * val;
}

double mySquare(double val) {
    return val * val;
}

double pass(double val) {
    return val;
}
