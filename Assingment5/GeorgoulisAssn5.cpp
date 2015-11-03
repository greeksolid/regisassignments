//******************************************************************************
// CODE FILENAME: GeorgoulisAssn5.cpp
// DESCRIPTION: The program will display a short description of what it does
// to the user. It will create a 2D array that holds the status of a parking
// garage. It then will check if an input file exists.
// If it exists it will initialize all 2D array elements to 0 and then read
// the data from the file and store them into the appropriate places of the
// array.
// If the file doesn't exist meaning that the program runs for the first time:
// The program will initialize all the 2D array elements to 5.
// It will then display an action menu to the user. The user can turn in a car,
// rent a car or exit the program.
// If he chooses to turn in or rent a car a sub menu will be displayed to the
// user to choose the type of the car or to return to the action menu.
// When the user chooses to exit the program any changes he made will be
// save to the file.
//
// CLASS/TERM: CS362 - 13F8W2
// DESIGNER: Anastasios Georgoulis
// FUNCTIONS:
//
// displayDescription - Displays a short description to the user.
//
// fileExists - Checks if the input file exists and that it can be opened.
//
// initGarage - Initializes the 2D garage elements to a specified value.
//
// optionsMenuHandler - Calls other functions depending on the user's choices.
//
// displayGarageStatus - Displays the status of the garage and returns the
// total number of cars stored in it.
//
// displayActionMenu - Displays the action menu, validates and returns
// the user's choice.
//
// displaySubMenu - Displays the sub menu, validates and returns
// the user's choice.
//
// turnInCar - Allows the user to turn in a car, changing the garage status.
//
// rentCar - Allows the user to rent a car, changing the garage status.
//
// displayLevelsDiagram - Displays a diagram of all the floors of the parking
// garage for the specified car type.
//
// readFromFile - Reads data from a binary file and stores them into the 2D
// garage array.
//
// saveToFile - Reads data from the 2D array and stores them into a binary
// file.
//
// charToEnum - Converts a char to enum and returns it.
//
// charToStrRepr - Converts a char to a string and returns it.
//
// toUppercase - Converts a string to all upper case.
//
// pause - Pauses the program and waits for input from the user.
//******************************************************************************

//*********************************
//***********Libraries*************
//*********************************

#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>

//*********************************
//***********Namespaces************
//*********************************

using namespace std;

// number of car sizes
const int NUM_OF_SIZES = 4;
// number of levels of the parking garage
const int NUM_OF_LEVELS = 6;

// Car Sizes
enum carSize {COMPACT, MIDSIZE, FULLSIZE, SUV};

//*********************************
//******Functions Prototypes*******
//*********************************

void displayDescription();

bool fileExists(ifstream &inputFile, string inputFileName);
void initGarage(int garage[][NUM_OF_LEVELS], int numOfSizes, int desiredValue);

void optionsMenuHandler(int garage[][NUM_OF_LEVELS], int numOfSizes,
						int garageIsFull, int garageIsEmpty, int emptyRow,
						int fullRow, int lowestLevel, int highestLevel,
						int fullValue, int emptyValue);
int displayGarageStatus(int garage[][NUM_OF_LEVELS], int numOfSizes);
char displayActionMenu(int totalCars, int garageIsFull, int garageIsEmpty);
char displaySubMenu();
void turnInCar(int garage[][NUM_OF_LEVELS], char vehicleType, int fullRow,
			   int lowestLevel, int highestLevel, int fullValue);
void rentCar(int garage[][NUM_OF_LEVELS], char vehicleType, int emptyRow,
			   int lowestLevel, int highestLevel, int fullValue,
			   int emptyValue);
void displayLevelsDiagram(string type, carSize size,
						  int garage[][NUM_OF_LEVELS],int &total);

void readFromFile(ifstream &inputFile, string inputFileName,
				  int garage[][NUM_OF_LEVELS]);
