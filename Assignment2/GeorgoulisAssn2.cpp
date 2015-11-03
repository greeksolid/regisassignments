//*************************************************************************************************************
// CODE FILENAME: GeorgoulisAssn1.cpp
// DESCRIPTION:
// This program will read data collected by a county Department
// of Motor Vehicles about car
// and motorcycle registrations charges that are stored in a file.
// Each line of the file holds information
// about a vehicle (type, license plate, registration charge).
// The program reads these data line by line and stores the results
// into two arrays (autoCharges and motoCharges).
// It then calculates the low, high, and average charge for each vehicle type
// and stores these statistics in another two arrays (autoStats and motoStats).
// It then formats and displays these statistics to the screen.
//
// CLASS/TERM: CS362 - 13F8W2
// DESIGNER: Anastasios Georgoulis
// FUNCTIONS:
//
// 			calculateLow : Calculates and returns the lowest charge
//                         from a list of registration charges.
//
//			calculateHigh: Calculates and returns the highest charge
// 						   from a list of registration charges.
//
//			calculateAverage: Calculates and returns the average charge for
//							  from a list of registration charges.
//
//			storeAutoCharges: Stores automobile registration charges
//							  to the provided array.
//
//			storeMotoCharges: Stores motorcycle registration charges
//							  to the provided array.
//
//			storeStats: Stores stats(low, high, average) to the provided array.
//
//			displayDescription : Shows a brief description of
// 								 what the program does to the user.
//
//			displayResults: Display stats to the user.
//
//			setFormatting: Sets the formatting of the console output.
//
//			pause: Pauses the program and waits for input from the user.
//*************************************************************************************************************


// Libraries
#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>

// Namespaces
using namespace std;

// Enumerations
enum resultsType {LOW, HIGH, AVERAGE}; // used for array indexing

// Function Prototypes
double calculateLow(const double charges[], const int counter);
double calculateHigh(const double charges[], const int counter);
double calculateAverage(const double charges[], const int counter);

void storeAutoCharges(int &autoChargesCounter, const int MAX_RECORDS,
					 double autoCharges[], const double registrationCharge,
					 bool &autoFullMsgShown);
void storeMotoCharges(int &motoChargesCounter, const int MAX_RECORDS,
				     double motoCharges[], const double registrationCharge,
				     bool &motoFullMsgShown);
void storeStats(double stats[], const double charges[],
		        const int counter);
void displayDescription();
void displayResults(const double stats[], const int counter,
		  	  	    const char vehicleType);

void setFormatting(ostream &cout);
void pause(const string msg="Press 'ENTER' to continue.");

//*********************************************************************
// FUNCTION: main
// DESCRIPTION: Initializes the variables and calls some helper functions.
// It displays a short description to the user and opens an input file
// If it can't it terminates the program with an error.
// If the input file exists and can be opened main enters a loop  and
// reads the input file line by line, reads and stores the data to variables
// and calls the functions storeAutoCharge and storeMotoCharge to store the
// registration charges to two arrays depending on the vehicle type.
// It then closes the input file, sets the formatting of the console output
// and calls another function (storeStats) to calculate and store various
// stats to two other arrays.
// It then calls another function displayResults to display these stats
// to the user.
// INPUT:
// File: Reads following info from each line of VEHICLES.txt:
// vehicle type, license plate, registration tax
// Data on each line is formatted as follows:
// 	A    ABC123   1674.00
// OUTPUT:
// Return Val: 0 -> If the program terminated without any problems.
//			   1 -> If it there were problems.
// CALLS TO: calculateLow, calculateHigh, calculateAverage,
//           storeAutoCharges, storeMotoCharges, storeStats,
//			 displayDescription, displayResults, setFormatting, pause
//**********************************************************************
int main(){
	// CONSTANT VARIABLES DECLARATION

	// The name of the input file.
	const string INPUT_FILE_NAME = "GeorgoulisVEHICLES.txt";
	// the size of the two arrays that will hold the registration charges
	const int MAX_RECORDS = 500;
	// the size of the two arrays that will hold the county stats(low,high,avg)
	const int STATS_SIZE = 3;

	// VARIABLES DECLARATION

	// input file
	ifstream inputFile;

	// two arrays that will hold the registration charges.
	// All their fields are initialized to 0.
	double autoCharges[MAX_RECORDS] = {0};
	double motoCharges[MAX_RECORDS] = {0};

	// Counter for the above two arrays. Used for array bounds checking.
	int autoChargesCounter = 0;
	int motoChargesCounter = 0;

	// booleans to store if the error message of each vehicle type
	// has already been shown to the user
	bool autoFullMsgShown = false;
	bool motoFullMsgShown = false;

	// two arrays that will hold the stats(low,high,average)
	// of the vehicles of the county.
	double autoStats[STATS_SIZE] = {0};
	double motoStats[STATS_SIZE] = {0};

	char vehicleType;    		// automobile or motorcycle
	string licensePlate; 		// to store useless data
	double registrationCharge;  // registration charge of each vehicle
								// found in the input .txt file

	// Display a short description
	displayDescription();

	// open the input text file
	inputFile.open(INPUT_FILE_NAME.c_str());

	// issue an error message if input file can't be opened
	// and wait for the user to press enter
	if (!inputFile){
		cout << "Could not open the input file " << INPUT_FILE_NAME << endl;
		pause("Press 'ENTER' to terminate the program.");

		return 1;  // terminate program with error
	}


	//priming read - read first line
	inputFile >> vehicleType >> licensePlate >> registrationCharge;

	while (inputFile){ // while not end of file(eof)

 		if (vehicleType == 'A'){ // if automobile
 			// store data into first array.
 			// If full, display message and ignore the following data.
 			storeAutoCharges(autoChargesCounter, MAX_RECORDS,
 					        autoCharges, registrationCharge,autoFullMsgShown);
 		}
 		else if (vehicleType == 'M'){ // if motorcycle
 			// store data into second array
 			// If full, display message and ignore the following data.
 			storeMotoCharges(motoChargesCounter, MAX_RECORDS,
 			 				motoCharges, registrationCharge,
 			 				motoFullMsgShown);
 		}

		// read next line
		inputFile >> vehicleType >> licensePlate >> registrationCharge;
	}

	// close input file
	inputFile.close();

	// console output formatting
	setFormatting(cout);

	// Calculate and store automobile stats(low, high, average)
	storeStats(autoStats, autoCharges, autoChargesCounter);

	// Calculate and store motorcycle stats(low, high, average)
	storeStats(motoStats, motoCharges, motoChargesCounter);

	// Display automobile results
	displayResults(autoStats, autoChargesCounter, 'A');
	//Display motorcycle results
	displayResults(motoStats, motoChargesCounter, 'M');

	pause("Press 'ENTER' to terminate the program.");

	return 0;  // exit without an error
}

