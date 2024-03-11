/*Bulgaru Alexandra - 312CD*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifndef _magic_band_h_
#define _magic_band_h_

typedef struct MagicTapeNode
{
    char elem;
    struct MagicTapeNode *next;
    struct MagicTapeNode *prev;
} MagicTapeNode;

typedef struct MagicTape
{
    MagicTapeNode *sentinel;
    MagicTapeNode *digit;
} MagicTape;

typedef struct CommandsQueue
{
    struct CommandsQueue *next;
    char instruction[]; // the commands that go into the queue
} CommandsQueue;

typedef struct Queue
{
    CommandsQueue *front;
    CommandsQueue *rear;
    long size;
} Queue;

typedef struct CommandsStack
{
    MagicTapeNode *pnt; // pointer to the current position of the finger for UNDO/REDO
    struct CommandsStack *next;
} CommandsStack;

typedef struct Stack
{
    CommandsStack *head;
    long size;
} Stack;

CommandsQueue *createInstruction(char instruction[])
{
    int length = strlen(instruction);
    CommandsQueue *NewInstruction = malloc(sizeof(struct CommandsQueue) + (length + 1) * sizeof(char)); // dynamic array allocation
    strcpy(NewInstruction->instruction, instruction);                                                   // initialize the node
    NewInstruction->next = NULL;
    return NewInstruction;
}

Queue *createQueue()
{
    Queue *q = malloc(sizeof(struct Queue));
    q->front = q->rear = NULL;
    q->size = 0;
    return q;
}

int isEmptyQueue(Queue *q)
{
    return q == NULL || q->front == NULL || q->size == 0;
}

void enqueue(Queue *q, char instruction[])
{
    CommandsQueue *end;
    if (isEmptyQueue(q))
    {
        q->rear = q->front = createInstruction(instruction);
        q->size++;
        return;
    }
    end = createInstruction(instruction);
    q->rear->next = end;
    q->rear = end;
    q->size++;
    return;
}

void dequeue(Queue *q)
{
    CommandsQueue *tmp;
    if (isEmptyQueue(q))
    {
        return;
    }
    tmp = q->front;
    q->front = q->front->next;
    q->size--;
    free(tmp);
    return;
}

char *front(Queue *q)
{
    if (q->front == NULL && q->rear == NULL)
    {
        return NULL;
    }
    else
    {
        return q->front->instruction;
    }
}

void freeQueue(Queue *q)
{
    while (q->front != NULL)
    {
        CommandsQueue *tmp = q->front;
        q->front = q->front->next; // free the memory of each node in the queue, one by one
        free(tmp);
    }
    free(q);
}

CommandsStack *create(MagicTapeNode *pnt)
{
    CommandsStack *command = malloc(sizeof(struct CommandsStack));
    command->pnt = pnt;
    command->next = NULL;
    return command;
}

Stack *createStack()
{
    Stack *s = malloc(sizeof(Stack));
    s->head = NULL;
    s->size = 0;
    return s;
}

int isEmptyStack(Stack *s)
{
    return s == NULL;
}

void push(Stack *s, MagicTapeNode *pnt)
{
    CommandsStack *top;
    if (isEmptyStack(s))
    {
        s->head = create(pnt);
        s->size++;
        return;
    }
    top = create(pnt);
    top->next = s->head;
    s->head = top;
    s->size++;
    return;
}

MagicTapeNode *top(Stack *s)
{
    if (isEmptyStack(s))
    {
        exit(1);
    }
    return s->head->pnt;
}

void pop(Stack *s)
{
    CommandsStack *tmp;
    if (s->head == NULL)
    {
        return;
    }
    tmp = s->head;
    s->head = s->head->next;
    s->size--;
    free(tmp);
    return;
}

void freeStack(Stack *s)
{
    while (!isEmptyStack(s))
    {
        CommandsStack *tmp = s->head;
        s->head = s->head->next;
        free(tmp);
    }
    free(s);
}

MagicTapeNode *createMagicNode(char elem)
{
    MagicTapeNode *node = malloc(sizeof(struct MagicTapeNode));
    node->elem = elem;
    node->prev = node->next = NULL;
    return node;
}

MagicTape *createMagicTape(char elem)
{
    MagicTape *tape = malloc(sizeof(struct MagicTape));
    tape->sentinel = createMagicNode(elem);
    tape->digit = createMagicNode('#'); // initially, the tape has a single character, #
    tape->sentinel->next = tape->digit;
    tape->digit->prev = tape->sentinel;
    tape->sentinel->prev = NULL;
    tape->digit->next = NULL;
    return tape;
}

MagicTape *freeMagicTape(MagicTape *tape)
{
    MagicTapeNode *tmp;
    while (tape != NULL)
    { // free the memory of each node in the doubly linked list, one by one
        tmp = tape->sentinel;
        tape->sentinel = tape->sentinel->next;
        free(tmp);
    }
    free(tape);
    return NULL;
}

void move_left(MagicTape *tape)
{
    if (tape == NULL)
    {
        return;
    }
    if (tape->sentinel->next == tape->digit)
    {
        return;
    }
    else
    {
        tape->digit = tape->digit->prev;
        return;
    }
}

void move_right(MagicTape *tape)
{
    if (tape == NULL)
    {
        return;
    }
    if (tape->digit->next != NULL)
    {
        tape->digit = tape->digit->next;
        return;
    }
    else
    {
        MagicTapeNode *add = createMagicNode('#');
        tape->digit->next = add;
        add->prev = tape->digit;
        tape->digit = add;
        return;
    }
}

int move_left_char(MagicTape *tape, char elem, FILE *out)
{
    if (tape == NULL)
    {
        return 0;
    }
    MagicTapeNode *iter = tape->digit; // index that starts traversing the list to the left starting with the finger position
    while (iter != tape->sentinel->next && iter->elem != elem)
    {
        iter = iter->prev;
    }
    if (iter->elem == elem)
    {
        tape->digit = iter;
    }
    else
    {
        fprintf(out, "ERROR\n");
    }
    return 1;
}

int move_right_char(MagicTape *tape, char elem)
{
    if (tape == NULL)
    {
        return 0;
    }
    MagicTapeNode *add_char = createMagicNode('#');
    MagicTapeNode *iter = tape->digit; // index that starts traversing the list to the right starting with the finger position
    while (iter->next != NULL && iter->elem != elem)
    {
        iter = iter->next;
    }
    if (iter->elem == elem)
    {
        tape->digit = iter;
    }
    else
    {
        iter->next = add_char;
        add_char->prev = iter;
        tape->digit = add_char;
    }
    return 1;
}

void write(MagicTape *tape, char elem)
{
    if (tape == NULL)
    {
        return;
    }
    tape->digit->elem = elem;
    return;
}

void insert_left(MagicTape *tape, char elem, FILE *out)
{
    if (tape == NULL)
    {
        return;
    }
    MagicTapeNode *NewNode = createMagicNode(elem);
    if (tape->sentinel->next == tape->digit)
    {
        fprintf(out, "ERROR\n");
        return;
    }
    NewNode->next = tape->digit;
    NewNode->prev = tape->digit->prev;
    tape->digit->prev->next = NewNode;
    tape->digit->prev = NewNode;
    tape->digit = NewNode;
    return;
}

void insert_right(MagicTape *tape, char elem)
{
    if (tape == NULL)
    {
        return;
    }
    MagicTapeNode *NewNode = createMagicNode(elem);
    NewNode->next = tape->digit->next; // make the connections of the new node
    NewNode->prev = tape->digit;
    if (tape->digit->next != NULL)
    { // check if there is a node after the one where the finger is to make the connection with the new introduced node
        tape->digit->next->prev = NewNode;
    }
    tape->digit->next = NewNode;
    tape->digit = NewNode;
}

void show_current(MagicTape *tape, FILE *out)
{
    MagicTapeNode *iter = tape->digit;
    fprintf(out, "%c", iter->elem);
}

void show(MagicTape *tape, FILE *out)
{
    if (tape == NULL)
    {
        return;
    }
    MagicTapeNode *iter = tape->sentinel->next;
    while (iter != NULL)
    {
        if (iter != tape->digit)
        {
            fprintf(out, "%c", iter->elem);
        }
        else
        {
            fprintf(out, "|%c|", iter->elem);
        }
        iter = iter->next;
    }
}

void undo(MagicTape *tape, Stack *s_undo, Stack *s_redo)
{
    MagicTapeNode *pnt = top(s_undo);
    pop(s_undo);
    push(s_redo, tape->digit);
    tape->digit = pnt;
}

void redo(MagicTape *tape, Stack *s_undo, Stack *s_redo)
{
    MagicTapeNode *pnt = top(s_redo);
    pop(s_redo);
    push(s_undo, tape->digit);
    tape->digit = pnt;
}

#endif
