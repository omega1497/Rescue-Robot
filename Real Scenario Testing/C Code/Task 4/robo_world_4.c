#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "robot_example4.h"

int driving_mode = 0;
int rescued = 0;
int energy = 0;
const int ENERGY_MOVE = 10;
const int ENERGY_TOGGLE = 30;
const int ENERGY_DESTROY = 70;
char coming_from = 'X'; 
char facing;


int find_path(int raw_count, int r_x, int r_y, int t_x, int t_y, char*world) {
    // In the target
    if (r_x == t_x && r_y == t_y) { 
        return 10;     
    }

    // Calculate possible ways
    int robot_index = r_x*raw_count + r_y;
    int down = (r_x+1)*raw_count + r_y;
    int right = r_x*raw_count + r_y+1;
    int up = (r_x-1)*raw_count + r_y;
    int left = r_x*raw_count + r_y-1;

    // Mark the index as visited so robot won't come again to the same position
    world[robot_index] = 'V';

    // [down, right, up, left] Characters of each side of the robot
    char character_copy[4] = {world[robot_index+raw_count], world[robot_index+1], world[robot_index-raw_count], world[robot_index-1]};

    // Target that the robot looking for
    if(world[t_x*raw_count + t_y] == 'T' || world[t_x*raw_count + t_y] == 't') {
        facing = world[t_x*raw_count + t_y];
    }
    char target_char = (rescued > 0) ? 'X' : facing;

    // Down
    if(r_x < t_x && (world[down] == 'O' || world[down] == '~' || world[down] == target_char) && find_path(raw_count, r_x+1, r_y, t_x, t_y, world)) {
        if(character_copy[0] != coming_from && character_copy[0] != 'T' && character_copy[0] != 't' && character_copy[0] != 'X' && ((coming_from == 'O' && driving_mode == 0) || (coming_from == '~' && driving_mode == 1) || (coming_from == 'X' && driving_mode == 0))) { 
            if(character_copy[0] == 'O' && coming_from == 'X') { return 3; }
            return 5; 
        } else if(character_copy[0] == coming_from && rescued == 1 && (facing == 'T' && driving_mode == 1) || (facing == 't' && driving_mode == 0)) { 
            facing = 'C';
            return 5;
        }
        return 3;
    }

    // Right 
    else if(r_y < t_y && (world[right] == 'O' || world[right] == '~' || world[right] == target_char) && find_path(raw_count, r_x, r_y+1, t_x, t_y, world)) {        
        if(character_copy[1] != coming_from && character_copy[1] != 'T' && character_copy[1] != 't' && character_copy[1] != 'X' && ((coming_from == 'O' && driving_mode == 0) || (coming_from == '~' && driving_mode == 1) || (coming_from == 'X' && driving_mode == 0))) { 
            if(character_copy[1] == 'O' && coming_from == 'X') { return 2; }
            return 5; 
        } else if(character_copy[1] == coming_from && rescued == 1 && (facing == 'T' && driving_mode == 1) || (facing == 't' && driving_mode == 0)) { 
            facing = 'C';
            return 5;
        }
        return 2;
    }

    // Up
    else if(r_x > t_x && (world[up] == 'O' || world[up] == '~' || world[up] == target_char) && find_path(raw_count, r_x-1, r_y, t_x, t_y, world)) {
        if(character_copy[2] != coming_from && character_copy[2] != 'T' && character_copy[2] != 't' && character_copy[2] != 'X' && ((coming_from == 'O' && driving_mode == 0) || (coming_from == '~' && driving_mode == 1) || (coming_from == 'X' && driving_mode == 0))) { 
            if(character_copy[2] == 'O' && coming_from == 'X') { return 1; }
            return 5; 
        } else if(character_copy[2] == coming_from && rescued == 1 && (facing == 'T' && driving_mode == 1) || (facing == 't' && driving_mode == 0)) { 
            facing = 'C';
            return 5;
        }
        return 1;
    } 

    // Left
    else if(r_y > t_y && (world[left] == 'O' || world[left] == '~' || world[left] == target_char) && find_path(raw_count, r_x, r_y-1, t_x, t_y, world)) {
        if(character_copy[3] != coming_from && character_copy[3] != 'T' && character_copy[3] != 't' && character_copy[3] != 'X' && ((coming_from == 'O' && driving_mode == 0) || (coming_from == '~' && driving_mode == 1) || (coming_from == 'X' && driving_mode == 0))) { 
            if(character_copy[3] == 'O' && coming_from == 'X') { return 4; }
            return 5; 
        } else if(character_copy[3] == coming_from && rescued == 1 && (facing == 'T' && driving_mode == 1) || (facing == 't' && driving_mode == 0)) { 
            facing = 'C';
            return 5;
        }
        return 4;
    } 
    
    // Destroy Down
    else if(r_x < t_x && (world[down] == 'O' || world[down] == '~' || world[down] == '*' || world[down] == target_char) && find_path(raw_count, r_x+1, r_y, t_x, t_y, world)) {
        return 8;
    }

    // Destroy Right 
    else if(r_y < t_y && (world[right] == 'O' || world[right] == '~' || world[right] == '*' || world[right] == target_char) && find_path(raw_count, r_x, r_y+1, t_x, t_y, world)) {        
        return 7;
    }

    // Deestroy Up
    else if(r_x > t_x && (world[up] == 'O' || world[up] == '~' || world[up] == '*' || world[up] == target_char) && find_path(raw_count, r_x-1, r_y, t_x, t_y, world)) {
        return 6;
    } 

    // Destroy Left
    else if(r_y > t_y && (world[left] == 'O' || world[left] == '~' || world[left] == '*' || world[left] == target_char) && find_path(raw_count, r_x, r_y-1, t_x, t_y, world)) {
        return 9;
    } 

    // There is no path further
    return 0;
}

