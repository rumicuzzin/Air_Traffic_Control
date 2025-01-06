/*
    This file has functions to free any memory allocaated on the heap. 
*/

#include "message.h"
#include "data_to_struct.h"

#ifndef FREE_LIST_H
#define FREE_LIST_H

// This function frees the enitre linked list 
void free_list(ADSBPacket *head);


#endif
