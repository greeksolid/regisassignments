//*************************************************************************************************************
// CODE FILENAME: GeorgoulisAssn3.cpp
// DESCRIPTION:
// This program will read data(a license plate and a registration charge)
// from an input file. It will store these data into two parallel arrays. One
// will hold the license plates and the other the registration charges.
// It then will sort these two arrays in descending order and write the sorted
// data to an output file. It will then display all the license plates to the
// screen, ask the user to enter a license plate, search the database for that
// license plate and display the registration charge for that vehicle if it's
// plate is stored in the license plate array.
//
// CLASS/TERM: CS362 - 13F8W2
// DESIGNER: Anastasios Georgoulis
// FUNCTIONS:
//          displayDescription: Displays a short description to the user.
//
//			readStoreData: Reads data from an input file and stores them
// 			into two parallel arrays.
//
//			writeSortedData: Writes the sorted data to an output file.
//
//			licensePlateHandler: Handles the prompt for input, validation,
// 			searching and displaying of the sorted data.
//
//			displayLicensePlates: Loops though the license plates array
// 		    and displays the license plates to the user.
//
//			validLicense: Checks if the user's input is formatted correctly.
//
//			displayErrorMessage: Displays an appropriate error message to the
// 			user.
//
//			searchLicensePlate: Filters through the license plates array
// 			to find the provided license plate.
//
//			sortParArrays: Sorts the two parallel arrays.
//
//			binarySearch: Searches the provided string array using binary sort.
//
//			setFormatting: Sets the formatting of the console output.
//
//			pause: Pauses the program and waits for input from the user.
//
//			toUppercase: Converts a string to all uppercase.
//
//		    fileExists: Checks if the input file exists and it can be opened.
//*************************************************************************************************************

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

//*********************************
//******Functions Prototypes*******
//*********************************

void displayDescription();
void readStoreData(ifstream &inputFile, string inputFileName,
				   string licensePlates[],double registrationCharges[],
				   int &parArraysCounter,int arrayLength);
void writeSortedData(ofstream &outFile, string outFileName,
		             string licensePlates[], double registrationCharges[],
		             int parArraysCounter);
void licensePlateHandler(const string licensePlates[],
		                 const double registrationCharges[],
		                 int parArraysCounter);
void displayLicensePlates(const string licensePlates[], int parArraysCounter);
bool validLicense(bool &wrongFormat, bool &tooLong, bool &tooShort,
		               bool &generalError, string userInput,
		               int userInputLength);
void displayErrorMessage(bool &generalError, bool &wrongFormat,
						 bool &tooLong, bool &tooShort, string userInput);
void searchLicensePlate(const string licensePlates[],
						const double registrationCharges[], int listLength,
						string userInput);

void sortParArrays(string licensePlates[], double registrationCharges[],
				   int listLength);
int binarySearch(const string list[], int listLength,
		         string searchItem);


// Helper Functions

void setFormatting(ostream &cout);
void pause(const string msg="Press 'ENTER' to continue.");
void toUppercase(string &str);
bool fileExists(ifstream &inputFile, string inputFileName);

