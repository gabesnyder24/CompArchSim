// Programmer: Gabriel Snyder & Anton Lessmeister
// Student ID: gls8hz & ajlwhr
// Date: 11/29/22
// File: main.cpp
// Section: 101
// Purpose: To simulate the restoring and non-restoring methods of dividing
//          floating point division

#include <iostream>
#include <string>
#include <vector>
using namespace std;


const vector<string> TEST_DND = {"111100001",  "001001101",   "000010101",
								 "100001101",  "1111111000001",  "0010110110010",
								 "1000010111101",             "1100111100010",    
								 "01111111000000001",         "10001011110110101", 
								 "00011011111001000",         "00000101001001011",
								 "111111111100000000001",     "100000111000101101100", 
								 "100011100011000111001",     "000010101100000010000", 
								 "0111111111110000000000001", "1000111000110111000111001",  
								 "1000000111110001100011001", "1000011100001000100100000" };
const vector<string> TEST_DOR = {"01111",     "11011",    "10011",
                                 "01101",     "1111111",  "1011011",
                                 "1000111",   "1110111",  "011111111",
                                 "000110011", "001110111", "101010101",
								 "11111111111",   "10000111101",   
								 "00101010101",   "00001111000",
								 "0111111111111", "1010101010101", 
								 "1000011111111", "0000011100000"
								 };


// descr: convertString() function takes a string and turns it into a vector of
//		  ints to be used in binary format
// pre: str is not empty string
// post: returns a vector of ints
vector<int> convertString(const string &str);

// descr: isDivOverflow() function checks if divid overflow will occur by
// 		  looking at each bit in both divisor and first half of dividend
// pre: dnd is twice the size of dor and they contain binary representation
// post: returns boolean corresponding to whether divide overflow occurs
bool isDivOverflow(const vector<int> &dnd, const vector<int> &dor);

// descr: divideRestoring() function handles simulating the division according
// 		  to the resoring method
// pre: dnd is one less than twice the size of dor and are odd numbered sizes
// post: outputs the simulation to the screen
void divideRestoring(vector<int> &dnd,vector<int> &dor,vector<int> &dor_invert);
					  
// descr: divideNonRestoring() function handles simulating division according
// 		  to the non-resoring method
// pre: dnd is one less than twice the size of dor and are odd numbered sizes
// post: outputs the simulation results to the screen
void divideNonRestoring(vector<int> &dnd,vector<int> &dor,
						vector<int> &dor_invert);
						
// descr: printEAQ() function outputs the dividend to the screen
// pre: dnd has a size whose value is an odd number
// post: prints to the screen the whole dividend
void printEAQ(const vector<int> &dnd);

// descr: printRem() function outputs the remainder to the screen
// pre: num is a positive integer
// post: prints the first half of the dividend to the screen
void printRem(const vector<int> &dnd);

// descr: printQuot() function outputs the second half of the dividend to the 
//		  screen
// pre: dnd is a vector of ints whose size is odd
// post: prints the quotient to the screen
void printQuot(const vector<int> &dnd);

// descr: shl() function shifts each value in the vector of ints to the left 
// 		  starting at the last element in the vector
// pre: dnd is not empty
// post: returns dnd by reference with values shifted 1 position to the left
//		 with the rightmost empty position being represented by -1
void shl(vector<int> &dnd);

// descr: add() function handles the arithmetic operations of 2 vectors
// pre: dnd is a vector of ints: the same size or twice the size of B minus 1
// post: returns by reference the vector dnd resulting from addition operations
void add(vector<int> &dnd, const vector<int> &B);

// descr: operator<<() function overloads the << operator in order to ouput 
// 		  vectors of ints in a specific format
// pre: vect is a non-empty vector of integers
// post: outputs to the screen the vector of ints
std::ostream &operator<<(std::ostream &out, const vector<int> &vect);

