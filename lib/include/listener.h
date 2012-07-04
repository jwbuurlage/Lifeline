// ----------------------------------------------------------------------------
// BYN SOFTWARE, COPYRIGHT 2012
// 
// Author: Jan-Willem Buurlage et al.
// Contact: j.buurlage@students.uu.nl
// 
// Part of the Lifeline project, handwriting recognition for scientific wr-
// iting. Tailored for touchscreens.
// ----------------------------------------------------------------------------

#ifndef LISTENER_H 
#define LISTENER_H

// function typedefs
typedef void (*callback_char_found)(char char_found, void* obj);

typedef struct
{
    // any data required 'on the other side' like obj-c instances
    void* obj; 
    
    // callback functions
    callback_char_found char_found;
} Listener;

#endif