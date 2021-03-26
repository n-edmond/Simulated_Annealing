#include <array>
#include <iostream>

using namespace std;

class RoomSorter{
  public:
  //FUNCTIONS
  void calcFitness(int roommates[4]);//calculates the fitness of the given roommates
  const string displayRooms();//function just displays the fitness score of the room
  //VARIABLES
  int roommate[4];//stores the number of roommates authorized per room
  int fit_val;//variable to store the fitness value 
};