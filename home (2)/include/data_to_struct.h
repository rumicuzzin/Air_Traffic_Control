/*
    This header file includes the functions necessary to store the packet data in the doubly linked list of structs.  
*/
#include "message.h"

#ifndef DATA_TO_STRUCT_H
#define DATA_TO_STRUCT_H

/*
remove_tags, takes in an ADSB packker, removes all the unecessary data and returns a aray of strings, each string is useful data that we need to use
*/
char **remove_tags(char *holder, char **cleaned_data);

/*
create_node is a function that calls the struct stored in message.h, and creates 1 singular node
to store bits of data
*/
ADSBPacket *create_node(int id, double time, double north, double east, int altitude, double heading, double speed);

/*
Append_node uses create node inside of it to create a node. It also sets up the double linked list, giving pointers to the previous and next nodes.
*/
ADSBPacket *append_node(ADSBPacket *head, int id, double time, double north, double east, int altitude,double heading, double speed);

/*
This function grabs the data from the temporary holder, and returns a pointer to the head node 
of a doubly linked list
*/

ADSBPacket *data_grab(char **cleaned_data, char *packet_holder, ADSBPacket *head);

#endif




