#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "message.h"
#include "data_to_struct.h"
#include "utilities.h"
#include "estimate_position.h"
#include "number_of_contacts.h"
#include "separation_issue.h"
#include "free_list.h"

#define BUFFER_SIZE 256
#define DATA_POINTS 8

int main(void) {
	// Declare Empty array to stor temporary data
	char packet_holder[BUFFER_SIZE];
	// Declare the initial head of the struct to == NULL
	ADSBPacket *head = NULL;
	
	// Run a while Loop to iterate through all the lines of the input 
	while (fgets(packet_holder, sizeof(packet_holder), stdin) != NULL)
	{
		// Dynamically allocate an array which will hold the cleaned version of the packet input 
		char **cleaned_data = (char**)malloc(sizeof(char *)*DATA_POINTS);
		// Determine if the argument is a packet
		if (packet_holder[0] == '#')
		{
			//Store the data in a struct and appended it to the doubly linked list
			head = data_grab(cleaned_data, packet_holder, head);
			free(cleaned_data);
		}
		// Determine what command to complete 
		else if (packet_holder[0] == '*')
		{
			// Could wrap all this up inside a function
			char **request_array = position_packet(packet_holder, cleaned_data);
			// If it an Estimated position 
			if (strcmp(request_array[2], "est_pos") == 0)
			{
				// Estimate position 
				estimate_position_print(request_array, head);		
			}
			// if it is asking for the number of conatacts
			else if (strcmp(request_array[2], "num_contacts") == 0)
			{
				int total = get_total_contacts(request_array, head);
				printf("Currently tracking %d aircraft\n", total);
			}
			// If it is asking to check separation issues
			else if (strcmp(request_array[2], "check_separation") == 0)
			{
				check_separation_final(request_array, head);
			}
			// If close
			else 
			{
				printf("closing\n");
				return 0;
			}
			free(request_array);
		}
	}
	// Free the linked list
	free_list(head);
	return 0;
}
	
	
	

