// ----------------------------------------------------------------------------
// BYN SOFTWARE, COPYRIGHT 2012
// 
// Author: Jan-Willem Buurlage et al.
// Contact: j.buurlage@students.uu.nl
// 
// Part of the Lifeline project, handwriting recognition for scientific wr-
// iting. Tailored for touchscreens.
// ----------------------------------------------------------------------------

/* Common data structures */

#ifndef DATASTRUCTURES_H
#define DATASTRUCTURES_H

#include <stdlib.h>
#include <string.h>

///////////////////////////////////////////////////////////////////////////////

/* DEFINITION: Linked List */

typedef struct ListElement_
{
    void *data;
    struct ListElement_ *next;
} ListElement;

typedef struct
{
    int size;
    
    int (*match)(const void *key1, const void *key2); 
    void (*destroy)(void *data);
    
    ListElement *head;
    ListElement *tail;
} List;

/* INTERFACE: Linked List */

void list_init(List *list, void (*destroy)(void *data));
void list_destroy(List *list);
int list_insert_next(List *list, ListElement *element, const void *data);
int list_remove_next(List *list, ListElement *element, void** data);

///////////////////////////////////////////////////////////////////////////////

#endif