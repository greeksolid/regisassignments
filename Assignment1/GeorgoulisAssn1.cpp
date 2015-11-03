//*************************************************************************************************************
// CODE FILENAME: GeorgoulisAssn1.cpp
// DESCRIPTION: This program will read data collected by a county Department of Motor Vehicles about car
//				and motorcycle registrations that are stored in a file. Each line of the file holds information
//				about a household's vehicle/s (type, license plate, year, MSRP).
//				The program reads these data, calculates taxes on the fly and outputs the results :
//
//					a) In a file(VEHICLES.txt) all the vehicles(model and license plate)
//					   and their corresponding tax.
//					b) In a file(HOUSEHOLDS.txt) each household and the sum of taxes of each household's
//					   vehicle.
//					c) On the screen, three headers(Household ID Number, Vehicles Registered, Total Charges)
//					   and under of each of them the corresponding household id, number of registered vehicles,
//					   and the total charges of each household.
//					   Finally it presents the total county collected.
//
// CLASS/TERM: CS362 - 13F8W2
// DESIGNER: Anastasios Georgoulis
// FUNCTIONS:
// 			displayDescription : Shows a brief description of
// 			what the program does to the user.
//
// 			displayHeaders : Displays headers(Household ID Number, Vehicles Registered, Total Charges)
//			to the screen.
//
//			displayData: Displays computed data to the screen in columns
//			aligned with the headers.
//
//			displayGrandTotal: Displays the total county collected in its proper column.
//
//			openFiles: opens all the files.
//
//			closeFiles: Closes all the files.
//
//			setFormatting: Sets the formatting of all the ofstream variables / objects.
//
//			calculateVehicleTax: Calculates the tax for each vehicle.
//
//*************************************************************************************************************

// Libraries
#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>

// Namespaces
using namespace std;


//*************************
//   Function Prototypes
//*************************

// Program Functions
void displayDescription();
void displayHeaders();
void displayData(string &householdId, int &numberOfVehicles, double &totalRegistrationCharges, int &linesDisplayed);
void displayGrandTotal(double &totalCounty);
double calculateVehicleTax(char &vehicleType, const int &CURRENT_YEAR, int &year, double &msrp);

//Helper Functions
void openFiles(ifstream &inputFile, ofstream &outFile1, ofstream &outFile2);
void closeFiles(ifstream &inputFile, ofstream &outFile1, ofstream &outFile2);
void setFormatting(ostream &cout, ostream &outFile1, ostream &outFile2);

int main(){
	//*********************************************************************
	// FUNCTION: main
	// DESCRIPTION: Initializes the variables and calls some helper functions.
	// It then enters a loop and reads the input file line by line, processes data by calling the
	// function calculateVehicleTax and outputs the results in two files (VEHICLES.txt and HOUSEHOLDS.txt)
	// and to the screen. It also keeps track of various helper variables.
	// INPUT:
	// File: Reads following info from each line of GeorgoulisRegister.txt:
	// household ID, vehicle type, license plate, model year, msrp
	// Data on each line is formatted as follows:
	// 	11111 A ABC123 2012 36666
	// Each household may hold multiple vehicles
	// OUTPUT:
	// Return Val: 0 -> If the program terminated without any problems 1 if it didn't.
	// File:
	//		Stores each vehicle's data(vehicle type, license plate, vehicle tax) to each line
	// of the file VEHICLES.txt
	// Data on each line is formatted as follows:
	// 	A ABC123 10050.00
	// Stores each household's data(id, total registration tax) to each line of the files HOUSEHOLDS.txt
	// Data on each line is formatted as follows:
	// 	111111  10050.00
	// CALLS TO: displayDescription, displayData, displayGrandTotal,
	//           openFiles, closeFiles, setFormatting, calculateVehicleTax
	//**********************************************************************

	// CONSTANT VARIABLES DECLARATION
	const int CURRENT_YEAR = 2013; // current year used to find the age of the vehicles

	// VARIABLES DECLARATION
	string householdId,
		   licensePlate;
	int modelYear; // vehicle's production year
	double msrp;   // vehicle's original taxable value
	char vehicleType;

	double vehicleTotalTax = 0; 			// computed tax of each vehicle
	double totalRegistrationCharges = 0;	// total charges for each household
	double totalCounty = 0; 				// the sum of all charges of every household

	// Helper Variables
	char ch; 				  // used to find the new lines of the input file
	int numberOfVehicles = 0; // counter to store the number of vehicles
	int linesDisplayed = 0;   // counter to keep track of how many lines appear on screen

	// Input/Output Stream variables
	ifstream inputFile; // input file
	ofstream outFile1, outFile2; // output files

	// shows a brief program description to the user
	displayDescription();

	// opens all the files needed for the program to run
	openFiles(inputFile, outFile1, outFile2);

	// Checks for any problem with the input file
	if ( !inputFile ) {
		cout << endl;
		cout << "Could not open the input file REGISTER.txt" << endl;
		cin.get();

		return -1;
	}

	//Sets the formatting of all the ofstream variables / objects.
	setFormatting(cout, outFile1, outFile2);

	// priming read / read first household ID
	inputFile >> householdId;

	while ( inputFile ) { // while there are more characters

		do {
			// read each vehicle's data
			inputFile >> vehicleType >> licensePlate >> modelYear >> msrp;

			// calculate and store each vehicle's tax
			vehicleTotalTax = calculateVehicleTax(vehicleType, CURRENT_YEAR, modelYear, msrp);

			// writes data to VEHICLES.txt
			outFile1 << vehicleType << setw(10) << licensePlate <<
			setw(10) << vehicleTotalTax << endl;

			// vehicles counter
			numberOfVehicles++;

			// add vehicle's tax to household total tax
			totalRegistrationCharges += vehicleTotalTax;

			// get next character
			ch = inputFile.peek();

		} while ( inputFile && (ch != '\n') ); // check for end of line

		// add each household's vehicle charges to the total county charges
		totalCounty += totalRegistrationCharges;

		// writes data to HOUSEHOLDS.txt
		outFile2 << householdId << setw(10) << totalRegistrationCharges << endl;

		// Displays computed data to the screen
		displayData(householdId, numberOfVehicles, totalRegistrationCharges, linesDisplayed);

		// Reset Counters
		numberOfVehicles = 0;
		totalRegistrationCharges = 0;

		inputFile >> householdId;
	} // end of while

	// closes all open files
	closeFiles(inputFile, outFile1, outFile2);

	// display the total county collected
	displayGrandTotal(totalCounty);

	//exit without errors
	return 0;
}

