#ifndef TRANSFORM_TREE_H
#define TRANSFORM_TREE_H

// Created by Joel D. Simard

// a simple data structure for implementing dynamic, branching, and mutable data transformations



// transform tree node struct
typedef struct transform_tree_node {
    // a pointer to function that will be evaluated on received value when computing the transform tree
    double (*node_function)(double);
    // a list of this nodes children
    struct transform_tree_node **children;
    // the number of children this node has
    int num_children;

    // a callback function that a node will execute on itself every time it calls *node_function
    double (*callback_function)(struct transform_tree_node*, double);
    // callback flag address that can be given to callback_function to set an external flag for checking state
    void *callback_flag_addr;

    // a variable that can be set to give the node an integer id (-1 by default)
    int uid;
} transform_tree_node;




// Tree manipulation functions

// create a new transform tree, passing the transform function that the head node will use
// returns a pointer to the head node
transform_tree_node *new_transform_tree(double (*node_function)(double));

// delete the tree, freeing any memory that was allocated
// pass this function the address of the pointer returned by new_transform_tree
// it takes a double pointer so that it can properly NULL all pointers to previously allocated memory after freeing
void delete_transform_tree(transform_tree_node **head_ref);

// create a new child node for the node parent, passing the transform function that thte child node will use
// returns a pointer to the new child node
transform_tree_node *new_child_node(transform_tree_node *parent, double (*node_function)(double));

// add callback_function to node, with an optional address for a flag to be set by the callback function
// callback function must be of the form: double callback(transform_tree_node*, double)
// where the callback function takes the double from the previous node and passes it to the current nodes transform function
void set_node_callback_function(transform_tree_node *node, double (*callback_function)(struct transform_tree_node*, double), void *flag_addr);

// set an integer to be an identifier for the node
// -1 by default when a node is constructed
// the uid will only be set for uid >= 0, and hence can only be negative for the default value
void set_node_uid(transform_tree_node *node, int uid);

// computes the transform tree with input input_value at the node head
// the computation is done recursively / left-to-right
// each node first passes the input value through its callback function, then through its node function, if they are not NULL
// the callback function is allowed to modified the node
void compute_transform_tree(transform_tree_node *head, double input_value);





// Premade callback functions

// a callback function that prints the input and output value of the node
double callback_print_node_input_and_output(transform_tree_node *node, double val);

// a callback function that sets the value at the nodes flag_addr to be the output of the node's transform function
double callback_retrieve_node_output_in_flag(transform_tree_node *node, double val);

// a combination of the two callback functions above
double callback_print_io_and_retrieve_value_in_flag(transform_tree_node *node, double val);






// Printing functions

// print a representation of the tree structure
void print_transform_tree(transform_tree_node *head);

// print data associated to a particular node
void print_transform_tree_node(transform_tree_node *node);













#endif