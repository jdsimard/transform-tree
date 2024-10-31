#include <stdio.h>
#include <stdlib.h>

#include "transform_tree\transform_tree.h"

// gcc .\example.c -o example.exe .\transform_tree\transform_tree.c


double myAbs(double val);
double myDouble(double val);
double myTriple(double val);
double mySquare(double val);

double pass(double val);

int main(void) {
    // will be used for setting each node's unique id
    int node_count = 0;

    // create a new transform tree and get the root node
    // make its transform function 'pass' which just returns its input value
    transform_tree_node *head = new_transform_tree(&pass);
    if (head == NULL) return -1;
    // set the uid of the head node
    set_node_uid(head, node_count++);
    // give the head node a callback function that runs on node update when computing the transform tree
    set_node_callback_function(head, &callback_print_node_input_and_output, NULL);
    // Note that setting a uid is not required, and setting a callback function is not required

    // print some diagnostics for the head node
    print_transform_tree_node(head);
    printf("\n");
    // print a representation of the transform tree
    print_transform_tree(head);
    printf("\n");



    // create the first child node for head, and get a pointer to the child
    // make the child node's transform function the absolute value function
    transform_tree_node *child_node = new_child_node(head, &myAbs);
    if (child_node == NULL) return -1;
    set_node_uid(child_node, node_count++);
    set_node_callback_function(child_node, &callback_print_node_input_and_output, NULL);

    print_transform_tree_node(head);
    print_transform_tree_node(child_node);
    printf("\n");
    print_transform_tree(head);
    printf("\n");



    // create two more child nodes for head
    child_node = new_child_node(head, &myDouble);
    if (child_node == NULL) return -1;
    set_node_uid(child_node, node_count++);
    set_node_callback_function(child_node, &callback_print_node_input_and_output, NULL);

    child_node = new_child_node(head, &myTriple);
    if (child_node == NULL) return -1;
    set_node_uid(child_node, node_count++);
    set_node_callback_function(child_node, &callback_print_node_input_and_output, NULL);

    print_transform_tree(head);


    // give the head's second child two child nodes
    child_node = new_child_node(head->children[1], &myAbs);
    if (child_node == NULL) return -1;
    set_node_uid(child_node, node_count++);
    set_node_callback_function(child_node, &callback_print_node_input_and_output, NULL);

    child_node = new_child_node(head->children[1], &mySquare);
    if (child_node == NULL) return -1;
    set_node_uid(child_node, node_count++);
    set_node_callback_function(child_node, &callback_print_node_input_and_output, NULL);

    print_transform_tree(head);



    // finally, we will make one more child node, however this time we will initialize a variable that will be used to store the node's output value
    // we tell the node to fill this variable when it is computed by giving it another premade callback function, along with the address of the variable
    double final_node_output = 0;
    child_node = new_child_node(child_node, &myTriple);
    if (child_node == NULL) return -1;
    set_node_uid(child_node, node_count++);
    // assign a callback function that will write the node's output to the variable final_node_output, and also print the input and output of the node when computing
    // note that if we only want to set the variable, and not print, we can use the premade callback function '&callback_retrieve_node_output_in_flag'
    set_node_callback_function(child_node, &callback_print_io_and_retrieve_value_in_flag, &final_node_output);

    // of course, we can set more callbacks and variables for retrieving data from any node
    // we can also just not set a callback function if we just want the node to transform the data and pass it on with doing or saying anything extra

    print_transform_tree(head);

    printf("\nWe have initialized the variable final_node_output = %f\n\n", final_node_output);

    // give an input value to the head of the transform tree
    // at each node it will first apply the callback function (if provided) to update the input
    // the value is then passed from the callback function to the transform function
    // finally, the node passes the output of the transform function to each of its child nodes
    // currently the tree is evaluated recursively, left-to-right
    double input_value = -9.5;
    compute_transform_tree(head, input_value);

    // note that each node has been given a callback function to print its input and output, so that will be done automatically
    // furthermore, the final node added will write its output to the variable final_node_output

    printf("\nAfter computing the transform tree, we have final_node_output = %f due to the assigned callback function\n", final_node_output);

    // finally, we can free all the memory that was dynamically allocated for the tree
    // the freeing of memory is recursive, depth-first
    // we pass a pointer to the head pointer so that each node can free and NULL itself after free and NULLing its children
    delete_transform_tree(&head);
    printf("\nThe tree has been deleted, and allocated memory freed\n\n");
    print_transform_tree(head);
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
