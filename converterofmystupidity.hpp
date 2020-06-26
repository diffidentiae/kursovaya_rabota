#pragma once
#include <iostream>
#include <string>
#include <map>
#include <utility>

using namespace std;
//This class solves 2 problems, first of all it makes string length even
//Secondly it redefines all characters of data to be able to work with key alphabet
//Can work with all ASCII simbols from 32 to 127 numbers in dec
class ConverterOtT
{
	string original;
	string converted;
	bool convertedQ = false;
	const map <char, string> replacements = { {32,"00"},{33,"01"},{34,"02"},{35, "03"} ,{36, "04"} ,{37, "05"}
		, { 38, "06" }, { 39, "07" }, { 40, "09" }, { 41, "10" }, { 42, "11" }, { 43, "12" }
		, { 44, "13" }, { 45, "14" }, { 46, "15" }, { 47, "16" }, { 48, "17" }, { 49, "18" }
		, { 50, "19" }, { 51, "20" }, { 52, "21" }, { 53, "22" }, { 54, "23" }, { 55, "24" }
		, { 56, "25" }, { 57, "26" }, { 58, "27" }, { 59, "28" }, { 60, "29" }, { 61, "30" }
		, { 62, "31" }, { 63, "32" }, { 64, "33" }, { 65, "34" }, { 66, "35" }, { 67, "36" }
		, { 68, "37" }, { 69, "38" }, { 70, "39" }, { 71, "40" }, { 72, "41" }, { 73, "42" }
		, { 74, "43" }, { 75, "44" }, { 76, "45" }, { 77, "46" }, { 78, "47" }, { 79, "48" }
		, { 80, "49" }, { 81, "50" }, { 82, "51" }, { 83, "52" }, { 84, "53" }, { 85, "54" }
		, { 86, "55" }, { 87, "56" }, { 88, "57" }, { 89, "58" }, { 90, "59" }, { 91, "60" }
		, { 92, "61" }, { 93, "62" }, { 94, "63" }, { 95, "64" }, { 96, "65" }, { 97, "66" }
		, { 98, "67" }, { 99, "68" }, { 100, "69" }, { 101, "70" }, { 102, "71" }, { 103, "72" }
		, { 104, "73" }, { 105, "74" }, { 106, "75" }, { 107, "76" }, { 108, "77" }, { 109, "78" }
		, { 110, "79" }, { 111, "80" }, { 112, "81" }, { 113, "82" }, { 114, "83" }, { 115, "84" }
		, { 116, "85" }, { 117, "86" }, { 118, "87" }, { 119, "88" }, { 120, "89" }, { 121, "90" }
		, { 122, "91" }, { 123, "92" }, { 124, "93" }, { 125, "94" }, { 126, "95" }, { 127, "96" } };
	const map <string, char> replacementsBack = { {"00", 32},{"01", 33},{"02", 34},{"03", 35} ,{"04", 36} ,{"05", 37}
		, { "06", 38 }, { "07", 39}, { "09", 40 }, { "10", 41 }, { "11", 42 }, { "12", 43 }
		, { "13", 44 }, { "14", 45 }, { "15", 46 }, { "16", 47 }, { "17", 48 }, { "18", 49 }
		, { "19", 50 }, { "20", 51 }, { "21", 52 }, { "22", 53 }, { "23", 54 }, { "24", 55 }
		, { "25", 56 }, { "26", 57 }, { "27", 58 }, { "28", 59 }, { "29", 60 }, { "30", 61 }
		, { "31", 62 }, { "32", 63 }, { "33", 64 }, { "34", 65 }, { "35", 66 }, { "36", 67 }
		, { "37", 68 }, { "38", 69 }, { "39", 70 }, { "40", 71 }, { "41", 72 }, { "42", 73 }
		, { "43", 74 }, { "44", 75 }, { "45", 76 }, { "46", 77 }, { "47", 78 }, { "48", 79 }
		, { "49", 80 }, { "50", 81 }, { "51", 82 }, { "52", 83 }, { "53", 84 }, { "54", 85 }
		, { "55", 86 }, { "56", 87 }, { "57", 88 }, { "58", 89 }, { "59", 90 }, { "60", 91 }
		, { "61", 92 }, { "62", 93 }, { "63", 94 }, { "64", 95 }, { "65", 96 }, { "66", 97 }
		, { "67", 98 }, { "68", 99 }, { "69", 100 }, { "70", 101 }, { "71", 102 }, { "72", 103 }
		, { "73", 104 }, { "74", 105 }, { "75", 106 }, { "76", 107 }, { "77", 108 }, { "78", 109 }
		, { "79", 110 }, { "80", 111 }, { "81", 112 }, { "82", 113 }, { "83", 114 }, { "84", 115 }
		, { "85", 116 }, { "86", 117 }, { "87", 118 }, { "88", 119 }, { "89", 120 }, { "90", 121 }
		, { "91", 122 }, { "92", 123 }, { "93", 124 }, { "94", 125 }, { "95", 126 }, { "96", 127 } };
	map<char, string> ::iterator iterCS;
	map<string, char> ::iterator iterSC;
public:
	//Empty class constructor
	ConverterOtT()
	{

	}
	//Initializes with string to convert and parameter that responsible for convertion method 
	ConverterOtT(string inputA, bool what)
	{
		if (what == true)
		{
			converted = inputA;
			convertedQ = true;
		}
		else
			original = inputA;
	}
	//Converts string to one that can be encrypted
	string convertTo()
	{
		if (convertedQ == true || original.length() == 0)
		{
			cerr << "Already converted";
			return "";
		}

		for (int i = 0; i < original.length(); i++)
		{
			converted += (*replacements.find(original[i])).second;
		}
		convertedQ = true;

		return converted;
	}
	//Converts back
	string convertBack()
	{
		if (convertedQ == false || converted.length() == 0)
		{
			cerr << "Already converted back";
			return "";
		}
		string temp;
		for (int i = 0; i < converted.length(); i += 2 )
		{
			temp.assign(converted, i, 2);
			original += (*replacementsBack.find(temp)).second;
		}
		convertedQ = false;

		return original;
	}

};