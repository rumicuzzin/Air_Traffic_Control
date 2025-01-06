#include <stdio.h>
#include <stdlib.h>

#include "data_to_struct.h"
#include "message.h"

#define BUFFER_SIZE 256
#define DATA_POINTS 8

// This function frees the enitre linked list 
void free_list(ADSBPacket *head){
    ADSBPacket *current = head;

    while (current != NULL) {
        ADSBPacket *next = current->next;
        free(current);
        current = next;
    }
}

