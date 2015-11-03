//******************************************************************************
// CODE FILENAME: GeorgoulisAssn6.cpp
// DESCRIPTION: This program will prompt the user to get real estate listings
// from an input file. Then depending on his choice will read and store these
// data to a linked list.
// It will then present a menu of choices. The user can :
// 	1. Display all listings.
// 	2. Add a new listing.
// 	3. Remove an existing listing.
//	4. Change asking prices (Read data from another file and change any listing
//  price found by its MLS number to a different one.)
// 	5. Exit the program.
// Before exiting the program, the user will be prompted to save any changes
// he made to an output file of his choice. If s/he chooses an existing file
// s/he will be prompted to overwrite its contents.
// CLASS/TERM: CS362 - 13F8W2
// DESIGNER: Anastasios Georgoulis
// FUNCTIONS:
//
// displayDescription - Displays a short description to the user.
//
// loadDataHandler - Prompts the user to enter a filename for reading data from
// an input file or go to the action menu.
//
// existingInfoPrompt - Prompts the user to enter a filename for reading data
// from an input file or skip this step.
//
// loadData - Reads data from a file and stores them to a linked list.
//
// actionMenuHandler - Handles the menu options and calls the appropriate
// functions.
//
// displayMenu - Displays a menu of choices to the user.
//
// displayAllHeaders - Displays headers in columns.
//
// displayAllListings - Displays all listings stored in the listings linked
// 						list.
//
// convertEnum - Converts the statusEnum parameter to string and
// prints it to the screen.
//
// ConvertStringEnum - Converts the string variable to statusEnum type and
// returns it.
//
// addListingOption - Calls other functions to get input from the user and
// adds new listings to the linked list.
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
// removeListingOption - Displays all MLS numbers, gets one as input from
// the user and deletes it from the linked list.
//
// displayAllMls - Displays all MLS numbers in columns.
//
// changePriceOption - Handles the menu options and calls the appropriate
// functions.
//
// exitOption - Prompts the user to exit the program and to save data to a
// file.
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
#include <fstream>
#include <iomanip>
#include <string>
#include <cstddef>

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
	listingInfo *next;
};

//*********************************
//******Functions Prototypes*******
//*********************************

void displayDescription();

bool loadDataHandler(ifstream &inputFile, string inputFileName);
bool existingInfoPrompt();
void loadData(ifstream &inputFile, string inputFileName, listingInfo *&head);

void actionMenuHandler(listingInfo *&head, int properMlsLength,
		  	  	  	  int minMls, int maxMls, int properZipCodeLength,
		  	  	  	  int properRealtyLength, ifstream &changesFile,
		  	  	  	  string changesFileName);

char displayMenu();
void displayAllHeaders();
void displayAllListings(listingInfo *head);
void convertEnum(statusEnum status);
statusEnum convertStringEnum(string str);

void addListingOption(listingInfo *&head, int properMlsLength,
					  int minMls, int maxMls, int properZipCodeLength,
					  int properRealtyLength);
int readMls(int properLength, int minMls, int maxMls);
double readPrice();
string readZipCode(int properLength);
statusEnum readStatus();
string readRealty(int properRealtyLength);

void removeListingOption(listingInfo *&head,
						 int properMlsLength, int minMls, int maxMls);
void displayAllMls(listingInfo *head);

void changePriceOption(ifstream &changesFile, string changesFileName,
					   listingInfo *head);

void exitOption(listingInfo *head);

// Helper Functions

void toUppercase(string &str);
void properCase(string &str);
bool isNumber(string &str);
bool onlyLetters(string &str);
void pause(string msg="Press 'ENTER' to continue.");
bool fileExists(ifstream &inputFile, string inputFileName);