//*********************************************************************
// FUNCTION: main
// DESCRIPTION: Initializes the variables and calls some helper functions.
// It displays a short description to the user and opens an input file
// If it can't it terminates the program with an error.
// If the input file exists and can be opened main calls readStoreData() to
// read the data from the input file and store them into two parallel arrays.
// It then calls sortParArrays() to sort these arrays in descending order
// and calls writeSortedData to write the sorted results to an output file.
// Then calls the licensePlateHandler() function that displays all the license
// plates prompts the user to enter a license plate, validates it and searches
// the license plate array to find it. These process continues until the user
// enters 'Q' and the program is terminated.
// OUTPUT:
// Return Val: 0 -> If the program terminated without any problems.
//			   1 -> If it there were problems.
// CALLS TO: displayDescription, fileExists, readStoreData, sortParArrays,
//           writeSortedData, licensePlateHandler
//**********************************************************************
int main(){
	// VARIABLES
	ifstream inputFile;
	ofstream outFile;

	bool file; // used to check if file exists

	// CONSTANT VARIABLES

	// Size of the parallel arrays
	const int ARRAY_LENGTH = 1000;

	const string INPUT_FILE_NAME = "GeorgoulisVEHICLES.txt";
	const string OUTPUT_FILE_NAME = "SORTED.txt";

	// Two parallel arrays to hold information about license plates
	// and registration charges.
	string licensePlates[ARRAY_LENGTH];
	double registrationCharges[ARRAY_LENGTH];
	// Counter for these two arrays
	int parArraysCounter = 0;

	// Display a short description of what the program does to the user
	displayDescription();

	// open the input text file
	inputFile.open(INPUT_FILE_NAME.c_str());

	// check if file exists
	file = fileExists(inputFile, INPUT_FILE_NAME);

	// if not, exit main with error code
	if ( !file ){
		return 1;
	}

	// Read the license plates from the input file
	// and store them in the two parallel arrays
	readStoreData(inputFile, INPUT_FILE_NAME, licensePlates,
			      registrationCharges,parArraysCounter, ARRAY_LENGTH);

	// Sort parallel arrays in descending order based on the license plate
	sortParArrays(licensePlates, registrationCharges, parArraysCounter);

	// Store sorted data to output file
	writeSortedData(outFile, OUTPUT_FILE_NAME, licensePlates,
					registrationCharges, parArraysCounter);

	// Call a handler function that calls various other functions to
	// prompt the user for an input, validates it, searches the parallel
	// arrays and displays the results to the user.
	licensePlateHandler(licensePlates, registrationCharges, parArraysCounter);

	return 0;
}

//*********************************
//****Functions Implementations****
//*********************************

//**********************************************************************
// FUNCTION: displayDescription
// DESCRIPTION: This function will present to the user a short description.
// of what the program will do.
//**********************************************************************
void displayDescription(){
	cout << "This program will read data from an input file." << endl;
	cout << "It will then store these data and sort them in descending order."
		 << endl;
	cout << "It will write the results to an output file, print the license"
			" plates to the screen" << endl;
	cout << "and ask for a license plate (3 letters followed by 3 numbers)"
		 << endl;
	cout << "Then it will check search the database for that license plate"
			"number and"
		 << endl;
	cout << "if found it will display it to the screen along with "
			"the registration charge for that vehicle"
		 << endl;
	cout << "until you quit by giving 'Q' or 'q' as input" << endl;
	cout << endl;
}

//*********************************************************************
// FUNCTION: readStoreData
// DESCRIPTION: Reads the data from the input file and stores it into two
// parallel arrays.
// INPUT:
// Parameters: inputFile - A reference to the output file.
//
// 			   inputFileName - The name of the output file.
//
//			   licensePlates - A string array that holds the license plates.
//
// 			   registrationCharges - A double array that holds the registration
//             charges.
//
//             parArraysCounter - A reference to the counter of the two
//             parallel arrays.
//
//			   arrayLength - the size of the parallel arrays
//
// File: Reads following info from each line of VEHICLES.txt:
// vehicle type, license plate, registration tax
// Data on each line is formatted as follows:
// 	A    ABC123   1674.00
//**********************************************************************
void readStoreData(ifstream &inputFile, string inputFileName,
		      	   string licensePlates[], double registrationCharges[],
		      	   int &parArraysCounter, int arrayLength)
{
	char vehicleType;
	string licensePlate;
	double registrationCharge;

	//priming read - read first line
	inputFile >> vehicleType >> licensePlate >> registrationCharge;

	while (inputFile){
		// array bounds checking
		if (parArraysCounter > arrayLength){
			cout << "Too many lines of data. No more license plates will "
					"be read." << endl;
			// exit loop
			break;
		}
		else{
			// store data to parallel arrays
			licensePlates[parArraysCounter] = licensePlate;
			registrationCharges[parArraysCounter] = registrationCharge;
		}

		// increment counter
		parArraysCounter++;

		// store data to variables
		inputFile >> vehicleType >> licensePlate >> registrationCharge;
	}

	// close the input file
	inputFile.close();
}