//*****************************
//   Function Implementations
//*****************************

void displayDescription(){
	//*********************************************************************
	// FUNCTION: displayDescription
	// DESCRIPTION: This function will present to the user a short description.
	// of what the program will do.
	//**********************************************************************
	cout << "The program will read data from an input file(REGISTER.txt) that holds information"
			" about a county's Department of Motor Vehicle." << endl;
	cout << "It will then process these data"
			" and create two output files VEHICLE.txt and HOUSEHOLDS.txt" << endl;
	cout << endl;

	cout << "VEHICLE.txt stores information about each registered vehicle of the county"
			" along with each vehicle's computed tax." << endl;
	cout << endl;
	cout << "HOUSEHOLDS.txt stores information about every household along with the computed tax "
			" for all the vehicles of each household." << endl;
	cout << endl;
	cout << "It will display to the user each household's ID, number of vehicles registered to"
			" every household and the total charges of each household." << endl;
	cout << "It will finally show the total county collected i.e. the sum of all the charges"
			" of all the households" << endl;
	cout << endl;
}

void displayHeaders(){
	//*********************************************************************
	// FUNCTION: displayHeaders
	// DESCRIPTION: Displays computed data to the screen in columns aligned with the headers.
	//**********************************************************************
	cout << endl;

	cout << setw(15) << left << "Household" << setw(15) << left << "Vehicles"
		 << setw(15) << left << "Total" << endl;

	cout << setw(15) << left << "ID Number" << setw(15) << left << "Registered"
		 << setw(15) << left << "Charges" << endl;

	cout << setw(15) << left << "----------" << setw(15) << left << "----------"
		 << setw(15) << left << "-----------" << endl;
}

void displayData(string &householdId, int &numberOfVehicles, double &totalRegistrationCharges, int &linesDisplayed){
	//*********************************************************************
	// FUNCTION: displayData
	// DESCRIPTION: Displays the computed data to the screen, under the corresponding headers.
	// INPUT:
	// Parameters: householdId - Reference to a string variable that holds the value of the household ID
	//			   numberOfVehicles - Reference to an int variable counter that keeps track of the
	//			   					  number of vehicles
	//             totalRegistrationCharges - Reference to a double variable with the total
	//			   							  charges of each household
	//			   linesDisplayed - Reference to an int variable counter that keeps track
	//								of how many lines are displayed
	// CALLS TO: displayHeaders
	//**********************************************************************

	// if screen is full
	if ( linesDisplayed >= 20 ){
		cout << "Press any key to continue...";
		cin.get(); // wait for input
		cout << string( 100, '\n' ); // prints 100 new lines to "clear" screen

		displayHeaders(); // Display headers
		linesDisplayed = 0; // reset counter
	}
	// shows initial headers
	else if ( linesDisplayed == 0 ){
		displayHeaders();
	}

	cout << setw(10) << right << householdId;
	cout << setw(10) << right << numberOfVehicles;
	cout << setw(11) << right << "$" << setw(10) << totalRegistrationCharges;
	cout << endl;

	linesDisplayed++;
}

