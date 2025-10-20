#include <iostream>
#include <cstdlib>
#include <iomanip>
#include <ctime>
#include <math.h>
#include "termcolor.hpp"

#define NUMR 20
#define NUMC 20

#define TRIALS 2000
#define GENS 75


class Wind{
    public:
        int x = 0;
        int y = 0;
};

class Land{
    public:
        int trees = 0;
        bool onFire = false;

};


bool DROUGHT = false;

bool isWindy = false; //turns wind on or off

Wind wind = Wind(); // this is the global wind variable 
//  (it is a vector so it has a direction and magnitude [speed])

using namespace std;



void printLand(Land area[NUMR][NUMC]){                      //prints the array in ✨color✨

    for(int i = 0; i < NUMR; i++){
        for(int j = 0; j < NUMC; j++){
            if(area[i][j].trees == 0){
                cout << termcolor::grey<< setw(4) << area[i][j].trees;
            }else if(area[i][j].onFire){
                cout << termcolor::red << setw(4) << area[i][j].trees;
            }else{
                cout << termcolor::green << setw(4) << area[i][j].trees;
            }
            
        }
        cout << "\n";
    }

    cout << termcolor::reset << "\n";

    if (isWindy){ 
            cout << "Wind Speed: x: " << wind.x << "    y: " << wind.y << " mph" << "\n \n";         // Know how fast wind is so we can observe differences in fire spread
    }

}

void cpyArr(Land gen1[NUMR][NUMC], Land gen2[NUMR][NUMC]){         // copies from the 2nd array
    for(int i = 0; i < NUMR; i++){                                 // into the first one
        for(int j = 0; j < NUMC; j++){
            gen1[i][j] = gen2[i][j];
        }
    }

}

void updateTrees(Land gen1[NUMR][NUMC], Land gen2[NUMR][NUMC], int row, int col){       // similar to the checker function in our previous program
                                                                                        // this one gets both arrays, rol and col, and runs a bunch
    Land tmp;    // used to copy the classes into each other without changing stuff     // of checks on the data. afterwards copies it into the 2nd array

    if(gen1[row][col].onFire && gen1[row][col].trees > 0){  // if on fire and more than 0 trees
        tmp = gen1[row][col];
        tmp.trees--;            // subtract 1 tree

        gen2[row][col] = tmp;

        int chance = 5;         // temp for the moment, just used to get a random number

        
        if (DROUGHT == true){   // increases fire spread rate by 5% to 25% chance of spread rate
            chance--;
        }

        //  fire spreading logic below

        //  is not out of bounds && is not 0 && is not on fire && (chance to spread)
        if(row - 1 >= 0 && gen1[row-1][col].trees > 0 && !gen1[row-1][col].onFire && rand() % (chance - wind.y) == 0){
            tmp = gen1[row-1][col];
            tmp.onFire = true;

            gen2[row-1][col] = tmp;
        }
        if(row + 1 < NUMR && gen1[row+1][col].trees > 0 && !gen1[row+1][col].onFire && rand() % (chance + wind.y) == 0){
            tmp = gen1[row+1][col];
            tmp.onFire = true;

            gen2[row+1][col] = tmp;
        }

        if(col - 1 >= 0 && gen1[row][col-1].trees > 0 && !gen1[row][col-1].onFire && rand() % (chance + wind.x) == 0){
            tmp = gen1[row][col-1];
            tmp.onFire = true;

            gen2[row][col-1] = tmp;
        }
        if(col + 1 < NUMC && gen1[row][col+1].trees > 0 && !gen1[row][col+1].onFire && rand() % (chance - wind.x) == 0){
            tmp = gen1[row][col+1];
            tmp.onFire = true;

            gen2[row][col+1] = tmp;
        }

        if(rand() % 50 == 0)
            gen2[row][col].onFire = false;
        
    }else if(gen1[row][col].trees == 0){    // if no trees, then no fire
        tmp = gen1[row][col];
        tmp.onFire = false;
        gen2[row][col] = tmp;
    }

}

int getPercentDestruction(Land area[NUMR][NUMC]){
    int total = 0;

    for(int i = 0; i < NUMR; i++){          // fill array with data
        for(int j = 0; j < NUMC; j++){
            if(area[i][j].onFire || area[i][j].trees == 0)
                total++;
        }
    }
    total *= 100;
    return total / (NUMR * NUMC);

}

int main() {

    Land startArray[NUMR][NUMC];

    Land area[NUMR][NUMC];
    Land gen2[NUMR][NUMC];

    int destructionAverage = 0;

    for(int i = 0; i < NUMR; i++){          // fill array with data
        for(int j = 0; j < NUMC; j++){
            area[i][j] = Land();
            area[i][j].trees = rand() % 12;
            
        }
    }

    srand(time(0));             // set seed after filling array to have the same forest, but different results

    area[6][4].onFire = true;           // set a starting point for fire
                                        //this will be changed at some point and a fire will start randomly

    cout << "Start:\n";
    printLand(area);
    cpyArr(gen2, area);
    cpyArr(startArray, area);

    for(int trial = 0; trial < TRIALS; trial++){
        cout << "Trial " << trial << ":\n";
        cpyArr(area, startArray);
        cpyArr(gen2, startArray);

     for(int gen = 2; gen < GENS + 1; gen++){    // generation loop     
            isWindy = true;
            DROUGHT = false;
            if (rand() % 5 == 0)                    // drought occurs 1 out of every 5 generations roughly
            {
                DROUGHT = true;
            }
            if (rand() % 2 == 0)                    // wind occurs 1 out of every 2 generations roughly
            {
                int direction = -1; // 50/50 chance to make the direction negative
                if(rand() % 2)
                    direction = 1;        

                if(rand() % 2){      // 50/50 chance to affect x or y of the wind vector
                    wind.x = direction * (rand() % 4);
                    wind.y = 0;
                }else{
                    wind.x = 0;
                    wind.y = direction * (rand() % 4);
                }
            }
            for(int i = 0; i < NUMR; i++){              // row loop
                for(int j = 0; j < NUMC; j++){              // column loop
                    updateTrees(area, gen2, i, j);              // update the selected trees
                    
                }
            }
            cpyArr(area, gen2);
//            cout << "Generation " << gen << ":\n";
//            printLand(area);
            
        }
//        printLand(area);
        cout << "percent destruction: " << getPercentDestruction(area) << "%\n";
        destructionAverage += getPercentDestruction(area);
    }

    destructionAverage /= TRIALS;
    cout << "\naverage destruction: " << destructionAverage << "%\n";
    return 0;
}