void saveToFile(ofstream &outputFile, string outputFileName,
				int garage[][NUM_OF_LEVELS], int numOfSizes);

// Helper Functions
carSize charToEnum(char size);
string charToStrRepr(char size);
void toUppercase(string &str);
void pause(string msg="Press 'ENTER' to continue.");

//**********************************************************************

//*********************************************************************
// FUNCTION: main
// DESCRIPTION: Initializes the variables and calls some helper functions.
// It then displays a short description to the user.
// It will call the fileExists function to check if the input file exists.
// If it does it will call the initGarage function to initialize the 2D
// array elements to 0, load data from the input file and store them to the
// array.
// If it doesn't exist it will call the initGarage function to initialize the
// 2D array elements to 5.
// It will then call the optionsMenuHandler function to display menus and get
// the user's input and when the user chooses to quit the program the
// saveToFile function is called to save the changes to the file.
//
// OUTPUT:
// Return Val: 0 -> If the program terminated without any problems.
//			   1 -> If it there were problems.
// CALLS TO: displayDescription, fileExists, initGarage, readFromFile,
//			 optionsMenuHandler, saveToFile, pause
//**********************************************************************
int main(){
	// CONSTANT VARIABLES

	// the maximum number of cars that can be stored in a column
	const int FULL_VALUE = 5;
	// the minimum number of cars that can be stored in a column
	const int EMPTY_VALUE = 0;
	// the total number of cars that the garage can have
	const int FULL_GARAGE = 120;
	// the amount of cars that the garage has if it's empty
	const int EMPTY_GARAGE = 0;
	// the maximum amount of cars of each type
	const int FULL_ROW = FULL_VALUE * NUM_OF_LEVELS;
	// the minimum amount of cars of each type
	const int EMPTY_ROW = 0;
	// the lowest level of the garage
	const int LOWEST_LEVEL = 0;
	// the highest level of the garage
	const int HIGHEST_LEVEL = NUM_OF_LEVELS - 1;

	const string INPUT_FILE_NAME = "GARAGE.BIN";
	const string OUPUT_FILE_NAME = "GARAGE.BIN";

	// 2D array that represents the garage
	int garage[NUM_OF_SIZES][NUM_OF_LEVELS];

	ifstream inputFile;
	ofstream outputFile;

	// used to check if the binary file already exists
	bool exists;

	displayDescription();

	exists = fileExists(inputFile, INPUT_FILE_NAME);

	// file exists / second and subsequent runs
	if (exists){
		// initialize the garage array to 0
		initGarage(garage, NUM_OF_SIZES, EMPTY_VALUE);

		// read data from the binary file and set proper values
		readFromFile(inputFile, INPUT_FILE_NAME, garage);

	}
	// file doesn't exist / first run
	else{
		// sets all garage cells to 5
		initGarage(garage, NUM_OF_SIZES, FULL_VALUE);
	}

	// display the menus, get input from the user and change the garage
	// status accordingly.
	optionsMenuHandler(garage, NUM_OF_SIZES, FULL_GARAGE, EMPTY_GARAGE,
					   EMPTY_ROW, FULL_ROW,LOWEST_LEVEL, HIGHEST_LEVEL,
					   FULL_VALUE, EMPTY_VALUE);

	// save changes to the binary file overwriting the data
	saveToFile(outputFile, OUPUT_FILE_NAME, garage, NUM_OF_SIZES);

	pause("Press 'ENTER' to exit the program");

	return 0;
}

//**********************************************************************
// FUNCTION: displayDescription
// DESCRIPTION: This function will present to the user a short description.
// of what the program will do.
//**********************************************************************
void displayDescription(){
	cout << "This program will display the status of a parking garage" << endl;
	cout << "You will be presented with a menu of choices." << endl;
	cout << "Depending on the available cars, you can rent," << endl;
	cout << "turn in a car or exit the program." << endl;
	cout << "Then you can choose the type of the car you want to" << endl;
	cout << "turn in or rent and the status of the parking garage" << endl;
	cout << "will change accordingly." << endl;
	cout << "When you decide to exit the program, any changes you" << endl;
	cout << "may have made will be saved." << endl;

	cout << endl;
}

