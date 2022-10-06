#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "robot_example.h"

int driving_mode = 0;
int rescued = 0;
int energy = 0;
char coming_from = 'X'; // We start on the space we need to return to

// THIS IS THE FUNCTION YOU IMPLEMENT
// A helper function to find the optimised path to the target
int find_path(int raw_count, int r_x, int r_y, int t_x, int t_y, char*world) {
    // In the target
    if (r_x == t_x && r_y == t_y) { 
        return 6;     
    }

    // Calculate possible ways
    int robot_index = r_x*raw_count + r_y;
    int down = (r_x+1)*raw_count + r_y;
    int right = r_x*raw_count + r_y+1;
    int up = (r_x-1)*raw_count + r_y;
    int left = r_x*raw_count + r_y-1;

    // Mark the index as visited so robot won't come again to the same position
    char character_copy[4] = {world[robot_index+raw_count], world[robot_index+1], world[robot_index-raw_count], world[robot_index-1]};
    world[robot_index] = 'V';
    char target_char = (rescued > 0) ? 'X' : 'T';

    // South
    if(r_x < t_x && (world[down] == 'O' || world[down] == '~' || world[down] == target_char) && find_path(raw_count, r_x+1, r_y, t_x, t_y, world)) {
        if(character_copy[0] != coming_from && character_copy[0] != 'T' && character_copy[0] != 'X' && ((coming_from == 'O' && driving_mode == 0) || (coming_from == '~' && driving_mode == 1) || (coming_from == 'X' && driving_mode == 0))) { 
            if(character_copy[0] == 'O' && coming_from == 'X') { return 3; }
            return 5; 
        }
        return 3;
    }

    // East 
    else if(r_y < t_y && (world[right] == 'O' || world[right] == '~' || world[right] == target_char) && find_path(raw_count, r_x, r_y+1, t_x, t_y, world)) {
        if(character_copy[1] != coming_from && character_copy[1] != 'T' && character_copy[1] != 'X' && ((coming_from == 'O' && driving_mode == 0) || (coming_from == '~' && driving_mode == 1) || (coming_from == 'X' && driving_mode == 0))) { 
            if(character_copy[1] == 'O' && coming_from == 'X') { return 2; }
            return 5; 
        }
        return 2;
    }

    // North
    else if(r_x > t_x && (world[up] == 'O' || world[up] == '~' || world[up] == target_char) && find_path(raw_count, r_x-1, r_y, t_x, t_y, world)) {
        if(character_copy[2] != coming_from && character_copy[2] != 'T' && character_copy[2] != 'X' && ((coming_from == 'O' && driving_mode == 0) || (coming_from == '~' && driving_mode == 1) || (coming_from == 'X' && driving_mode == 0))) { 
            if(character_copy[2] == 'O' && coming_from == 'X') { return 1; }
            return 5; 
        }
        return 1;
    } 

    // West
    else if(r_y > t_y && (world[left] == 'O' || world[left] == '~' || world[left] == target_char) && find_path(raw_count, r_x, r_y-1, t_x, t_y, world)) {
        if(character_copy[3] != coming_from && character_copy[3] != 'T' && character_copy[3] != 'X' && ((coming_from == 'O' && driving_mode == 0) || (coming_from == '~' && driving_mode == 1) || (coming_from == 'X' && driving_mode == 0))) { 
            if(character_copy[3] == 'O' && coming_from == 'X') { return 4; }
            return 5; 
        }
        return 4;
    }
    
    // There is no path further
    return 0;
}

