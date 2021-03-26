#include "SimAnnealing.h"

using namespace std;

/**
*parameterized constructor
params file is the name of the input file, temp is the starting temperature, reduc is the proportional reduction value
**/
SimAnnealing::SimAnnealing(int temp, float reduc){
    srand(time(0));//for the random functions used later in prog
    //retrieving parameter info
    this->start_temp = temp;
    this->curr_temp= static_cast<float>(temp);
    this->reduction = reduc;
}

/** 
*getter. 
*Returns boolean indicating if the problem has been solved and the app can end.
**/
bool SimAnnealing::getSolveCheck(){
  return is_solved;
}

/**
*setter
*param check is the boolean value to see fif the problem has been solved
*no return value
**/
void SimAnnealing::setSolveCheck(bool check){
  this->is_solved = check;
}

/**
*loads the data from the file 'roommates.txt'
*param filename is the name of the file we read the data from 
*no return value
**/
void SimAnnealing::loadInputFile(string filename) {
  //Function will load the given text file into our array
  ifstream input_file;
  input_file.open(filename);//opens the given input file
  //checks if the file name is valid. If not, exit program. Otherwise, read the file info
  if(!input_file){
      cout << "Error opening file. Please check your file name."<<endl;
      exit(-1); 
  }else{
      for(int i = 0; i < 40000; i++) {
      input_file >> rm_compat_list[i];   
  }
  //close the file
  input_file.close();
  }  
}

/**
*fills the rooms up with possible roommates. Just assigned in order
*no return value
**/
void SimAnnealing::popRooms(){
  //Function places 4 roommates into each of the 50 rooms
  //first for loop will iterate through the 50 available rooms
  int rm = 0;//roommate variable
  for (int i = 0; i < 50; i++){
    //second for loop iterates through the possible roommates and populates the array
    for (int j = 0; j < 4; j++){
      num_rooms[i].roommate[j] = rm++;
      //cout <<num_rooms[i].roommate[j]<<endl;
    }
    //calculate the fitness value of the room at index i
    num_rooms[i].calcFitness(rm_compat_list);
  }
}

/** 
*Will determine if the program should end. If specified conditions are met, we reduce our current temperature and increment the total number of swap changes and attepts made
*no return value
**/
void SimAnnealing::tempReduction(){
  //Function doees the proportional reduction for the temp
  //checks for an iteration with 20,000 attempts or over 2000 accepted changes
  if ( change_attempted >= 20000  || change_accepted >= 2000 ){
    //if we reach 0 for change accepted, the program should finish and begin output
    if(change_accepted == 0){
      setSolveCheck(true);
      return;
    }
    //we need to begin the temperature reduction since one of the above conditions has been met
    else{
    //reduce the current temp
    curr_temp *= reduction;
    //add the change accepted to the total changes
    total_changes += change_accepted;
    //reset change accepted back to 0
    change_accepted = 0; 
    //add the changes attempted to the total attempts
    total_attempts += change_attempted;
    //reset the change attempted back to 0
    change_attempted = 0;
    }  
  }
}

/**
*function will determine accept the fitness score of roommates. Will automatically be better if the old_val is higher than new val. Calculates acceptance probability otherwise
*params old_val is the old fitness value, new_val is the new proposed value
*returns boolean indicating if the change should be accepted
**/
bool SimAnnealing::acceptChangeCheck(const int old_val, const int new_val){
  //Function checks if the values should be accepted; returns true if so
  //if the new value is less than the old value, this indicates the change is better and we should accept it. Total fitness val should be as low as possible
    if(new_val <= old_val) {
        return true;
    }
    float acceptance_prob = (float)rand()/RAND_MAX;
    //calculates the acceptance chance. 
    double accept_chance = exp( (old_val - new_val) /curr_temp);
    if(accept_chance >= acceptance_prob) {
      //cout<< "\t returning true. acceptance prob = " << acceptance_prob<<endl;
        return true;
    }
  
    return false;

}

/**
*This function will allow for 2 rooms and will select one student at random from each room and exchange them. 
*params curr_room1 is the randomized room, curr_room2 is the randomized room to swap with
*no return value
**/
void SimAnnealing::roomSwapOneStudent(const int curr_room1, const int curr_room2){
  //Select 2 rooms at random, and 1 student at random from each room; exchange them. 
  //creates room 1 to change 
  RoomSorter new_room1 = RoomSorter(num_rooms[curr_room1]);
  //creates room 2 to change
  RoomSorter new_room2 = RoomSorter(num_rooms[curr_room2]);
  //creates a roomate via random selection
  int roommate1 = rand() % 4;
  //creates second roommate to swap with via random selection
  int roommate2 = rand() % 4;
  //temp val used for swapping
  int temp_val = new_room1.roommate[roommate1];
  //swap
  new_room1.roommate[roommate1] = new_room2.roommate[roommate2];
  new_room2.roommate[roommate2] = temp_val;
  swapHelper(new_room1,new_room2, curr_room1,curr_room2);
}

