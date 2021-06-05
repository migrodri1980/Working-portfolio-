#include<iostream>
#include<stdio.h>
#include<sqlite3.h>
#include<string>
#include<list>
#include<limits>

using namespace std;

static int createDB(const char* s);
static int createTable(const char* s);
static int insertData(const char* s);
static int updateData(const char* s);
static int deleteData(const char* s);
static int deleteSingleData(const char* s);
static int selectData(const char* s);
static int callback(void* NotUsed, int argc, char** argv, char** azColName);

bool empMatch = false; //set to false
bool keyExists = false;//set to false


//HASH TABLE CLASS STRUCT
class HashTable {
private:
	static const int hashGroups = 10; //total size of 10  can be increased for larger groups
	list<pair<int, string>> table[hashGroups];

public:
	bool isEmpty();
	int hashFunction(int key);
	void insertItem(int key, string value);
	void removeItem(int key);
	bool searchTable(int key, string value);
	void printTable();
};

//HASH FUNCTION TO CHECK IF TABLE EMPTY
bool HashTable::isEmpty()
{
	int sum{};
	for (int i{}; i < hashGroups; i++) {
		sum += table[i].size();
	}
	if (!sum) {
		return true;

	}
	return false;
}

//HASH FUNCTION
int HashTable::hashFunction(int key) {
	return key % hashGroups;
}

//HASH FUNCTION FOR INSERT VALUE
void HashTable::insertItem(int key, string value) {
	int hashValue = hashFunction(key);
	auto& cell = table[hashValue];
	auto tItr = begin(cell);
	for (; tItr != end(cell); tItr++) {
		if (tItr->first == key) {
			keyExists = true; //if key exists replace value
			tItr->second = value; //becomes value of input
			cout << "Warning key exists. value replaced\n";
			break;
		}

	}
	if (!keyExists) {// If key does not exist then put back key and value
		cell.emplace_back(key, value);
	}
	return;

}

//HASH FUNCTION REMOVE
void HashTable::removeItem(int key) {
	int hashValue = hashFunction(key);
	auto& cell = table[hashValue];
	auto tItr = begin(cell);
	for (; tItr != end(cell); tItr++) {
		if (tItr->first == key) {
			keyExists = true;
			tItr = cell.erase(tItr);
			cout << "item removed\n";
			break;
		}
	}
	if (!keyExists) {
		cout << "warning key not found";
	}
	return;
}


//HASH SEARCH TABLE FUNCTION FOR USER INPUT KEY AND USER INPUT VALUE
bool HashTable::searchTable(int key, string value) {

	int hashValue = hashFunction(key);  //hash function to key input
	auto& cell = table[hashValue];
	auto tItr = begin(cell); //iterator

	for (; tItr != end(cell); tItr++) {
		if (tItr->first == key) {
			keyExists = true;
			if (keyExists) {
				if (tItr->second == value) {//tests value of input is matching
					empMatch = true;


				}
				else {
					cout << "access denied\n";
					empMatch = false;
				}



			}

			if (!keyExists) {
				cout << "warning key not found\n";
				empMatch = false;

			}
		}
	}
	return empMatch;
}


//HASH PRINT TABLE FUNCTION
void HashTable::printTable() {
	for (int i{}; i < hashGroups; i++) {
		if (table[i].size() == 0) continue;

		auto tItr = table[i].begin();
		for (;tItr != table[i].end(); tItr++) {
			cout << "Employee ID : " << tItr->first << "     Password " << tItr->second << endl;

		}
	}
	return;
}


//SQL DELETE ALL DATA VALUES FUNCTION 
static int deleteData(const char* s)
{
	sqlite3* DB;
	int exit = sqlite3_open(s, &DB);
	
	string sql = "DELETE FROM CUSTINFO;";
	sqlite3_exec(DB, sql.c_str(), callback, NULL, NULL);

	return 0;
}

//SQL DELETE SINGLE DATA VALUE FUNCTION 
static int deleteSingleData(const char* s)
{
	char* messageError;
	string fName;
	string lName;

	sqlite3* DB;
	int exit = sqlite3_open(s, &DB);

	cout << "Please enter customer first name\n";
	cin >> fName;
	cout << "Please enter customer last name\n";
	cin >> lName;

	string sql = "DELETE FROM CUSTINFO WHERE FIRSTNAME ='"+fName+"'AND LASTNAME='"+lName+"';";

	sqlite3_exec(DB, sql.c_str(), callback, NULL, NULL);

	exit = sqlite3_exec(DB, sql.c_str(), NULL, 0, &messageError);
	if (exit != SQLITE_OK) {
		cerr << "Error delete" << endl;
		sqlite3_free(messageError);
	}
	else
		cout << "Record deleted successfully" << endl;

	return 0;
}