// Move the robot accordingly
int move(char *world) {
    int robot_index, target_index, raw_count, check = 1;
    int r_x, r_y, t_x, t_y;

    // Get a copy
    char temp[200];
    for (int i = 0; i < 200; i++) { temp[i] = world[i]; }
    
    unsigned int elements = sizeof(temp)/sizeof(temp[0]);
    
    // Calculate actual index
    for(int i = 0; i < elements; i++) {
        if (temp[i] == 'R') { robot_index = i; }
        if (temp[i] == 'T' && rescued == 0) { target_index = i; }
        if (temp[i] == 'X' && rescued > 0) { target_index = i; }

        if (check == 1 && temp[i] == '\n') { 
            raw_count = i+1;
            check = 0;
        }
    }

    // Map indexes into x,y coordinates
    r_x = robot_index / raw_count;
    t_x = target_index / raw_count;
    r_y = robot_index % raw_count;
    t_y = target_index % raw_count;

    
    return find_path(raw_count, r_x, r_y, t_x, t_y, temp);
}

// Return target index
int update_world(int movement, char *world, int robot_index, int width) {
    int target_index = 0;
    // NORTH
    if(movement == 1) {
        energy += 10;
        target_index = robot_index-(width+1); // +1 for the newline
    }
    // SOUTH
    else if(movement == 3) {
        energy += 10;
        target_index = robot_index+(width+1); // +1 for the newline
    }
    // EAST
    else if(movement == 2) {
        energy += 10;
        target_index = robot_index+1;
    }
    // WEST
    else if(movement == 4) {
        energy += 10;
        target_index = robot_index-1;
    }
    else if(movement == 5) {
        energy += 30;
        printf("Toggling mode!\n");
        target_index = robot_index;
        if(driving_mode == 0) { driving_mode = 1; }
        else { driving_mode = 0; }
    }
    printf("Energy used so far: %d \n", energy);
    
    // ACTION
    if(world[target_index] == 'O') {
        if(driving_mode != 0) {
            printf("FAILURE, tried to drive on land in water mode!");
            exit(1);
        }
        else {
            world[target_index] = 'R';
            world[robot_index] = coming_from;
            coming_from = 'O';
            return target_index;
        }
    }
    // Stay in the same location (used for driving mode toggling)
    else if(world[target_index] == 'R') {
        return target_index;
    }
    // Walls
    else if(world[target_index] == '#') {
        printf("%s", world);
        printf("%c", '\n');
        printf("FAILURE, crashed into a wall!");
        exit(1);
    }
    // Obstacles (can be destroyed later)
    else if(world[target_index] == '*') {
        printf("%s", world);
        printf("%c", '\n');
        printf("FAILURE, crashed into an obstacle!");
        exit(1);
    }
    else if(world[target_index] == '~') {
        if(driving_mode != 1) {
            printf("FAILURE, entered water in land mode!");
            exit(1);
        }
        else {
            world[target_index] = 'R';
            world[robot_index] = coming_from;
            coming_from = '~';
            return target_index;
        }
        
    }
     else if(world[target_index] == 'T') {
        world[target_index] = 'R';
        world[robot_index] = coming_from;
        rescued++;
        coming_from = 'O'; // Targets are alwas considered to be on 'O' spaces
        return target_index;
    }
    else if(world[target_index] == 'X') {
        world[target_index] = 'R';
        world[robot_index] = coming_from;
        if(rescued > 0) {
            printf("%s", world);
            printf("%c", '\n');
            printf("SUCCESS, target returned home!");
            exit(0);
        } else {
            coming_from = 'X';
            return target_index;
        }

    }
}

