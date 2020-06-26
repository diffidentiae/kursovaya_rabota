#include <iostream>
#include <fstream>
#include <cstdlib>
#include <string>
#include <list>
#include <iterator>
#include <stdio.h>
#include <stdlib.h>
#include <utility>
#include "unit_class.hpp"
#include "mycrypto.hpp"
#include "Kuznyechik.hpp"
#include "converterofmystupidity.hpp"

using namespace std;

//Loads defaults, file of key and data location
void loadDefaults(string & keyLoc_, string & dataLoc_){
	ifstream defaults ("defaults.txt");
	if(!defaults)
		return;
	
	cout << endl << "Загрузить местоположения ключа и данных по умолчанию? Y/n" << endl;
	string s;
	getline(cin, s);
	if (s == "Y" || s == "y" ) {
		string c;
		getline(defaults, c);
		keyLoc_ = c;
		getline(defaults, c);
		dataLoc_ = c;
	}
}

//Gets string and checks for compatibility
//If input string is blank, overwrite it with "<blank>" string
//If input string doesnt match rules, shows an error message and requests re-enter
void checkString(string tempString) {
	if (tempString == "") {
		tempString = "<blank>";
		return;
	}
	for (auto letter_ : tempString)
		if (letter_ < 32 || letter_ > 127) {
			cerr << "Ошибка ввода, используйте прописные и строчные буквы английского алфавита и символы" << endl;
			cout << "Введите строку заново" << endl;
			getline(cin, tempString);
			checkString(tempString);
		}
	return;
}

//Gets encryption method and string
//encrypts string and return back encrypted version of string
string encryptString(CFB_Mode<Kuznyechik>& samp, string temp) {
	ByteBlock mesage;
	ByteBlock result;
	mesage = hex_to_bytes(temp);
	samp.encrypt(mesage, result);
	return hex_representation(result);

}

//Gets encryption method and string
//decrypts string and return back decrypted version of string
string decryptString(CFB_Mode<Kuznyechik>& samp, string temp) {
	ByteBlock mesage;
	ByteBlock result;
	mesage = hex_to_bytes(temp);
	samp.decrypt(mesage, result);
	return hex_representation(result);
}

//Gets File location string, opens file and arranges contents into list of unit_t 
void openDataFile(CFB_Mode<Kuznyechik>& samp, list <unit_t>& unitTemp, string dataFileLocation, const bool newFileInCheck_) {
	string dataFileLocationTemp = dataFileLocation;
	if (newFileInCheck_ == true)
	{
		cout << "Введите путь нового файла: " << endl;
		getline(cin, dataFileLocationTemp);
	}
	ifstream dataInput(dataFileLocationTemp);
	if (!dataInput)
	{
		cerr << "Файл данных не открывается!" << endl;
		dataFileLocation.erase();
	}

	string tempN = "<empty>", tempM = "<empty>", tempP = "<empty>",
		tempD = "<empty>", tempL = "<empty>";

	while (dataInput)
	{
		getline(dataInput, tempN);
		getline(dataInput, tempM);
		getline(dataInput, tempP);
		getline(dataInput, tempD);
		getline(dataInput, tempL);
		if ((tempM == "" || tempP == "") && (unitTemp.begin() == unitTemp.end()))
		{
			cerr << "Ошибка ввода данных, отсутствуют ключевые элементы" << endl;
			break;
		}

		if (tempN == "" || tempM == "" || tempP == "" || tempD == "" || tempL == "")
			break;

		tempN = decryptString(samp, tempN);
		ConverterOtT tN(tempN, 1);
		tempN = tN.convertBack();
		tempM = decryptString(samp, tempM);
		ConverterOtT tM(tempM, 1);
		tempM = tM.convertBack();
		tempP = decryptString(samp, tempP);
		ConverterOtT tP(tempP, 1);
		tempP = tP.convertBack();
		tempD = decryptString(samp, tempD);
		ConverterOtT tD(tempD, 1);
		tempD = tD.convertBack();
		tempL = decryptString(samp, tempL);
		ConverterOtT tL(tempL, 1);
		tempL = tL.convertBack();

		unit_t temp1(tempN, tempM, tempP, tempD, tempL);
		unitTemp.push_back(temp1);
	}
}

