/*
 *		2048 written by Waleed El-Geresy (thankfully not in the same number of lines)
 *      Idea for game not my own
 *      i = row, j=column (subset of row) in the code (in case it isn't obvious)
 *      Note that when entering the name of the input file, you need not enter the .txt extension
 */

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <cstdlib>
using namespace std;

struct reference //to be used for creating a vector of free spaces
{
	int i;
	int j;
};

void gameGenerate(bool &gridFull, vector< vector<int> > &gameSpace); 		//generates the next 2 in a random empty space using the struct reference and checks for a full grid
bool gameOverCheck(bool gridFull, const vector< vector<int> > gameSpace); 	//checks the game is not over before input by seeing whether any two squares are alike
void gameDisplay(const vector< vector<int> > gameSpace); 					//displays the game grid on the screen each move
int gamePlay(char command, vector< vector<int> >& gameSpace); 				//makes a move and checks for the end of the game

int main()
{
	//DEFINE STARTING VARIABLES
	vector< vector<int> > gameSpace;//define the game space (a four by four grid)
	int gameOver = 0; //will become 1 when the game is over
	char command; //will be w, a, s or d
	ifstream infile;
	bool gridFull=false;

	//LOAD THE GAMESPACE
	string filename;
	cout << "Please could you enter the name of the file containing the starting values: ";
	cin >> filename;
	string filename2 = filename + ".txt";
	infile.open(filename2.c_str());
	//FILL THE GAMESPACE
	int temp;
	vector<int> smallVector;
	if(infile.is_open())
	{
		cout << "Found file... loading game..." << endl;
		for(unsigned int i=0; i<4; i++)
		{
			smallVector.clear();
			for(unsigned int j=0; j<4; j++)
			{
				infile >> temp;
				smallVector.push_back(temp);
			}
			gameSpace.push_back(smallVector);
		}
	}
	else
	{
		temp=0;
		cout << "Could not find a file by that name... Loading default initial configuration..." << endl;
		for(unsigned int i=0; i<4; i++)
		{
			smallVector.clear();
			for(unsigned int j=0; j<4; j++)
			{
				smallVector.push_back(temp);
			}
			gameSpace.push_back(smallVector);
		}
		gameSpace[3][3]=2; //Make sure that the last value is 2.
	}

	//ASK FOR INPUT AND INTITIAL GRID DISPLAY
	cout << "Please enter w, a, s, d for the direction and q to quit.";
	gameDisplay(gameSpace);

	//MAIN GAME LOOP (command, plays command, generates 2 in empty element, checks if the grid is full after play
	//displays the output, if the game is full after play then game over
	while(gameOver!=1)
	{
		cin >> command;
		gameOver = gamePlay(command, gameSpace); //will quit if q is entered
		if(gameOver!=2){ //only do this if a command actually does something
			gameGenerate(gridFull, gameSpace);
			gameDisplay(gameSpace);
			if(gameOver!=1){ //we may have quit already
				gameOver = gameOverCheck(gridFull, gameSpace);
			}
		}
	}

	//GAME OVER (when main game loop is finished)
	cout << "GAME OVER" << endl;
	if(command=='q')
	{
		cout << "(You quit the game voluntarily!)";
	}

	return 0;
}

void gameGenerate(bool &gridFull, vector< vector<int> > &gameSpace)
{
	vector<reference> freeElements;
	reference zeroCell;
	gridFull=true;

	for(unsigned int i=0; i<gameSpace.size(); i++) //check every cell for a zero
	{
		for(unsigned int j=0; j<gameSpace[i].size(); j++)
		{
			if(gameSpace[i][j]==0)
			{
				gridFull=false;
				zeroCell.i=i;
				zeroCell.j=j;
				freeElements.push_back(zeroCell);
			}
		}
	}

	if(gridFull==true) //if the grid is filled: game over
	{
		return;
	}

	int randomCell = rand()%(freeElements.size()); //pick a random cell with a zero in it
	gameSpace[freeElements[randomCell].i][freeElements[randomCell].j]=2; //add a 2 to that cell

	if(freeElements.size()==1) //if there was only one space: it has been filled
	{
		gridFull=true;
		return;
	}

	return;
}

