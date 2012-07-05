// ----------------------------------------------------------------------------
// BYN SOFTWARE, COPYRIGHT 2012
// 
// Author: Jan-Willem Buurlage et al.
// Contact: j.buurlage@students.uu.nl
// 
// Part of the Lifeline project, handwriting recognition for scientific wr-
// iting. Tailored for touchscreens.
// ----------------------------------------------------------------------------

#include "../include/DataStructures.h"

///////////////////////////////////////////////////////////////////////////////

/* IMPLEMENTATION: Linked List */

void list_init(List *list, void (*destroy)(void *data))
{
    list->size = 0;
    list->destroy = destroy;
    list->head = 0;
    list->tail = 0;
    
    return;
}

void list_destroy(List *list)
{
    void *data;
    
    while((list->size) > 0)
    {
        if(list_remove_next(list, 0, (void**)&data) == 0 && list->destroy != 0)
        {
            list->destroy(data);
        }
    }
    memset(list, 0, sizeof(List));
}

int list_insert_next(List *list, ListElement *element, const void *data)
{
    ListElement *new_element;
    
    if((new_element = (ListElement *)malloc(sizeof(ListElement))) == 0)
        return -1;
    
    new_element->data = (void*)data;
    
    if(element == 0)
    {
        if ((list->size) == 0)
            list->tail = new_element;
        
        new_element->next = list->head;
        list->head = new_element;
    }
    else 
    {
        if(element->next == 0)
            list->tail = new_element;
        
        new_element->next = element->next;
        element->next = new_element;
    }
    
    list->size++;
    
    return 0;
}

int list_remove_next(List *list, ListElement *element, void** data)
{
    ListElement *old_element;
    
    if((list->size) == 0)
        return -1;
    
    if(element == 0)
    {
        *data = list->head->data;
        old_element = list->head;
        list->head = list->head->next;
        
        if((list->size) == 1)
            list->tail = 0;
    }
    else
    {
        if(element->next == 0)
            return -1;
        
        *data = element->next->data;
        old_element = element->next;
        element->next = element->next->next;
        
        if(element->next == NULL)
            list->tail = element;
    }
    
    free(old_element);
    
    list->size--;
    
    return 0;
}

///////////////////////////////////////////////////////////////////////////////

