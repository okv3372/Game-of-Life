#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#include "life.h"

int neighborCount();

// Fills a grid with dead values so that the regular one can be copied into it
void buildDeadGrid(char deadGrid[GRIDY+2][GRIDX+2]){
    for(int i = 0; i<GRIDY+2; i++){
        for(int j=0; j<GRIDX+2; j++){
            deadGrid[i][j] = DEAD;
        }
    }
}

//copies the regular grid into the dead grid make in buildDeadGrid so that the edges don't need to be checked
void addBorders(char original[GRIDY][GRIDX], char borderGrid[GRIDY+2][GRIDX+2]){
    for(int i = 0; i<GRIDY; i++){ //start from the inside of the border grid, [1][1]
        for(int j=0; j<GRIDX; j++){
            borderGrid[i+1][j+1] = original[i][j]; //set the inside to the original grid so [1][1] should be [0][0] ect
        }
    }
}

//Prints the grid
void printGrid(char grid[GRIDY+2][GRIDX+2]){
    if(grid == NULL){
        printf("Invalid life \n");
        return;
        }
    for(int i = 1; i<GRIDY+1; i++){ //Don't print the border
        for(int j = 1; j<GRIDX+1; j++){
            // putchar(grid[j][i]);
            printf("%c", grid[i][j]);
          
        }
        putchar('\n');
    }
}

//Converts the string passed into argument into an int to get the generation number
/*int convGenToInt(char *str){
    int gen = 0;
    for(int i = 0; str[i]!='\0';i++){
        gen += (gen * 10) + (str[i] - '0');
    }
    return gen;
    }*/

//Gets the number of neighbors of a given cell
int neighborCount(char grid[GRIDY+2][GRIDX+2], int row, int col){
    int count = 0;

            //check northwest
            if(grid[row-1][col-1] == LIVE){ count ++;}
            //check north
            if(grid[row-1][col] == LIVE){ count ++;}
            //check northeast
            if(grid[row-1][col+1] == LIVE){ count ++;}
            //check east
            if(grid[row][col+1] == LIVE){ count ++;}
            //check southeast
            if(grid[row+1][col+1] == LIVE){ count ++;}
            //check south
            if(grid[row+1][col] == LIVE){ count ++;}
            //check southwest
            if(grid[row+1][col-1] == LIVE){ count ++;}
            //check west
            if(grid[row][col-1] == LIVE){ count ++;}
       
        
   
    return count;
}


void getGrid(char gridCopy[GRIDY+2][GRIDX+2], char doubleGrid[2][GRIDY+2][GRIDX+2], int layer){
    for(int i = 0; i<GRIDY+2; i++){
        for(int j = 0; j<GRIDX+2; j++){
            gridCopy[i][j] = doubleGrid[layer][i][j];
        }
    }
}


/* Be sure to read life.h and the other given source files to understand
 * what they provide and how they fit together.  You don't have to
 * understand all of the code, but you should understand how to call
 * parse_life() and clearterm().
 */

/* This is where your program will start.  You should make sure that it
 * is capable of accepting either one or two arguments; the first
 * argument (which is required) is a starting state in one of the Life
 * formats supported by parse_life(), and the second (which is optional)
 * is a number of generations to run before printing output and stoppin *
 * The autograder will always call your program with two arguments.  The
 * one-argument format (as described in the handout) is for your own
 * benefit!
 */
int main(int argc, char *argv[])
{

    const char *filename;
    int generation;
    
    if(argc < 2 || argc > 3){
        printf("Incorrect arg number");
        return 1;
    } else if(argc == 2){
        filename = argv[1];
        generation = 0;
    } else{
        filename = argv[1];
        generation = atoi(argv[2]);
    }

    char grid[GRIDY][GRIDX];
    char **lifeGrid = parse_life(filename);

    for(int i = 0; i<GRIDY; i++){
        for(int j = 0; j<GRIDX; j++){
            grid[i][j] = lifeGrid[i][j];//turn parse life into a regular array so its easier to pass it into functions
        }
    }

    char borderGrid[GRIDY+2][GRIDX+2];

    buildDeadGrid(borderGrid); //fills the borderGrid with dead values before copying the original in
    addBorders(grid, borderGrid); //copy the live grid into the inside of the dead one

    char doubleGrid[2][GRIDY+2][GRIDX+2]; //A 3d matrix so that it can go back and fourth like describe in handout
    for(int i = 0; i<GRIDY+2; i++){ 
        for(int j = 0; j<GRIDX+2; j++){
            doubleGrid[0][i][j] = borderGrid[i][j]; //Copies the values from the border grid into both the grids in
            doubleGrid[1][i][j] = borderGrid[i][j]; // the borderGrid so they can be worked from. Both gen 0.
        }
       }
    
    int genNum = 0;

    char finalGrid[GRIDY+2][GRIDX+2];
    
    
    for(int current = 1; ;current = (current + 1) % 2){
        int copy = 0;
        if(current == 0){ //defines which one I should change the data based off of, opposite of the current one.
            copy = 1; // makes sure copy isnt the same as current
        }
        
        if(genNum == generation){ //Checks if i've reached the generation
            getGrid(finalGrid, doubleGrid, current);
            break;
        } else{
            for(int i = 1; i<GRIDY+1; i++){
                for(int j = 1; j<GRIDX+1; j++){
                    char isAlive = doubleGrid[copy][i][j];
                    int neighbors = neighborCount(doubleGrid[copy], i, j);
                    if((neighbors == 3) && (isAlive == ' ')){
                        doubleGrid[current][i][j] = 'X';
                    } else if((neighbors < 2) && (isAlive == 'X')){
                        doubleGrid[current][i][j] = ' ';
                    } else if((neighbors > 3) && (isAlive == 'X')){
                        doubleGrid[current][i][j] = ' ';
                    } else if(((neighbors == 2) || (neighbors == 3)) && (isAlive == 'X')){
                        doubleGrid[current][i][j] = 'X';
                    } else{
                        doubleGrid[current][i][j] = ' ';
                    }
                }
            }
        }
        genNum += 1;
    }
    
    printGrid(finalGrid);
   
    return 0;
}
