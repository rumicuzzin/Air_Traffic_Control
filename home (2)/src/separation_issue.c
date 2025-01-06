#include "number_of_contacts.h"
#include "estimate_position.h"
#include "utilities.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define AREA_OF_OPERATION 350
#define ALTITUDE_TEST 750

/*
Grab the plane that we are assesings alittude and return it as an integer
*/
int find_plane_alt(int id, ADSBPacket *head){
	// Set current node
	ADSBPacket *current = head_to_tail(head); 
	// search through Linked list to find the plane
	while (current != NULL)
	{
		// Determine if current node is the dataa we need
		if (current->id == id)
		{
			// Get the planes alittude
			int altitude = current->altitude;
			return altitude;
		}
		current = current->prev; 
	}
	return 0;
}

/*
This fucntion will traverse the linked list and look for how many different planes are in the system. Any duplicates and ones outside of area of operation are disregarded. 

Returns an Array of the Id's of all the current planes in the system. 
*/
int *get_plane_id_array(char **request_array, ADSBPacket *head, int *plane_id_store) {
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

            if (estpos != NULL) {
				// Get the distance of the plane from the tower
                double distance = sqrt(estpos[0] * estpos[0] + estpos[1] * estpos[1]);
				// Check if it is in area of operation
                if (distance <= AREA_OF_OPERATION) {
                    plane_id_store[plane_counter] = current->id;
					plane_counter++;
                }
            }
        }
        current = current->next;
    }
    return plane_id_store;
}

/*
This fucntion will be checking if two planes are flying at close altitude and return a boolean value. 
*/

int check_altitude(int main_alt, int alt){
	// test if alt is within 750 meters if main_alt
	if (abs(main_alt - alt) <= ALTITUDE_TEST)
	{
		return 1;
	}
	return 0;
}

/*
This function will store the velocity and position of plane 1 and 2 through a counter

*/

float *both_plane_positions(float *data_holder, int main_id, int id_test, double time, ADSBPacket *head){
	// This will store the data for est position_calculations
	float *data_storer1 =  malloc(sizeof(float)*2);
	// Get the position of each plane that we are testing
	float *main_est_pos = estimate_position(time, head, main_id, data_storer1);
	// Create another empty array to store
	float *data_storer2 = malloc(sizeof(float)*2);
	float *test_est_pos = estimate_position(time, head, id_test, data_storer2);
	// change in position at tim_check for north
	float north_pos_change = test_est_pos[0] - main_est_pos[0];
	// change in position for east
	float east_pos_change = test_est_pos[1] - main_est_pos[1];


	data_holder[0] = north_pos_change;
	data_holder[1] = east_pos_change; 

	free(data_storer1);
	free(data_storer2);
	return data_holder;

}

/*
This function calcualtes the velocity, by finding the necessary node in vhe linked list. It will return a pointer to a float, which stores velocity east and velocity north. 

Return's NULL if the plane cannot be found. 
*/