/**
*This function will allow for 2 rooms and will swap the first 2 students in one room with the last 2 students in the other room
*params curr_room1 is the randomized room, curr_room2 is the randomized room to swap with
*no return value
**/
void SimAnnealing::roomSwapTwoStudent(const int curr_room1, const int curr_room2){
  //Select 2 rooms at random; swap the first 2 students in one room with the last 2 students in the other. 
  //creates room 1 to change 
  RoomSorter room1 = RoomSorter(num_rooms[curr_room1]);
  //creates room 2 to change
  RoomSorter room2 = RoomSorter(num_rooms[curr_room2]);
  //creating a temp val for swapping
  int temp_val;
  //because we only want the first 2 students swapped with the last 2, we iterate up to 2
  for(int i = 0; i < 2; i++){
    temp_val = room1.roommate[i];
    room1.roommate[i] = room2.roommate[4- i - 1];
    room2.roommate[4 - i - 1] = temp_val;
  }
  swapHelper(room1,room2, curr_room1, curr_room2);
}

/**
*This function serves as a helper for the student swapping function. Will calculate fitness value and check if the values should be accepted
*params room1 hold RoomSorter objects, room2 holds RoomSorter objects, r1 is the current room, r2 is the current room to be swapped
*no return value
**/
void SimAnnealing::swapHelper(RoomSorter room1, RoomSorter room2, int r1, int r2){
  //This will caclulate the fitness of each room and save the old/new values. Checks if they should accept changes
  //calculate fitness compatiability
  room1.calcFitness(rm_compat_list);
  room2.calcFitness(rm_compat_list);
  //creating old and new fitness values
  int new_val = room1.fit_val + room2.fit_val;
  int old_val = num_rooms[r1].fit_val + num_rooms[r2].fit_val;
  //if the values are better change and increment the change_accepted var, otherwise do nothing
  if(acceptChangeCheck(old_val, new_val)){
    num_rooms[r1] = room1;
    num_rooms[r2] = room2;
    change_accepted++;
  }
}

/**
*This function will randomly determine which type of swap to use. Rooms are also randomized inside
*no return value
**/
void SimAnnealing::randomSwapDetermination(){
  //Function is to determinw which type of swap to use
  //when this is called, it is trying to make some sort of change and should be incremented
  change_attempted++;

  //creates two rooms, randomly selects the room
  int room1 = rand() % 50; //randomizes a room from 0-49
  int room2 = rand() % 50; //randomizes a room from 0-49
  //if the two rooms are the same, run the randomizer again. cannot swap the same room
  while(room1 == room2) {
    room2 = rand() % 50;
  }
  //if the rand_num_gen results in 1, then use one type of swap. otherwise use the other type
  int swap_selection = rand() % 2;//randomizes between 0 and 1
  if(swap_selection == 1) {
    roomSwapOneStudent(room1,room2);
  }
  else {
    roomSwapTwoStudent(room1,room2);
  }
}

/**
*This function will produce the output file with all details of the program
*no return value
**/
void SimAnnealing::createOutputFile(){
  //Function that will create the output file with the requested info
  ofstream output_file;//output file
  int best = num_rooms[0].fit_val;//best fit val
  int worst = num_rooms[0].fit_val;//worst fit val
  int avg = 0;//average fit val
  int best_room_num= 0;//best room number
  int worst_room_num= 0;//worst room number

  output_file.open( "output.txt");
  //small error handling
  if(!output_file) {
    cout << "File could not be created. Check files";
    exit(-1);
  }
  //iterate through all the rooms to find out best,worst,average score, and best/worst room number
  for(int i = 0; i < 50; i++){
    //increment the average by the fit val of this specific room
    avg += num_rooms[i].fit_val;
    //if the best value is larger than the current value, then that value is better than the current best value and it should be replaced
    if(best > num_rooms[i].fit_val){
      best = num_rooms[i].fit_val;
      if (i == 0){
        best_room_num = 1;
      }else{
        best_room_num = i+1;
      }
    }
    //if the worst value is smaller than the current value, then that value is worse than the current worst value and should be replaced
    else if(worst < num_rooms[i].fit_val){
      worst = num_rooms[i].fit_val;
      worst_room_num = i+1;
    }
  }

  //display the best, worst, and average room compatibility score along with the parameters of your search
  output_file
  <<"SCORES:\n"
  << "\nBest fitness value of:  \t" <<best << "\t in room number \t"
  <<best_room_num
  << "\nWorst fitness value of: \t" <<worst<< "\t in room number \t"
  <<worst_room_num
  //display search parameters
  <<"\n-------------------------------------------------------------\n"
  <<"SEARCH PARAMETERS:\n"
  << "\nStarting temperature:            \t"<< start_temp
  << "\nProportional Reduction:          \t"<< reduction
  <<"\n-------------------------------------------------------------\n"
  <<"PROGRAM INFORMATION:"
  << "\nTotal swaps attempted:           \t" <<total_attempts
  << "\nTotal swaps completed:           \t" <<total_changes
  << "\nAverage fitness value:           \t" <<(avg / 50)
  <<"\n-------------------------------------------------------------\n"
  //display room assignments 
  <<"ROOM ASSIGNMENTS:\n"<<endl;
  for (int i = 1; i <= 50; ++i) {
    output_file << "Room " << i << "\t----> \t" <<num_rooms[i-1].displayRooms()<<endl;
  }
  output_file.close();
}

