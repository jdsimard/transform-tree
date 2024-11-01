#include <stdlib.h>
#include <stdio.h>
#include "transform_tree.h"



transform_tree_node *create_transform_tree_node(int uid, transform_tree_node *parent, double (*transform_fn)(double), double (*callback_fn)(struct transform_tree_node*, double), void *flag_addr) {
    transform_tree_node *new_node = NULL;

    // new root node, initializaed with transform function
    if (parent == NULL) new_node = new_transform_tree(transform_fn);
    // new child node, initialized with transform function
    else new_node = new_child_node(parent, transform_fn);
    // check memory was allocated properly
    if (new_node == NULL) return NULL;

    // initial the node with uide, callback function, and flag address
    set_node_uid(new_node, uid);
    set_node_callback_function(new_node, callback_fn, flag_addr);
    return new_node;
}



transform_tree_node *new_transform_tree(double (*node_function)(double)) {
    transform_tree_node *node = malloc(sizeof(transform_tree_node));
    if (node != NULL) {
        node->transform_fn = node_function;
        node->children = NULL;
        node->num_children = 0;
        node->callback_function = NULL;
        node->callback_flag_addr = NULL;
        node->uid = -1;
    }
    return node;
}



void delete_transform_tree(transform_tree_node **root_ref) {
    // use a double pointer to the head node so that the head node pointer call NULL itself after free

    // depth-first / recursive delete
    transform_tree_node *root = *root_ref;

    if (root->num_children > 0) {
        // if the node has children, delete each child node first
        while (root->num_children > 0) {
            delete_transform_tree(&(root->children[root->num_children - 1]));
            //root->children[head->num_children - 1] = NULL;
            root->num_children -= 1;
        }
        // now delete the memory allocated for storing pointers to child nodes
        free(root->children);
        root->children = NULL;
    }
    // now the the child nodes are deleted, the node can delete itself
    free(root);
    *root_ref = NULL;
}



transform_tree_node *new_child_node(transform_tree_node *parent, double (*node_function)(double)) {
    if (parent == NULL) return new_transform_tree(node_function);
    else {
        transform_tree_node *new_child = malloc(sizeof(transform_tree_node));
        if (new_child != NULL) {
            new_child->transform_fn = node_function;
            new_child->children = NULL;
            new_child->num_children = 0;
            new_child->callback_function = NULL;
            new_child->callback_flag_addr = NULL;
            new_child->uid = -1;

            if (parent->num_children == 0) {
                parent->children = malloc(sizeof(transform_tree_node*));
                *(parent->children) = new_child;
            }
            else {
                parent->children = realloc(parent->children, sizeof(transform_tree_node*) * (parent->num_children + 1));
                transform_tree_node **add_ptr = parent->children + parent->num_children;
                *add_ptr = new_child;
            }
            parent->num_children += 1;
            
        }
        return new_child;
    }
}



void set_node_callback_function(transform_tree_node *node, double (*callback_function)(transform_tree_node*, double), void *flag_addr) {
    // you can set a callback function regardless of if a flag_addr is provided
    if (node != NULL && callback_function != NULL) {
        node->callback_function = callback_function;
    }
    // you can only set a flag_addr if there is a callback function to use it with
    // however, this allows you to set a new flag_addr without updating the callback function if a callback function has already been set
    if (node != NULL && node->callback_function != NULL && flag_addr != NULL) {
        node->callback_flag_addr = flag_addr;
    }

    // if both the callback function pointer and the flag_addr are NULL, then assume the intention is to erase the fields
    if (node != NULL && callback_function == NULL && flag_addr == NULL) {
        node->callback_function = NULL;
        node->callback_flag_addr = NULL;
    }
}



void set_node_uid(transform_tree_node *node, int uid) {
    if (node != NULL && uid >= 0) node->uid = uid;
}



void compute_transform_tree(transform_tree_node *root, double input_value) {
    if (root == NULL) return;
    
    double next_value = input_value;
    // if the node has a callback function, call it, passing the input value through
    if (root->callback_function != NULL){
        next_value = root->callback_function(root, next_value);
    }
    // if the node has a node function, call it, passing the input value through
    if (root->transform_fn != NULL) {
        next_value = root->transform_fn(next_value);
    }

    // compute the transform sub tree for each child of this node using the new input value
    for (int i = 0; i < root->num_children; i++) {
        compute_transform_tree(root->children[i], next_value);
    }
}



double callback_print_io(transform_tree_node *node, double val) {
    if (node == NULL) return val;

    
    if (node->transform_fn != NULL) printf("Node: %i, input: %.2f, output: %.2f\n", node->uid, val, node->transform_fn(val));
    else printf("Node: %i, input: %f, output: %f\n", node->uid, val, val);

    return val;
}



double callback_write_node_output_in_flag(transform_tree_node *node, double val) {
    if (node != NULL && node->callback_flag_addr != NULL) {
        double *output_flag_ptr = node->callback_flag_addr;
        if (node->transform_fn != NULL) *output_flag_ptr = node->transform_fn(val);
        else *output_flag_ptr = val;
    }
    else return val;
}



double callback_print_io_and_write_output_in_flag(transform_tree_node *node, double val) {
    return callback_write_node_output_in_flag(node, callback_print_io(node, val));
}



void print_transform_tree_base(transform_tree_node *node, int current_depth, int child_number) {
    //recursive (left-to-right)
    for (int i = 0; i < current_depth; i++) {
        printf("       ");
    }
    printf("   |--> ");
    printf("child %i (%i)\n", child_number, node->uid);
    for (int i = 0; i < node->num_children; i++) {
        print_transform_tree_base(node->children[i], current_depth + 1, i);
    }
}



void print_transform_tree(transform_tree_node *root) {
    //recursive (left-to-right)
    if (root != NULL) {
        printf("root (%i)\n", root->uid);
        for (int i = 0; i < root->num_children; i++) {
            print_transform_tree_base(root->children[i], 0, i);
        }
    }
    else {
        printf("empty tree\n");
    }
}



void print_transform_tree_node(transform_tree_node *node) {
    if (node != NULL) {
        printf("Node - uid: %i, node_function: %p, children: %p, num_children: %i, callback_function: %p, callback_flag: %p\n", node->uid, node->transform_fn, node->children, node->num_children, node->callback_function, node->callback_flag_addr);
    }
    else {
        printf("empty node\n");
    }
}