//Adds new unit to the list
//Requests for data and checks compatibility(eng letters and symbols)
unit_t addNewMail() {
	string tempN, tempM, tempP, tempD, tempL;
	cout << endl << "Введите название почты: " << endl;
	getline(cin, tempN);
	checkString(tempN);
	cout << endl << "Введите адресс почты: " << endl;
	getline(cin, tempM);
	checkString(tempM);
	cout << endl << "Введите пароль почты: " << endl;
	getline(cin, tempP);
	checkString(tempP);
	cout << endl << "Введите описание почты: " << endl;
	getline(cin, tempD);
	checkString(tempD);
	cout << endl << "Введите ссылку на почту: " << endl;
	getline(cin, tempL);
	checkString(tempL);

	unit_t temp1(tempN, tempM, tempP, tempD, tempL);
	return temp1;
}

//Removes selected mail
//If there are more than 1 unit with entered mail, outputs list of units and suggests to select one to remove
void removeMail(list <unit_t>& unitTemp) {
	cout << endl << "Введите адрес почты для которого будет произведено удаление:" << endl;
	string searchM;
	getline(cin, searchM);
	bool greatSuccess = false;
	int mailToRemove = 0, pos = 0;
	vector<int> posHolder;
	list <unit_t> ::iterator iterUnit_;

	for (iterUnit_ = unitTemp.begin(); iterUnit_ != unitTemp.end(); iterUnit_++)
	{
		if (iterUnit_->mail_() == searchM)
		{
			mailToRemove++;
			greatSuccess = true;
			cout << "Удалить объект номер: " << mailToRemove << endl;
			iterUnit_->unitOutput(cout);
			posHolder.push_back(pos);
		}
		pos++;
	}

	if (greatSuccess == false) {
		cerr << "Такой почты не найдено" << endl;
		return;
	}

	cout << endl << "Введите номер элемента для удаления, для отмены удаления введите 0: ";
	int posIn;
	cin >> posIn;
	if (posIn > 0 && posIn < mailToRemove + 1) {
		iterUnit_ = unitTemp.begin();
		advance(iterUnit_, posHolder.at(posIn));
		unitTemp.erase(iterUnit_);
		cout << endl << "Элемент удален" << endl;
	}
	else
	{
		cout << endl << "Удаление отменено" << endl;
	}
}

//Allows to edit unit, select unit by entering mail
//Then select line to edit by entering its number
void editUnit(list <unit_t>& unitTemp) {
	string searchM;
	cout << endl << "Введите адрес почты для которого будет произведено изменение: " << endl;
	getline(cin, searchM);
	bool greatSuccess = false;
	int mailToEdit = 0, pos = 0;
	vector<int> posHolder;
	list <unit_t> ::iterator iterUnit_;

	for (iterUnit_ = unitTemp.begin(); iterUnit_ != unitTemp.end(); iterUnit_++)
	{
		if (iterUnit_->mail_() == searchM)
		{
			mailToEdit++;
			greatSuccess = true;
			cout << "Изменить объект: " << mailToEdit << endl;
			iterUnit_->unitOutput(cout);
			posHolder.push_back(pos);
		}
		pos++;
	}

	if (greatSuccess == false) {
		cerr << "Такой почты не найдено" << endl;
		return;
	}
	
	cout << endl << "Введите номер элемента для изменения, для отмены изменения введите 0: " << endl;
	int posIn;
	cin >> posIn;
	if (posIn > 0 && posIn < mailToEdit + 1) {
		iterUnit_ = unitTemp.begin();
		advance(iterUnit_, posHolder.at(posIn));
	}
	else
	{
		cout << endl << "Удаление отменено" << endl;
	}
	
	int passLength_  = 8;
	cout << endl << "Введите длину пароля:" << endl;
	cin >> passLength_;
	if (passLength_ < 1  && passLength_ > 99) {
		passLength_ = 8;
		cerr << "Ошибка ввода длины, значение длины изменено на 8";
	}
	
	bool noSymbols_ = false;
	cout << "Убрать символы из пароля? Y/n" << endl;
	string c;
	getline(cin, c);
	if (c == "Y" || c == "y")
		noSymbols_ = true;
	iterUnit_->generatePassword(passLength_, noSymbols_);
}