//*********************************************************************
// FUNCTION: writeSortedData
// DESCRIPTION: Writes the sorted data to an output file.
// INPUT:
// Parameters: outFile - A reference to the output file.
//
// 			   outFileName - The name of the output file.
//
//			   licensePlates - A string array that holds the license plates.
//
// 			   registrationCharges - A double array that holds the registration
// charges.
//
//             parArraysCounter - A counter for the two parallel arrays.
//
// OUTPUT:
// File: Stores license plates and registration charges for vehicles.
// Data on each line is formatted as follows:
// XYZ789   2349.00
//
// CALLS TO: setFormatting
//**********************************************************************
void writeSortedData(ofstream &outFile, string outFileName,
		             string licensePlates[], double registrationCharges[],
		             int parArraysCounter)
{
	string licensePlate;
	double registrationCharge;
	int counter = 0;

	// opens output file
	outFile.open(outFileName.c_str());

	// set formatting for the output file
	setFormatting(outFile);

	// while there are more data in the arrays
	while (counter < parArraysCounter){
		// get each license plate
		licensePlate = licensePlates[counter];
		// get each registration charge
		registrationCharge = registrationCharges[counter];

		// write to output file
		outFile << licensePlate << setw(10) << registrationCharge << endl;

		// increment counter
		counter++;
	}

	// close output file
	outFile.close();
}

//*********************************************************************
// FUNCTION: licensePlateHandler
// DESCRIPTION: Keeps track of various variables and calls other functions to
// prompt the user for an input, validates it, searches the parallel
// arrays and displays the results to the user.
// INPUT:
// Parameters: licensePlates - A string array that holds the license plates.
//
// 			   registrationCharges - A double array that holds the registration
// charges.
//
//             parArraysCounter - A counter for the two parallel arrays.
//
// CALLS TO: setFormatting, displayLicensePlates, toUppercase,
// validLicense, displayErrorMessage, searchLicensePlate
//**********************************************************************
void licensePlateHandler(const string licensePlates[],
		                 const double registrationCharges[],
		                 int parArraysCounter)
{
	string userInput;   // the user's input
	int userInputLength;// the user's input length

	bool loop = true;   // used to control the while loop

	// error flags
	bool wrongFormat;	// input string is not properly formatted
	bool tooLong;		// input string is too long
	bool tooShort;		// input string is too short
	bool generalError;  // any of the previous error flags was set to true
	bool error;			// used to hold the return value of validLicense().

	// set the formatting of the console output
	setFormatting(cout);

	while (loop){
		// Error flags
		wrongFormat = false;
		tooLong = false;
		tooShort = false;
		generalError = false;

		// Display all license plates of the output file
		displayLicensePlates(licensePlates, parArraysCounter);

		// Prompt user for input
		cout << "Enter a license plate or 'Q' to quit : ";
		// Store user's input
		cin >> userInput;
		cout << endl;

		// Convert to the user's input to uppercase
		toUppercase(userInput);
		// Get user's input length
		userInputLength = userInput.length();

		if (userInput == "Q" || userInput == "q"){
			loop = false; // exit the loop
		}
		else{
			// check if user's input is valid
			error = validLicense(wrongFormat, tooLong, tooShort, generalError,
					          userInput,userInputLength);

			if (error){
				// display appropriate message
				displayErrorMessage(generalError, wrongFormat, tooLong,
									tooShort, userInput);
			}
			else{
				// search through license plate and display it's
				// registration charge to the user
				searchLicensePlate(licensePlates, registrationCharges,
								   parArraysCounter, userInput);
			}

		}
	} // end while loop
}

//*********************************************************************
// FUNCTION: displayLicensePlates
// DESCRIPTION: Displays all the license plates horizontally.
// INPUT:
// Parameters: licensePlates - Reference to the array that holds the
// license plates.
//             parArraysCounter - The length of the license plates array.
//**********************************************************************
void displayLicensePlates(const string licensePlates[], int parArraysCounter){
	cout << "LICENSE PLATES on File are: " << endl;

	for (int i=0; i < parArraysCounter; i++){
		// new line after every 6 license plates
		if (i % 6 == 0 && i != 0){
			cout << endl;
		}

		// print license plate
		cout << left << setw(10) << licensePlates[i];
	}
	cout << endl << endl;
}

