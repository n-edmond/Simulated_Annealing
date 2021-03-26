#include <array>
#include <iostream>

using namespace std;
//NOTE: Originally I was going to include a function to sort the roommates (hence the class name) for making it easier to compare results. After finishing the program I opted to remove it since I did not really see a noticable difference between sorting it and not having it sorted. 

class RoomSorter{
  public:
  //FUNCTIONS
  void calcFitness(int roommates[4]);//calculates the fitness of the given roommates
  const string displayRooms();//function just displays the fitness score of the room
  //VARIABLES
  int roommate[4];//stores the number of roommates authorized per room
  int fit_val;//variable to store the fitness value 
};