//Assistive funciton for following sorting function, that sorts list
void sorterForMail(list <unit_t>& unitTemp) {
	list <unit_t> ::iterator iterUnit_;
	list <unit_t> unitAssistant;
	
	list <unit_t> ::iterator currentMinimum_ = unitTemp.begin();
	while (unitTemp.begin() != unitTemp.end()) {
		for (iterUnit_ = unitTemp.begin(); iterUnit_ != unitTemp.end(); iterUnit_++) {
			if(iterUnit_->mail_().compare(currentMinimum_->mail_()) < 0)
				currentMinimum_ = iterUnit_;
		}
		unitAssistant.push_back(*currentMinimum_);
		unitTemp.erase(currentMinimum_);
		currentMinimum_ = unitTemp.begin();
	}
	unitTemp.clear(); 
	for (iterUnit_ = unitAssistant.begin(); iterUnit_ != unitAssistant.end(); iterUnit_++) {
		unitTemp.push_back(*iterUnit_);
	}
}

//Sorts list of units by Mail
void sortListByMail(list <unit_t>& unitTemp) {
	list <unit_t> ::iterator iterUnit_;
	list <unit_t> unitCopyOfTemp;
	for (iterUnit_ = unitTemp.begin(); iterUnit_ != unitTemp.end(); iterUnit_++) {
		unitCopyOfTemp.push_back(*iterUnit_);
	}
	sorterForMail(unitCopyOfTemp);
	if (unitCopyOfTemp.begin() == unitCopyOfTemp.end())
		cerr << "Error appeared while sorting units, please retry" << endl;
	else {
		unitTemp.clear();
		for (iterUnit_ = unitCopyOfTemp.begin(); iterUnit_ != unitCopyOfTemp.end(); iterUnit_++) {
			unitTemp.push_back(*iterUnit_);
		}
	}
}

//Assistive funciton for following sorting function, that sorts list 
void sorterForName(list <unit_t>& unitTemp) {
	list <unit_t> ::iterator iterUnit_;
	list <unit_t> unitAssistant;
	
	list <unit_t> ::iterator currentMinimum_ = unitTemp.begin();
	while (unitTemp.begin() != unitTemp.end()) {
		for (iterUnit_ = unitTemp.begin(); iterUnit_ != unitTemp.end(); iterUnit_++) {
			if (iterUnit_->mail_().compare(currentMinimum_->mail_()) < 0)
				currentMinimum_ = iterUnit_;
		}
		unitAssistant.push_back(*currentMinimum_);
		unitTemp.erase(currentMinimum_);
		currentMinimum_ = unitTemp.begin();
	}
	unitTemp.clear(); 
	for (iterUnit_ = unitAssistant.begin(); iterUnit_ != unitAssistant.end(); iterUnit_++) {
		unitTemp.push_back(*iterUnit_);
	}
}

