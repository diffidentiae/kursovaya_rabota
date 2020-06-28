#pragma once
#include <iostream>
#include <chrono>
#include <ctime>
#include <string>
#include <algorithm>
#include <time.h>
#include <stdlib.h>
#include <fstream>


using namespace std;

class unit_t
{
private:
	string name; // title
	string mail; // mail
	string pass; // password for related mail
	string desc; // notes
	string link; // link to
	//bool oldPass;

public:
	//Empty constructor that samples each string with "<empty>"
	unit_t();
	//Initializes class with incoming strings
	unit_t(string nameT, string mailT, string passT, string descT, string linkT);
	//Copy constructor
	unit_t(unit_t const& unitTemp);
	//Destructor of class
	~unit_t();
	//Redefines operation = to proper class copying
	unit_t& operator =(unit_t const& unitTemp);

	//Returnes name string of class cannot be modified
	string name_() const;
	//Returnes mail string of class cannot be modified
	string mail_() const;
	//Returnes pass string of class can be modified
	string pass_();
	//Returnes pass string of class cannot be modified
	string pass_() const;
	//Returnes desc string of class cannot be modified
	string desc_() const;
	//Returnes link string of class cannot be modified
	string link_() const;
	//questionable variable, suppose to reflect aging of password and signal when u should update one
	//bool oldPass_() const;
	//Generates new password with selected options.
	//It defines length of password
	void generatePassword(int passLength, bool noSymbols);
	//Outputs all data
	ostream& unitOutput(ostream& stream) const;
//	ostream& unitOutputToFile(ostream& stream) const;
};