int main() 
{
	vector<int> dnd;			// vector of ints representing the dividend
	vector<int> dor;			// vector of ints representing the divisor
    vector<int> dor_invert;     // holds 2's complement of the divisor
    vector<int> dndN; 		    // copy of dividend for NonRestoring function
    vector<int> dorN; 		    // copy of divisor for NonRestoring function
    vector<int> one;			// represents the value 1 in vector form

    // this loops through each test data set
    for (int i = 0; i < TEST_DND.size(); i++) 
    {
		// outputs message to screen indicating test data number
		cout << "\n\n\nTest Case " << i + 1 << ": ";
		// outputs the dividend and divisor **includes the sign bit
		cout << "\nDividend: " << TEST_DND[i] 
			 << "\t\tDivisor: "<< TEST_DOR[i] <<endl;

		// converts the string of divisor and dividend of the data set to a 
		// vector of ints
		dnd = convertString(TEST_DND[i]);
		dor = convertString(TEST_DOR[i]);

		// creates the 1's comp of B
		for (int j = 0; j < dor.size(); j++) 
		{
		  dor_invert.push_back((dor[j] ? 0 : 1));
		}

		// making empty vector of B size
		for (int i = 0; i < dor.size() - 1; i++) {
		  one.push_back(0);
		}
		// adding one to the back to represent the value 1
		one.push_back(1);
		// adds 1 to the 1's comp of divisor in order to get the 2's complement
		add(dor_invert, one);
		
		// creates copies of both the dividend and divisor to be used in the 
		// non-restoring method function because they will be modified in the 
		// restoring method function
		dndN = dnd;
		dorN = dor;

		// checks for divide overflow by checking if the left half of dnd is 
		// greater than or equal to dor, if so then divide overflow occurs and
		// error message appears
		if (!isDivOverflow(dnd,dor)) 
		{
			// calls the functions that handle restoring and non-restoring 
			// division
			divideRestoring(dnd, dor, dor_invert);
			divideNonRestoring(dndN, dorN, dor_invert);
		} 
		else
		{
			cout << "\nERROR: DIVIDE OVERFLOW OCCURRED!" << endl;
		}

		// clears all the data in the vectors used
		dnd.clear();
		dor.clear();
		dor_invert.clear();
		dndN.clear();
		one.clear();
	}
	return 0;
}

vector<int> convertString(const string &str) 
{
	vector<int> nums; // represents the register of binary nums
	
    // goes through each position in the string and stores the
	// integer value of the number into the register vector
	for (int i = 0; i < str.length(); i++) 
	{
		nums.push_back(static_cast<int>(str[i]) - 48);
	}
	
	return nums;
}

bool isDivOverflow(const vector<int> &dnd, const vector<int> &dor) 
{
	bool isDivOF = false;	  // boolean to store whether divide overflow occurs
    int count = 1;		      // counter for position in dividend and divisor
						      // starts at 1 to ignore the sign bit
	// increments position counter until there is a value difference or end of
	// divisor is reached
    while (count < dor.size()-1 && dnd.at(count) == dor.at(count)) 
    {
		count++;	// increments position
	}
	// checks if the bit value of Dividend at position count is greater than
	// divisor, if so there is divide overflow
    if (dnd.at(count) >= dor.at(count)) 
    {
		isDivOF = true;				// divide overflow occurs
	}
	
    return isDivOF;
}