//SQL CREATE DB FUNCTION
static int createDB(const char* s) {
	sqlite3* DB;
	int exit = 0;

	exit = sqlite3_open(s, &DB);
	sqlite3_close(DB);

	return 0;


}

// SQL CREATE TABLE FUNCTION 
static int createTable(const char *s)
{
	sqlite3* DB;

	string sql = "CREATE TABLE IF NOT EXISTS CUSTINFO("
		"ID INTEGER PRIMARY KEY AUTOINCREMENT, "
		"FIRSTNAME		TEXT NOT NULL,"
		"LASTNAME		TEXT NOT NULL,"
		"AGE			CHAR(3) NOT NULL,"
		"CITY			TEXT(20),"
		"POLICYCHOICE	CHAR(1));";

	try
	{
		int exit = 0;
		exit = sqlite3_open(s, &DB); //open our database 


		char* messageError;
		exit = sqlite3_exec(DB, sql.c_str(), NULL, 0, &messageError); //callback command 

		if (exit != SQLITE_OK) {  //if this does not work pass error 
			cerr << "Error Create Table" << endl;
			sqlite3_free(messageError);

		}
		else
			cout << "Table created successfully" << endl;
		sqlite3_close(DB);
	}
	catch (const exception & e) 
	{
		cerr << e.what();
	}
	return 0;
}

//SQL INSERT DATA FUNCTION 
static int insertData(const char* s)
{
	sqlite3* DB;
	char* messageError;
	string firstName;
	string lName;
	string cAge;
	string cAddress;
	string cPolicy;


	int exit = sqlite3_open(s, &DB);
	cout << "Please enter first name\n";
	cin >> firstName;
	cout << "Please enter last name\n";
	cin >> lName;
	cout << "Please enter age\n";
	cin >> cAge;
	cout<< "Please enter city\n";
	cin >> cAddress;
	cout << "Please enter policy option (1-9)or (A-Z)\n";
	cin >> cPolicy;

	string sql = "INSERT INTO CUSTINFO (FIRSTNAME, LASTNAME, AGE, CITY, POLICYCHOICE) VALUES('" + firstName + "','"
		+ lName +"','" + cAge + "','" +cAddress+"','"+cPolicy+"')";

	exit = sqlite3_exec(DB, sql.c_str(), NULL, 0, &messageError);
	if (exit != SQLITE_OK) {
		cerr << "Error insert" << endl;
		sqlite3_free(messageError);
	}
	else
		cout << "Record created successfully" << endl;
	return 0;

}

//SQL SELECT DATA FUNCTION 
static int selectData(const char* s)
{
	sqlite3* DB;
	int exit = sqlite3_open(s, &DB);

	string sql = "SELECT * FROM CUSTINFO;"; //can enter where statement 

	sqlite3_exec(DB, sql.c_str(), callback, NULL, NULL);
	return 0;
}

//RETRIEVES CONTENTS OF THE DATABASE USED BY SELECT DATA () argc holds number of results, azColName holds each column return in array, argv holds each value in array
static int callback(void* NotUsed, int argc, char** argv, char** azColName)
{
	for (int i = 0; i < argc; i++) {
		//column name and value 
		cout << azColName[i] << ":" << argv[i] << endl;
	}
	cout << endl;
	return 0;
}

//UPDATE DATA FUNCTION 
static int updateData(const char* s)
{
	
	sqlite3* DB;
	char* messaggeError;
	string lName;
	string fName;
	string pChoice;

	int exit = sqlite3_open(s, &DB);
	cout << "Please enter First Name\n";
	cin >> fName;
	cout << "Please enter Last Name\n";
	cin >> lName;
	cout << "Please enter new policy number (1-9)or(A-Z)\n";
	cin >> pChoice;



	string sql="UPDATE CUSTINFO SET POLICYCHOICE = '"+pChoice+"' WHERE LASTNAME ='"+lName+"'AND FIRSTNAME ='"+fName+"';";
	exit = sqlite3_exec(DB, sql.c_str(), NULL, 0, &messaggeError);
	if (exit != SQLITE_OK) {
		cerr << "Error insert" << endl;
		sqlite3_free(messaggeError);

	}
	else
		cout << "Records updated" << endl;
	return 0;
}

//MENU OPTIONS
void MenuOptions() {
	cout << "ACCESS GRANTED\n";
	cout << "\nPLEASE SELECT FROM THE FOLLOWING MENU\n";
	cout << "-------------------------------------------\n";
	cout << "DISPLAY customer list (enter1)\n";
	cout << "CHANGE customer policy choice (enter2)\n";
	cout << "ADMIN functions (enter3)\n";
	cout << "EXIT the program(enter4)\n";
	cout << "-------------------------------------------\n";
	return;
}


