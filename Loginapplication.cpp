//============================================================================
// Name        : Project.cpp
// Author      : miguel
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include<list>
#include<limits>
using namespace std;


//TODO implement a function that checks password against database or hashmap
bool CheckUserPermissionAccess(string userPass){ //matches user password and returns true or false instead of password to main.
	if (userPass !="123"){
			cout<<"Invalid Password";
			return false;

	}else{
		cout<<"password accepted\n";
		return true;

		}
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



int main(){
	string userName;//currently blank strings
	string userPass;//curently blank strings
	int userChoice = 0; //intialized to 0
	void DisplayInfo();
	void changeCustomerChoice();




	cout<<"\nHello! Welcome to our Investment Company \n Created by Miguel\n";
	cout<<"Please enter your user name and password:\n";
	cin>>userName;  //need to creat function that checks user name as well
	cin>>userPass;  //check only completed on user password but also needs to be altered for unique passwords

	if(CheckUserPermissionAccess(userPass)){ //checks to make sure user pass is accepted.

		cout<<"What would you like to do?\n";
		cout<<"DISPLAY the client list (enter1)\n";
		cout<<"CHANGE a clients choice (enter2)\n";
		cout<<"Exit the program(enter3)\n";
		cin>> userChoice;

//this block checks that cin is getting an int
		while(1){ //this is an infinite check for the cin
			if (cin.fail()){//this returns true when input failure occurs
					cin.clear();// this clears the error state of the buffer
					cin.ignore(numeric_limits<streamsize>::max(),'\n');//ignores the first instance of error
					cout<<"You have entered the wrong input";
					exit(0);
			}
			if(!cin.fail()){ //if not fail continue with cases

				switch(userChoice){

				case 1:
					DisplayInfo();
					main();
					break;

				case 2:
					changeCustomerChoice();
					main();
					break;

			    case 3:
			        exit(0); //exit program

			    default: //secures input from usering being malicious in nature
			        cout << "Invalid Selection" << endl;
			        cin.clear(); //clears the error state of the buffer
			        main(); // got back to main function
				}

			}
		}


		}

	return 0;

}
