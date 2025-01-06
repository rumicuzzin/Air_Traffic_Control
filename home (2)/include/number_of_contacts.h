/*
    This file includes the function that is used when the program wans to determine how many planes are currently flying inside the area of operation. 
*/
#include "message.h"

#ifndef NUMBER_OF_CONTACTS_H
#define NUMBER_OF_CONTACTS_H

/*
This function will be able to determine the number of aircrafts that the tower is currently tracking
*/

int get_total_contacts(char **request_array, ADSBPacket *head) ;

#endif
