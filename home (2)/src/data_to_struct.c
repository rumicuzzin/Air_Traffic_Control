#include "utilities.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


/*
This function will take in a temporary array where the packet is stored, it will return a multidimensional array with each bit of information stored as an individual string
*/

char **remove_tags(char *holder, char **cleaned_data){
	// set index to keep count of the array that will hold the data
	int index = 0;
	if (holder[0] == '#')
	{
		char *token = strtok(holder, ":,");
		while (token != NULL)
		{
			// check if token is digit
			// If the first character is a digit assume the token is useful data
			if ((atof(token)!= 0) || (strcmp(token,"0.0") == 0))
			{
				cleaned_data[index] = token;
				index++;
			}
			// move to next token	
			token = strtok(NULL, ":,");
		}
	}
	return cleaned_data;
}

/* 
This function will take in each part of the MD array, that will be converted to the necessary type in the function
calling. It will created a node for the linked list. 

This is a Doubly linked list that will have pointers to the next and previous nodes 
*/

ADSBPacket *create_node(int id, double time, double north, double east, int altitude, double heading, double speed){
    ADSBPacket *new_node = (ADSBPacket*)malloc(sizeof(ADSBPacket));
    new_node->id = id;
    new_node->time = time;
	new_node->north = north;
	new_node->east = east;
	new_node->altitude = altitude;
	new_node->heading = heading; 
	new_node->speed = speed;
	new_node->next = NULL;
	new_node->prev = NULL;
    return new_node;
}

/* 
This function which appends a new node to a doubly linked list keeping track of the nodes that are next and previous
*/

ADSBPacket *append_node(ADSBPacket *head, int id, double time, double north, double east, int altitude,double heading, double speed){
	ADSBPacket *new_node = create_node(id, time, north, east, altitude, heading, speed);
	if (head == NULL)
	{
		// current->prev = head;
		head = new_node;
	}
	else 
	{
		ADSBPacket *current = head;
		while (current->next != NULL)
		{
			current = current->next;
		}	
		// Set the next and previosu nodes
		current->next = new_node;
		new_node->prev = current; 
	}
	return head; 
}

/*
This function grabs the data from the temporary holder, and returns a pointer to the head node of a doubly linked list
*/

ADSBPacket *data_grab(char **cleaned_data, char *packet_holder, ADSBPacket *head){
	// Set 3D array
	char **new_array = remove_tags(packet_holder, cleaned_data);
	// Get time into a double
	double time = get_double_time(new_array[1], new_array[2]);
	head = append_node(head, atoi(new_array[0]), time, atof(new_array[3]) , atof(new_array[4]), atoi(new_array[5]), atof(new_array[6]), atof(new_array[7]));
	return head;
}