//Sorts list of units by Name
void sortListByName(list <unit_t>& unitTemp) {
	list <unit_t> ::iterator iterUnit_;
	list <unit_t> unitCopyOfTemp;
	for (iterUnit_ = unitTemp.begin(); iterUnit_ != unitTemp.end(); iterUnit_++) {
		unitCopyOfTemp.push_back(*iterUnit_);
	}
	sorterForMail(unitCopyOfTemp);
	if (unitCopyOfTemp.begin() == unitCopyOfTemp.end())
		cerr << "Error appeared while sorting units, please retry" << endl;
	else {
		unitTemp.clear();
		for (iterUnit_ = unitCopyOfTemp.begin(); iterUnit_ != unitCopyOfTemp.end(); iterUnit_++) {
			unitTemp.push_back(*iterUnit_);
		}
	}
}

//Generates new password for entered mail
//Multiple objects with same mail 
void generateNewPassword(list <unit_t>& unitTemp) {
	string searchM;
	cout << endl << "Введите адрес почты для которого будет произведена замена: " << endl;
	getline(cin, searchM);
	bool greatSuccess = false;
	int mailToEdit = 0, pos = 0;
	vector<int> posHolder;
	list <unit_t> ::iterator iterUnit_;

	for (iterUnit_ = unitTemp.begin(); iterUnit_ != unitTemp.end(); iterUnit_++)
	{
		if (iterUnit_->mail_() == searchM)
		{
			mailToEdit++;
			greatSuccess = true;
			cout << "Изменить пароль в объекте: " << mailToEdit << endl;
			iterUnit_->unitOutput(cout);
			posHolder.push_back(pos);
		}
		pos++;
	}

	if (greatSuccess == false) {
		cerr << "Такой почты не найдено" << endl;
		return;
	}
	
	cout << endl << "Введите номер элемента для изменения почты, для отмены изменения введите 0: " << endl;
	int posIn;
	cin >> posIn;
	if (posIn > 0 && posIn < mailToEdit + 1) {
		iterUnit_ = unitTemp.begin();
		advance(iterUnit_, posHolder.at(posIn));
	}
	else
	{
		cout << endl << "Удаление отменено" << endl;
	}
	
	int passLength_  = 8;
	cout << endl << "Введите длину пароля:" << endl;
	cin >> passLength_;
	if (passLength_ < 1  && passLength_ > 99) {
		passLength_ = 8;
		cerr << "Ошибка ввода длины, значение длины изменено на 8";
	}
	
	bool noSymbols_ = false;
	cout << "Убрать символы из пароля? Y/n" << endl;
	string c;
	getline(cin, c);
	if (c == "Y" || c == "y")
		noSymbols_ = true;
	iterUnit_->generatePassword(passLength_, noSymbols_);
}

//Outputs unit for entered mail
void outputByMail(list <unit_t>& unitTemp) {
	cout << endl << "Введите адрес почты для которого будет произведен вывод:" << endl;
	string searchM;
	getline(cin, searchM);
	bool greatSuccess = false;
	list <unit_t> ::iterator iterUnit_;
	for (iterUnit_ = unitTemp.begin(); iterUnit_ != unitTemp.end(); iterUnit_++)
	{
		if (iterUnit_->mail_() == searchM)
		{
			greatSuccess = true;
			iterUnit_->unitOutput(cout);
		}
	}
	if (greatSuccess == false)
		cerr << "Такой почты не найдено" << endl;
}

//Outputs unit for entered name
void outputByName(list <unit_t>& unitTemp) {
	cout << endl << "Введите название почты для которогой будет произведен вывод:" << endl;
	string searchN;
	getline(cin, searchN);
	bool greatSuccess = false;
	list <unit_t> ::iterator iterUnit_;
	for (iterUnit_ = unitTemp.begin(); iterUnit_ != unitTemp.end(); iterUnit_++)
	{
		if (iterUnit_->name_() == searchN)
		{
			greatSuccess = true;
			iterUnit_->unitOutput(cout);
		}
	}
	if (greatSuccess == false)
		cerr << "Такого названия не найдено" << endl;
}

