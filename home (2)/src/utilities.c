#include "utilities.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define BUFFER_SIZE 256
#define PERIOD 180
#define SECONDS_IN_HOURS 3600
#define SECONDS_IN_MINUTES 60
#define PERCENTAGE_MULTIPLY 100

/*
This function will combine the two separate string which we need to get the time variable from 
*/

double get_double_time(char *one, char *two){
	char combined_string[BUFFER_SIZE];

	// combine the strings together
	strcpy(combined_string, one);
	strcat(combined_string, ".");
	strcat(combined_string, two);

	// turn into a double
	double time = atof(combined_string);

	return time; 
}

/*
MAth functions are calculate in radians
This function will convert degrees to radians
*/
double degreesToRadians(double degrees) {
    return degrees * (M_PI / PERIOD);
}

/*
This function will conveert time into seconds. It will return a value which is the time differnce
(t - t(ADSB))
*/

int time_converter(double time_0, double time_1){
    double difference = time_1 - time_0;
    int hours = (int)difference;
	float minfloat = ((difference - hours)*PERCENTAGE_MULTIPLY);
	int minutes = round(minfloat);
    int seconds = (hours * SECONDS_IN_HOURS) + (minutes * SECONDS_IN_MINUTES);
    return seconds;
}

/*
This function will traverse through the linked list until it reaches the last node that we have
received information for. It will return a pointer to the last node
*/

ADSBPacket *head_to_tail(ADSBPacket *head) {
    ADSBPacket *current = head;
	ADSBPacket *tail = NULL;

    while (current != NULL) {
		tail = current;
        current = current->next;
    }
	return tail; 
}