//*********************************************************************
// FUNCTION: validLicense
// DESCRIPTION: Checks if the provided license is valid.
// INPUT:
// Parameters: generalError - A reference to a boolean variable that keeps
// track for a generic error of the input string given by the user.
//
//			   wrongFormat - A reference to a boolean variable that keeps
// track for a wrong format error of the input string given by the user.
//
//			   tooLong - A reference to a boolean variable that keeps
// track if the input string is too long or not.
//
//			   tooLong - A reference to a boolean variable that keeps
// track if the input string is too short or not.
//
// 			   userInput - The input string provided by the user.
//
//   		   userInputLength - An int variable that holds the length of
// the input string.
// OUTPUT:
// Return Val: True if the license plate is valid, false otherwise.
//**********************************************************************
bool validLicense(bool &wrongFormat, bool &tooLong, bool &tooShort,
		               bool &generalError, string userInput,
		               int userInputLength)
{
	const int PROPER_LICENSE_LENGTH = 6;

	if (userInputLength == PROPER_LICENSE_LENGTH){
		// get the first three letters of the user's input string
		string firstThreeChars = userInput.substr(0,3);
		// get the last three letters of the user's input string
		string laterThreeChars = userInput.substr(3,6);

		const int SUBSTR_LENGTH = 3;

		// check if the first three characters are alphanumeric
		for (int i=0; i < SUBSTR_LENGTH; i++){
			if ( !isalpha(firstThreeChars[i]) ){
				// set wrong format flag if not
				wrongFormat = true;
			}
		}

		// check if the first three characters are alphanumeric
		for (int i=0; i < SUBSTR_LENGTH; i++){
			if ( !isdigit(laterThreeChars[i]) ){
				// set wrong format flag if not
				wrongFormat = true;
			}
		}

		if (wrongFormat)
			generalError = true;
	}
	else{
		generalError = true;

		// if the user's input is too long
		if (userInputLength > PROPER_LICENSE_LENGTH){
			tooLong = true;
		}
		// if the user's input is too short
		else if (userInputLength < PROPER_LICENSE_LENGTH){
			tooShort = true;
		}
	}

	if (!generalError)
		return false;
	else
		return true;
}

//*********************************************************************
// FUNCTION: displayErrorMessage
// DESCRIPTION: Displays a proper error message.
// INPUT:
// Parameters: generalError - A reference to a boolean variable that keeps
// track for a generic error of the input string given by the user.
//
//			   wrongFormat - A reference to a boolean variable that keeps
// track for a wrong format error of the input string given by the user.
//
//			   tooLong - A reference to a boolean variable that keeps
// track if the input string is too long or not.
//
//			   tooLong - A reference to a boolean variable that keeps
// track if the input string is too short or not.
//
// 			   userInput - The input string provided by the user.
//
// CALLS TO: binarySearch
//**********************************************************************
void displayErrorMessage(bool &generalError, bool &wrongFormat,
						 bool &tooLong, bool &tooShort, string userInput)
{
	// display appropriate message
	if (generalError){

		if (tooLong){
			cout << "License " << userInput << " is too long." << endl;
		}
		else if (tooShort){
			cout << "License " << userInput << " is too short." << endl;
		}
		else if (wrongFormat){
			cout << "License " << userInput << " is formatted incorrectly."
				 << endl;
		}
		cout << "Must be 3 letters followed by 3 digits."
				"Try again." << endl;
		cout << endl;

	}
}