void divideRestoring(vector<int> &dnd, vector<int> &dor,vector<int> &dor_invert) 
{
	int i = 0;					// stores the number of iterations
	int sign;					// holds the sign of the resulting quotient and
								// divisor
	int add_sub_counter = 0;	// stores number of additions and subtractions
	
	
	sign=(dnd[0] != dor[0] ? 1 : 0);	// XOR sign of dividend and divisor
	dnd.at(0)=0;						// initializes E bit to zero
	dor.at(0)=0;						// initializes divisor E bit to zero
	dor_invert.at(0)=1;					// E bit set to 1 in case of subtraction
	
	// ------------------------------OUTPUT TO SCREEN---------------------------
    cout << "_____________________RESTORING METHOD_____________________\n";
	printEAQ(dnd); 				//prints dividend to the screen
    cout << "shl" << endl; 		
	//--------------------------------------------------------------------------
	
	// repeats while number of iterations is less than size of divisor not
	// including the sign bit
    while (i < dor.size() - 1) 
    { 
		shl(dnd);				// shifts left the bits in the dividend
		printEAQ(dnd);			// prints new value of dividend to screen

		// subtracts dnd and dor by taking 2's comp of divisor and adding
		add(dnd, dor_invert);
		add_sub_counter++;			// increments number of add/subtractions		

		//-------------- OUTPUTS STEP TO SCREEN------------------------
		cout << "sub B\n[" << dor_invert.at(0) << " ";
		for (int j = 1; j < dor_invert.size(); j++) 
		{
			cout << dor_invert.at(j);
		}
		
		//------------------CODE FOR DISPLAY PURPOSES------------------------
		// this makes the output more neat so that the values line up better
		int num_spaces=dor.size();	   // holds value of number of spaces needed
		// loops until number of spaces needed is 0
		while(num_spaces)
		{
			cout<<" ";			// displays a space
			num_spaces--;		// decrements number of spaces needed
		}
		
		
		cout << "]\n-----------------------------" << endl;
		printEAQ(dnd);			// prints dividend to screen
		//------------------END OF CODE FOR DISPLAY PURPOSES--------------------
		
		// check E and restore if result E=1
		if (dnd.at(0) == 1) 
		{
			dnd.at(dnd.size() - 1) = 0; 	// q[0] set to 0
			add(dnd, dor);              	// restore A (remainder)
			add_sub_counter++;				// incremenet number of add/subtract
			
			//---------------------OUTPUT--------------------------------------
			cout << "E=1, Q0=0, Restore A" << endl;
			printEAQ(dnd);					// prints dividend to screen
			
			cout << (i==dor.size()-2?"":"shl\n");
			//this makes sure that the "shl" doesn't print on the last iteration
			//---------------------OUTPUT END-----------------------------------
			
		} 
		else 
		{
			dnd.at(dnd.size() - 1) = 1; 	// q[0] set to 1
			
			//---------------------OUTPUT--------------------------------------
			cout << "E=0, Q0=1" << endl;
			printEAQ(dnd);				// prints the dividend to the screen
			
			cout << (i==dor.size()-2?"":"shl") <<endl;
			//this makes sure that the "shl" doesn't print on the last iteration
			//---------------------OUTPUT END-----------------------------------
			
		}
		i++;			// increments the number of iterations
		
	}

	// outputs to the screen the resulting Remainder and quiotient with their 
	// resulting signs
	cout << "\n\nRemainder: " << sign;
	printRem(dnd);
    cout << "\nQuotient: " << sign;
    printQuot(dnd);
	
	// outputs to the screen the number of iterations and the number of add/sub
	// operations performed for the calculation
	cout << "\nNumber of Iterations: " << i << endl;
	cout << "Number of additions/subtractions: " << add_sub_counter << endl;
	cout << "__________________________________________________________"<< endl;
	return;
}

