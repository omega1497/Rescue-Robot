#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// THIS IS THE FUNCTION YOU IMPLEMENT
int move(char* world) {
	int dir = 1; // direction in which robot move
	unsigned int size = strlen(world);	// find length of world
	int target = 0, robot = 0, width = 0; 
	for (int i = 0; i < size; i++) // find width of world
	{
		if (world[i] == '\n') {
			width = i;
			break;
		}
	}
	for (int i = 0; i < size; i++) { // find robot index
		if (world[i] == 'R') {
			robot = i;
			break;
		}
	}
	for (int i = 0; i < size; i++) { // find target index
		if (world[i] == 'T') {
			target = i;
			break;
		}
	}
	// NORTH
	if (robot - target > width) {
		dir = 1;		
	}
	// SOUTH
	else if (target - robot > width) {
		dir = 3;
	}
	// EAST
	else if (robot - target < 0) {
		dir = 2;
	}
	// WEST
	else if (robot - target > 0) {
		dir = 4;
	}	
	return dir; // REPLACE THE RETURN VALUE WITH YOUR CALCULATED RETURN VALUE
}
// Return target index
int update_world(int movement, char *world, int robot_index, int width) {
    int target_index = 0;
    // NORTH
    if(movement == 1) {
        target_index = robot_index-(width+1); // +1 for the newline
    }
    // SOUTH
    else if(movement == 3) {
        target_index = robot_index+(width+1); // +1 for the newline
    }
    // EAST
    else if(movement == 2) {
        target_index = robot_index+1;
    }
    // WEST
    else if(movement == 4) {
        target_index = robot_index-1;
    }
    
    // ACTION
    if(world[target_index] == 'O') {
        world[target_index] = 'R';
        world[robot_index] = 'O';
        return target_index;
    }
    else if(world[target_index] == '#') {
        printf("%s", world);
        printf("%c", '\n');
        printf("FAILURE, crashed into a wall!");
        exit(1);
    }
     else if(world[target_index] == 'T') {
        world[target_index] = 'R';
        world[robot_index] = 'O';
        printf("%s", world);
        printf("%c", '\n');
        printf("SUCCESS, target found!");
        exit(0);
    }
}

int main() {
    const int MAX_STEPS = 200;
    int step = 1;
    
    int movement;
    int width = 20; // excluding newlines

     // The world
    char world[200] = {
        '#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','\n',
        '#','T','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','#','\n',
        '#','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','#','\n',
        '#','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','#','\n',
        '#','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','#','\n',
        '#','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','#','\n',
        '#','O','R','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','#','\n',
        '#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','\n',
    };

    // Initialize target and robot positions
    // Assumes only one target, one robot
    unsigned int elements = sizeof(world)/sizeof(world[0]);

    // Initialize the index of the robot and the target.
    // Assumes exactly one robot and one target
    int robot_index;
    int target_index;

    for(int i = 0; i < elements; ++i) {
    if (world[i] == 'R') {
            robot_index = i;
            break;
        }
    }
    for(int i = 0; i < elements; ++i) {
    if (world[i] == 'T') {
            target_index = i;
            break;
        }
    }

    // Print the initial world
    printf("Starting position: %c", '\n');
    // Debug output
    // printf("Robot index: %i / target index: %i %c", robot_index, target_index, '\n');
    printf("%s", world);
    printf("%c", '\n');
    
    while(step <= MAX_STEPS) {
        printf("After step number %i: %c", step, '\n');

        movement = move(world);
        robot_index = update_world(movement, world, robot_index, width);
        printf("%s", world);
        printf("%c", '\n');
        step = step+1;
    }
    if(step >= MAX_STEPS) {
        printf("FAILURE, maximum number of steps exceeded.");
    }
    
}