void displayGrandTotal(double &totalCounty){
	//*********************************************************************
	// FUNCTION: displayGrandTotal
	// DESCRIPTION: Displays to the screen the total county collected
	// INPUT:
	// Parameters: totalCounty - Reference to a double variable that stores the total county collected
	//**********************************************************************
	cout << setw(42) << right << "-------------" << endl;
	cout << setw(30) << left << "Total County Collected" << setw(4) << "$"
		 << totalCounty << endl;
}

void openFiles(ifstream &inputFile, ofstream &outFile1, ofstream &outFile2){
	//*********************************************************************
	// FUNCTION: openFiles
	// DESCRIPTION: Opens all the stream files
	// INPUT:
	// Parameters: inputFile - Reference to the input file(REGISTER.txt)
	// 			   outFile1 - Reference to the first output file(VEHICLES.txt)
	//			   outFile2 - Reference to the second output file(HOUSEHOLDS.txt)
	//**********************************************************************
	inputFile.open("GeorgoulisREGISTER.txt");
	outFile1.open("GeorgoulisVEHICLES.txt");
	outFile2.open("GeorgoulisHOUSEHOLDS.txt");
}

void closeFiles(ifstream &inputFile, ofstream &outFile1, ofstream &outFile2){
	//*********************************************************************
	// FUNCTION: closeFiles
	// DESCRIPTION: Close all open stream files
	// INPUT:
	// Parameters: inputFile - Reference to the input file(REGISTER.txt)
	// 			   outFile1 - Reference to the first output file(VEHICLES.txt)
	//			   outFile2 - Reference to the second output file(HOUSEHOLDS.txt)
	//**********************************************************************
	inputFile.close();
	outFile1.close();
	outFile2.close();
}

void setFormatting(ostream &cout, ostream &outFile1, ostream &outFile2){
	//*********************************************************************
	// FUNCTION: setFormatting
	// DESCRIPTION: Sets desired formatting for the provided output streams
	// i.e. floating-point values are written using fixed-point notation,
	// always showing the decimal point and with a decimal precision of 2.
	// INPUT:
	// Parameters: cout - Reference to the screen output
	// 			   outFile1 - Reference to the first output file(VEHICLES.txt)
	//			   outFile2 - Reference to the second output file(HOUSEHOLDS.txt)
	//**********************************************************************
	cout << fixed << showpoint << setprecision(2);
	outFile1 << fixed << showpoint << setprecision(2);
	outFile2 << fixed << showpoint << setprecision(2);
}

double calculateVehicleTax(char &vehicleType, const int &CURRENT_YEAR, int &year, double &msrp){
	//*********************************************************************
	// FUNCTION: calculateVehicleTax
	// DESCRIPTION: Calculates the registration tax of each vehicle.
	// INPUT:
	// Parameters: vehicleType - A reference to a character variable that holds the vehicle type.
	//			   CURRENT_YEAR - A reference constant variable that holds the current year.
	//             year - A reference to an int variable that holds each vehicle's year of production.
	//		       msrp - A reference to a double variable that holds the vehicle's original taxable value.
	// OUTPUT:
	// Return Val: The computed taxable value of each vehicle.
	//**********************************************************************
	int age = CURRENT_YEAR - year; 			// the age of the vehicle
	int roadFee;							// road fee depending of the type of vehicle
	double licenseFee = msrp * 0.25 / 100;	// license fee of each vehicle
	double ownershipTax;					// ownership tax of each vehicle


	if ( vehicleType == 'A' ){   // automobile

		roadFee = 24;

		if ( age <= 5 ){
			ownershipTax = msrp * 3 / 100 - ( age * msrp * 0.15 / 100 );
		}
		else if ( ( age > 5 ) && ( age <= 10) ){
			ownershipTax = msrp * 2.75 / 100 -  ( age * msrp * 0.12 / 100 );
		}
		else if ( ( age > 10 ) && ( age <= 15) ){
			ownershipTax = msrp * 2.5 / 100 -  ( age * msrp * 0.09 / 100  );
		}
		else{
			ownershipTax = 20;
		}
	}
	else if ( vehicleType == 'M' ){ // motorcycle
		roadFee = 12;

		if ( age <= 5 ){
			ownershipTax = msrp * ( 2.15 / 100 ) - ( age * msrp * 0.09 / 100 );
		}
		else if ( ( age > 5 ) && ( age <= 10) ){
			ownershipTax = msrp * (2.25 / 100) -  ( age * msrp * 0.09 / 100 );
		}
		else if ( ( age > 10 ) && ( age <= 15 ) ){
			ownershipTax = msrp * ( 2.0 / 100 ) -  ( age * msrp * 0.09 / 100 );
		}
		else{
			ownershipTax = 10;
		}
	}

	return roadFee + licenseFee + ownershipTax; // the total tax of the vehicle
}