//*********************************************************************
// FUNCTION: fileExists
// DESCRIPTION: Checks if the input file exists and it can be opened.
// Returns true if does or false otherwise.
// INPUT:
// Parameters: inputFile - Reference to the input file.
// OUTPUT:
// Return Val: True if file exists, false otherwise.
//**********************************************************************
bool fileExists(ifstream &inputFile, string inputFileName){
	// issue an error message if input file can't be opened
	// and wait for the user to press enter

	inputFile.open(inputFileName.c_str());

	if (!inputFile)
		return false;

	inputFile.close();

	return true;
}

//*********************************************************************
// FUNCTION: initGarage
// DESCRIPTION: Initializes the 2D garage elements to a specified value.
// OUTPUT:
// Parameters: garage - A 2D array that holds the garage status.
//
//			   NUM_OF_LEVELS - The number of the levels of the garage.
//
// 			   numOfSizes - The number of car sizes.
//
//             desiredValue - The value that the 2D array will be initialized
//							  to.
//**********************************************************************
void initGarage(int garage[][NUM_OF_LEVELS], int numOfSizes, int desiredValue){
	for (int row=0; row < numOfSizes; row++){
		for (int col=0; col < NUM_OF_LEVELS; col++){
			garage[row][col] = desiredValue;
		}
	}
}

//*********************************************************************
// FUNCTION: optionsMenuHandler
// DESCRIPTION: Calls other functions depending on the user's choices.
// INPUT:
// Parameters: garageIsFull - The number that represents a full garage.
//
//			   garageIsEmpty - The number that represents an empty garage.
//
//			   emptyRow - the minimum amount of cars of each type
//
//			   fullRow - the maximum amount of cars of each type
//
//			   lowestLevel - the lowest level of the garage
//
//			   highestLevel - the highest level of the garage
//
//			   fullValue - the maximum number of cars that can be stored
//			   in a column
//
//			   emptyValue - the minimum number of cars that can be stored
//			   in a column
// OUTPUT:
// Parameters: garage - A 2D array that holds the garage status.
//
//			   NUM_OF_LEVELS - The number of the levels of the garage.
//
// 			   numOfSizes - The number of car sizes.
// CALLS TO: displayGarageStatus, displayActionMenu, displaySubMenu,
//			 turnInCar, rentCar
//**********************************************************************
void optionsMenuHandler(int garage[][NUM_OF_LEVELS], int numOfSizes,
						int garageIsFull, int garageIsEmpty, int emptyRow,
						int fullRow, int lowestLevel, int highestLevel,
						int fullValue, int emptyValue)
{
	bool whileStatus = true;  // used to exit the while loop gracefully
	int garageStatus;		  // the total number of cars in the garage
	char actionMenuChoice;    // holds the action menu input
	char subMenuChoice;   	  // holds the sub menu input

	while (whileStatus){
		// displays the garage status and stores the total number of cars
		// in the garage.
		garageStatus = displayGarageStatus(garage, numOfSizes);
		// displays the action menu and stores the users's choice.
		actionMenuChoice = displayActionMenu(garageStatus, garageIsFull,
											 garageIsEmpty);
		// if the user chooses to rent or turn in a car.
		if (actionMenuChoice == 'T' || actionMenuChoice == 'R'){
			// displays the sub menu and stores the users's choice.
			subMenuChoice = displaySubMenu();

			// if user doesn't choose to return to the action menu
			if (subMenuChoice != 'R'){
				cout << endl;

				// user chose to turn in a car.
				if (actionMenuChoice == 'T'){
					turnInCar(garage, subMenuChoice, fullRow, lowestLevel,
										  highestLevel, fullValue);
				}
				// user chose to rent a car.
				else{
					rentCar(garage, subMenuChoice, emptyRow, lowestLevel,
							highestLevel, fullValue, emptyValue);
				}
			}


		}
		else{
			// exit the program
			whileStatus = false;
		}

	} // end while
}