int main() {
    const int MAX_STEPS = 200;
    int step = 1;
    
    int movement;
    int width = 20; // excluding newlines

    // The maps
    char world8[200] = {
        '#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','\n',
        '#','O','T','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','#','\n',
        '#','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','#','\n',
        '#','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','#','\n',
        '#','O','R','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','#','\n',
        '#','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','#','\n',
        '#','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','#','\n',
        '#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','\n'
    };

    char world2[200] = {
        '#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','\n',
        '#','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','#','\n',
        '#','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','#','\n',
        '#','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','#','\n',
        '#','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','#','\n',
        '#','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','#','\n',
        '#','O','R','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','T','#','\n',
        '#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','\n'
    };

    char world3[200] = {
        '#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','\n',
        '#','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','#','\n',
        '#','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','#','\n',
        '#','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','#','\n',
        '#','O','O','O','O','O','O','O','O','O','R','O','O','O','O','O','O','O','O','#','\n',
        '#','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','#','\n',
        '#','O','O','O','T','O','O','O','O','O','O','O','O','O','O','O','O','O','O','#','\n',
        '#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','\n'
    };

    char world4[200] = {
        '#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','\n',
        '#','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','R','#','\n',
        '#','O','O','O','O','O','O','O','O','O','O','O','O','T','O','O','O','O','O','#','\n',
        '#','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','#','\n',
        '#','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','#','\n',
        '#','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','#','\n',
        '#','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','#','\n',
        '#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','\n'
    };

    char world5[200] = {
        '#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','\n',
        '#','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','#','\n',
        '#','O','O','O','O','#','#','#','#','#','O','O','O','O','O','O','O','O','O','#','\n',
        '#','O','O','O','O','#','T','O','O','#','O','O','O','O','O','O','O','O','O','#','\n',
        '#','O','O','O','O','#','#','#','O','#','O','O','O','O','O','O','O','O','O','#','\n',
        '#','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','#','\n',
        '#','O','O','O','O','O','O','O','O','O','O','R','O','O','O','O','O','O','O','#','\n',
        '#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','\n'
    };

    char world6[200] = {
        '#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','\n',
        '#','O','T','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','#','\n',
        '#','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','#','\n',
        '#','~','~','~','~','O','O','O','O','O','O','O','O','O','O','O','O','O','O','#','\n',
        '#','~','~','~','~','O','O','O','O','O','O','O','O','O','O','O','O','O','O','#','\n',
        '#','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','#','\n',
        '#','O','R','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','#','\n',
        '#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','\n'
    };

    char world7[200] = {
        '#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','\n',
        '#','O','O','~','O','O','O','O','~','O','O','O','O','O','O','O','O','O','O','#','\n',
        '#','O','O','R','O','O','O','O','~','O','O','O','O','O','O','O','O','O','O','#','\n',
        '#','~','~','~','~','O','O','O','~','O','O','O','O','O','O','O','O','O','O','#','\n',
        '#','~','T','~','~','O','O','O','~','O','O','O','O','O','O','O','O','O','O','#','\n',
        '#','~','O','~','O','O','O','O','~','O','O','O','O','O','O','O','O','O','O','#','\n',
        '#','O','O','O','O','O','O','O','~','O','O','O','O','O','O','O','O','O','O','#','\n',
        '#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','\n'
    };

    char world1[200] = {
        '#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','\n',
        '#','O','O','~','O','O','O','O','~','O','O','O','O','O','O','O','O','O','O','#','\n',
        '#','O','O','R','O','O','O','O','~','O','O','O','O','O','O','O','O','O','O','#','\n',
        '#','*','*','*','*','O','O','O','~','O','O','O','O','O','O','O','O','O','O','#','\n',
        '#','O','T','~','~','O','O','O','~','O','O','O','O','O','O','O','O','O','O','#','\n',
        '#','O','O','~','O','O','O','O','~','O','O','O','O','O','O','O','O','O','O','#','\n',
        '#','O','#','O','O','O','O','O','~','O','O','O','O','O','O','O','O','O','O','#','\n',
        '#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','\n'
    };

    // Set the world to use
    char world[200];
    memcpy(world, world1, sizeof(world1));

    // Initialize target and robot positions
    // Assumes only one target, one robot
    unsigned int elements = sizeof(world)/sizeof(world[0]);

    // Initialize the index of the robot and the target.
    // Assumes exactly one robot and one target
    int robot_index;
    int target_index;

    for(int i = 0; i < elements; i++) {
    if (world[i] == 'R') { robot_index = i; }
    if (world[i] == 'T') { target_index = i; }
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