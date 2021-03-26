#include <iostream>
#include "SimAnnealing.h"

using namespace std;

int main() {

//PLEASE NOTE THAT THE PROGRAM MAY TAKE UP TO 20 SECONDS TO RUN. 

 //declaring initial temp, reduction values, and the name of the input file.
 int temperature = 1000;
 float reduction = 0.95;
 string input_text = "roommates.txt";
 
 cout << "Program running....."<<endl;

  //creating objects here
  SimAnnealing a(temperature, reduction);
  bool solve_check = false;
  a.setSolveCheck(solve_check);
  a.loadInputFile(input_text);
  a.popRooms();

  //Run the program
  while(!solve_check) {
    a.randomSwapDetermination();
    a.tempReduction();
    solve_check = a.getSolveCheck();
  }
  //print output file  
  a.createOutputFile();  
  cout << "Program complete. Please check for a file named 'output.txt'.Thank you!"<<endl;

}