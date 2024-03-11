/*Bulgaru Alexandra - 312CD*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "magic-band.h"
#define MAX 100

int main()
{
    char input_filename[] = "tema1.in";
    FILE *in = fopen(input_filename, "rt");
    char instruction[MAX][MAX];
    int nr_comenzi = 0, i = 0;
    fscanf(in, "%d", &nr_comenzi); // read the first number from the file = number of commands
    for (i = 0; i < nr_comenzi; i++)
    {
        fscanf(in, " %[^\n]", instruction[i]); // read each line until I reach newline
    }
    fclose(in);
    char output_filename[] = "tema1.out";
    FILE *out = fopen(output_filename, "wt");
    MagicTape *tape = createMagicTape('#');
    Queue *q_instr = createQueue(); // queue of instructions
    Stack *s_undo = createStack();  // undo stack
    Stack *s_redo = createStack();  // redo stack
    char chr_to_write;              // the character it writes in place of the one the finger is on
    char chr_mv_lf;                 // the character it looks for to the left
    char chr_mv_rg;                 // the character it looks for to the right
    char chr_ins_lf;                // the character it inserts to the left
    char chr_ins_rg;                // the character it inserts to the right
    for (i = 0; i < nr_comenzi; i++)
    {
        if (strcmp(instruction[i], "MOVE_LEFT") == 0)
        {
            enqueue(q_instr, instruction[i]);
        }
        if (strcmp(instruction[i], "MOVE_RIGHT") == 0)
        {
            enqueue(q_instr, instruction[i]);
        }
        if (strncmp(instruction[i], "WRITE", 5) == 0)
        {
            enqueue(q_instr, instruction[i]);
        }
        if (strncmp(instruction[i], "MOVE_LEFT_CHAR", 14) == 0)
        {
            enqueue(q_instr, instruction[i]);
        }
        if (strncmp(instruction[i], "MOVE_RIGHT_CHAR", 15) == 0)
        {
            enqueue(q_instr, instruction[i]);
        }
        if (strncmp(instruction[i], "INSERT_LEFT", 11) == 0)
        {
            enqueue(q_instr, instruction[i]);
        }
        if (strncmp(instruction[i], "INSERT_RIGHT", 12) == 0)
        {
            enqueue(q_instr, instruction[i]);
        }
        if (strcmp(instruction[i], "SHOW") == 0)
        {
            show(tape, out);
            fprintf(out, "\n");
        }
        if (strcmp(instruction[i], "SHOW_CURRENT") == 0)
        {
            show_current(tape, out);
            fprintf(out, "\n");
        }
        if (strcmp(instruction[i], "UNDO") == 0)
        {
            undo(tape, s_undo, s_redo);
        }
        if (strcmp(instruction[i], "REDO") == 0)
        {
            redo(tape, s_undo, s_redo);
        }
        if (strcmp(instruction[i], "EXECUTE") == 0)
        {
            char *command = front(q_instr);
            if (strcmp(command, "MOVE_LEFT") == 0)
            {
                if (tape->sentinel->next == tape->digit)
                { // if it's on the first position, it doesn't execute any more moves and it doesn't need to be added to the stack
                    move_left(tape);
                }
                else
                {
                    move_left(tape);
                    push(s_undo, tape->digit->next);
                }
            }
            else if (strcmp(command, "MOVE_RIGHT") == 0)
            {
                move_right(tape);
                push(s_undo, tape->digit->prev);
            }
            else if (strncmp(command, "WRITE", 5) == 0)
            {
                chr_to_write = command[strlen(command) - 1]; // it finds the last character from the command
                write(tape, chr_to_write);
                while (s_undo->head != NULL)
                {
                    CommandsStack *tmp = s_undo->head;
                    s_undo->head = s_undo->head->next;
                    free(tmp);
                }
                s_undo->size = 0;
                while (s_redo->head != NULL)
                {
                    CommandsStack *tmp = s_redo->head;
                    s_redo->head = s_redo->head->next;
                    free(tmp);
                }
                s_redo->size = 0;
            }
            else if (strncmp(command, "MOVE_LEFT_CHAR", 14) == 0)
            {
                chr_mv_lf = command[strlen(command) - 1];
                move_left_char(tape, chr_mv_lf, out);
            }
            else if (strncmp(command, "MOVE_RIGHT_CHAR", 15) == 0)
            {
                chr_mv_rg = command[strlen(command) - 1];
                move_right_char(tape, chr_mv_rg);
            }
            else if (strncmp(command, "INSERT_LEFT", 11) == 0)
            {
                chr_ins_lf = command[strlen(command) - 1];
                insert_left(tape, chr_ins_lf, out);
            }
            else if (strncmp(command, "INSERT_RIGHT", 12) == 0)
            {
                chr_ins_rg = command[strlen(command) - 1];
                insert_right(tape, chr_ins_rg);
            }
            dequeue(q_instr);
        }
    }
    fclose(out);
    freeQueue(q_instr);
    free(tape);
    return 0;
}
