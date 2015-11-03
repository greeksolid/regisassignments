//******************************************************************************
// CODE FILENAME: GeorgoulisAssn4.cpp
// DESCRIPTION: This program will prompt the user to get real estate listings
// from an input file. Then depending on his choice will read and store these
// data to an array of records or continue with an empty array.
// It will then present a menu of choices. The user can :
// 	1. Display all listings.
// 	2. Add a new listing.
// 	3. Remove an existing listing.
// 	4. Exit the program.
// Before exiting the program, the user will be prompted to save any changes
// he made to an output file which is the same as the input file, overwriting
// any existing data.
// CLASS/TERM: CS362 - 13F8W2
// DESIGNER: Anastasios Georgoulis
// FUNCTIONS:
//
// displayDescription - Displays a short description to the user.
//
// loadFilePrompt - Prompts the user to choose between loading existing
// data from an input file or not.
//
// storeFromInput - Reads and stores data from an input file to an array
// of records.
//
// exitProgramOption - Prompts the user to exit the program and calls
// storeToOutput to save any changes made.
//
// displayMenu - Displays a menu of choices to the user.
//
// convertEnum - Converts the statusEnum parameter to string and
// prints it to the screen.
//
// ConvertStringEnum - Converts the string variable to statusEnum type and
// returns it.
//
// displayAllHeaders - Displays headers in columns.
//
// displayAllListings - Displays all listings stored in the listings array.
//
// displayAllOption - Calls functions to display the listings to the user.
//
// readMls - Prompts the user for a MLS number, validates it and returns it.
//
// readPrice - Prompts the user for a listing price, validates it and
// returns it.
//
// readZipCode - Prompts the user for a zip code, validates it and returns it.
//
// readStatus - Prompts the user for a status in string format, converts it to
// statusEnum type, validates it and returns it.
//
// readRealty - Prompts the user for a company name, validates it, converts
// it to proper case and returns it.
//
// addListingOption - Calls other functions to get input from the user and
// adds new listings to the listings array.
//
// displayAllMls - Displays all MLS numbers in columns.
//
// findMls - Calls other functions to get input from the user and
// adds new listings to the listings array.
//
// deleteItem - Deletes the array element that is passed and reorders the
// listings array.
//
// removeListingOption - Displays all MLS numbers, gets one as input from
// the user and deletes it from the listings array.
//
// storeToOutput - Writes the sorted data to an output file.
//
// loadDataHandler - Opens the input file, if it exists and calls the
// storeFromInput function to read the data from the file.
// If the file doesn't exist it prompts the user to continue without
// listings or to quit the program.
//
// menuHandler - Displays and calls other functions depending on which option
// the user chooses
//
// pause - Pauses the program and waits for input from the user.
//
// toUppercase - Converts a string to all upper case.
//
// properCase - Converts a string to proper case (Like This).
//
// fileExists: Checks if the input file exists and it can be opened.
//
// isNumber - Checks if string consists only of digits.
//
// onlyLetters - Checks if string consists only of letters and whitespace.
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

//*********************************
//*************Global**************
//*********************************

enum statusEnum {AVAILABLE, CONTRACT, SOLD};

struct listingInfo{
	int mls;
	double price;
	statusEnum status;
	string zipCode;
	string realtyCompany;
};

//*********************************
//******Functions Prototypes*******
//*********************************
void displayDescription();
string loadFilePrompt();
void storeFromInput(ifstream &inputFile, listingInfo listings[], int arraySize,
		            int &listingsCounter);
void exitProgramOption(ofstream &outputFile, string outputFileName,
					   listingInfo listings[], int listingsCounter,
					   int arraySize);
string displayMenu();
void convertEnum(statusEnum status);
statusEnum convertStringEnum(string str);
void displayAllHeaders();
void displayAllListings(const listingInfo listings[], int listingsCounter);
void displayAllOption(const listingInfo listings[], int listingsCounter);
int readMls(int properLength, int minMls, int maxMls);
double readPrice();
string readZipCode(int properLength);
statusEnum readStatus();
string readRealty(int properRealtyLength);
void addListingOption(listingInfo listings[], int &listingsCounter,
		              int arraySize,
		              int properMlsLength, int minMls, int maxMls,
		              int properZipCodeLength, int properRealtyLength);