//Outputs all units
void output(list <unit_t>& unitTemp) {
	list <unit_t> ::iterator iterUnit_;
	for (iterUnit_ = unitTemp.begin(); iterUnit_ != unitTemp.end(); iterUnit_++) {
		iterUnit_->unitOutput(cout);
	}
}

//Outputs information about commands
void help() {
	cout << "Список доступных команд:" << endl
		<< "open data file / open - открывает файл зашифрованных данных" << endl
		<< "add mail / add - добавляет новый объект почтовых данных" << endl
		<< "remove mail / remove - удаляет выбранный объект почтовых данных" << endl
		<< "edit / e - изменяет выбранный объект, выбор производится по введенной почте" << endl
		<< "generate new password / gnp - создает случайный пароль выбранной длины для определенного объекта" << endl
		<< "sort by mail - сортирует все объекты по возрастанию почты" << endl
		<< "sort by name - сортирует все объексты по возрастанию имени" << endl
		<< "save defaults - предлагает сохранить текущее положение файла ключа и данных, как значение по умолчанию" << endl
		<< "output by mail - выводит все объекты с введенной почтой" << endl
		<< "output by name - выводит все объекты с введенным именем" << endl
		<< "output / o - выводит все объекты" << endl
		<< "help - выводит справку" << endl;
}

//Asks if u want to save as defaults current key and data location
//Otherwise requests to input key and data location
void saveDefaults(string keyLoc, string dataLoc) {
	cout << "Сохранить текущее положение ключа: " << keyLoc << endl
		<< "и данных: " << dataLoc << " как значение по умолчанию? Y/n" << endl;
	string c;
	getline(cin, c);
	if (c == "Y" || c == "y") {
		ofstream defaults("defaults.txt");
		defaults << keyLoc << endl << dataLoc;
	}
	else {
		cout << "Желаете ввести новые данные? Y/n" << endl;
		getline(cin, c);
		if (c == "Y" || c == "y") {
			cout << "Введите положение файла ключа: " << endl;
			getline(cin, c);
			ofstream defaults("defaults.txt");
			defaults << c << endl;
			cout << "Введите положение файла данных: " << endl;
			getline(cin, c);
			defaults << c;
		}
	}
}

enum command_case { OPEN_DATA, ADD_MAIL, REMOVE_MAIL, EDIT, GENERATE_NEW_PASS, 
	SORT_BY_NAME, SORT_BY_MAIL, OUTPUT_BY_MAIL, OUTPUT_BY_NAME, OUTPUT, HELP, SAVE_DEFAULTS};

command_case detectCommand(string input_temp) {
	if (input_temp == "open data file" || input_temp == "open")
		return OPEN_DATA;

	if (input_temp == "add mail" || input_temp == "add")
		return ADD_MAIL;

	if (input_temp == "remove mail" || input_temp == "remove")
		return REMOVE_MAIL;

	if (input_temp == "edit" || input_temp == "e")
		return EDIT;

	if (input_temp == "generate new password" || input_temp == "gnp")
		return GENERATE_NEW_PASS;
	
	if (input_temp == "sort by name")
		return SORT_BY_NAME;
	
	if (input_temp == "sort by mail")
		return SORT_BY_MAIL;

	if (input_temp == "output by mail")
		return OUTPUT_BY_MAIL;

	if (input_temp == "output by name")
		return OUTPUT_BY_NAME;

	if (input_temp == "output" || input_temp == "o")
		return OUTPUT;

	if (input_temp == "help")
		return HELP;

	if (input_temp == "save defaults")
		return SAVE_DEFAULTS;

}

