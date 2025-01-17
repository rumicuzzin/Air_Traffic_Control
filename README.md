# Air_Traffic_Control

The entire program is made up of functions that are defined across many modules. All files are compiled into the main object file via the makefile.

The program is shaped by the interpretation of text based standard input which is embedded with all the necessary data to construt the Air Traffic Control System. A while loop is constructed to take in the standard input untill the user wishes to close or there is no input (=NULL). 

Inside this loop, tests are run to determine what the line of standard input is.

- If it is a packet of data:

    Functions are used from data_to_struct.h to extrapolate data from the line of text. This flow of functions, removes any unecessary text from the string and stores it in a new cleaned string. This cleaned string is then analysed and the time is converted into a double. Finally, data is converted to a suitable form for storage and is appended into a doubly linked list with each node consisting of a struct retreived from message.h.

- If it is a request:

    - The program will assess which type of request has been sent:

        - Estimate position
         
            Functions are used from the estimate_position.h file. It finds the most recent node of the plane requested in the linked list. Then uses the data from this struct to complete mathematical equations to calculate velocity and position. Then checks if this position is inside the area of operation and prints the necessary statements. 

        - Number of Contacts

            Number of contacts is determined by the function in number_of_contacts.h. This function iterates through the linked list, upping a counter everytime it encounters a unique plane ID. The counter is only increased if the plane is inside the area of operation. The function then returns an integer that represents the amount of planes the control system is in contact with. The satement is then printed in the main file.  

        - Separation Issues

            Functions are used from the separation_issue.h file. The program flows as such; 

            1. Checks to see how many planes are currently in the area of operation
            2. Enters a for loop to test the requested plane against all other planes currently operating
            3. Determines if the planes are flying at a similar alititude
            4. Calculations are done to determine the coefficients in the quadratic with collision time as the unkown variable.
            5. Uses quadratic formula to determine if there are any collsions. Where the intercepts represent the time of collision.
            6. Exits for loop and sorts through collision times to find the earliest time a collision has occured.
            7. Using estimate position and collision time found, calculate the position of first collision. 
            8. Prints necessary statemnts regarding separation issues. 

            (NOTE: An ordered list of flows was used here for clarity) 

        - Close

            Exits the while loop. 

After the loop the main free's any lingering memory on the heap and returns 0 when the program is complete.  