void divideNonRestoring(vector<int> &dnd, vector<int> &dor,
						vector<int> &dor_invert) 
{
	int i = 0;					// stores the number of iterations
	int needRestore=0;			// determines whether an add or subtract needed 
	int sign;					// holds the sign of the resulting quotient and
								// divisor
	int add_sub_counter = 0;	// stores number of additions and subtractions
	
	
	sign=(dnd[0] != dor[0] ? 1 : 0);	// XOR sign of dividend and divisor
	dnd.at(0)=0;						// initializes E bit to zero
	dor.at(0)=0;						// initializes divisor E bit to zero
	dor_invert.at(0)=1;					// E bit set to 1 in case of subtraction
	
	// ------------------------------OUTPUT TO SCREEN---------------------------
	cout << "__________________NON-RESTORING METHOD_____________________\n";
	printEAQ(dnd);
	cout << "shl" << endl;
	//--------------------------------------------------------------------------
	
	// repeats while number of iterations is less than size of divisor not
	// including the sign bit
	while (i < dor.size() - 1) 
	{
		shl(dnd);				// shifts left the bits in the dividend
		printEAQ(dnd);			// prints new value of dividend to screen
		
		// checks whether to add or subtract & if it is the first iteration
		if (needRestore && i > 0) 
		{	
			
			// adds dor to dnd in order to restore back to previous values
			// after an unsuccessful subtraction
		    add(dnd, dor);
			
		    add_sub_counter++;			// increment number of add/subtractions

			//-------------- OUTPUTS STEP TO SCREEN------------------------
		    cout << "add B\n[" << dor.at(0) << " ";
		    for (int j = 1; j < dor.size(); j++) 
			{
				cout << dor.at(j);
			}
			//------------------END OF OUTPUT-----------------------------
		} 
		else
		{
			// subtracts dnd and dor by taking 2's comp of divisor and adding
			add(dnd, dor_invert);
			
			add_sub_counter++;			// increment number of add/subtractions
			
			//-------------- OUTPUTS STEP TO SCREEN------------------------
			cout << "sub B\n[" << dor_invert.at(0) << " ";
			for (int j = 1; j < dor_invert.size(); j++) 
			{
				cout << dor_invert.at(j);
			}
			//------------------END OF OUTPUT-----------------------------
		}
		
		//------------------CODE FOR DISPLAY PURPOSES------------------------
		// this makes the output more neat so that the values line up better
		int num_spaces=dor.size();	   // holds value of number of spaces needed
		// loops until number of spaces needed is 0
		while(num_spaces)
		{
			cout<<" ";			// displays a space
			num_spaces--;		// decrements number of spaces needed
		}
		
		
		cout << "]\n-----------------------------" << endl;
		printEAQ(dnd);			// prints dividend to screen
		//------------------END OF CODE FOR DISPLAY PURPOSES--------------------
		
		// check E and store opposite of its value in first position of quotient
		if (dnd.at(0) == 1)
		{
			dnd.at(dnd.size() - 1) = 0; 		// q[0]=0
			//---------------------OUTPUT--------------------------------------
			cout << "E=1, Q0=0" << endl;
			printEAQ(dnd);					// prints dividend to screen
			
			cout << (i==dor.size()-2?"":"shl\n");
			//this makes sure that the "shl" doesn't print on the last iteration
			//---------------------OUTPUT END-----------------------------------
		} 
		else 
		{
			dnd.at(dnd.size() - 1) = 1; 		// q[0]=1
			//---------------------OUTPUT--------------------------------------
			cout << "E=0, Q0=1" << endl;
			printEAQ(dnd);					// prints dividend to screen
			
			cout << (i==dor.size()-2?"":"shl\n");
			//this makes sure that the "shl" doesn't print on the last iteration
			//---------------------OUTPUT END-----------------------------------
		}
		needRestore=dnd[0];		// holds value of E bit before next shift
								// which determines whether to add or subtract
		i++;					// increments number of iterations
	}
	
	// handles the case where the last iteration doesn't contain the correct
	// result for the quotient and remainder and requires a restore
	if (dnd.at(0) == 1) 		// checks E bit to signify need to restore
	{
		// adds dor to dnd in order to restore back to previous values
		// after an unsuccessful subtraction
		add(dnd, dor); 
		add_sub_counter++;		// increments number of add/subtractions
		
		
		//-------------- OUTPUTS STEP TO SCREEN------------------------
		cout << "add B\n[" << dor.at(0) << " ";
		for (int j = 1; j < dor.size(); j++) 
		{
			cout << dor.at(j);
		}
		//------------------END OF OUTPUT-----------------------------
		i++;				// increments number of iterations
	}
	//------------------CODE FOR DISPLAY PURPOSES------------------------
	// this makes the output more neat so that the values line up better
	int num_spaces=dor.size();	   // holds value of number of spaces needed
	// loops until number of spaces needed is 0
	while(num_spaces)
	{
		cout<<" ";			// displays a space
		num_spaces--;		// decrements number of spaces needed
	}
	//-------------------------------------------------------------------

	// outputs to the screen the resulting Remainder and quiotient with their 
	// resulting signs
	cout << "\n\nRemainder: " << sign;
	printRem(dnd);
	cout << "\nQuotient: " << sign;
	printQuot(dnd);

	// outputs to the screen the number of iterations and the number of add/sub
	// operations performed for the calculation
	cout << "\nNumber of Iterations: " << i<< endl;
	cout << "Number of additions/subtractions: " << add_sub_counter << endl;
	cout << "__________________________________________________________"<< endl;
	
	return;
}