void displayAllMls(const listingInfo listings[], int listingsCounter);
int findMls(const listingInfo listings[], int listingsCounter, int target);
void deleteItem(listingInfo listings[], int &listingsCounter, int location);
void removeListingOption(listingInfo listings[], int &listingsCounter,
						 int properMlsLength, int minMls, int maxMls);
void storeToOutput(ofstream &outputFile, string outputFileName,
				   listingInfo listings[], int listingsCounter,
				   int arraySize);
int loadDataHandler(ifstream &inputFile, string inputFileName,
				    listingInfo listings[], int &listingsCounter,
				    int arraySize);
void menuHandler(listingInfo listings[], int &listingsCounter, int arraySize,
		        int properMlsLength, int minMls, int maxMls,
		        int properZipCodeLength, int properRealtyLength,
		        ofstream &outputFile, string outputFileName);


// Helper Functions

void pause(string msg="Press 'ENTER' to continue.");
void toUppercase(string &str);
void properCase(string &str);
bool fileExists(ifstream &inputFile, string inputFileName);
bool isNumber(string &str);
bool onlyLetters(string &str);

//*********************************************************************
// FUNCTION: main
// DESCRIPTION: Initializes the variables and calls some helper functions.
// It displays a short description to the user and asks the user
// if he wants to load data from an input file.
// If the user chooses to load existing data, the function loadDataHandler
// is called to get the data.
// It then calls the menuHandler function to display a menu with choices.
// Depending on what he chooses, the user can display all listings,
// add a new listing, remove a listing or quit the program.
// If the user chooses to quit he will be asked to save any changes he made.
//
// OUTPUT:
// Return Val: 0 -> If the program terminated without any problems.
//			   1 -> If it there were problems.
// CALLS TO: displayDescription, loadDataHandler, menuHandler
//**********************************************************************
int main(){
	// CONSTANT VARIABLES
	const string INPUT_FILE_NAME = "LISTINGS.txt";
	const string OUTPUT_FILE_NAME = "LISTINGS.txt";
	const int MIN_MLS = 100000;
	const int MAX_MLS = 999999;
	const int PROPER_MLS_LENGTH = 6;
	const int PROPER_ZIPCODE_LENGTH = 10;
	const int PROPER_REALTY_LENGTH = 20;
	const int ARRAY_SIZE = 750;

	// VARIABLES
	ifstream inputFile;
	ofstream outputFile;
	listingInfo listings[ARRAY_SIZE];
	int listingsCounter = 0;

	// display short description
	displayDescription();

    // get data from file
	int loadData = loadDataHandler(inputFile, INPUT_FILE_NAME, listings,
								   listingsCounter, ARRAY_SIZE);

	// if file can't be found return error code
	if (loadData == 1)
		return 1;

	menuHandler(listings, listingsCounter, ARRAY_SIZE, PROPER_MLS_LENGTH,
				MIN_MLS, MAX_MLS, PROPER_ZIPCODE_LENGTH, PROPER_REALTY_LENGTH,
			    outputFile, OUTPUT_FILE_NAME);

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
	cout << "This program will ask if you want" << endl;
	cout << "to read existing data from an input file." << endl;
	cout << "It will then display a menu and ask you to" << endl;
	cout << "choose an option." << endl;
	cout << endl;

	cout << "You can display all the listings," << endl;
	cout << "add a new listings, remove an existing listing" << endl;
	cout << "or exit the program." << endl;
	cout << endl;

	cout << "When you choose to exit" << endl;
	cout << "the program you will be asked whether you want" << endl;
	cout << "to save any changes you may have made." << endl;
	cout << endl;
}