//*********************************************************************
// FUNCTION: main
// DESCRIPTION: Initializes the variables and calls some helper functions.
// It displays a short description to the user and asks the user
// if he wants to load data from an input file.
// If the user chooses to load existing data, the function loadData
// is called to get the data and store them in a linked list.
// It then calls the actionMenuHandler function to display a menu with choices.
// Depending on what he chooses, the user can display all listings,
// add a new listing, remove a listing, change the asking prices of some
// listings or exit the program.
// If the user chooses to quit he will be asked to save any changes he made.
//
// OUTPUT:
// Return Val: 0 -> If the program terminated without any problems.
//			   1 -> If it there were problems.
// CALLS TO: displayDescription, loadDataHandler, loadData, actionMenuHandler,
//			 pause
//**********************************************************************
int main(){
	// CONSTANTS
	const int MIN_MLS = 100000;
	const int MAX_MLS = 999999;
	const int PROPER_MLS_LENGTH = 6;
	const int PROPER_ZIPCODE_LENGTH = 10;
	const int PROPER_REALTY_LENGTH = 20;

	// VARIABLES
	ifstream inputFile, changesFile;
	string inputFileName;
	string changesFileName = "CHANGES.txt";
	listingInfo *head = NULL; 	// head of the linked list
	bool load;					// set to true if user wants to load data
								// from a file

	// show short description
	displayDescription();

	// ask user if s/he wants to load data from a file
	load = loadDataHandler(inputFile, inputFileName);

	if (load){
		// load and save data to a linked list
		loadData(inputFile, inputFileName, head);
	}

	// display menu and call appropriate functions
	actionMenuHandler(head, PROPER_MLS_LENGTH, MIN_MLS, MAX_MLS,
					  PROPER_ZIPCODE_LENGTH, PROPER_REALTY_LENGTH,
					  changesFile, changesFileName);

	pause("Press 'ENTER' to exit.");

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
	cout << "to save any changes you may have made in a file." << endl;
	cout << endl;
}

//*********************************************************************
// FUNCTION: loadDataHandler
// DESCRIPTION: Prompts the user to enter a filename for reading data from
// an input file or go to the action menu.
// INPUT:
// Parameters: inputFile - A reference to the input file.
//			   inputFileName - The name of the file
// File: The file keeps track of homes for sale in the area.
//       The file contains the MLS listing number; the asking
// 		 price; whether the listing is available, under contract, or sold;
//  	 the zip code; and the representing realty company.
//				Data on each line is formatted as follows:
//  			123456 300000 0 45645-5445 Tuscon Real Estate
// Return Val: True if the user wants to load data, false otherwise
// CALLS TO: existingInfoPrompt, fileExists, toUppercase,
//**********************************************************************
bool loadDataHandler(ifstream &inputFile, string inputFileName){
	bool loadExistingInfo;
	bool exists;
	bool valid = false;
	bool load = false;

	loadExistingInfo = existingInfoPrompt();

	if (loadExistingInfo){
		cout << "Enter a filename : ";
		cin >> inputFileName;

		exists = fileExists(inputFile, inputFileName);

		if (!exists){
			while (!valid){
				string choice;
				cout << endl << inputFileName << " doesn't exist." << endl;
				cout << endl << "Do you want to enter another file name,"
					 << endl;
				cout << "or go to the action menu?" << endl << endl;
				cout << "(F)ilename" << endl;
				cout << "(A)ction menu" << endl;
				cin >> choice;

				toUppercase(choice);

				if (choice == "F" || choice == "A"){
					if (choice == "F"){
						// clear the filestream error
						inputFile.clear();

						cout << "Enter a filename : ";
						cin >> inputFileName;

						exists = fileExists(inputFile, inputFileName);

						if (exists){
							// file exists -> load data
							valid = true;
							load = true;
						}
					}
					else{	// choice is 'A'
						valid = true;
					}
				}
			} // end while
		}
		else{
			// file exists -> load data
			load = true;
		}
	}


	if (load)
		return true;
	return false;
}