int main() {
	setlocale(LC_ALL, "Russian");
	string input, keyFileLocation, dataFileLocation;
	ByteBlock key;
	ByteBlock iv;
	ByteBlock msg;
	ByteBlock result;
	list <unit_t> unitList;
	list <unit_t> ::iterator iterUnit;
	bool keyIn = false;
	
	loadDefaults(keyFileLocation, dataFileLocation);
L2:
	if (keyFileLocation.length() == 0) {
		cout << endl << "Введите место расположения файла ключа:" << endl;
		getline(cin, keyFileLocation);
	}
	ifstream keyInput(keyFileLocation);
	if (!keyInput)
	{
		cerr << "Файл ключа не открывается!" << endl;
		keyFileLocation.erase();
		goto L2;
	}

	string inputT1, inputT2;
	getline(keyInput, inputT1);
	key = hex_to_bytes(inputT1);
	inputT2 = inputT1;
	getline(keyInput, inputT1);
	iv = hex_to_bytes(inputT1);

	if (inputT1.length() == 0 || inputT2.length() == 0)
	{
		cerr << "Ключ отсутствует!" << endl;
		keyFileLocation.erase();
		goto L2;
	}
	
	if (dataFileLocation.length() == 0) {
		cout << endl << "Введите место расположения файла данных:" << endl;
		getline(cin, dataFileLocation);
	}
	keyIn = true;
	bool newFileInCheck = false;
	CFB_Mode<Kuznyechik> encryptor(Kuznyechik(key), iv);
	cout << "Программа готова к работе, вводите команды:" << endl;
	getline(cin, input);

	while ((input != "quit") && (input != "q"))
	{
		command_case command_ = detectCommand(input);
		string dataFileLocationTemp;

		switch (command_) {
		case OPEN_DATA:
			openDataFile(encryptor, unitList, dataFileLocation, newFileInCheck);
			newFileInCheck = true;
			break;

		case ADD_MAIL:
			unitList.push_back(addNewMail());
			break;

		case REMOVE_MAIL:
			removeMail(unitList);
			break;
		
		case EDIT:
			editUnit(unitList);
			break;
		
		case GENERATE_NEW_PASS:
			generateNewPassword(unitList);
			break;

		case SORT_BY_NAME:
			sortListByName(unitList);
			break;

		case SORT_BY_MAIL:
			sortListByMail(unitList);
			break;
		
		case OUTPUT_BY_MAIL:
			outputByMail(unitList);
			break;

		case OUTPUT_BY_NAME:
			outputByName(unitList);
			break;

		case OUTPUT:
			output(unitList);
			break;

		case HELP:
			help();
			break;

		case SAVE_DEFAULTS:
			saveDefaults(keyFileLocation, dataFileLocation);
			break;
		
		default:
			cout << "Неизвестная команда, для справки используйте команду help" << endl;
		}

		getline(cin, input);
	}

	ofstream kek1(dataFileLocation);
	ofstream kek2(keyFileLocation);
	if (!kek1)
	{
		cerr << "Файл данных не открывается!" << endl;
		goto L2;
	}
	string tempN, tempM, tempP, tempD, tempL, tempMY;
	for (iterUnit = unitList.begin(); iterUnit != unitList.end(); iterUnit++)
	{
		tempN = iterUnit->name_();
		tempM = iterUnit->mail_();
		tempP = iterUnit->pass_();
		tempD = iterUnit->desc_();
		tempL = iterUnit->link_();

		ConverterOtT teN(tempN, 0);
		tempN = teN.convertTo();
		tempN = encryptString(encryptor, tempN);
		ConverterOtT teM(tempM, 0);
		tempM = teM.convertTo();
		tempM = encryptString(encryptor, tempM);
		ConverterOtT teP(tempP, 0);
		tempP = teP.convertTo();
		tempP = encryptString(encryptor, tempP);
		ConverterOtT teD(tempD, 0);
		tempD = teD.convertTo();
		tempD = encryptString(encryptor, tempD);
		ConverterOtT teL(tempL, 0);
		tempL = teL.convertTo();
		tempL = encryptString(encryptor, tempL);


		kek1 << tempN << endl << tempM << endl << tempP << endl << tempD << endl << tempL << endl;
	}
	kek2 << hex_representation(key) << endl << hex_representation(iv) << endl;


	return 0;


}