//*****************************
//   Functions Implementations
//*****************************

//*********************************************************************
// FUNCTION: calculateLow
// DESCRIPTION: Calculates and returns the lowest charge
//              from a list of registration charges.
// INPUT:
// Parameters: charges[] - Reference to an array that holds vehicle charges
//			   counter - Reference to a counter that keeps track of how many
// 						 data the charges[] array has.
//**********************************************************************
double calculateLow(const double charges[], const int counter){
	// set first element as the lowest one
	double low = charges[0];

	for (int i=0; i < counter; i++){ // for every element of the array

		if (charges[i] < low){ // if the current element is less than low
			low = charges[i];  // update low with current element
		}

	}

	return low;
}

//*********************************************************************
// FUNCTION: calculateHigh
// DESCRIPTION: Calculates and returns the highest charge
//              from a list of registration charges.
// INPUT:
// Parameters: charges[] - Reference to an array that holds vehicle charges
//			   counter - Reference to a counter that keeps track of how many
// 						 data the charges[] array has.
//**********************************************************************
double calculateHigh(const double charges[], const int counter){
	// set first element as the highest one
	double high = charges[0];

	for (int i=0; i < counter; i++){ // for every element of the array

		if (charges[i] > high){ // if the current element is bigger than high
			high = charges[i];  // update high with current element
		}

	}

	return high;
}

//*********************************************************************
// FUNCTION: calculateAverage
// DESCRIPTION: Calculates and returns the average charge
//              from a list of registration charges.
// INPUT:
// Parameters: charges[] - Reference to an array that holds vehicle charges
//			   counter - Reference to a counter that keeps track of how many
// 						 data the charges[] array has.
//**********************************************************************
double calculateAverage(const double charges[], const int counter){
	double sum = 0;

	for (int i=0; i < counter; i++){ // for every element of the array
		sum += charges[i]; 			 // add charges to sum
	}

	// divide sum with array length to get the average and return it.
	return sum / counter;
}

//*********************************************************************
// FUNCTION: storeAutoCharges
// DESCRIPTION: Stores automobile registration charges to the provided array.
// INPUT:
// Parameters: autoChargesCounter - A reference to a counter that keeps track
//			   of how many data the autoCharges[] has.
//			   MAX_RECORDS - A reference to a constant variable
//			   that holds the maximum amount of records for the autoCharges
//             array.
//             autoCharges[] - A reference to an array that holds all
//			   registration charges for all automobiles.
//		       registrationCharge - A reference to a double variable that
// 			   holds the current registration charge.
//			   autoFullMsgShown: A reference to a boolean variable that is
//             used to check if an error message has been shown.
//**********************************************************************
void storeAutoCharges(int &autoChargesCounter, const int MAX_RECORDS,
				      double autoCharges[], const double registrationCharge,
				      bool &autoFullMsgShown)
{
//	cout << "auto charges counter : " << autoChargesCounter << endl;
	if (autoChargesCounter < MAX_RECORDS){ // if there is empty array space
		autoCharges[autoChargesCounter] = registrationCharge; // store charges
		autoChargesCounter++; // increment counter
	}
	else{ // automobiles array is full
		// issue a warning(if it hasn't be shown already) and ignore the data.
		if (!autoFullMsgShown){
			cout << "Can't store more registration charges "
					"for automobiles." << endl;
			cout << "Future entries will be ignored." << endl;

			// change flag to show that the motorcycle message
			// was shown to the user.
			autoFullMsgShown = true;

			// wait for the user to press enter
			pause();
		}
	}
}