//*********************************************************************
// FUNCTION: existingInfoPrompt
// DESCRIPTION: Prompts the user to enter a filename for reading data from
// an input file or skip this step.
// Return Val: true if the user wants to use existing information,
// false otherwise.
// CALLS TO: toUppercase
//**********************************************************************
bool existingInfoPrompt(){
	string yn;
	string filename;
	bool valid = false;

	while(!valid){
		cout << "Do you want to use existing information? (Y/N) ";
		cin >> yn;

		toUppercase(yn);

		if (yn == "Y" || yn == "N")
			valid = true;
	}

	if (yn == "Y")
		return true;

	return false;
}

//*********************************************************************
// FUNCTION: loadData
// DESCRIPTION: Reads data from a file and stores them to a linked list.
// INPUT:
// Parameters: inputFile - A reference to the input file.
//			   inputFileName - The name of the file.
//
// File: The file keeps track of homes for sale in the area.
//       The file contains the MLS listing number; the asking
// 		 price; whether the listing is available, under contract, or sold;
//  	 the zip code; and the representing realty company.
//				Data on each line is formatted as follows:
//  			123456 300000 0 45645-5445 Tuscon Real Estate
// OUTPUT:
// Parameters: *head - A reference to a pointer that points to the head
// 					   of the list.
//**********************************************************************
void loadData(ifstream &inputFile, string inputFileName, listingInfo *&head){
	int mlsNumber;
	double price;
	int tempStatus;
	string zipCode;
	string company;

	listingInfo *oneListing;	// used to allocate memory

	// priming read
	inputFile >> mlsNumber >> price >> tempStatus >> zipCode;
	getline(inputFile, company);
	// remove leading whitespace from company
	company.replace(0,1,"");

	while(inputFile){
		// create a new listing and save it's address to a pointer
		oneListing = new (nothrow) listingInfo;

		// if there is no more space in the heap
		if (oneListing == NULL){
		    cout << "Heap Error -- out of heap memory!" << endl;
		    cout << "Cannot allocate space" << endl;
		    cout << "No more data will be read" << endl;
		    pause();
		}
		else{
			// add data to the listing
			oneListing->mls = mlsNumber;
			oneListing->price = price;
			oneListing->status = static_cast<statusEnum>(tempStatus);
			oneListing->zipCode = zipCode;
			oneListing->realtyCompany = company;

			// point the next field of the listing to the head of the list
			oneListing->next = head;
			// point head to the listing
			head = oneListing;

			// get data
			inputFile >> mlsNumber >> price >> tempStatus >> zipCode;
			getline(inputFile, company);
			// remove leading whitespace from company
			company.replace(0,1,"");
		}
	} // end while
}

//*********************************************************************
// FUNCTION: actionMenuHandler
// DESCRIPTION: Handles the menu options and calls the appropriate functions.
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
//			   changesFile - A reference to an input file.
//
//			   changesFileName - The name of the input file.
// OUTPUT:
// Parameters: *head - A reference to a pointer that points to the head
// 					   of the list.
// CALLS TO: readMls, readPrice, readZipCode, readStatus, readRealty
//**********************************************************************
void actionMenuHandler(listingInfo *&head, int properMlsLength,
		  	  	  	  int minMls, int maxMls, int properZipCodeLength,
		  	  	  	  int properRealtyLength, ifstream &changesFile,
		  	  	  	  string changesFileName)
{
	bool exit = false;
	char choice;

	while (!exit){
		// display the actions menu and get the user's choice
		choice = displayMenu();

		// call the appropriate function depending on the user's choice
		if (choice == 'D')
			displayAllListings(head);
		else if (choice == 'A')
			addListingOption(head, properMlsLength, minMls, maxMls,
							 properZipCodeLength, properRealtyLength);
		else if (choice == 'R')
			removeListingOption(head, properMlsLength, minMls, maxMls);
		else if (choice == 'C')
			changePriceOption(changesFile, changesFileName, head);
		else if (choice == 'E'){
			exitOption(head);
			exit = true;
		}
	}
}

