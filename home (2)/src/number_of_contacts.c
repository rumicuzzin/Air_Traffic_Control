#include "estimate_position.h"
#include "utilities.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define AREA_OF_OPERATION 350

/*
This function will be able to determine the amount of aircrafts that the tower is currently tracking

Iterate through each node until we reach the end of the list
Everytime we encounter a different plane ID we have a counter which goes up

This can be done through going to the next node, then checking if it matches the previous nodes ID. 

Before we plus 1, we must check the time, and the estimated position of each
plane to make sure it is within the boundaray.

If clear, counter + 1

Return counter.
*/

int get_total_contacts(char **request_array, ADSBPacket *head) {
    // Define our time
    double time = get_double_time(request_array[0], request_array[1]);
    ADSBPacket *current = head;
    // Plane counter
    int plane_counter = 0;

    while (current != NULL) {
        // Get the current plane ID
        int current_plane_id = current->id;
		// Flag to track if the plane ID is a duplicate
        int duplicate = 0;

        // Check all previous nodes for the same plane ID
        ADSBPacket *prev_node = current->prev;
        while (prev_node != NULL) {
            if (prev_node->id == current_plane_id) {
                duplicate = 1;
                // No need to continue checking
				break;
            }
            prev_node = prev_node->prev;
        }

        // If the plane ID is not a duplicate, proceed to check boundary and count
        if (!duplicate) {
            // Check if the plane is within the boundary
            float *positions = malloc(2 * sizeof(float));
            float *estpos = estimate_position(time, current, current_plane_id, positions);

            // If the plane has a valid position
            if (estpos != NULL) {

                // Check if plane is inside area of operation
                double distance = sqrt(estpos[0] * estpos[0] + estpos[1] * estpos[1]);
                if (distance <= AREA_OF_OPERATION) {
                    plane_counter++;
                }
            }
        }
        // traverse
        current = current->next;
    }
    return plane_counter;
}