//*********************************************************************
// FUNCTION: loadFilePrompt
// DESCRIPTION: Asks the user to choose between loading data from an input
// file or not and return that choice.
// OUTPUT:
// Return Val: "Y" if the user wants to load data from the file
// 			   "N" otherwise.
// CALLS TO: toUppercase
//**********************************************************************
string loadFilePrompt(){
	bool valid = false;
	// users input
	string input;

	// loop until valid input
	while (!valid){
		cout << "Load existing data from file (Y/N)? ";
		cin >> input;

		// convert input to upper case
		toUppercase(input);

		cout << endl;

		// return choice
		if (input == "Y" || input == "N"){
			valid = true;
		}
		else{
			cout << "Not a valid input." << endl;
		}
	}

	if (input == "Y")
		return "Y";

	return "N";
}

//*********************************************************************
// FUNCTION: storeFromInput
// DESCRIPTION: Writes the sorted data to an output file.
// INPUT:
// Parameters: inputFile - A reference to the input file.
// File: The file keeps track of homes for sale in the area.
//       The file contains the MLS listing number; the asking
// 		 price; whether the listing is available, under contract, or sold;
//  	 the zip code; and the representing realty company.
//				Data on each line is formatted as follows:
//  			123456 300000 0 45645-5445 Tuscon Real Estate
// OUTPUT:
// Parameters: listings - A record array that holds the listings.
//
// 			   listingsCounter - A reference to the listings array counter.
//
//             arraySize - The maximum size of the listings array.
//**********************************************************************
void storeFromInput(ifstream &inputFile, listingInfo listings[], int arraySize,
		            int &listingsCounter)
{
	int mlsNumber;
	double price;
	int tempStatus;
	string zipCode;
	string company;

	listingInfo listing;

	// priming read

	inputFile >> mlsNumber >> price >> tempStatus >> zipCode;
	getline(inputFile, company);
	// remove leading whitespace from company
	company.replace(0,1,"");

	while (inputFile){
		if (listingsCounter >= arraySize){
			cout << "Too many lines of data. No more license plates will "
					"be read." << endl;
			break;
		}

		// add listing to array
		listing.mls = mlsNumber;
		listing.price = price;
		listing.status = static_cast<statusEnum>(tempStatus);
		listing.zipCode = zipCode;
		listing.realtyCompany = company;

		listings[listingsCounter] = listing;

		inputFile >> mlsNumber >> price >> tempStatus >> zipCode;
		getline(inputFile, company);
		// remove leading whitespace from company
		company.replace(0,1,"");

		listingsCounter++;
	}
}

//*********************************************************************
// FUNCTION: loadDataHandler
// DESCRIPTION: Opens the input file, if it exists and calls the
// storeFromInput function to read the data from the file.
// If the file doesn't exist it prompts the user to continue without
// listings or to quit the program.
// INPUT:
// Parameters: inputFile - A reference to the input file.
//
//			   listings - A record array that holds the listings.
//
// 			   listingsCounter - A reference to the listings array counter.
//
//             arraySize - The maximum size of the listings array.
//
// CALLS TO: fileExists, storeFromInput, toUppercase
//**********************************************************************
int loadDataHandler(ifstream &inputFile, string inputFileName,
				    listingInfo listings[], int &listingsCounter,
				    int arraySize)
{
	inputFile.open(inputFileName.c_str());

	string load = loadFilePrompt();
	if (load == "Y"){
		bool exists = fileExists(inputFile, inputFileName);

		if (exists){
			storeFromInput(inputFile, listings, arraySize, listingsCounter);
		}
		else{
			string action;

			while (action != "C"){
				cout << "Press 'C' to continue the program starting "
						"with no data" << endl;
				cout << "or 'Q' to quit." << endl;
				cin >> action;
				toUppercase(action);

				if (action == "Q"){
					pause("Press 'ENTER' to terminate the program");

					return 1;
				}
				else if (action == "C"){
					cout << "Continue with empty array" << endl;
				}
			} // end while

		}
	}

	// close input file
	inputFile.close();

	return 0;
}