//*********************************************************************
// FUNCTION: displayMenu
// DESCRIPTION: Displays the menu, asks the user for an input and return
// that input.
// OUTPUT:
// Return Val: The user's choice.
// CALLS TO: toUppercase
//**********************************************************************
char displayMenu(){
	bool valid = false;

	cout << endl;
	cout << setw(2) << left << 'D' << setw(2) << left
		 << '-' << "Display all" << endl;

	cout << setw(2) << left << 'A' << setw(2) << left
		 << '-' << "Add listing" << endl;

	cout << setw(2) << left << 'R' << setw(2) << left
		 << '-' << "Remove a listing" << endl;

	cout << setw(2) << left << 'C' << setw(2) << left
			 << '-' << "Change asking prices" << endl;

	cout << setw(2) << left << 'E' << setw(2) << left
		 << '-' << "Exit the program" << endl;

	// string variable that will hold the user's input
	string choice;

	// loop until a valid input is given
	while (!valid){
		cout << endl << "Enter one of the above options : ";
		cin >> choice;

		// convert input to upper case
		toUppercase(choice);

		cout << endl;

		if (choice == "D" || choice == "A" || choice == "R" ||
			choice == "C" || choice == "E")
		{
			valid = true;
		}
	}

	if (choice == "D")
		return 'D';
	else if (choice == "A")
		return 'A';
	else if (choice == "R")
		return 'R';
	else if (choice == "C")
		return 'C';
	else
		return 'E';
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
// DESCRIPTION: Gets all the listings stored in the linked list and
// prints them to the screen.
// INPUT:
// Parameters: *head - A reference to a pointer that points to the head
// 					   of the list.
// CALLS TO: convertEnum
//**********************************************************************
void displayAllListings(listingInfo *head){
	if (head == NULL){
		cout << "No listings are stored" << endl;
	}
	else{
		displayAllHeaders();

		while (head != NULL){
			cout << setw(15) << left << head->mls;
			cout << setw(15) << left << head->price;
			cout << setw(15) << left;
			// convert enumeration type to word
			convertEnum(head->status);
			cout << setw(15) << left << head->zipCode;
			cout << setw(15) << left << head->realtyCompany;
			cout << endl;

			head = head->next;
		}
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

//*********************************************************************
// FUNCTION: addListingOption
// DESCRIPTION: Calls other functions to get input from the user and
// adds new listings to the linked list.
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
// Parameters: *head - A reference to a pointer that points to the head
// 					   of the list.
// CALLS TO: readMls, readPrice, readZipCode, readStatus, readRealty,
//			 toUppercase
//**********************************************************************
void addListingOption(listingInfo *&head, int properMlsLength,
					  int minMls, int maxMls, int properZipCodeLength,
					  int properRealtyLength)
{
	bool done = false;
	bool valid = false;
	string prompt;

	// initialize variables
	int mls;
	double price;
	statusEnum status;
	string zipCode;
	string realtyCompany;

	listingInfo *newListing;	// used to allocate memory

	// while there are more listings to add and there is available space
	while (!done){
		// get input
		mls = readMls(properMlsLength, minMls, maxMls);
		price = readPrice();
		zipCode = readZipCode(properZipCodeLength);
		status = readStatus();
		realtyCompany = readRealty(properRealtyLength);

		newListing = new (nothrow) listingInfo;

		// if there is no space in heap memory issue a warning and exit loop
		if (newListing == NULL){
			cout << "No more space in memory. "
					"Can't add more listings."
				 << endl;
			done = true;
		}
		else{
			newListing->mls = mls;
			newListing->price = price;
			newListing->status = status;
			newListing->zipCode = zipCode;
			newListing->realtyCompany = realtyCompany;

			// point the next field of the listing to the head of the list
			newListing->next = head;
			// point head to the listing
			head = newListing;

			// ask to add more listings
			while (!valid){
				cout << "Add more listings? (Y/N) : " << endl;
				cin >> prompt;

				toUppercase(prompt);

				if (prompt == "N"){
					done = true;
					valid = true;
				}
				else{
					if (prompt == "Y"){
						valid = true;
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
// FUNCTION: removeListingOption
// DESCRIPTION: Displays all MLS numbers, gets one as input from the user
// and deletes it from the linked list.
// INPUT:
// Parameters: properLength - The proper length of the MLS.
//
//			   minMls - the minimum valid value for the MLS number.
//
//			   maxMls - the maximum valid value for the MLS number.
// OUTPUT:
// Parameters: *head - A reference to a pointer that points to the head
// 					   of the list.
// CALLS TO: displayAllMls, readMls, findMls
//**********************************************************************
void removeListingOption(listingInfo *&head,
						 int properMlsLength, int minMls, int maxMls){
	int mls;
	listingInfo *prev,
				*here;

	if (head == NULL){
		cout << "There are no listings to delete." << endl;
	}
	// if there are listings stored
	else{
		// show all MLS numbers in columns
		displayAllMls(head);

		// get and validate the MLS number
		mls = readMls(properMlsLength, minMls, maxMls);

		// search for MLS number

		if (head->mls == mls){		// if MLS found in first node
			here = head;
			head = head->next;		// delete top
			delete here;

			cout << endl << "The listing with MLS number " << mls
				 << " was deleted from the list." << endl;
		}
		else{						// search list
			// initialize prev and here to first two nodes of the list
			prev = head;
			here = head->next;

			while ( (here != NULL) && (here->mls != mls) ){
				// move prev and here, down one
				prev = here;
				here = here->next;
			}

			if (here != NULL){		// if found, delete item from list
				prev->next = here->next;
				delete here;

				cout << endl << "The listing with MLS number " << mls
					 << " was deleted from the list." << endl;
			}
			else{					// give error message
				cout << mls << " not found in the list" << endl;
			}
		}

	}
}

//*********************************************************************
// FUNCTION: displayAllMls
// DESCRIPTION: Displays all MLS numbers in columns.
// INPUT:
// Parameters: *head - A pointer that points to the head
// 					   of the list.
//**********************************************************************
void displayAllMls(listingInfo *head){
	const int LINE_BREAK = 6;	// how many MLS numbers per line
	int counter = 0;			// used to count the listings and add a new line
								// every 6th listing.

	cout << "Listings" << endl;
	cout << "----------" << endl;

	while (head != NULL){
		// new line after every 6 MLS numbers
		if (counter % LINE_BREAK == 0 && counter != 0){
			cout << endl;
		}

		// print MLS number
		cout << left << setw(10) << head->mls;

		head = head->next;
		counter++;
	}
	cout << endl << endl;
}

//*********************************************************************
// FUNCTION: changePriceOption
// DESCRIPTION: Handles the menu options and calls the appropriate functions.
// INPUT:
// Parameters:
//			   changesFile - A reference to an input file.
//
//			   changesFileName - The name of the input file.
//
//			   *head - A pointer that points to the head
// 					   of the linked list.
// File:
// CHANGES.txt contains MLS numbers and corresponding dollar amounts.
// Data on each line is formatted as follows:
// 		111111 999999
// CALLS TO: readMls, readPrice, readZipCode, readStatus, readRealty
//**********************************************************************
void changePriceOption(ifstream &changesFile, string changesFileName,
					   listingInfo *head)
{
	bool exists = false;
	bool noMatches = true;
	int mls;
	double price;
	listingInfo *oneListing = head;

	if (head == NULL){
		cout << "There are no listings to match" << endl;
	}
	else{
		exists = fileExists(changesFile, changesFileName);

		if (!exists){
			cout << changesFileName << " doesn't exist." << endl;
		}
		else{
			// if there are price changes display headers
			if (!noMatches){
				cout << "MLS Number" << right << setw(20) << "New Asking Price"
					 << endl;
				cout << "----------" << right << setw(20) << "----------------"
					 << endl;
			}

			// priming read
			changesFile >> mls >> price;

			// while there are data in the file
			while (changesFile){

				// while not the end of the list
				while (oneListing != NULL){

					// if there is a matching mls
					if (mls == oneListing->mls){

						// if no matches is true -> set it to false
						if (noMatches)
							noMatches = false;

						oneListing->price = price;

						cout << mls << right << setw(14)
							 << price << endl;
					}

					// get next listing
					oneListing = oneListing->next;

				} // end while

				// get MLS# and price from the file
				changesFile >> mls >> price;
				// reset oneListing to the head of the list
				oneListing = head;

			} // end while

			// if there are no price changes
			if (noMatches){
				cout << "No price reductions were made" << endl;
			}
		}
	}
}

//*********************************************************************
// FUNCTION: exitOption
// DESCRIPTION: Prompts the user to exit the program and to save data to a file
// INPUT:
// Parameters: *head - A pointer that points to the head
// 					   of the list.
// CALLS TO: toUppercase, fileExists
//**********************************************************************
void exitOption(listingInfo *head)
{
	string yn;					// holds input from a yes or no prompt
	string overwritePrompt;		// holds input from a yes or no prompt

	bool exists;				// used to check if file exists
	bool overwriteLoop = false;	// controls loop
	bool ynLoop = true;			// controls loop
	bool valid = false;			// controls loop
	bool saveData = false;		// if set to true data will be saved to a file

	ifstream tempFile;			// temporary file, used to check if file exists
	ofstream outputFile;		// output file to write to
	string outputFileName;		// output file name

	listingInfo *oneListing;	// temporary listing pointer to traverse
								// linked list

	while (!valid){
		cout << "Do you want to save the data to the file(Y/N) :";
		cin >> yn;

		// convert input to upper case
		toUppercase(yn);

		if (yn == "Y" || yn == "N"){
			valid = true;
		}
	}

	// set to false to be used again
	valid = false;

	while (ynLoop){
		if (yn == "Y"){
			// set to true to stop yn while loop
			ynLoop = false;

			while (!valid){
				// clear the filestream error
				tempFile.clear();

				cout << "Enter a filename : ";
				cin >> outputFileName;

				// check if file exists
				exists = fileExists(tempFile, outputFileName);

				// if it exists
				if (exists){
					cout << outputFileName << " already exists." << endl;
					cout << "You are about to overwrite an already existing "
							"file." << endl << endl;

					while (!overwriteLoop){
						cout << "Do you want to overwrite "
							 << outputFileName << "? (Y/N) ";
						cin >> overwritePrompt;

						// convert input to upper case
						toUppercase(overwritePrompt);

						if (overwritePrompt == "Y" || overwritePrompt == "N"){
							// set to true to stop both while loops
							valid = true;
							overwriteLoop = true;

							if (overwritePrompt == "Y"){
								// data will be saved to a file
								saveData = true;
							}
						}
					} // end overwrite while loop
				}
				else{
					// set to true to stop while loop
					valid = true;
					// data will be saved to a file
					saveData = true;
				}
			} // end while loop

			// if saveData flag is set to true, save data to a file
			if (saveData){
				// save data
				cout << "Save data" << endl;

				// set temp listing to the head of the linked list
				oneListing = head;

				// open file
				outputFile.open(outputFileName.c_str());

				while (oneListing != NULL){
					// write data to file
					outputFile << oneListing->mls << " "
							   << oneListing->price << " "
							   << oneListing->status << " "
							   << oneListing->zipCode << " "
							   << oneListing->realtyCompany
							   << endl;

					// get next listing
					oneListing = oneListing->next;
				}

				// close file
				outputFile.close();
			}
		}
		else if (yn == "N"){
			// set to true to stop yn while loop
			ynLoop = false;
		}
	} // end yn while loop
}

// Helper functions

//*********************************************************************
// FUNCTION: toUppercase
// DESCRIPTION: Converts a string to all uppercase.
// INPUT:
// Parameters: str - A reference to str that will be converted.
//**********************************************************************
void toUppercase(string &str){
	for (int i=0; i < str.length(); i++){
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
	cout << endl;
	cout << msg << endl;
	cin.ignore();
	cin.get();
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

	inputFile.open(inputFileName.c_str());

	return inputFile;
}