//*********************************************************************
// FUNCTION: displayGarageStatus
// DESCRIPTION: Displays the status of the garage and returns the
// total number of cars stored in it.
// INPUT:
// Parameters: garage - A 2D array that holds the garage status.
//
//			   NUM_OF_LEVELS - The number of the levels of the garage.
//
// 			   numOfSizes - The number of car sizes.
//
// Return Val: The total amount of all car types on all levels.
//**********************************************************************
int displayGarageStatus(int garage[][NUM_OF_LEVELS], int numOfSizes){
	// this array holds the string representation of all the car types.
	const string CAR_TYPE[] = {"Compact", "Mid-size", "Full size", "SUV"};
	int typeTotal = 0;  // the amount of cars of each type.
	int grandTotal = 0; // the total amount of all car types.

	cout << "Level:" << setw(13) << '0' << setw(5) << '1' << setw(5)
		 << '2' << setw(5) << '3' << setw(5) << '4' << setw(5) << '5'
		 << setw(10) << "Total" << endl;
	cout << endl;

	for (int row=0; row < numOfSizes; row++){
		cout << left << setw(18) << CAR_TYPE[row];

		for (int col=0; col < NUM_OF_LEVELS; col++){
			// add available cars of each column to the row total.
			typeTotal += garage[row][col];
			// print available cars of each column
			cout << setw(5) << garage[row][col];
		}
		// add vehicles stored on each column to the grand total.
		grandTotal += typeTotal;
		// print the total of each row
		cout << right << setw(6) << typeTotal << endl;
		// reset the total of the current row
		typeTotal = 0;
	}
	// prints the grand total
	cout << setw(54) << "-----" << endl;
	cout << "Total Cars Available for Rental" << right
		 << setw(23) << grandTotal;
	cout << endl;

	return grandTotal;
}

//*********************************************************************
// FUNCTION: displayActionMenu
// DESCRIPTION: Displays the action menu, validates and returns
// the user's choice.
// INPUT:
// Parameters: totalCars - The total amount of all car types on all levels.
//
//			   garageIsFull - The number that represents a full garage.
//
//			   garageIsEmpty - The number that represents an empty garage.
//
// Return Val: The user's choice.
// CALLS TO: toUppercase
//**********************************************************************
char displayActionMenu(int totalCars, int garageIsFull, int garageIsEmpty){
	bool valid = false;
	string choice;

	while (!valid){
		cout << endl;
		// display menu
		if (totalCars == garageIsEmpty){
			cout << "T-Turn in a car" << endl;
			cout << "E-Exit the program" << endl;
		}
		else if (totalCars == garageIsFull){
			cout << "R-Rent a car" << endl;
			cout << "E-Exit the program" << endl;
		}
		else{
			cout << "R-Rent a car" << endl;
			cout << "T-Turn in a car" << endl;
			cout << "E-Exit the program" << endl;
		}

		cout << endl;

		cout << "Enter your choice : ";
		cin >> choice;
		toUppercase(choice);

		if (choice == "R"){
			if (totalCars == garageIsEmpty){
				cout << "There are no available cars to rent." << endl;
			}
			else{
				valid = true;
			}
		}
		else if (choice == "T"){
			if (totalCars == garageIsFull){
				cout << "There is no space for your car." << endl;
			}
			else{
				valid = true;
			}
		}
		else if (choice == "E"){
			valid = true;
		}
	} // end while

	// returns first char of the choice string
	return choice[0];
}