//*********************************************************************
// FUNCTION: displayMenu
// DESCRIPTION: Displays the menu, asks the user for an input and return
// that input.
// OUTPUT:
// Return Val: The user's choice.
// CALLS TO: toUppercase
//**********************************************************************
string displayMenu(){
	cout << setw(2) << left << 'D' << setw(2) << left
		 << '-' << "Display all." << endl;

	cout << setw(2) << left << 'A' << setw(2) << left
		 << '-' << "Add listing." << endl;

	cout << setw(2) << left << 'R' << setw(2) << left
		 << '-' << "Remove a listing." << endl;

	cout << setw(2) << left << 'E' << setw(2) << left
		 << '-' << "Exit the program." << endl;

	// string variable that will hold the user's input
	string choice;

	// loop until a valid input is given
	while (true){
		cout << "Enter one of the above options : ";
		cin >> choice;

		// convert input to upper case
		toUppercase(choice);

		cout << endl;

		if (choice == "D")
			return "D";
		else if (choice == "A")
			return "A";
		else if (choice == "R")
			return "R";
		else if (choice == "E")
			return "E";
	}
}

//*********************************************************************
// FUNCTION: convertEnum
// DESCRIPTION: Converts the statusEnum parameter to string and
// prints it to the screen.
// INPUT:
// Parameters: status - A statusEnum variable.
//**********************************************************************
void convertEnum(statusEnum status){
	switch (status){
		case AVAILABLE:
			cout << "Available";
			break;
		case CONTRACT:
			cout << "Contract";
			break;
		case SOLD:
			cout << "Sold";
			break;
	} // end switch
}

//*********************************************************************
// FUNCTION: convertStringEnum
// DESCRIPTION: Converts the string variable to statusEnum type and
// returns it.
// INPUT:
// Parameters: str - A string variable.
// Output:
// Return Val: statusEnum type (AVAILABLE, CONTRACT or SOLD)
//**********************************************************************
statusEnum convertStringEnum(string str){
	if (str == "A")
		return AVAILABLE;
	else if (str == "C")
		return CONTRACT;
	else if (str == "S")
		return SOLD;
}

//**********************************************************************
// FUNCTION: displayAllHeaders
// DESCRIPTION: Displays the headers to the screen.
//**********************************************************************
void displayAllHeaders(){
	cout << setw(21) << right << "Asking" << setw(14) << right
		 << "Price" << endl;
	cout << setw(15) << left << "MLS#" << setw(15) << "Price" << setw(15)
		 << "Status" << setw(15) << "Zip Code" << setw(15)
		 << "Realtor" << endl;
	cout << setw(15) << left << "------" << setw(15) << "-------" << setw(15)
		 << "-------" << setw(15) << "------------" << setw(15)
		 << "--------" << endl;
}

//*********************************************************************
// FUNCTION: displayAllListings
// DESCRIPTION: Gets all the listings stored in the listings array and
// prints it to the screen.
// INPUT:
// Parameters: listings - A record array that holds the listings passed as
// a constant.
//			   listingsCounter - The listings array counter.
// CALLS TO: convertEnum
//**********************************************************************
void displayAllListings(const listingInfo listings[], int listingsCounter){
	for (int i=0; i < listingsCounter; i++){
		cout << setw(15) << left << listings[i].mls;
		cout << setw(15) << left << listings[i].price;
		cout << setw(15) << left;
		// convert enumeration type to word
		convertEnum(listings[i].status);
		cout << setw(15) << left << listings[i].zipCode;
		cout << setw(15) << left << listings[i].realtyCompany;
		cout << endl;
	}
}

//*********************************************************************
// FUNCTION: displayAllOption
// DESCRIPTION: Calls functions to display the listings to the user.
// INPUT:
// Parameters: listings - A record array that holds the listings passed as
// a constant.
//			   listingsCounter - The listings array counter.
// CALLS TO: displayAllHeaders, displayAllListings
//**********************************************************************
void displayAllOption(const listingInfo listings[], int listingsCounter){
	// if the listings array is empty
	if (listingsCounter == 0){
		cout << "There are no listings stored." << endl;
	}
	else{
		displayAllHeaders();
		displayAllListings(listings, listingsCounter);
	}
	cout << endl;
}

