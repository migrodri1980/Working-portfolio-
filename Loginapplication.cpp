//============================================================================
// Name        : Loginapplication.cpp
// Author      : miguel
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================
#include <iostream>
#include<list>
#include<limits>
#include<string>

using namespace std;

// GLOBAL VARIABLES

bool empMatch= false; //set to false
bool keyExists = false;//set to false

//HASH TABLE CLASS STRUCT
class HashTable{
    private:
        static const int hashGroups = 10; //total size of 10  can be increased for larger groups
        list<pair<int, string>> table[hashGroups];

    public:
        bool isEmpty();
        int hashFunction(int key);
        void insertItem(int key, string value);
        void removeItem(int key);
        bool searchTable(int key,string value);
        void printTable();
};

//HASH FUNCTION TO CHECK IF TABLE EMPTY
bool HashTable::isEmpty()
{
    int sum{};
    for(int i{}; i< hashGroups; i++){
        sum += table[i].size();
    }
    if(!sum){
        return true;

    }
    return false;
}

//HASH FUNCTION
int HashTable::hashFunction(int key){
    return key % hashGroups;
}

//HASH FUNCTION FOR INSERT VALUE
void HashTable::insertItem(int key, string value){
    int hashValue = hashFunction(key);
    auto& cell = table[hashValue];
    auto tItr = begin(cell);
    for(; tItr != end(cell); tItr++){
        if(tItr->first==key){
            keyExists = true; //if key exists replace value
            tItr->second = value; //becomes value of input
            cout<<"Warning key exists. value replaced\n";
            break;
        }

    }
    if (!keyExists){// If key does not exist then put back key and value
        cell.emplace_back(key,value);
    }
    return ;

}

//HASH FUNCTION REMOVE
void HashTable::removeItem(int key){
    int hashValue = hashFunction(key);
    auto& cell = table[hashValue];
    auto tItr = begin(cell);
    for(; tItr != end(cell); tItr++){
        if(tItr->first==key){
            keyExists = true;
            tItr = cell.erase(tItr);
            cout<<"item removed\n";
            break;
        }
    }
    if(!keyExists){
        cout<<"warning key not found";
    }
    return;
}


//HASH SEARCH TABLE FUNCTION FOR USER INPUT KEY AND USER INPUT VALUE
bool HashTable::searchTable(int key, string value){

	int hashValue = hashFunction(key);  //hash function to key input
    auto& cell = table[hashValue];
    auto tItr = begin(cell); //iterator

    for(; tItr != end(cell); tItr++){
        if(tItr->first==key){
            keyExists = true;
            if(keyExists){
                if(tItr->second==value){//tests value of input is matching
                    empMatch=true;


                }
                else{
                    cout<<"access denied\n";
                    empMatch=false;
                }



            }

            if(!keyExists){
            cout<<"warning key not found\n";
            empMatch= false;

            }
        }
    }
    return empMatch;
}


//HASH PRINT TABLE FUNCTION
void HashTable::printTable(){
    for (int i{}; i< hashGroups ; i++){
        if (table[i].size() == 0) continue;

        auto tItr = table[i].begin();
        for(;tItr !=table[i].end(); tItr++){
            cout<<"Employee ID : "<< tItr->first<<"     Password "<< tItr->second<<endl;

        }
    }
    return;
}



void DisplayInfo(){
	string clients[2][5] = {{"Bob Jones","Sarah Davis","Amy Friendly","Johnny Smith","Carol Spears"},{"2","2","1","1","2"}};

	cout << "You chose 1\n" << "Client's Name Service Selected (1 = Brokerage, 2 = Retirement)\n";
		    for(int i=0;i<5;i++) {
		    	cout << clients[0][i] << " selected option " << clients[1][i] << endl;
		        }
 }

void changeCustomerChoice(){
	string clients[2][5] = {{"Bob Jones","Sarah Davis","Amy Friendly","Johnny Smith","Carol Spears"},{"","2","1","1","2"}};
	int temp= 0; //intialized at 0

	cout << "You chose 2\n";
    cout << "Enter the number of client you wish to change : \n";

    cin >> temp;  //this will check the type of input by the user.
	while(1){ //this is an infinite check for the cin
		if (cin.fail()){//this returns true when input failure occurs
				cin.clear();// this clears the error state of the buffer
				cin.ignore(numeric_limits<streamsize>::max(),'\n');//ignores the first instance of error
				cout<<"You have entered the wrong input";
				break;

		}
		if(temp>5||temp<1){
			cin.clear();
			cout<<"Selection out of range";
			break;
		}
		if(!cin.fail()){// allow selection of services if input is correct
		    cout << "Please enter the client's new service choice (1 = Brokerage, 2 = Retirement)\n";

		    cin >> clients[1][temp-1]; //input needs to be adjusted to limit only 1 or 2 for change to new service
		    cout << "Changed option to : "<<clients[1][temp-1];
		    break;

		}

	}


}

