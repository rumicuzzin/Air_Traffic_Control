#include "utilities.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define AREA_OF_OPERATION 350
#define KM_METERS 1000

/*
This function will look at the time packets for estimated position
eg. *time:09:25,est_pos,7285
It will extract the time, the request and the ID
*/
char **position_packet(char *holder, char **clean){

	// Set index to keep track of 
	int index =0; 
	// Strtok all the bits of info
	char *token = strtok(holder, ":,");

	// RUn a while loop to iterate through the tokens 

	while (token != NULL)
	{
		// Extract useful data from the packet
		if (atoi(token) != 0)
		{
			clean[index] = malloc(strlen(token) + 1);
			clean[index] = token;
			index++;
		}

		// If it wants to close
		else if (strcmp(token, "close\n") == 0)
		{
			clean[index] = malloc(strlen(token) + 1);
			//Get rid of newline character on the end
			token[strlen(token)-1] = '\0';		
			clean[index] = token;
			index++;
		}
			
		// If the packet is reffering to contacts
		else if (strcmp(token, "num_contacts\n") == 0)
		{
			clean[index] = malloc(strlen(token) + 1);
			//Get rid of newline character on the end
			token[strlen(token)-1] = '\0';
			clean[index] = token;
			index++;
		}

		// If the packet is referring to estimated position
		else if (strcmp(token, "est_pos") == 0)
		{
			clean[index] = malloc(strlen(token) + 1);
			clean[index] = token;
			index++;
		}

		else if (strcmp(token, "check_separation") == 0)
		{
			clean[index] = malloc(strlen(token) + 1);
			clean[index] = token;
			index++;
		}
		token = strtok(NULL, ":,");		
	}
	return clean; 
}

/*
This fucntion takes in the current node, and returns P(n,t) or P(e,t)
*/

float velocity_calculations(int i, ADSBPacket *current){
	// if we are calculating  north
	if (i == 0)
	{
		double theta = degreesToRadians(current->heading);
		return (current->speed)*sin(theta);
	}
	else 
	{
		double theta = degreesToRadians(current->heading);
		return (current->speed)*cos(theta);
	}
}

/*
This function will calculate the point, taking in the velocity east/north
It will return a float that refers to the position point
*/

float position_calculations(float velocity, ADSBPacket *current,double time, int i){
	
	if  (i == 0)
	{
		return (current->north) + ( (velocity * (time_converter(current->time, time))) / KM_METERS);
	}
	else 
	{
		return (current->east) + ( (velocity * (time_converter(current->time, time))) / KM_METERS);
	}
}

/*
This fucnction will take in variables, move through the linked list to find the relevant plane and estimates the planes postition at the time given time given

It will return NULL if we have no information on the plane ID passed in
*/

float *estimate_position(double time, ADSBPacket *head, int plane_id, float *positions){
	// Use a counter to determine north opr east
	int counter = 0;

	// Get a pointer for the last node
	ADSBPacket *tail = NULL;
	// reverse the linked list to traverse from most recent data. 
	tail = head_to_tail(head);
	// Set current node to the tail
	ADSBPacket *current = tail;
	
	while (current != NULL && counter < 2)
	{
		// If the currnet node is the plane that's position has been requested
		if (current->id == plane_id)
		{

			// Calculate position
			float velocity = velocity_calculations(counter, current);
			float point = position_calculations(velocity, current, time, counter);
			positions[counter]= point; 
			counter++;
		}
		// Traverse back through the nodes to find the relevant node
		else
		{
			if (current->prev != NULL)
			{
				current = current->prev;
			}
			else
			{
				return NULL;
			}
		}
		
	}
	// If we have no data of that particular plane
	if (counter == 0)
	{
		return NULL;
	}
	return positions;
}

/*
This function will take care of any estimated position request and print the necessary statements. 
*/

void estimate_position_print(char** request_array, ADSBPacket *head){
	// Bring out the data into variables
	double time = get_double_time(request_array[0], request_array[1]);
	int id = atoi(request_array[3]);
	// Malloc a array to hold the float of postition east and north
	float *positions = malloc(2*sizeof(float));
	
	// Check if there are nodes with data in them 
	if (head == NULL)
	{
		printf("No planes have come");
	}
	else 
	{	
		// Store positions 
		float *estpos = estimate_position(time, head, id, positions);
		
		// If there is no data on the plane
		if (estpos == NULL)
		{
			printf("Aircraft (ID:%d) not currently in area of operation\n", id);
		}
		// If the lane is outside the area of operation, do this by calculating the hypotenuse
		else if (sqrt((estpos[0]*estpos[0]) + (estpos[1]*estpos[1])) > AREA_OF_OPERATION)
		{
			printf("Aircraft (ID:%d) not currently in area of operation\n", id);
		}

		//  All else is clear, we have valid positions value. 	
		else if (estpos != NULL)
		{
		printf("Aircraft (ID:%d): Estimated Position: N:%.1f,E:%.1f\n", id, estpos[0], estpos[1]);
		}							
	}
}

