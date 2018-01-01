#include <cstdlib>
#include <iostream>
#include "bot.h"
#include "screen.h"

using namespace std;

int ROWS;
int COLS;
int iter = 0;
int nextr;
int nextc;
bool found=false;
bool missH=false; 
string arr[35][35]={""}; //initial array to empty 
/* Initialization procedure, called when the game starts:

   init (rows, cols, num, screen, log) 
 
   Arguments:
    rows, cols = the boards size
    num        = the number of ships 
    screen     = a screen to update your knowledge about the game
    log        = a cout-like output stream
*/
void init(int rows, int cols, int num, Screen &screen, ostream &log) 
{
  ROWS = rows;
  COLS = cols;
  log << "Start." << endl;
}


/* The procedure handling each turn of the game:
 
   next_turn(sml, lrg, num, gun, screen, log)
 
   Arguments:
    sml, lrg = the sizes of the smallest and the largest ships that are currently alive
    num      = the number of ships that are currently alive
    gun      = a gun.
               Call gun.shoot(row, col) to shoot: 
                  Can be shot only once per turn. 
                  Returns MISS, HIT, HIT_N_SUNK, ALREADY_HIT, or ALREADY_SHOT.
    screen   = a screen to update your knowledge about the game
    log      = a cout-like output stream
*/
void next_turn(int sml, int lrg, int num, Gun &gun, Screen &screen, ostream &log)
{
 int r, c;  
 if(found==true){ //Ship is found
  if (missH==false){ //Ship is horizontal, no miss
  	r = nextr;
  	c = nextc+1; //hit to the right
  }
  else{ //Miss, therefore ship is vertical, checked at @line 82
   r=nextr+1; //hit below instead
   c=nextc;
  }
 }
 else{ //Otherwise no ship found or just sunk one
  r= iter/COLS;
  c= iter% COLS;
  iter+=1;
  while (arr[r][c]=="empty" || arr[r][c]=="hit" || arr[r][c]=="sunk"){ //Make sure you dont get an already marked coord
	r= iter/COLS;
    c= iter% COLS;
    iter+=1;
  }
 }
  
  log << "Smallest: " << sml << " Largest: " << lrg << ". ";
  log << "Shoot at " << r << " " << c << endl;

  result res = gun.shoot(r, c);

  // add result on the screen
  if (res == MISS){
    screen.mark(r, c, 'x', BLUE);
    arr[r][c]="empty"; //Coord = hit
	if (found==true){ //If you found a ship but the c+1 is a miss
	 missH=true; //Then the ship must be vertical
	}
  }
  else 
    if (res == HIT){
    screen.mark(r, c, '@', GREEN);
	arr[r][c]="hit"; //Coord = hit
	found=true;    //Hit a ship
	nextr=r; // Store r
 	nextc=c; //Store c
  }
  else 
    if (res == HIT_N_SUNK){
      screen.mark(r, c, 'S', RED);
      arr[r][c]="sunk"; //Ship is now sunk
	  
	  if(missH==false){//If it was horizontal 
	    arr[r][c+1]="empty"; //One to the right is empty since ships cant be next to each other
        int i=0;
        while(arr[r][c-i]=="hit" || arr[r][c-i]=="sunk"){ //The entire ship getting marked sunk and around is marked empty
		  arr[r][c-i]="sunk"; //Ship is marked sunk here
		  arr[r+1][c-i]="empty"; // Everything below the ship is empty
		  int errorR= r-1;
		  if(errorR<0){} //Check if its r-1 is less than 0, do nothing, otherwise
		  else{
		  	arr[r-1][c-i]="empty"; // Everything above the ship is empty
		  }		  
		  i++;
 		}
      }
	  else{//If it was vertical
		arr[r+1][c]="empty";
	    int i=0;
        while(arr[r-i][c]=="hit" || arr[r-i][c]=="sunk" ){  //The entire ship getting marked sunk and around is marked empty
		  arr[r-i][c]="sunk";
		  arr[r-i][c+1]="empty"; // Everything right the ship is empty
		  int errorC=c-1;
          if(errorC<0){} //Check if its c-1 is less than 0, do nothing, otherwise
		  else{
		  	arr[r-i][c-1]="empty"; // Everything left the ship is empty
		  }
		  i++;
 		}
      }
      found=false; //Reset
	  missH=false; //Reset
  }
}
