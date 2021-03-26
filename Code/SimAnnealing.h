#include <array>
#include <random>
#include <time.h>
#include <iomanip>
#include <fstream>
#include <iostream>
#include <time.h>
#include <string>
#include <tgmath.h>
#include "RoomSorter.h"

using namespace std;

/*info on how to use uniform_int_distribution came from
 https://stackoverflow.com/questions/288739/generate-random-numbers-uniformly-over-an-entire-range
*/
class SimAnnealing {
    public: 
    //CONSTRUCTOR
    SimAnnealing(string file, int temp, float reduc);
    //SETTERS
    bool getSolveCheck();//DONE getter
    void setSolveCheck(bool check);//DONE setter
    //FUNCTIONS
    void loadInputFile(string filename);// DONE reads in info
    void popRooms();//DONE loads the 50 rooms with 4 roommates
    void randomSwapDetermination();//DONE Choose randomly (uniformly) between two possible variations roomSwap 1 student, roomSwap 2 students
    void roomSwapOneStudent(const int room1, const int room2);//DONE Select 2 rooms at random, and 1 student at random from each room; exchange them. 
    void roomSwapTwoStudent(const int room1, const int room2);//DONE Select 2 rooms at random; swap the first 2 students in one room with the last 2 students
    void swapHelper(RoomSorter room1, RoomSorter room2, int r1, int r2);//DONE a function that serves as a helper for the two types of swaps.
    void tempReduction();//DONE reduces the temperature
    bool acceptChangeCheck(const int old_val, const int new_val);//DONE checks if change should be accepted
    void createOutputFile();//DONE creates the output file


    private:
    //VARIABLES
    int rm_compat_list[40000]; //list that holds the 200*200 compatabilities of rroommates from the text file. essentially 2d array
    RoomSorter num_rooms[50];//holds the roomsorter objects to populate the rooms
    int change_accepted = 0; //used to count the number of changes accepted
    int change_attempted = 0; //used to count the number of changes attempted
    int total_changes = 0; //used to count the number of total actual changes
    int total_attempts = 0; //used to count the number of total attempted changes
    int start_temp; //the starting temp
    float curr_temp; //the current temp
    float reduction; //temp reduction
    bool is_solved;//used to check if the program has run to completeness
};