// Move the robot accordingly
int move(char *world, int map_id) {
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
        if (temp[i] == 't' && rescued == 0) { target_index = i; }
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

void reset_world() {
    driving_mode = 0;
    rescued = 0;
    energy = 0;
    coming_from = 'X'; // We start on the space we need to return to
}

// Some ugly hardcoding of the height
void print_world(char *world, int size) {
    for(int i = 0; i< (size+1)*8; i++) {
        if(world[i] == 'O') { printf("%c", 'O'); }
        else { printf("%c", world[i]); }
    }
    printf("\n");
}

// Return target index
int update_world(int movement, char *world, int robot_index, int width) {
    int target_index = 0;
    // NORTH
    if(movement == 1) {
        energy += ENERGY_MOVE;
        target_index = robot_index - (width + 1); // +1 for the newline
    }
    // SOUTH
    else if(movement == 3) {
        energy += ENERGY_MOVE;
        target_index = robot_index + (width + 1); // +1 for the newline
    }
    // EAST
    else if(movement == 2) {
        energy += ENERGY_MOVE;
        target_index = robot_index+1;
    }
    // WEST
    else if(movement == 4) {
        energy += ENERGY_MOVE;
        target_index = robot_index-1;
    }
    else if(movement == 5) {
        energy += ENERGY_TOGGLE;
        printf("Toggling mode!\n");
        target_index = robot_index;
        if(driving_mode == 0) { driving_mode = 1; }
        else { driving_mode = 0; }
    }
    // Obstacles can be destroyed north (6), south (8), east (7) and west (9)
    else if(movement == 6) {
        energy += ENERGY_DESTROY;
        printf("Trying to destroy obstacle (NORTH). ");
        if (world[robot_index - (width + 1)] == '*') {
            printf("SUCCESS.\n");
            world[robot_index - (width + 1)] = 'O';
        }
        else {
            printf("Nothing to destroy. Energy wasted.\n");
        }
        target_index = robot_index;
    }
    else if(movement == 7) {
        energy += 70;
        printf("Trying to destroy obstacle (EAST). ");
        if (world[robot_index+1] == '*') {
            printf("SUCCESS.\n");
            world[robot_index+1] = 'O';
        }
        else {
            printf("Nothing to destroy. Energy wasted.\n");
        }
        target_index = robot_index;
    }
    else if(movement == 8) {
        energy += 70;
        printf("Trying to destroy obstacle (SOUTH). ");
        if (world[robot_index + (width + 1)] == '*') {
            printf("SUCCESS\n");
            world[robot_index + (width + 1)] = 'O';
        }
        else {
            printf("Nothing to destroy. Energy wasted.\n");
        }
        target_index = robot_index;
    }
    else if(movement == 9) {
        energy += 70;
        printf("Trying to destroy obstacle (WEST). ");
        if (world[robot_index-1] == '*') {
            printf("SUCCESS\n");
            world[robot_index-1] = 'O';
        }
        else {
            printf("Nothing to destroy. Energy wasted.\n");
        }
        target_index = robot_index;
    }

    printf("Energy used so far: %d \n", energy);
    
    // ACTION
    if(world[target_index] == 'O') {
        if(driving_mode != 0) {
            print_world(world, width);
            printf("FAILURE, tried to drive on land in water mode!");
            return -2;
            //exit(1);
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
        print_world(world, width);
        printf("FAILURE, crashed into a wall! \n");
        return -2;
        // exit(1);
    }
    // Obstacles (can be destroyed later)
    else if(world[target_index] == '*') {
        print_world(world, width);
        printf("FAILURE, crashed into an obstacle! \n");
        return -2;
        // exit(1);
    }
    else if(world[target_index] == '~') {
        if(driving_mode != 1) {
            printf("FAILURE, entered water in land mode! \n");
            return -2;
            // exit(1);
        }
        else {
            world[target_index] = 'R';
            world[robot_index] = coming_from;
            coming_from = '~';
            return target_index;
        }
        
    }
    // 'T' on land, 't' on water
    else if(world[target_index] == 'T' || world[target_index] == 't') {
        if(world[target_index] == 'T') {
            coming_from = 'O';
        } else {
            coming_from = '~';
        }
        world[target_index] = 'R';
        world[robot_index] = coming_from;
        rescued++;
        return target_index;
    }
    else if(world[target_index] == 'X') {
        world[target_index] = 'R';
        world[robot_index] = coming_from;
        if(rescued > 0) {
            print_world(world, width);
            printf("SUCCESS, target returned home! \n");
            return -1;
            // exit(0);
        } else {
            coming_from = 'X';
            return target_index;
        }
    } else {
        // Catchall fail
        return -3;
    }
}

int main() {
    const int MAX_STEPS = 500;
    int step = 1;
    
    int movement;
    int width = 20; // excluding newlines

    // The maps
    char world1[200] = {
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

    char world8[200] = {
        '#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','\n',
        '#','O','O','~','O','O','O','O','~','O','O','O','O','O','O','O','O','O','O','#','\n',
        '#','O','O','R','O','O','O','O','~','O','O','O','O','O','O','O','O','O','O','#','\n',
        '#','*','*','*','*','O','O','O','~','O','O','O','O','O','O','O','O','O','O','#','\n',
        '#','O','T','~','~','O','O','O','~','O','O','O','O','O','O','O','O','O','O','#','\n',
        '#','O','O','~','O','O','O','O','~','O','O','O','O','O','O','O','O','O','O','#','\n',
        '#','O','#','O','O','O','O','O','~','O','O','O','O','O','O','O','O','O','O','#','\n',
        '#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','\n'
    };

    char world9[200] = {
        '#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','\n',
        '#','O','T','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','#','\n',
        '#','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','#','\n',
        '#','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','#','\n',
        '#','*','*','*','*','O','O','O','O','O','O','O','O','O','O','O','O','O','O','#','\n',
        '#','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','#','\n',
        '#','O','R','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','#','\n',
        '#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','\n'
    };

    char world10[200] = {
        '#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','\n',
        '#','~','~','~','~','~','#','O','O','O','O','O','O','O','O','O','O','O','O','#','\n',
        '#','~','~','~','t','~','#','O','O','O','O','O','O','O','O','O','O','O','O','#','\n',
        '#','~','~','~','~','~','#','O','O','O','O','O','O','O','O','O','O','O','O','#','\n',
        '#','~','~','~','~','~','#','O','O','O','O','O','O','O','O','O','O','O','O','#','\n',
        '#','~','O','~','~','~','#','O','O','O','O','O','O','O','O','O','O','O','O','#','\n',
        '#','~','R','~','~','~','#','O','O','O','O','O','O','O','O','O','O','O','O','#','\n',
        '#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','\n'
    };

    char world11[200] = {
        '#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','\n',
        '#','O','T','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','#','\n',
        '#','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','#','\n',
        '#','O','*','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','#','\n',
        '#','*','R','*','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','#','\n',
        '#','O','*','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','#','\n',
        '#','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','#','\n',
        '#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','\n'
    };

    char world12[200] = {
        '#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','\n',
        '#','~','t','~','*','O','O','O','O','O','O','O','O','O','O','O','O','O','O','#','\n',
        '#','~','~','~','*','O','O','O','O','O','O','O','O','O','O','O','O','O','O','#','\n',
        '#','*','*','*','*','O','O','O','O','O','O','O','O','O','O','O','O','O','O','#','\n',
        '#','O','O','O','O','O','R','O','O','O','O','O','O','O','O','O','O','O','O','#','\n',
        '#','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','#','\n',
        '#','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','#','\n',
        '#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','\n'
    };

    char world13[200] = {
        '#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','\n',
        '#','O','O','O','O','#','#','#','#','#','#','#','O','O','O','O','O','O','O','#','\n',
        '#','O','O','O','O','#','O','O','T','O','O','#','O','O','O','O','O','O','O','#','\n',
        '#','O','O','O','O','#','O','O','O','O','O','#','O','O','O','O','O','R','O','#','\n',
        '#','O','O','O','O','*','O','O','#','O','O','#','O','O','O','O','O','O','O','#','\n',
        '#','O','O','O','O','#','#','#','*','#','#','#','O','O','O','O','O','O','O','#','\n',
        '#','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','#','\n',
        '#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','\n'
    };

    // Set the world to use
    char *maps[] = {world1, world2, world3, world4, world5, world6, world7, world8, world9, world10, world11, world12, world13};
    unsigned long number_of_maps = sizeof(maps)/sizeof(*maps);
    char world[200];
    int energy_scores[number_of_maps];

    // TODO: Loop over all maps here
    for(int i = 0; i < number_of_maps; i++) {
        memcpy(world, maps[i], sizeof(world));
        // Initialize target and robot positions
        // Assumes only one target, one robot
        unsigned int elements = sizeof(world)/sizeof(world[0]);
        // Initialize the index of the robot and the target.

        // Assumes exactly one robot and one target
        int robot_index;
        int target_index;

        for(int i = 0; i < elements; i++) {
            if (world[i] == 'R') {
                robot_index = i; facing = world[i];
            }
            if (world[i] == 'T' || world[i] == 't') { target_index = i; }
        }

        // Print the initial world
        printf("Starting position: \n");
        print_world(world, width);
        // Debug output
        // printf("Robot index: %i / target index: %i %c", robot_index, target_index, '\n');

        while (step <= MAX_STEPS)
        {
            printf("After step number %i: \n", step);

            movement = move(world, i);
            robot_index = update_world(movement, world, robot_index, width);
            if (robot_index >= 0)
            {
                print_world(world, width);
                step = step + 1;
            }
            else if (robot_index == -1)
            {
                // Reset
                step = 1;
                energy_scores[i] = energy;
                reset_world();
                break; // SUCCESS
            }
            else if (robot_index == -2)
            {
                // Reset
                step = 1;
                energy_scores[i] = -1;
                reset_world();
                break; // FAILURE
            }
        }
        if (step >= MAX_STEPS)
        {
            energy_scores[i] = -1;
            printf("FAILURE, maximum number of steps exceeded.");
        }
    }
    
    // Print an overview
    printf("\n*** SCORE *** \n");
    int energy_sum = 0;
    int fail_counter = 0;
    int total_scores = sizeof(energy_scores)/sizeof(energy_scores[0]);
    for(int i = 0; i < total_scores; i++) {
        if(energy_scores[i] > 0) {
            printf("Map %d: %d \n", i+1, energy_scores[i]);
            energy_sum += energy_scores[i];
        }
        else {
            printf("Map %d: FAIL \n", i);
            fail_counter++;
        }
    }
    int finished_runs = total_scores-fail_counter;
    if(finished_runs > 0) {
        printf("Total: %d \n", energy_sum);
        printf("Average: %d ", energy_sum/(total_scores-fail_counter));
    } else {
        printf("Total: FAILED ALL \n");
        printf("Average: FAILED ALL ");
    }
    if(fail_counter > 0) { printf("/ %d failures \n", fail_counter); }
    else {printf("\n"); }
}