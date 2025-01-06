/*
    This file stores the struct that is used to hold the data of the planes. 
*/
#ifndef MESSAGE_H
#define MESSAGE_H

typedef struct packet{
    int id;
    double time;
    double north;
    double east;
    int altitude;
    double heading;
    double speed;
    struct packet *next;
    struct packet *prev;
} ADSBPacket;


#endif