//*********************************************************************
// FUNCTION: readMls
// DESCRIPTION: Asks user for a MLS input and returns it if it is valid.
// INPUT:
// Parameters: properLength - The proper length of the MLS.
//
//			   minMls - the minimum valid value for the MLS number.
//
//			   maxMls - the maximum valid value for the MLS number.
// OUTPUT:
// Return Val: The MLS number.
//**********************************************************************
int readMls(int properLength, int minMls, int maxMls){
	bool valid = false;
	int mls;

	while (!valid){
		cout << "Enter a 6-digit MLS number (first digit cannot be 0) : ";
		cin >> mls;

		if (mls >= minMls && mls <= maxMls){
			valid = true;
		}
		else{
			cout << "Not valid MLS number." << endl;
		}
	} // end while

	return mls;
}

//*********************************************************************
// FUNCTION: readPrice
// DESCRIPTION: Asks user for a price input and returns it, if it is
// >= 0.
// OUTPUT:
// Return Val: The price.
//**********************************************************************
double readPrice(){
	bool valid = false;
	double price;

	while (!valid){
		cout << "Price : ";
		cin >> price;

		if (price >= 0){
			valid = true;
		}
		else{
			cout << "Price must be >= 0" << endl;
		}
	}

	return price;
}

//*********************************************************************
// FUNCTION: readZipCode
// DESCRIPTION: Asks user for a zip code and returns it if it's valid.
// INPUT:
// Parameters: properLength - The proper length of the zip-code.
// OUTPUT:
// Return Val: The zip code.
//**********************************************************************
string readZipCode(int properLength){
	string zipCode;
	int zipCodeLength;
	string firstPart;
	string secondPart;
	char dash;
	bool valid = false;

	while (!valid){
		cout << "Zip code (5 numbers, dash, then 4 numbers) : ";
		cin >> zipCode;

		zipCodeLength = zipCode.length();

		if (zipCodeLength == properLength){
			// split zipCode to parts
			firstPart = zipCode.substr(0,4);
			dash = zipCode[5];
			secondPart = zipCode.substr(6,10);

			// validate
			if ( isNumber(firstPart) && isNumber(secondPart) && dash == '-'){
				valid = true;
			}
			else{
				cout << "The zip code is not formatted correctly." << endl;
				cout << "Must be : ";
				cout << "Five numbers followed by a dash followed by "
						"four numbers." << endl;
			}
		}
		else{
			cout << "The zip code must me 10 characters." << endl;
		}
	}

	return zipCode;
}

//*********************************************************************
// FUNCTION: readStatus
// DESCRIPTION: Displays 3 choices to the user, converts the input to
// statusEnum and returns it.
// OUTPUT:
// Return Val: The choice as a statusEnum variable.
//**********************************************************************
statusEnum readStatus(){
	bool valid = false;
	string status;					// temp string status variable
	statusEnum convertedStatus;	    // enum type status variable

	while (!valid){
		// display options
		cout << "Choices" << endl;
		cout << "-------" << endl;
		cout << "A - Available" << endl;
		cout << "C - Contract" << endl;
		cout << "S - Sold" << endl;
		cout << endl;

		// get status and convert to upper case
		cout << "Status : ";
		cin >> status;
		toUppercase(status);

		// validate input
		if ( (status == "A" || status == "C" || status == "S") ){
			// convert to statusEnum type
			convertedStatus = convertStringEnum(status);
			valid = true;
		}
		else{
			cout << "Wrong input " << endl;
			cout << endl;
		}
	}

	return convertedStatus;
}

//*********************************************************************
// FUNCTION: readRealty
// DESCRIPTION: Ask the user for the realty's name.
// INPUT:
// Parameters: properRealtyLength - The proper length for the realty.
// OUTPUT:
// Return Val: The realty converted to proper case.
// CALLS TO: onlyLetters, properCase
//**********************************************************************
string readRealty(int properRealtyLength){
	string realty;
	int realtyLength;
	bool correctFormat;
	bool valid = false;

	while (!valid){
		cout << "Realty company name : ";
		// ignore characters in buffer
		cin.ignore();
		// read user's input
		getline(cin, realty);

		realtyLength = realty.length();
		// check if string consists only of letters and whitespace
		correctFormat = onlyLetters(realty);

		// validation
		if (realtyLength <= properRealtyLength){
			if (correctFormat){
				valid = true;
			}
			else{
				cout << "Company name must contain only "
						"letters and spaces." << endl;
			}
		}
		else{
			cout << "Company name must be less than "
				 << properRealtyLength << " characters." << endl;
		}

		cout << endl;
	} // end while

	// convert to proper case
	properCase(realty);

	return realty;

}