float *get_velocity(ADSBPacket *head, int id){
	// malloc array that we want to store the velocity values in
	float *data_holder = malloc(sizeof(float)*2);
	// Use a counter to determine north or east
	int counter = 0;

	// Get a pointer for the last ndo
	ADSBPacket *tail = NULL;
	tail = head_to_tail(head);
	// Set current node to the tail
	ADSBPacket *current = tail;
	
	while (current != NULL && counter < 2)
	{
		// If the currnet node is the plane that's position has been requested
		if (current->id == id)
		{

			// Calculate position
			float velocity = velocity_calculations(counter, current);
			data_holder[counter]= velocity; 
			counter++;
		}
		// Traverse back through the nodes to find the releveant node
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

	return data_holder;
}

/*
This function returns the change in velocity between the 2 planes that are being test.
*/

float *get_velocity_change(ADSBPacket *head, int main_id, int id_test, float *data_holder){
	// set arrays to hold the data for both planes to then complete the calcualtions
	float *temp_holder_main = malloc(sizeof(float)*2);
	float *temp_holder_test = malloc(sizeof(float)*2);
	// Get main and test velocity values
	temp_holder_main = get_velocity(head, main_id);
	temp_holder_test = get_velocity(head, id_test);


	// Run the calculations
	data_holder[0] = temp_holder_test[0] - temp_holder_main[0];
	data_holder[1] = temp_holder_test[1] - temp_holder_main[1];

	free(temp_holder_main);
	free(temp_holder_test);

	return data_holder;
}

/* 
This fucntion will take in parameters to calculate the discriminant, it will return a float value which will be equal to the discriminant
*/

float get_discriminant(float a, float b, float c){
	return (b*b) - 4*(a*c);
}

/*
This function will take in the parameters of the quadratic to and solve the intercepts of t. Returning these t values into a array. 
*/

float *get_intercepts(float a, float b, float d){
	// Allocate space for an array to hold the 2 intercept values
	float *intercepts = malloc(sizeof(double)*2);

	intercepts[0] = ((-b + sqrt(d)) / (2*a));
	intercepts[1] = ((-b - sqrt(d)) / (2*a));

	return intercepts; 
}

/*
This function will calculate the positon of when the planes are intecepting and eturn the positions in a array of floats. 
*/

float *calculate_intersection_position(ADSBPacket *head,int main_id,float time_min, float *main_position){
		
	float *intersection_velocity = malloc(sizeof(float)*2);
	intersection_velocity = get_velocity(head, main_id);

	float *final_positions = malloc(sizeof(float)*2);
	final_positions[0] = (main_position[0] + (intersection_velocity[0] * time_min));
	final_positions[1] = (main_position[1] + (intersection_velocity[1] * time_min));

	return final_positions;
}

/*
This function will do calculations to find the quadratic values needed to get the time intersection. It will return an array of values; a, b, c and d
*/

float *get_quadratic_values(ADSBPacket *head, int number_of_planes, int main_id, int i, int *planes_in_system, float separation_time, int safe_distance){
	// This array will store the values that we will be returning
	float *quadratic_values = malloc(sizeof(float)*4);
	// This array will stor the values of the change in the position
	float *position_change = malloc(sizeof(float)*number_of_planes);
	position_change = both_plane_positions(position_change, main_id, planes_in_system[i], separation_time, head);

	// Now we need to find the change in velocity north and east
	float *velocity_change = malloc(sizeof(float)*number_of_planes);
	velocity_change = get_velocity_change(head, main_id, planes_in_system[i], velocity_change);

	//a
	quadratic_values[0] = ((velocity_change[0] * velocity_change[0]) + (velocity_change[1] * velocity_change[1]));

	//b 
	quadratic_values[1] = 2 * ((velocity_change[0]*position_change[0]) + (velocity_change[1]* position_change[1]));

	//c
	quadratic_values[2] = (position_change[0]*position_change[0]) + (position_change[1]*position_change[1]) - (safe_distance*safe_distance);

	//d
	quadratic_values[3] = get_discriminant(quadratic_values[0], quadratic_values[1], quadratic_values[2]);
	
	// Free heap 
	free(position_change);
	free(velocity_change);

	return quadratic_values;
}

/*
This function will take in the quadratic values and return the smallest positive time intercept as a float. 

If the time intercepts are less than 0, we can disregard it as it either means no collision or the a current collision

We only want to return the smallest positive value of the time intersection. 
*/

float get_first_collision(float *quadratic_values_array)
{
	// solve intercepts 
	float *time_intercepts = get_intercepts(quadratic_values_array[0], quadratic_values_array[1], quadratic_values_array[3]);

	// Initialize with a negative value
	float smallest_positive = -1; 

	// Iterate through o find smallest positive time
	for (int i = 0; i < 2; i++) 
	{
		if (time_intercepts[i] > 0 && (smallest_positive < 0 || time_intercepts[i] < smallest_positive)) 
		{
			smallest_positive = time_intercepts[i];
		}
	}

	// If both times are negative
	if (smallest_positive == -1)
	{
		return 0; 
	}

	// Free heap
	free(time_intercepts);

	return smallest_positive;
}

/*
This function finds the soonest time of intersection out of all the collisions that have occured and prints the separation issue statement
*/

void print_separation_issue(ADSBPacket *head, int time_counter, int main_id, float *mains_position, float *time_array){
	// Intitialise min time to be the first variable
	float time_min = time_array[0];
	// get the first time out of all the time values
	for (int w=0; w < time_counter; w++)
	{
		if (time_array[w]< time_min)
		{
			time_min = time_array[w];
		}
	}
	// Print the collision position 
	float *position_at_x = calculate_intersection_position(head, main_id, time_min, mains_position);
	printf("Separation issue: N:%.1lf,E:%.1lf\n", position_at_x[0], position_at_x[1]);					
}

/*
This is the final check separation function, which ultimately determines if there are going to be any separation issues the plane requested. Printing the necessary statements and returning void. 
*/

void check_separation_final(char **request_array, ADSBPacket *head){
	// plane we are testings id
	int main_id = atoi(request_array[3]);
	// Get the time that we are assesing
	double separation_time = get_double_time(request_array[0], request_array[1]);
	// Get the safe distance
	int safe_distance = atoi(request_array[4]);
	// Find out how many planes are currently operating
	int number_of_planes = get_total_contacts(request_array, head);
	// Check if any planes have close altitude 
	int altitude = find_plane_alt(main_id, head);	
	// store all the current operating plane ID's in an array
	int *planes_in_system = malloc(sizeof(int)*number_of_planes);
	planes_in_system = get_plane_id_array(request_array, head, planes_in_system);
	// Store the current posiiton of the planes_in_system
	float *mains_position = malloc(sizeof(float)*2);
	mains_position = estimate_position(separation_time, head, main_id, mains_position);
	// This counter will increment everytime a plane in the list is cleared of separation issues
	// It will be used at the end for the print statement of no separation
	int counter = 0;
	// Create an array whcih we will store all the time stamps inside of to calculate which plane has the first intersection
	float *time_array = malloc(sizeof(float)*(number_of_planes-1)*2);
	// This is a counter to index the time array
	int time_counter = 0;

	// RUn a for loop to test, depends on how many planes are in the area
	for (int i=0; i<number_of_planes; i++)
	{
		// Assess all other planes in system
		if (planes_in_system[i] != main_id)
		{		
			int test_altitude = find_plane_alt(planes_in_system[i], head);
			// Determine if the planes are in an assessible altitude
			if (check_altitude(altitude, test_altitude))
			{
				
				float *quadratic_values_array = get_quadratic_values(head, number_of_planes, main_id, i, planes_in_system, separation_time, safe_distance);
				// If the discriminant is less than or equal 0
				if (quadratic_values_array[3] <= 0)
				{
					counter++;
				}

				// If there is 2 intersecitons 
				else 
				{
					// Get the first collion time if there is one
					// if there is not, float will == 0
					float collision_time = get_first_collision(quadratic_values_array);

					// Store useful times in an array 
					if (collision_time != 0)
					{
						time_array[time_counter] = collision_time;
						time_counter++;
					}
					// If the collision occured in the past
					else
					{
						counter++;
					}
				}
			}
			//if planes are not in assesable alititude
			else 
			{
				counter++;
			}
		}
		// Bottom of for loop: if the non-collision counter is equal to number of assesable planes
		// Print no separation issues
		if (counter == (number_of_planes -1))
		{
			printf("No separation issues\n");
		}
	}
	// if there are no separation issues
	if (time_counter != 0)
	{
		print_separation_issue(head, time_counter, main_id, mains_position, time_array);
	}
	// Free allocated memory 
	free(planes_in_system);
	free(time_array);
}
