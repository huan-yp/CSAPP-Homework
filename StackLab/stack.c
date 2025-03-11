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
  stack *p = malloc(sizeof(stack));
  if(p == NULL)return NULL;
  p->top = NULL, p->bottom = NULL, p->cnt=0;
  return p;
}

/* Free all storage used by stack */
void s_free(stack *s)
{
  if(s == NULL)return ;
  while(s->top!=s->bottom){
    s_node *tmp = s->top->next;
    free(s->top);
    s->top = tmp;
  }
  free(s);
}

/*
  Attempt to insert element at top of stack.
  Return true if successful.
  Return false if s is NULL or could not allocate space.
 */
bool s_push(stack *s, int v)
{
  if(s == NULL)return false;
  s_node *tmp = malloc(sizeof(s_node));
  if(!tmp) return false;
  tmp->value = v;
  tmp->next = s->top;
  s->top = tmp;
  s->cnt++;
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
  if(s == NULL || s->top == s->bottom)return false;
  *vp = s->top->value;
  s_node *tmp=s->top;
  s->top = s->top->next, s->cnt--;
  free(tmp);
  return true;
}

/*
  Return number of elements in stack.
  Return 0 if s is NULL or empty
 */
int s_size(stack *s)
{
  return s == NULL ? 0 : s->cnt;
}

/*
  Discriminate whether the stack is empty.
  Return true if s is empty or NULL, otherwise return false.
 */
bool s_empty(stack *s)
{
  return s == NULL || s->top == s->bottom;
}

/*
  Reverse elements in stack.

  Your implementation must not allocate or free any elements (e.g., by
  calling push or pop).  Instead, it should modify
  the pointers in the existing data structure.
 */
void s_reverse(stack *s){
  // s_node *
  if(s == NULL || s->top == s->bottom) return;
  s_node *cur=s->top;
  s_node *nxt=cur->next;
  s_node *pre=NULL;
  while(nxt != s->bottom){
    cur->next = pre;
    pre = cur;
    cur = nxt;
    nxt = nxt->next;
  }
  cur->next = pre;
  s->top = cur;
}