bool gameOverCheck(bool gridFull, const vector< vector<int> > gameSpace)
{
	if(gridFull==false)
	{
		return 0;
	}

	for(unsigned int i=0; i<gameSpace.size(); i++) //check rows
	{
		for(unsigned int j=0; j<gameSpace[i].size()-1; j++)
		{
			if(gameSpace[i][j]==gameSpace[i][j+1])
			{
				return 0;
			}
		}
	}
	for(unsigned int i=0; i<gameSpace.size(); i++) //check columns
		{
			for(unsigned int j=0; j<gameSpace[i].size()-1; j++)
			{
				if(gameSpace[j][i]==gameSpace[j+1][i])
				{
					return 0;
				}
			}
		}
	return 1; // if there are no more moves to be made
}

int move(int direction, char direction2, vector< vector<int> >& gameSpace)  //executes a move - a daughter function of gamePlay
{
	int startValue; //values for the starting and ending limits for the vector columns (horizontal) or rows (vertical)
	int endValue;	//note that the end values are one above or below the last value due to the nature of the for loop condition checks

	if(direction==1)
	{
		startValue=0;
		endValue=4;
	}
	else if(direction==-1)
	{
		startValue=3;
		endValue=-1;
	}

	vector< vector<int> > gameSpaceCopy = gameSpace; //created to check for a useless move

	//otherwise, carry on as normal
	if(direction2=='h') //for the horizontal direction
	{
		for(int i=startValue; i!=endValue; i=i+direction) //for each row
		{
			int j = startValue;
			int check = j+direction;
			while(check!=endValue)
			{
				if(gameSpace[i][check]!=0)
				{
					if(gameSpace[i][check]==gameSpace[i][j]) //double original if they are equal
					{
						gameSpace[i][j]=2*gameSpace[i][j];
						gameSpace[i][check]=0;
						j = check;
					}
					else //otherwise ignore the last value and use the new value as the reference
					{
						j = check;
					}
				}
				check = check+direction;
			}
			for(int m=0; m<3; m++) //loop 3 times to ensure that any possibility is accounted for
			{
				for(int k=startValue; k!=endValue; k=k+direction) //shift the cells to fill the empty space and then continue to try combinations
				{
					if(gameSpace[i][k]==0&&k!=endValue-direction)
					{
						gameSpace[i][k]=gameSpace[i][k+direction];
						gameSpace[i][k+direction]=0;
					}
				}
			}
		}
	}

	if(direction2=='v') //for the vertical direction
	{
		for(int j=startValue; j!=endValue; j=j+direction) //for each column
		{
			int i = startValue;
			int check = i+direction;
			while(check!=endValue)
			{
				if(gameSpace[check][j]!=0)
				{
					if(gameSpace[check][j]==gameSpace[i][j]) //double orignal if they are equal
					{
						gameSpace[i][j]=2*gameSpace[i][j];
						gameSpace[check][j]=0;
						i = check;
					}
					else //otherwise ignore the last value and use the new value as the reference
					{
						i = check;
					}
				}
				check = check+direction;
			}
			for(int m=0; m<3; m++)
			{
				for(int k=startValue; k!=endValue; k=k+direction) //shift the cells to fill the empty space and then continue to try combinations
				{
					if(gameSpace[k][j]==0&&k!=endValue-direction)
					{
						gameSpace[k][j]=gameSpace[k+direction][j];
						gameSpace[k+direction][j]=0;
					}
				}
			}
		}
	}

	if(gameSpace==gameSpaceCopy) //if nothing has changed then we will skip the move number generation and display in the main program loop
	{
		return 2;
	}

	return 0;
}

int gamePlay(char command, vector< vector<int> >& gameSpace) //chooses a move using move() - returns 1 to quit, 2 to do nothing, 0 for normal moves
{
	int direction;
	char direction2;

	if(command=='w'||command=='a') //forward direction keys
	{
		direction = 1;
	}
	else if(command=='s'||command=='d') //backward direction keys
	{
		direction = -1;
	}
	else if(command=='q') //quit case
	{
		return 1;
	}
	else //unknown key case (skip command)
	{
		return 2;
	}

	if(command=='w'||command=='s') //vertical direction
	{
		direction2 = 'v';
	}
	else if(command=='a'||command=='d') //horizontal direction
	{
		direction2 = 'h';
	}
	else //unkown key case (skip command)
	{
		return 2;
	}

	return move(direction, direction2, gameSpace);
}

void gameDisplay(const vector< vector<int> > gameSpace)
{
	cout << endl;
	for(unsigned int i=0; i<gameSpace.size(); i++)
	{
		for(unsigned int j=0; j<gameSpace[i].size();j++)
		{
			cout << gameSpace[i][j] << "\t";
		}
		cout << "\n";
	}
	cout << endl;
}
