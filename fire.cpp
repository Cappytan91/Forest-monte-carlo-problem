#include <iostream>
#include <cstdlib>
#include <iomanip>
#include <ctime>
#include "rang.hpp"
//#include <windows.h>

#define NUMR 10
#define NUMC 10

#define GENS 25


using namespace std;

class land{
    public:
        int trees = 0;
        bool onFire = false;

};

void printLand(land area[NUMR][NUMC]){                      //prints the array in ✨color✨
    //HANDLE  hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    //SetConsoleTextAttribute(hConsole, 10);  // green

    for(int i = 0; i < NUMR; i++){
        for(int j = 0; j < NUMC; j++){
            if(area[i][j].trees == 0){
                //SetConsoleTextAttribute(hConsole, 8);  // white
                cout << rang::fg::black << setw(4) << area[i][j].trees;
            }else if(area[i][j].onFire){
                //SetConsoleTextAttribute(hConsole, 12);  // red
                cout << rang::fg::red << setw(4) << area[i][j].trees;
            }else{
                //SetConsoleTextAttribute(hConsole, 10);  // green
                cout << rang::fg::green << setw(4) << area[i][j].trees;
            }
            
        }
        cout << "\n";
    }
    //SetConsoleTextAttribute(hConsole, 15);  // white
    cout << rang::fg::reset << "\n";

}

void cpyArr(land gen1[NUMR][NUMC], land gen2[NUMR][NUMC]){         // copies from the 2nd array
    for(int i = 0; i < NUMR; i++){                                 // into the first one
        for(int j = 0; j < NUMC; j++){
            gen1[i][j] = gen2[i][j];
        }
    }

}

void updateTrees(land gen1[NUMR][NUMC], land gen2[NUMR][NUMC], int row, int col){       // similar to the checker function in our previous program
                                                                                        // this one gets both arrays, rol and col, and runs a bunch
    land tmp;    // used to copy the classes into each other without changing stuff     // of checks on the data. afterwards copies it into the 2nd array

    if(gen1[row][col].onFire && gen1[row][col].trees > 0){  // if on fire and more than 0 trees
        tmp = gen1[row][col];
        tmp.trees--;            // subtract 1 tree

        gen2[row][col] = tmp;

        int chance = 5;         // temp for the moment, just used to get a random number

        //  fire spreading logic below

        //  is not out of bounds && is not 0 && is not on fire && (chance to spread)
        if(row - 1 >= 0 && gen1[row-1][col].trees > 0 && !gen1[row-1][col].onFire && rand() % chance == 0){
            tmp = gen1[row-1][col];
            tmp.onFire = true;

            gen2[row-1][col] = tmp;
        }
        if(row + 1 < NUMR && gen1[row+1][col].trees > 0 && !gen1[row+1][col].onFire && rand() % chance == 0){
            tmp = gen1[row+1][col];
            tmp.onFire = true;

            gen2[row+1][col] = tmp;
        }

        if(col - 1 >= 0 && gen1[row][col-1].trees > 0 && !gen1[row][col-1].onFire && rand() % chance == 0){
            tmp = gen1[row][col-1];
            tmp.onFire = true;

            gen2[row][col-1] = tmp;
        }
        if(col + 1 < NUMC && gen1[row][col+1].trees > 0 && !gen1[row][col+1].onFire && rand() % chance == 0){
            tmp = gen1[row][col+1];
            tmp.onFire = true;

            gen2[row][col+1] = tmp;
        }
        
    }else if(gen1[row][col].trees == 0){    // if no trees, then no fire
        tmp = gen1[row][col];
        tmp.onFire = false;
        gen2[row][col] = tmp;
    }

}

int main() {

    land area[NUMR][NUMC];
    land gen2[NUMR][NUMC];

    for(int i = 0; i < NUMR; i++){          // fill array with data
        for(int j = 0; j < NUMC; j++){
            area[i][j] = land();
            area[i][j].trees = rand() % 12;
            
        }
    }

    srand(time(0));             // set seed after filling array to have the same forest, but different results

    area[4][4].onFire = true;           // set a starting point for fire
                                        //this will be changed at some point and a fire will start randomly

    cout << "Generation " << 1 << ":\n";
    printLand(area);
    cpyArr(gen2, area);


    for(int gen = 2; gen < GENS + 1; gen++){    // generation loop
        for(int i = 0; i < NUMR; i++){              // row loop
            for(int j = 0; j < NUMC; j++){              // column loop
                updateTrees(area, gen2, i, j);              // update the selected trees
                
            }
        }
        cpyArr(area, gen2);
        cout << "Generation " << gen << ":\n";
        printLand(area);
    }

    cout << "Hello World!";
    return 0;
}