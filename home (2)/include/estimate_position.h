/*
    This file includes all the functions necessary to calculate a given planes position at a given time. 
*/
#include "message.h"

#ifndef ESTIMATE_POSITION_H
#define ESTIMATE_POSITION_H

/*
This function will look at the time packets for estimated position
It will extract the time, the request and the ID
*/
char **position_packet(char *holder, char **clean);

/*
This fucntion takes in the current node, and returns a float for the velocity either north or east. 
*/
float velocity_calculations(int i, ADSBPacket *current);

/*
This function will calculate the position point. Returning a float for the position either north or east. 
*/
float position_calculations(float velocity, ADSBPacket *current,double time, int i);

/*
This fucnction will take in variables, move through the linked list to find the relevant plane and estimates the planes postition at the time given time given

It will return NULL if we have no information on the plane ID passed in
*/
float *estimate_position(double time, ADSBPacket *head, int plane_id, float *positions);

/*
This function will take care of any estimated position requests, printing the necessary statements
*/
void estimate_position_print(char** request_array, ADSBPacket *head);



#endif