//MAIN FUNCTION 
int main()
{
	//intialize and delcare variables to set values
	int empNum = 0;
	string empPass;
	int userChoice = 0;
	int adminPass = 0;
	int adminChoice = 0;
	int adminKey = 0;
	string adminValue;


	HashTable HT; //hashtable object

//INSERT INITIAL VALUES INTO HASHTABLE

	HT.insertItem(905, "trick");
	HT.insertItem(9415, "bob");
	HT.insertItem(5105, "sany");
	HT.insertItem(685, "Jasdfm");
	HT.insertItem(9565, "amrk");

//SQL PATH, POINTER AND CREATE CALLS 
	const char* dir = "J:\\DeleteMe\\CUSTOMERS.db"; //PATH FOR DB MODIFY BASE ON PLACEMENT 
	sqlite3* DB;
	createDB(dir);
	createTable(dir);
	//deleteData(dir); function to delete values


	//LOGIN OUTPUT 
	cout << "Please enter Employee Number:\n";
	cin >> empNum;
	cout << "Please enter Employee Password:\n";
	cin >> empPass;


	HT.searchTable(empNum, empPass);

	while (empMatch) {

		MenuOptions();
		cin >> userChoice;

		switch (userChoice) {

		case 1:
			selectData(dir);
			cout << "\n";
			break;

		case 2:
			updateData(dir);
			break;

		case 3:
			cout << "Please enter Adminstrator Password\n";
			cin >> adminPass;

			if (adminPass == 5691) {

				//////////////////////////////////////////////////
				cout << "\nPLEASE SELECT AN OPTION FROM THE FOLLOWING MENU\n";
				cout << "-------------------------------------------\n";
				cout << "EMPLOYEE OPTIONS\n";
				cout << "Option 1 INSERT employee\n";
				cout << "Option 2 REMOVE employee\n";
				cout << "Option 3 PRINT active employee list\n";
				cout << "-------------------------------------------\n";
				cout << "CUSTOMER OPTIONS\n";
				cout << "Option 4 INSERT new customer into database\n";
				cout << "Option 5 DELETE ONE customer value in database\n";
				cout << "Option 6 DELETE ALL customer values in database\n";
				cout << "Option 7 Exit\n";


				cout << "-------------------------------------------\n";
				cin >> adminChoice;

				while (1) {
					if (cin.fail()) {//this returns true when input failure occurs
						cin.clear();// this clears the error state of the buffer
						cin.ignore(numeric_limits<streamsize>::max(), '\n');//ignores the first instance of error
						cout << "You have entered the wrong input\n";
						break;
					}

					if (!cin.fail()) {

						switch (adminChoice) {

						case 1:
							cout << "Please enter employee id (whole number integer) to insert\n";
							cin >> adminKey;

							if (cin.fail()) {
								cin.clear();// this clears the error state of the buffer
								cin.ignore(numeric_limits<streamsize>::max(), '\n');//ignores the first instance of error
								cout << "You have entered the wrong input\n";
								break;

							}
							if (!cin.fail()) {
								cout << "Please enter password for employee id\n";
								cin >> adminValue;
								if (cin.fail()) {
									cin.clear();// this clears the error state of the buffer
									cin.ignore(numeric_limits<streamsize>::max(), '\n');//ignores the first instance of error
									cout << "You have entered the wrong input\n";
									break;

								}
								if (!cin.fail()) {
									HT.insertItem(adminKey, adminValue);
									cout << "Insertion Complete\n";
									break;
								}

							}

						case 2:
							cout << "Please enter employee id (whole number integer) to remove\n";
							cin >> adminKey;
							if (cin.fail()) {
								cin.clear();// this clears the error state of the buffer
								cin.ignore(numeric_limits<streamsize>::max(), '\n');//ignores the first instance of error
								cout << "You have entered the wrong input\n";
								break;

							}
							if (!cin.fail()) {
								HT.removeItem(adminKey);
								cout << "Removal Complete\n";
								break;
							}


						case 3:
							HT.printTable();
							break;


						case 4:
							insertData(dir);
							break;
							
						case 5:
							deleteSingleData(dir);
							break;


						case 6:
							deleteData(dir);
							break;

						case 7:
							exit(0);
						}
					}
					break;
				}

				break;
			}

			else {
				cout << "ADMIN MENU DENIED... EXITING PROGRAM\n";
				exit(0);
			}

		case 4:
			cout << "EXITING PROGRAM";
			exit(0); //exit program

		default: //secures input from usering being malicious in nature
			cout << "Invalid Selection" << endl;
			cin.clear(); //clears the error state of the buffer
			main(); // got back to main function
		}
	}



	if (!empMatch) {
		cout << "The creditionals you have entered do not match\n";
		exit(0);
	}

	return 0;

}