void printEAQ(const vector<int> &dnd) 
{
	// this formats the dnd vector to represent the dividend in the simulator
	
	cout << "[" << dnd.at(0) << " ";
	
	// this prints out the A part or the remainder part
	for (int i = 1; i < (dnd.size() + 1) / 2; i++) 
	{
		cout << dnd.at(i);
	}
	cout << " ";
	// this prints out the Q or quotient part of the dividend
	for (int i = (dnd.size() + 1) / 2; i < dnd.size()-1; i++) 
	{
		cout << dnd.at(i);
	}
	// at the end of the dividend it checks for -1 which is an empty data point
	if(dnd.at(dnd.size()-1)==-1)
	{
		cout<<"?";				// prints '?' in place of empty data point
	}
	else
	{
		cout<<dnd.at(dnd.size()-1)		// prints data value in last position
	}
	cout << "]";
	return;
}

void printRem(const vector<int> &dnd) 
{
	// this iterates through the dnd vector and prints only the remainder part
	for (int i = 1; i < (dnd.size() + 1) / 2; i++) 
	{
		cout << dnd.at(i);
	}
	return;
}

void printQuot(const vector<int> &dnd) 
{
	// this loops through the vector dnd and prints out the quotient part
	for (int i = (dnd.size() + 1) / 2; i < dnd.size(); i++) 
	{
		cout << dnd.at(i);
	}
	return;
}

void shl(vector<int> &dnd) 
{
	// loops through the vector dnd and moves the data from right to left
	// the last element in the vector shifts left and the last element becomes
	// -1 for the purpose of indicating a missing data point
	for (int i = 0; i < dnd.size() - 1; i++) 
	{
		dnd.at(i) = dnd.at(i + 1);		
	}
	dnd[dnd.size() - 1] = -1;			// last position is set to -1 
    return;
}

void add(vector<int> &dnd, const vector<int> &B) 
{
	int c = 0;			// holds value of the carry bit
    int start;			// holds starting location of add operation
	
	start=(dnd.size() != B.size() ? ((dnd.size() - 1) / 2) : (dnd.size() - 1));
	// this ternary operator does the following:
	// if: the first operand is not the same size as the second operand, the
	// starting point is assumed to be the size divided by 2 of the dividend,
	// which means we start halfway into the dividend in order to perform add
	// on the left half.
	// else: assume they are the same size which accounts for when we add the
	// value 1 to the 1's complement of the divisor to get dor_invert

	// loop starting at rightmost position and carries out arithmetic operations
	for (int i = start; i >= 0; i--) 
	{
		// checks each case for adding binary numbers
		if (dnd.at(i) == 0 && B.at(i) == 0 && c == 1) 
		{
			dnd.at(i) = 1;		// 0+0+1=1
			c = 0;				// clear carry bit
		} 
		else if (dnd.at(i) == 0 && B.at(i) == 0 && c == 0) 
		{
			dnd.at(i) = 0;		// 0+0+0=0
		} 
		else if (dnd.at(i) == 1 && B.at(i) == 0 && c == 1) 
		{
			dnd.at(i) = 0;		// 1+0+1=0  with carry bit set
			c = 1;				// sets the carry bit
		} 
		else if (dnd.at(i) == 1 && B.at(i) == 0 && c == 0) 
		{
			dnd.at(i) = 1;		// 1+0+0=1
		} 
		else if (dnd.at(i) == 0 && B.at(i) == 1 && c == 1) 
		{
			dnd.at(i) = 0;		// 0+1+1=0 with carry bit set
			c = 1;				// carry bit set
		} 
		else if (dnd.at(i) == 0 && B.at(i) == 1 && c == 0) 
		{
			dnd.at(i) = 1;		// 0+1+0=1
		}
		else if (dnd.at(i) == 1 && B.at(i) == 1 && c == 1) 
		{
			dnd.at(i) = 1;		// 1+1+1=1 with carry bit set
			c = 1;				// carry bit is set
		} 
		else if (dnd.at(i) == 1 && B.at(i) == 1 && c == 0) 
		{
			dnd.at(i) = 0;		// 1+1+0=0
			c = 1;				// carry bit is set
		}
	}
    return;
}

std::ostream &operator<<(std::ostream &out, const vector<int> &vect) 
{
	// loops through the vector and outputs each value with no spaces in between
	for (int i = 0; i < vect.size(); i++) 
    {
    	out << vect.at(i);
	}
	return out;
}