//*********************************************************************
// FUNCTION: storeMotoCharges
// DESCRIPTION: Stores motorcycle registration charges to the provided array.
// INPUT:
// Parameters: motoChargesCounter - A reference to a counter that keeps track
//			   of how many data the motoCharges[] has.
//			   MAX_RECORDS - A reference to a constant variable
//			   that holds the maximum amount of records for the motoCharges
//             array.
//             motoCharges[] - A reference to an array that holds all
//			   registration charges for all motorcycles.
//		       registrationCharge - A reference to a double variable that
// 			   holds the current registration charge.
//			   motoFullMsgShown: A reference to a boolean variable that is
//             used to check if an error message has been shown.
//**********************************************************************
void storeMotoCharges(int &motoChargesCounter, const int MAX_RECORDS,
					  double motoCharges[], const double registrationCharge,
					  bool &motoFullMsgShown)
{
	if (motoChargesCounter < MAX_RECORDS){ // if there is empty array space
		motoCharges[motoChargesCounter] = registrationCharge; // store charges
		motoChargesCounter++; // increment counter
	}
	else{ // motorcycles array is full
		// issue a warning(if it hasn't be shown already) and ignore the data.
		if (!motoFullMsgShown){
			cout << "Can't store more registration charges "
					"for motorcycles." << endl;
			cout << "Future entries will be ignored." << endl;

			// change flag to show that the motorcycle message
			// was shown to the user.
			motoFullMsgShown = true;

			// wait for the user to press enter
			pause();
		}
	}
}

//*********************************************************************
// FUNCTION: storeStats
// DESCRIPTION: Stores stats(low, high, average) to the provided array
// INPUT:
// Parameters:
//             stats[] - A reference to an array that holds all
//			   the statistics for a vehicle type.
//             charges[] - A reference to an array that holds all
//			   registration charges for a vehicle type.
//			   counter - A variable that holds how many data there are in the
// 			   provided array.
//**********************************************************************
void storeStats(double stats[], const double charges[], const int counter){
	// store stats(low, high, average) in the provided array
	stats[LOW] = calculateLow(charges, counter);
	stats[HIGH] = calculateHigh(charges, counter);
	stats[AVERAGE] = calculateAverage(charges, counter);
}

//**********************************************************************
// FUNCTION: displayDescription
// DESCRIPTION: This function will present to the user a short description.
// of what the program will do.
//**********************************************************************
void displayDescription(){
	cout << "This program will read data from the file VEHICLES.txt." << endl;
	cout << "As it reads the data it will store them in two "
			"separate arrays." << endl;
	cout << "The first array will hold the charges of all the "
			"automobiles of " << endl;
	cout << "the county and the second one will hold the charges "
			"of all the " << endl;
	cout << "motorcycles of the county. It will then calculate the "
			"low, high " << endl;
	cout << "and average charges of each vehicle type and "
			"display the results " << endl;
	cout << "to the screen." << endl;
	cout << endl;
}

//*********************************************************************
// FUNCTION: displayResults
// DESCRIPTION: Display stats(low,high,average) to the user.
// INPUT:
// Parameters:
//             stats[] - A reference to an array that holds all
//			   the statistics for a vehicle type.
//             const - A variable that has stored the amount of vehicles
// 			   stored in the provided array.
// 			   vehicleType - A char variable that holds the vehicle type.
//**********************************************************************
void displayResults(const double stats[], const int counter,
		            const char vehicleType)
{
	if (vehicleType == 'A'){ // if automobile

		if (counter != 0){ // more than one registration(s)
			cout << "For " << counter
				 << " automobile registrations in the county: "
				 << endl;
			cout << right << setw(35) << "Lowest registration charge = "
				 << "$" << setw(8) << stats[LOW] << endl;
			cout << right << setw(35) << "Highest registration charge = "
				 << "$" << setw(8) << stats[HIGH] << endl;
			cout << right << setw(35) << "Average registration charge = "
				 << "$" << setw(8) << stats[AVERAGE] << endl;
		}
		else{ // no registrations
			cout << "There were no registration charges for any "
				 << "automobiles in the county." << endl;
		}

	}
	else if (vehicleType == 'M'){ // if motorcycle

		if (counter != 0){ // more than one registration(s)
			cout << "For " << counter
				 << " motorcycle registrations in the county: "
				 << endl;
			cout << right << setw(35) << "Lowest registration charge = "
				 << "$" << setw(8) << stats[LOW] << endl;
			cout << right << setw(35) << "Highest registration charge = "
				 << "$" << setw(8) << stats[HIGH] << endl;
			cout << right << setw(35) << "Average registration charge = "
				 << "$" << setw(8) << stats[AVERAGE] << endl;
		}
		else{ // no registrations
			cout << "There were no registration charges for any "
				 << "motorcycles in the county." << endl;
		}

	}
	cout << endl;
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

