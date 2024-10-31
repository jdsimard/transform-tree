# Transform Tree

A simple data structure for implementing dynamic, branching, and mutable data transformations in C.


# Functionality

When adding nodes to the tree, each node is given a function to transform the data it receives from a parent node. A node can also be given a callback function to perform additional operations, such as print input/output values or writing the transformed output value to a provided address for user retrieval.

When compute_transform_tree is called, the root node is given an initial input value. It first passes the input value through its callback function (if provided), and then passes the output of the callback function through is transform function (the node_function parameter). The root node then calls compute_transform_tree recursively on each of its children with the new input value being the output of the transform function.

This allows for the input to be propagated through branching transformations, and the data at any node in the transform tree can be retrieved after calling compute_transform_tree by setting appropriate callback functions.


# Usage

## Compiling

For now, just statically link the code into your project, e.g. if your project is in main.c
```
gcc .\main.c -o main.exe .\transform_tree\transform_tree.c
```


## Building Transform Trees

For now, see example.c in the root of this project. More details will be added here later.



# Example Uses

A dynamic and mutable data pipeline with automatic logging of outlier values or forbidden values.



# Future Goals

Make the input to a node more general (currently it is restricted to double)
- will allow for tree operations on more abstract objects (structs, etc.)

Allow more than one input to a node
- would allow for transform trees to implement more complex things (e.g. neural networks)