//*********************************************************************
// FUNCTION: displaySubMenu
// DESCRIPTION: Displays the sub menu, validates and returns
// the user's choice.
//
// Return Val: The user's choice.
// CALLS TO: toUppercase
//**********************************************************************
char displaySubMenu(){
	string choice;

	while (true){
		cout << endl;
		cout << "C-Compact" << endl;
		cout << "M-Mid-size" << endl;
		cout << "F-Full size" << endl;
		cout << "S-SUV" << endl;
		cout << "R-Return to main menu" << endl;
		cout << endl;

		cout << "Enter your choice : ";
		cin >> choice;
		toUppercase(choice);

		if (choice == "C")
			return 'C';
		else if (choice == "M")
			return 'M';
		else if (choice == "F")
			return 'F';
		else if (choice == "S")
			return 'S';
		else if (choice == "R")
			return 'R';

	} // end while
}

//*********************************************************************
// FUNCTION: turnInCar
// DESCRIPTION: Allows the user to turn in a car, changing the garage status.
// INPUT:
// Parameters: vehicleType - The type of vehicle as a char.
//
//			   fullRow - the maximum amount of cars of each type
//
//			   lowestLevel - the lowest level of the garage
//
//			   highestLevel - the highest level of the garage
//
//			   fullValue - the maximum number of cars that can be stored
//			   in a column
// OUTPUT:
// Parameters: garage - A 2D array that holds the garage status.
//
//			   NUM_OF_LEVELS - The number of the levels of the garage.
//
// CALLS TO: charToStrRepr, charToEnum, displayLevelsDiagram, pause
//**********************************************************************
void turnInCar(int garage[][NUM_OF_LEVELS], char vehicleType, int fullRow,
			   int lowestLevel, int highestLevel, int fullValue)
{
	bool valid = false; // used for while loop control
	string strType;	  	// type of the car as a string
	carSize enumType; 	// type of the car as an enum
	int total = 0;    	// total number of all cars of the provided type
	int levelChoice;  	// used to hold the user's choice

	// store string representation of the car type
	strType = charToStrRepr(vehicleType);

	// store enum representation of the car type
	enumType = charToEnum(vehicleType);

	// display diagram of cars on each level
	displayLevelsDiagram(strType, enumType, garage, total);

	// if all the rows are full for the chosen car size
	if (total == fullRow){
		cout << "This car is returned to the wrong company." << endl;
	}
	else{
		while (!valid){
			cout << "Your car can be parked on any level with less than"
					" 5 cars" << endl;
			cout << "Which level will the car be parked on? ";
			cin >> levelChoice;

			cout << endl;

			// if level exists (0-5)
			if ( levelChoice >= lowestLevel && levelChoice <= highestLevel ){
				if (garage[enumType][levelChoice] == fullValue){
					cout << "Error - Level " << levelChoice << " already"
							" contains 5 cars." << endl;
					cout << "Pick a level with less than 5 cars already parked."
						 << endl;
					cout << endl;
				}
				// valid level
				else{
					// increment level by one
					garage[enumType][levelChoice]++;

					// increments the total number of cars by one
					// and issues a message okaying the action.
					cout << "Okay to park car in " << strType
						 << " row of level " << levelChoice << endl;
					cout << ++total << " " << strType << " cars will now be"
							" available" << endl;

					valid = true;
				}
			}
			else{
				cout << "Level " << levelChoice << " doesn't exist." << endl;
			}

		} // end while
	}

	cout << endl;
	pause();
}