//*********************************************************************
// FUNCTION: addListingOption
// DESCRIPTION: Calls other functions to get input from the user and
// adds new listings to the listings array.
// INPUT:
// Parameters:
// 			   properMlsLength - The proper length of the MLS.
//
//			   minMls - the minimum valid value for the MLS number.
//
//			   maxMls - the maximum valid value for the MLS number.
//
// 			   properZipCodeLength - The proper length of the zip code.
//
// 			   properRealtyLength - The proper length of the name of
// 									the realty.
// OUTPUT:
// Parameters: listings - A record array that holds the listings passed.
//
//			   listingsCounter - A reference to the listings array counter.
//
//			   arraySize - The max length of the listings array.
// Return Val: The MLS number.
// CALLS TO: readMls, readPrice, readZipCode, readStatus, readRealty
//**********************************************************************
void addListingOption(listingInfo listings[], int &listingsCounter,
		              int arraySize,
		              int properMlsLength, int minMls, int maxMls,
		              int properZipCodeLength, int properRealtyLength)
{
	bool done = false;
	string prompt;

	// initialize variables
	int mls;
	double price;
	string zipCode;
	statusEnum status;
	string realtyCompany;

	// while there are more listings to add and there is available space
	while (!done && listingsCounter < arraySize){
		// get input
		mls = readMls(properMlsLength, minMls, maxMls);
		price = readPrice();
		zipCode = readZipCode(properZipCodeLength);
		status = readStatus();
		realtyCompany = readRealty(properRealtyLength);

		// add data to the listings array
		listings[listingsCounter].mls = mls;
		listings[listingsCounter].price = price;
		listings[listingsCounter].zipCode = zipCode;
		listings[listingsCounter].status = status;
		listings[listingsCounter].realtyCompany = realtyCompany;

		listingsCounter++;

		// if there is space
		if (listingsCounter < arraySize){
			// ask to add more listings
			while (true){
				cout << "Add more listings? (Y/N) : " << endl;
				cin >> prompt;

				if (prompt == "N" || prompt == "n"){
					done = true;
					break;
				}
				else{
					if (prompt == "Y" || prompt == "y"){
						break;
					}
					else{
						cout << "Wrong input." << endl;
					}
				}
			} // end while
		}
	} // end while
}

//*********************************************************************
// FUNCTION: displayAllMls
// DESCRIPTION: Displays all MLS numbers in columns.
// Parameters: listings - A record array that holds the listings passed as
// a constant.
//			   listingsCounter - The listings array counter.
//**********************************************************************
void displayAllMls(const listingInfo listings[], int listingsCounter){
	cout << "Listings" << endl;
	cout << "----------" << endl;
	for (int i=0; i < listingsCounter; i++){
		// new line after every 6 MLS numbers
		if (i % 6 == 0 && i != 0){
			cout << endl;
		}

		// print MLS number
		cout << left << setw(10) << listings[i].mls;
	}
	cout << endl << endl;
}

//*********************************************************************
// FUNCTION: findMls
// DESCRIPTION: Calls other functions to get input from the user and
// adds new listings to the listings array.
// INPUT:
// Parameters: listings - A record array that holds the listings passed as
// a constant.
//
//			   listingsCounter - The listings array counter.
//
//			   target - The search target.
// OUTPUT:
// Return Val: The MLS location if it is found or -1 if it's not.
//**********************************************************************
int findMls(const listingInfo listings[], int listingsCounter, int target){
	int index = 0;

	for (int i=0; i < listingsCounter; i++){
		if (listings[i].mls == target){
			return index;
		}

		index++;
	}

	return -1;
}

