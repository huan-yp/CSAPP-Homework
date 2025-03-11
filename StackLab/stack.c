/* 
 * Code for basic C skills diagnostic.
 */

/*
 * This program implements a stack supporting both FILO operations.
 *
 * It uses a singly-linked list to represent the set of stack elements
 */

#include <stdlib.h>
#include <stdio.h>

#include "harness.h"
#include "stack.h"


/*
  Create empty stack.
  Return NULL if could not allocate space. Use malloc 
*/
stack *s_new()
{
  return NULL
}

/* Free all storage used by stack */
void s_free(stack *s)
{}

/*
  Attempt to insert element at top of stack.
  Return true if successful.
  Return false if s is NULL or could not allocate space.
 */
bool s_push(stack *s, int v)
{
  return true;
}


/*
  Attempt to pop an element from the top of stack.
  Return true if successful.
  Return false if s is NULL or empty.
  If vp non-NULL and element removed, store removed value at *vp.
  Any unused storage should be freed.
 */
bool s_pop(stack *s, int *vp)
{
  return true;
}

/*
  Return number of elements in stack.
  Return 0 if s is NULL or empty
 */
int s_size(stack *s)
{
  return -1
}

/*
  Discriminate whether the stack is empty.
  Return true if s is empty or NULL, otherwise return false.
 */
bool s_empty(stack *s)
{
  return true;
}

/*
  Reverse elements in stack.

  Your implementation must not allocate or free any elements (e.g., by
  calling push or pop).  Instead, it should modify
  the pointers in the existing data structure.
 */
void s_reverse(stack *s)
{}