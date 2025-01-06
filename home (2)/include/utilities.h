/*
    This file contains functions that are useed throughout my program across multiple other files.
*/
#include "message.h"

#ifndef UTILITIES_H
#define UTILITIES_H

/*
This function will combine the two separate strings which hold 'hours' and 'minutes' and return a double that represents the same time
*/
double get_double_time(char *one, char *two);

/*
This function will convert degrees to radians
*/
double degreesToRadians(double degrees);

/*
This function will conveert time into seconds. It will return a value which is the time differnce
(t - t(ADSB))
*/
int time_converter(double time_0, double time_1);

/*
This function will traverse through the linked list until it reaches the last node that we have received information for. It will return a pointer to the last node
*/
ADSBPacket *head_to_tail(ADSBPacket *head);

#endif
