#include "RoomSorter.h"

using namespace std;

/**
*calculates the fitness value of roommates in a room
*param roommate_compat is the roommate compatability value
*no return value
**/
void RoomSorter::calcFitness(int roommate_compat[4]){
  fit_val = 0;//intilizing the fitess score
  for(int rm_1 = 0; rm_1 <3;  rm_1++) {
    for(int rm_2= rm_1 + 1; rm_2 < 4; rm_2++) {
      //calculating fitness value here
      fit_val += roommate_compat[roommate[rm_1] * 200 + roommate[rm_2]];
    }
  }
  //cout <<"calc fitness function " << fit_val <<endl;
}

/**
*file serves to format printing the room information
*no return value
**/
const string RoomSorter::displayRooms(){
  //creates an output string that will concatenate the fitness values and the students listed in the room according the the input file
  string output = "\tFitness Score of ";
  output += to_string(fit_val);
  output += "\t \t Students inside {";

  for(int student = 0; student < 3; student++) {
    //concatenating the roommates into the output string
    output += to_string(roommate[student]);
    output += ", ";
  }
    //adding the final roommate without the comma
  output += to_string(roommate[3]);
  output += "}";

  return output;
}