//*********************************************************************
// FUNCTION: deleteItem
// DESCRIPTION: Deletes the array element that is passed and reorders the
// listings array.
// INPUT:
// Parameters: location - The location of the MLS that will be deleted.
// OUTPUT:
// Parameters: listings - A record array that holds the listings passed.
//
//			   listingsCounter - A reference to the listings array counter.
//**********************************************************************
void deleteItem(listingInfo listings[], int &listingsCounter, int location){
	// copy last element's value to deleted element's location
	listings[location].mls = listings[listingsCounter-1].mls;
	listings[location].price = listings[listingsCounter-1].price;
	listings[location].realtyCompany = listings[listingsCounter-1].
			                           realtyCompany;
	listings[location].status = listings[listingsCounter-1].status;
	listings[location].zipCode = listings[listingsCounter-1].zipCode;

	// decrement counter
	listingsCounter--;
}

//*********************************************************************
// FUNCTION: removeListingOption
// DESCRIPTION: Displays all MLS numbers, gets one as input from the user
// and deletes it from the listings array.
// INPUT:
// Parameters: listings - A record array that holds all the listings.
//
//			   listingsCounter - A reference to the listings array counter.
//
//			   properLength - The proper length of the MLS.
//
//			   minMls - the minimum valid value for the MLS number.
//
//			   maxMls - the maximum valid value for the MLS number.
// CALLS TO: displayAllMls, readMls, findMls, deleteItem
//**********************************************************************
void removeListingOption(listingInfo listings[], int &listingsCounter,
						 int properMlsLength, int minMls, int maxMls){
	int mls;
	int location;

	if (listingsCounter == 0){
		cout << "There are no listings stored to remove." << endl;
		cout << endl;
	}
	// if there are listings stored
	else{
		// show all MLS numbers in columns
		displayAllMls(listings, listingsCounter);

		// validate MLS
		mls = readMls(properMlsLength, minMls, maxMls);
		// search MLS number
		location = findMls(listings, listingsCounter, mls);

		// if it's not found
		if (location == -1){
			cout << "MLS, #" << mls << " is not in the list." << endl;
		}
		else{
			// delete the provided MLS number from the listings array
			deleteItem(listings, listingsCounter, location);
			cout << "MLS, #" << mls << " got removed from the list." << endl;
		}

		cout << endl;
	}
}

//*********************************************************************
// FUNCTION: storeToOutput
// DESCRIPTION: Writes the sorted data to an output file.
// INPUT:
// Parameters: outputFile - A reference to the output file.
//
//			   outputFileName - The name of the output file.
//
//			   listings - A record array that holds the listings.
//
// 			   listingsCounter - The listings array counter.
//
//             arraySize - The maximum size of the listings array.
//
// OUTPUT:
// 	File: The file keeps track of homes for sale in the area.
//      The file contains the MLS listing number; the asking
// 		price; whether the listing is available, under contract, or sold;
//  	the zip code; and the representing realty company.
//	Data on each line is formatted as follows:
//  123456 300000 0 45645-5445 Tuscon Real Estate
//**********************************************************************
void storeToOutput(ofstream &outputFile, string outputFileName,
				   listingInfo listings[], int listingsCounter,
				   int arraySize)
{
	// declare variables
	int mlsNumber;
	double price;
	statusEnum status;
	string zipCode;
	string realty;

	outputFile.open(outputFileName.c_str());

	for (int i=0; i < listingsCounter; i++){
		// get data from the listings array
		mlsNumber = listings[i].mls;
		price = listings[i].price;
		status = listings[i].status;
		zipCode = listings[i].zipCode;
		realty = listings[i].realtyCompany;

		// write data to file
		outputFile << mlsNumber << " " << price << " " << status
				   << " " << zipCode << " " << realty;
		outputFile << '\n';
	}

	outputFile.close();
}

//*********************************************************************
// FUNCTION: exitProgramOption
// DESCRIPTION: Prompts the user to exit the program and calls
// storeToOutput to save any changes made.
// INPUT:
// Parameters: outputFile - A reference to the output file.
//
//			   outputFileName - The name of the output file.
//
//			   listings - A record array that holds the listings.
//
// 			   listingsCounter - The listings array counter.
//
//             arraySize - The maximum size of the listings array.
//
// CALLS TO: toUppercase, storeToOutput, pause
//**********************************************************************
void exitProgramOption(ofstream &outputFile, string outputFileName,
					   listingInfo listings[], int listingsCounter,
					   int arraySize)
{
	while (true){
		string yesOrNo;
		cout << "Do you want to save the data to the " << outputFileName
			 << ". (Y/N)" << endl;
		cin >> yesOrNo;

		toUppercase(yesOrNo);

		if (yesOrNo == "Y"){
			storeToOutput(outputFile, outputFileName,
					      listings, listingsCounter, arraySize);
			pause("Press 'ENTER' to exit the program");
			break;
		}
		else if (yesOrNo == "N"){
			cout << "You will lose all changes made." << endl;
			pause("Press 'ENTER' to exit the program");
			break;
		}
	} // end while
}