//*********************************************************************
// FUNCTION: searchLicensePlate
// DESCRIPTION: Searches the license plate and displays the registration
// charge for that vehicle or a message informing the user that the license
// plate he searched for is not in the database.
// INPUT:
// Parameters: licensePlates - A string array that holds the license plates.
//
// 			   registrationCharges - A double array that holds the registration
//
//             listLength - An integer array that holds the length of the
// parallel arrays.
//
//			   userInput - The input that the user has given.
//
// CALLS TO: binarySearch
//**********************************************************************
void searchLicensePlate(const string licensePlates[],
						const double registrationCharges[], int listLength,
						string userInput)
{
	// find and store the index of the desired license plate
	int binarySearchResult = binarySearch(licensePlates, listLength, userInput);

	// display appropriate message to the user
	if (binarySearchResult == -1){
		cout << "Vehicle " << userInput << " not in database" << endl;
	}
	else{
		cout << "Vehicle with license " << userInput
			 << " was charged $ "
			 << registrationCharges[binarySearchResult] << endl;
	}

	cout << endl;
}

//*********************************************************************
// FUNCTION: sortParArrays
// DESCRIPTION: Sorts both parallel arrays in descending order based on
// the license plates.
//
// INPUT:
// Parameters: licensePlates - A string array that holds the license plates.
//
// 			   registrationCharges - A double array that holds the registration
// charges.
//
//             listLength - An integer array that holds the length of the
// parallel arrays.
//*********************************************************************
void sortParArrays(string licensePlates[], double registrationCharges[],
				   int listLength)
{
	int biggestIndex; 	// current biggest index
	int location;     	// index for the inner loop

	// Temporary values used for swapping
	string temp;		// temporary value for the license plate.
	double temp2;		// temporary value for the registration charge

	for (int index=0; index < listLength - 1; index++){
		biggestIndex = index;

		for (location = index + 1; location < listLength; location++){
			if ( licensePlates[location] > licensePlates[biggestIndex] ){
				biggestIndex = location;
			}
		}

		// Swap elements of the license plates array
		temp = licensePlates[biggestIndex];
		licensePlates[biggestIndex] = licensePlates[index];
		licensePlates[index] = temp;

		// Swap elements of the registration charges array
		temp2 = registrationCharges[biggestIndex];
		registrationCharges[biggestIndex] = registrationCharges[index];
		registrationCharges[index] = temp2;
	}
}

//*********************************************************************
// FUNCTION: binarySearch
// DESCRIPTION: Searches the provided sorted(in descending order) list
// for a target value(string) and returns its position if found.
// Otherwise it returns -1
//
// INPUT:
// Parameters: msg - A string variable to pass a message to the user.
// 			   It has a default value specified in the function declaration.
//*********************************************************************
int binarySearch(const string list[], int listLength, string searchItem){
	bool found = false;
	int mid;
	int first = 0;
	int last = listLength - 1;

	while ( last >= first && !found ){
		mid = (first + last) / 2;

		if ( list[mid] == searchItem )
			found = true;
		else if ( list[mid] > searchItem )
			first = mid + 1;
		else
			last = mid - 1;
	}

	// Return the index if the item was found
	// or -1 if it wasn't.
	if (found)
		return mid;

	return -1;
}

//*********************************************************************
// FUNCTION: setFormatting
// DESCRIPTION: Sets desired formatting for the console output.
// i.e. floating-point values are written using fixed-point notation,
// always showing the decimal point and with a decimal precision of 2.
// INPUT:
// Parameters: cout - Reference to the console output
//**********************************************************************
void setFormatting(ostream &cout){
	cout << fixed << showpoint << setprecision(2);
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
void pause(const string msg){
	cout << msg << endl;
	cin.get();
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
		// convert each character to uppercase
		str[i] = toupper(str[i]);
	}
}

//*********************************************************************
// FUNCTION: fileExists
// DESCRIPTION: Checks if the input file exists and it can be opened.
// Returns true if does or false otherwise.
// INPUT:
// Parameters: inputFile - Reference to the input file.
//             inputFileName - The name of the input file.
// OUTPUT:
// Return Val: True if file exists, false otherwise.
// CALLS TO: pause
//**********************************************************************
bool fileExists(ifstream &inputFile, string inputFileName){
	// issue an error message if input file can't be opened
		// and wait for the user to press enter
	if (!inputFile){
		cout << "Could not open the input file " << inputFileName.c_str()
			 << endl;
		pause("Press 'ENTER' to terminate the program.");

		return false;
	}

	return true;
}