//MENU OPTIONS
void MenuOptions(){
	cout<<"ACCESS GRANTED\n";
	cout<<"\nPLEASE SELECT FROM THE FOLLOWING MENU\n";
	cout<<"-------------------------------------------\n";
	cout<<"DISPLAY the client list (enter1)\n";
	cout<<"CHANGE a clients choice (enter2)\n";
	cout<<"ADMIN functions (enter3)\n";
	cout<<"EXIT the program(enter4)\n";
	cout<<"-------------------------------------------\n";
	return;
}


//MAIN PROGRAM
int main()
{
    //intialize and delcare variables to set values
    int empNum= 0;
    string empPass;
    int userChoice=0;
    int adminPass=0;
    int adminChoice=0;
    int adminKey=0;
    string adminValue;

    HashTable HT; //hashtable object


//INSERT INITIAL VALUES INTO HASHTABLE

    HT.insertItem(905,"trick");
    HT.insertItem(9415,"bob");
    HT.insertItem(5105,"sany");
    HT.insertItem(685,"Jasdfm");
    HT.insertItem(9565,"amrk");

    //TEST INSERT REPLACE
// HT.insertItem(905,"trick");

//TEST PRINT TABLE
   // HT.printTable();

    cout<<"Please enter Employee Number:\n";
    cin>>empNum;
    cout<<"Please enter Employee Password:\n";
    cin>>empPass;


    HT.searchTable(empNum,empPass);

    while(empMatch){

    	MenuOptions();
		cin>> userChoice;

		switch(userChoice){

		case 1:
			DisplayInfo();
			cout<<"\n";
    		break;

		case 2:
			changeCustomerChoice();
    		break;

		case 3:
			cout<<"Please enter Adminstrator Password\n";
    		cin>> adminPass;

			if(adminPass == 5691){

				//////////////////////////////////////////////////
				cout<<"\nPLEASE SELECT AN OPTION FROM THE FOLLOWING MENU\n";
					cout<<"-------------------------------------------\n";
					cout<<"Option 1 INSERT employee\n";
					cout<<"Option 2 REMOVE employee\n";
					cout<<"Option 3 PRINT active employee list\n";
					cout<<"OPtion 4 Exit\n";
					cout<<"-------------------------------------------\n";
					cin>>adminChoice;

					while(1){
						if (cin.fail()){//this returns true when input failure occurs
								cin.clear();// this clears the error state of the buffer
								cin.ignore(numeric_limits<streamsize>::max(),'\n');//ignores the first instance of error
								cout<<"You have entered the wrong input\n";
								break;
						}

						if(!cin.fail()){

							switch(adminChoice){

							case 1:
								cout<<"Please enter employee id (whole number integer) to insert\n";
								cin>>adminKey;

								if(cin.fail()){
									cin.clear();// this clears the error state of the buffer
									cin.ignore(numeric_limits<streamsize>::max(),'\n');//ignores the first instance of error
									cout<<"You have entered the wrong input\n";
									break;

								}
								if(!cin.fail()){
									cout<<"Please enter password for employee id\n";
									cin>>adminValue;
									if(cin.fail()){
										cin.clear();// this clears the error state of the buffer
										cin.ignore(numeric_limits<streamsize>::max(),'\n');//ignores the first instance of error
										cout<<"You have entered the wrong input\n";
										break;

									}
									if(!cin.fail()){
										HT.insertItem(adminKey,adminValue);
										cout<<"Insertion Complete\n";
										break;
									}

								}

							case 2:
								cout<<"Please enter employee id (whole number integer) to remove\n";
								cin>>adminKey;
								if(cin.fail()){
									cin.clear();// this clears the error state of the buffer
									cin.ignore(numeric_limits<streamsize>::max(),'\n');//ignores the first instance of error
									cout<<"You have entered the wrong input\n";
									break;

								}
								if(!cin.fail()){
									HT.removeItem(adminKey);
									cout<<"Removal Complete\n";
									break;
								}


							case 3:
							    HT.printTable();
							    break;


							 case 4:
							    exit(0);
							}
						}
						break;
					}

					break;
			}

			else{
				cout<<"ADMIN MENU DENIED... EXITING PROGRAM\n";
				exit(0);
			}

		case 4:
			cout<<"EXITING PROGRAM";
			exit(0); //exit program

		default: //secures input from usering being malicious in nature
			cout << "Invalid Selection" << endl;
    		cin.clear(); //clears the error state of the buffer
    		main(); // got back to main function
    		}
    }



    if(!empMatch){
    	cout<<"The creditionals you have entered do not match\n";
    	exit(0);
    }

	return 0;

}