//*********************************************************************
// FUNCTION: rentCar
// DESCRIPTION: Allows the user to turn in a car, changing the garage status.
// INPUT:
// Parameters: vehicleType - The type of vehicle as a char.
//
//			   emptyRow - the minimum amount of cars of each type
//
//			   lowestLevel - the lowest level of the garage
//
//			   highestLevel - the highest level of the garage
//
//			   fullValue - the maximum number of cars that can be stored
//			   in a column
//
//			   emptyValue - the minimum number of cars that can be stored
//			   in a column
// OUTPUT:
// Parameters: garage - A 2D array that holds the garage status.
//
//			   NUM_OF_LEVELS - The number of the levels of the garage.
//
// CALLS TO: charToStrRepr, charToEnum, displayLevelsDiagram, pause
//**********************************************************************
void rentCar(int garage[][NUM_OF_LEVELS], char vehicleType, int emptyRow,
			   int lowestLevel, int highestLevel, int fullValue, int emptyValue)
{
	bool valid = false; // used to exit the while loop properly
	bool found = false; // used to control the for loop
	string strType;	    // type of the car as a string
	carSize enumType;   // type of the car as an enum
	int total = 0;      // total number of all cars of the provided type

	// store string representation of the car type
	strType = charToStrRepr(vehicleType);

	// store enum representation of the car type
	enumType = charToEnum(vehicleType);

	// display diagram of cars on each level
	displayLevelsDiagram(strType, enumType, garage, total);

	while (!valid){
		//no available cars
		if (total == emptyRow){
			cout << "There are no " << strType << " cars available." << endl;
		}
		// there are available cars
		else{
			for (int level=highestLevel; level >= lowestLevel; level--){
				if (!found){
					if (garage[enumType][level] > emptyValue){
						// decrease the number of cars in the level by one
						--garage[enumType][level];

						cout << "Pick up car from " << strType
							 << " row of level " << level << endl;
						cout << --total << " " << strType
							 << " cars will now be available" << endl;

						valid = true;
						found = true;
					}
				}
			} // end for loop
		}

		cout << endl;
		pause();

	} // end while
}

//*********************************************************************
// FUNCTION: displayLevelsDiagram
// DESCRIPTION: Displays a diagram of all the floors of the parking
// garage for the specified car type.
// INPUT:
// Parameters: garage - A 2D array that holds the garage status.
//
//			   NUM_OF_LEVELS - The number of the levels of the garage.
//
// 			   type - The type of vehicle as a string.
//
//			   size - The type of vehicle as a carSize enum.
//
//			   total - A reference to the total number of cars stored in
// 			   the garage.
//**********************************************************************
void displayLevelsDiagram(string type, carSize size,
						  int garage[][NUM_OF_LEVELS], int &total)
{
	cout << "Number of " << type << " Cars Parked on each Level" << endl;
	cout << endl;

	cout << "Level:" << setw(13) << '0' << setw(5) << '1' << setw(5)
			 << '2' << setw(5) << '3' << setw(5) << '4' << setw(5) << '5'
			 << setw(10) << "Total" << endl;
	cout << endl;
	cout << left << setw(18) << type;

	// add the cars of each level to the total
	for (int level=0; level < NUM_OF_LEVELS; level++){
		cout << setw(5)<< garage[size][level];
		total += garage[size][level];
	}

	cout << right << setw(6) << total;
	cout << endl << endl;
}

//*********************************************************************
// FUNCTION: readFromFile
// DESCRIPTION: Reads data from a binary file and stores them into the 2D
// garage array.
// INPUT:
// Parameters: inputFile - A reference to the input binary file.
//
//			   inputFileName - The name of the input file.
//
// File: The file holds values for the row,level and number of cars stored in
// a level/row combination.
// OUTPUT:
// Parameters: garage - A 2D array that holds the garage status.
//
//			   NUM_OF_LEVELS - The number of the levels of the garage.
//**********************************************************************
void readFromFile(ifstream &inputFile, string inputFileName,
				  int garage[][NUM_OF_LEVELS])
{
	int rowIndex;
	int levelIndex;
	int numberOfCars;

	// open file in binary mode
	inputFile.open(inputFileName.c_str(), ios::binary);

	// while there are data
	while (inputFile){
		// read values from the binary file
		inputFile.read(reinterpret_cast<char *> (&rowIndex),
										  sizeof(rowIndex));
		inputFile.read(reinterpret_cast<char *> (&levelIndex),
										  sizeof(levelIndex));
		inputFile.read(reinterpret_cast<char *> (&numberOfCars),
										  sizeof(numberOfCars));
		// store values to the garage array
		garage[rowIndex][levelIndex] = numberOfCars;
	}

	cout << "All the data have been read." << endl << endl;

	// close file
	inputFile.close();
}

