# Magic-Band Project

This project implements a doubly linked list, a queue, and a stack to simulate a magic band.

## Structures

- **Doubly Linked List**: Represents the tape. Nodes contain a char type element and pointers to the next and previous nodes. The list has a pointer to the finger position and to the sentinel.
- **Queue**: Implemented through a singly linked list. Contains commands introduced in it. Queue operations include creating a node, checking if it's empty, enqueueing, dequeueing, returning the first element, and freeing memory.
- **Stack**: Similar to the queue, but follows the LIFO rule. Stack operations include creating a node, checking if it's empty, pushing, popping, returning the top node, and freeing memory.

## Functions

- **Tape Functions**: Includes creating a node, creating the list, and freeing memory.
- **Movement Functions**: Includes `move_left`, `move_right`, `move_left_char`, `move_right_char`.
- **Manipulation Functions**: Includes `write`, `insert_left`, `insert_right`, `show_current`, `show`.
- **Undo/Redo Functions**: Includes `undo` and `redo`.

## Implementation

The `magic-band.c` file reads commands from `tema1.in` and writes the output to `tema1.out`. It executes commands directly or adds them to the queue for later execution. Memory is freed at the end of the process.