//*********************************************************************
// FUNCTION: menuHandler
// DESCRIPTION: Displays and calls other functions depending on which option
// the user chooses
// INPUT:
// Parameters:
//			   listings - A record array that holds the listings.
//
// 			   listingsCounter - A reference to the listings array counter.
//
//             arraySize - The maximum size of the listings array.
//
//			   properMlsLength - The proper length of the MLS.
//
//			   minMls - the minimum valid value for the MLS number.
//
//			   maxMls - the maximum valid value for the MLS number.
//
// 			   properZipCodeLength - The proper length of the zip code.
//
// 			   properRealtyLength - The proper length of the name of
// 									the realty.
//
// 			   outputFile - A reference to the output file.
//
//			   outputFileName - The name of the output file.
// CALLS TO: displayMenu, displayAllOption, addListingOption,
// removeListingOption, exitProgramOption
//**********************************************************************
void menuHandler(listingInfo listings[], int &listingsCounter, int arraySize,
		        int properMlsLength, int minMls, int maxMls,
		        int properZipCodeLength, int properRealtyLength,
		        ofstream &outputFile, string outputFileName)
{
	while (true){
		// display menu and return choice
		string menu = displayMenu();

		if (menu == "D"){
			displayAllOption(listings, listingsCounter);
		}
		else if (menu == "A"){
			addListingOption(listings, listingsCounter, arraySize,
							 properMlsLength, minMls, maxMls,
							 properZipCodeLength, properRealtyLength);
		}
		else if (menu == "R"){
			removeListingOption(listings, listingsCounter, properMlsLength,
					            minMls, maxMls);
		}
		else if (menu == "E"){
			exitProgramOption(outputFile, outputFileName, listings,
					  	      listingsCounter, arraySize);
			break;
		}
	} // end while
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
// FUNCTION: properCase
// DESCRIPTION: Converts a string to proper case (Like This).
// INPUT:
// Parameters: str - A reference to str that will be converted.
//**********************************************************************
void properCase(string &str){
	int strLength = str.length();

	// always convert the first letter to upper case
	str[0] = toupper(str[0]);

	for (int i=0; i < strLength; i++){
		// if character is whitespace and next character at most
		// the last one
		if (str[i] == ' ' && i+1 < strLength){
			// convert next character to upper case
			str[i+1] = toupper(str[i+1]);
		}
		else{
			// convert all other characters to lower case
			str[i+1] = tolower(str[i+1]);
		}
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
		cout << "The input file " << inputFileName.c_str();
		cout << " does not exist." << endl;

		return false;
	}

	return true;
}

//*********************************************************************
// FUNCTION: isNumber
// DESCRIPTION: Checks if string consists only of digits.
// INPUT:
// Parameters: str - Reference to a string variable
// OUTPUT:
// Return Val: True if file it consists only of digits,
// false otherwise.
//**********************************************************************
bool isNumber(string &str){
	int strLength = str.length();

	for (int i=0; i < strLength; i++){
		if ( !isdigit(str[i]) ){
			return false;
		}
	}

	return true;
}
//*********************************************************************
// FUNCTION: onlyLetters
// DESCRIPTION: Checks if string consists only of letters and whitespace.
// INPUT:
// Parameters: str - Reference to a string variable
// OUTPUT:
// Return Val: True if file it consists only of letters and whitespace,
// false otherwise.
//**********************************************************************
bool onlyLetters(string &str){
	int strLength = str.length();

	for (int i=0; i < strLength; i++){
		if ( !isalpha(str[i]) && !isspace(str[i]) ){
			return false;
		}
	}

	return true;
}