//*********************************************************************
// FUNCTION: saveToFile
// DESCRIPTION: Writes the sorted data to an output file.
// INPUT:
// Parameters: outputFile - A reference to the output file.
//
//			   outputFileName - The name of the output file.
//
//			   garage - A 2D array that holds the garage status.
//
//			   NUM_OF_LEVELS - The number of the levels of the garage.
//
// 			   numOfSizes - The number of car sizes.
//
// OUTPUT:
// File: The file holds values for the row,level and number of cars stored in
// a level/row combination.
// Data is stored in binary format.
//**********************************************************************
void saveToFile(ofstream &outputFile, string outputFileName,
				int garage[][NUM_OF_LEVELS], int numOfSizes)
{
	int rowIndex;
	int levelIndex;
	int numberOfCars;

	// open file in binary mode
	outputFile.open(outputFileName.c_str(), ios::binary);

	for (int row=0; row < numOfSizes; row++){
		for (int col=0; col < NUM_OF_LEVELS; col++){
			// if the rows are not empty
			if (garage[row][col] != 0){
				// get the row, level and number of cars values
				rowIndex = row;
				levelIndex = col;
				numberOfCars = garage[row][col];

				// write these values to the binary file
				outputFile.write(reinterpret_cast<const char *> (&rowIndex),
						         sizeof(rowIndex));
				outputFile.write(reinterpret_cast<const char *> (&levelIndex),
										         sizeof(levelIndex));
				outputFile.write(reinterpret_cast<const char *> (&numberOfCars),
										         sizeof(numberOfCars));
			}
		}
	}

	cout << "All the data have been saved." << endl << endl;

	// close file
	outputFile.close();
}

// Helper Functions

// charToEnum - Converts a char to enum and returns it.
//
// charToStrRepr - Converts a char to a string and returns it.

//*********************************************************************
// FUNCTION: charToEnum
// DESCRIPTION: Converts the size parameter to an enum and returns it.
// INPUT:
// Parameters: size - A char variable that holds the size of the car.
// Return Val: carSize enumeration type (COMPACT, MIDSIZE, FULLSIZE, SUV)
//**********************************************************************
carSize charToEnum(char size){
	if (size == 'C')
		return COMPACT;
	else if (size == 'M')
		return MIDSIZE;
	else if (size == 'F')
		return FULLSIZE;
	else if (size == 'S')
		return SUV;
}

//*********************************************************************
// FUNCTION: charToEnum
// DESCRIPTION: Converts the size parameter to a string and returns it.
// INPUT:
// Parameters: size - A char variable that holds the size of the car.
// Return Val: The appropriate String (Compact, Mid-size, Full size, SUV)
//**********************************************************************
string charToStrRepr(char size){
	if (size == 'C')
		return "Compact";
	else if (size == 'M')
		return "Mid-size";
	else if (size == 'F')
		return "Full size";
	else if (size == 'S')
		return "SUV";
}


//*********************************************************************
// FUNCTION: toUppercase
// DESCRIPTION: Converts a string to all uppercase.
// INPUT:
// Parameters: str - A reference to str that will be converted.
//**********************************************************************
void toUppercase(string &str){
	int len = str.length();

	for (int i=0; i < len; i++){
		// convert each character to upper case
		str[i] = toupper(str[i]);
	}
}

//*********************************************************************
// FUNCTION: pause
// DESCRIPTION: Pauses the program, prints a message and waits
//				for input from the user.
//
// INPUT:
// Parameters: msg - A string variable to pass a message to the user.
// 			   It has a default value specified in the function declaration.
//**********************************************************************
void pause(string msg){
	cout << msg << endl;
	cin.ignore();
	cin.get();
}
