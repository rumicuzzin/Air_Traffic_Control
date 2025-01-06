/*
This file is used when a check separation command is given, it gives a modular approach on how to find any separation issues for a given plane at a given time.  
*/
#include "message.h"

/*
    This file includes all the function that area necessary to find out if a plane has or is going to have any separtion issues. 
*/

#ifndef SEPARATION_ISSUE_H
#define SEPARATION_ISSUE_H

/*
this will grab the plane that we are assesings alittude
*/
int find_plane_alt(int id, ADSBPacket *head);

/*
Go through linked list and store all the plane ID's in an array
*/
int *get_plane_id_array(char **request_array, ADSBPacket *head, int *plane_id_store) ;

/*
This fucntion will be checking if there are any planes that are in close altitude with the plane that we are assesing
*/

int check_altitude(int main_alt, int alt);

/*
This function will store the velocity and position of plane 1 and 2 through a counter
*/
float *both_plane_positions(float *data_holder, int main_id, int id_test, double time, ADSBPacket *head);

/*
This function we will calculate the velocity of the plane we are assesing 
Returning 2 floats stored in an array, which refer to north and east.
*/
float *get_velocity(ADSBPacket *head, int id);

/*
This function will use the get velocity function to calculate the velocity change
*/
float *get_velocity_change(ADSBPacket *head, int main_id, int id_test, float *data_holder);

/* 
This fucntion will take in parameters to calculate the discriminant, it will return a float value which will be equal to the discriminant
*/
float get_discriminant(float a, float b, float c);

/*
This function will take in the parameters of the quadratic to and solve the intercepts of t
returning these t values into a array
*/
float *get_intercepts(float a, float b, float d);

/*
This function will calculate the positon of when the planes are intecepting
*/
float *calculate_intersection_position(ADSBPacket *head,int main_id,float time_min, float *main_position);

/*
This function will take in parameters and then return an array of pointers for the values of a, b, c and data
*/
float *get_quadratic_values(ADSBPacket *head, int number_of_planes, int main_id, int i, int *planes_in_system, float separation_time, int safe_distance);

/*
This function will take in the quadratic values and return the smallest positive time intercept
*/
float get_first_collision(float *quadratic_values_array);

/*
This function finds the soonest time of intersection from an array of times and prints the separation issue statement
*/
void print_separation_issue(ADSBPacket *head, int time_counter, int main_id, float *mains_position, float *time_array);

/*
This is the final check separation function, which ultimately determines if there are going to be any separation issues with the plane requested. 
*/
void check_separation_final(char **request_array, ADSBPacket *head);


#endif
