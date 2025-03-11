/* 
 * Code for basic C skills diagnostic and stack structures.
 */

/*
 * This program implements a stack supporting push and pop operations.
 *
 * It uses a singly-linked list to represent the set of stack elements
 */

#include <stdbool.h>

/************** Data structure declarations ****************/

/* Linked list element (You shouldn't need to change this) */
typedef struct node{
    int value;
    struct node *next;   
} s_node;

/* Stack structure */
typedef struct {
    s_node *top;  /* Linked list of elements */
    s_node *bottom;
    int cnt;
} stack;

/************** Operations on stack ************************/

/*
  Create empty stack.
  Return NULL if could not allocate space.
*/
stack *s_new();

/*
  Free all storage used by stack.
  No effect if s is NULL
*/
void s_free(stack *s);

/*
  Attempt to insert element at top of stack.
  Return true if successful.
  Return false if s is NULL or could not allocate space.
 */
bool s_push(stack *s, int v);

/*
  Attempt to insert element at tail of stack.
  Return true if successful.
  Return false if s is NULL or could not allocate space.
 */
bool s_pop(stack *s, int *vp);

/*
  Attempt to remove element from top of stack.
  Return true if successful.
  Return false if stack is NULL or empty.
  If vp non-NULL and element removed, store removed value at *vp.
  Any unused storage should be freed
*/
int s_size(stack *s);

bool s_empty(stack *s);

void s_reverse(stack *s);
