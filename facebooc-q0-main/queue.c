#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "queue.h"

/*
 * Create empty queue.
 * Return NULL if could not allocate space.
 */
queue_t *q_new()
{
    queue_t *que = malloc(sizeof(queue_t));
    if(!que) return NULL;
    que->head = NULL;
    que->tail = NULL;
    que->size = 0;
    return que;
    
    
}

/* Free all storage used by queue */
void q_free(queue_t *q)
{
    if(q==NULL) return;

    element_t *curr = q->head;
    element_t *prev = curr;
    while(prev) {
        curr = curr->next;
        free(prev->value);
        free(prev);
        prev = curr;
    }
    free(q);

}

/*
 * Attempt to insert element at head of queue.
 * Return true if successful.
 * Return false if q is NULL or could not allocate space.
 * Argument s points to the string to be stored.
 * The function must explicitly allocate space and copy the string into it.
 */

bool q_insert_head(queue_t *q, char *s)
{
    if(!q) return false;
    element_t* temp = malloc(sizeof(element_t));
    if(!temp) return false;

    temp->value= strdup(s);
    if(!temp->value){
        free(temp);
        return false;
    }

    temp->next = q->head;
    q->head = temp;
    // Assume queue is empty, we assign the same node to tail.
    if(!q->tail) q->tail = temp;
    
    (q->size)++;
    return true;
}

/*
 * Attempt to insert element at tail of queue.
 * Return true if successful.
 * Return false if q is NULL or could not allocate space.
 * Argument s points to the string to be stored.
 * The function must explicitly allocate space and copy the string into it.
 */
bool q_insert_tail(queue_t *q, char *s)
{
    if(q==NULL) return false;

    element_t* temp = malloc(sizeof(element_t));
    temp->value= strdup(s);
    if(!temp->value){
        free(temp);
        return false;
    }
    temp->next = NULL;
    
    if(q->tail) {
        q->tail->next = temp;
        q->tail = temp;
    }  
    if(!q->head){
        q->head = temp;
    }
    (q->size)++;
    return true;
}

/*
 * Attempt to remove element from head of queue.
 * Return true if successful.
 * Return false if queue is NULL or empty.
 * If sp is non-NULL and an element is removed, copy the removed string to *sp
 * (up to a maximum of bufsize-1 characters, plus a null terminator.)
 * The space used by the list element and the string should be freed.
 */
bool q_remove_head(queue_t *q, char *sp, size_t bufsize)
{
    // if(!(q && q->head && q->head->next)) return false;

    // element_t* first = q->head;
    
    // int *csp = memccpy(sp, first->value,'\0', bufsize-1);
    // if(!csp) return false;
    // // sp[(((char *) csp) - first->value)] = '\0';
    // sp[bufsize - 1] = '\0';

    // q->head = first->next;
    // free(first->value);
    // free(first);
    // (q->size)--;

    if(!(q && q->head && q->head->next)) return false;

    int *dang = memccpy(sp, (q->head)->value, '\0', bufsize - 1);
    if (!dang) return false;
    sp[(((char *) dang) - (q->head)->value)] = '\0';

    element_t *init = q->head;
    q->head = init->next;

    free(init->value);
    free(init);
    
    q->size--;
    return true;
}

/*
 * Return number of elements in queue.
 * Return 0 if q is NULL or empty
 */
size_t q_size(queue_t *q)
{
    if(!q)
        return 0;
    return q->size;
}

/*
 * Reverse elements in queue
 * No effect if q is NULL or empty
 * This function should not allocate or free any list elements
 * (e.g., by calling q_insert_head, q_insert_tail, or q_remove_head).
 * It should rearrange the existing ones.
 */
void q_reverse(queue_t *q)
{
    element_t* prev=NULL, *next=NULL, *curr = q->head;
    while(curr!=NULL) {
        next = curr->next;
        curr->next = prev;
        prev = curr;
        curr = next;
    }
    q->head = prev;
}

void find_mid(element_t *head, element_t **left,element_t **right)
{
    
    element_t *a = head;
    element_t *b = head->next;
    
    while(b && b->next) {
        a = a->next;
        b = b->next->next;
    }
    *left = head;
    *right = a->next;
    a->next = NULL;

}
void SortedMerge(element_t **head, element_t *left,element_t *right){

    element_t *result = NULL;
    element_t **curr;
    for(curr=&result; left && right; curr = &(*curr)->next)
    {
        if(strcmp(left->value, right->value)< 0){
            *curr = left;
            left = left->next;
        }
        else{
            *curr = right;
            right = right->next;
        }
    }
    *curr = left ? left :right;
    *head = result;

    // element_t *result;
    // for(result=NULL;left && right ;result = result->next){
    //     if(left->value < right->value){
    //         result = left;
    //         left = left->next; 
    //     }
    //     else{
    //         result = right;
    //         right = right->next;
    //     }
    // }
    // result = left ? left : right;
    // *head = result;
}

/*
 * The function's sorting algorithm should be merge sort.
 */
void merge_sort(element_t **head)
{
    if (!(*head && (*head)->next)) return;
    
    element_t *left = NULL;
    element_t *right = NULL;
    find_mid(*head, &left, &right);
    merge_sort(&left);
    merge_sort(&right);
    SortedMerge(head, left, right);

}

/*
 * Sort elements of queue in ascending order
 * No effect if q is NULL or empty. In addition, if q has only one
 * element, do nothing.
 */
void q_sort(queue_t *q)
{
    if (!q || !q->head)
        return;

    merge_sort(&q->head);
    element_t *walk = q->head;
    while (walk->next)
        walk = walk->next;

    q->tail = walk;
}
