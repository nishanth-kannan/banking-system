#include <iostream>
#include <cstdlib>
#include <fstream>
#include <vector>
#include <map>
using namespace std;

#define MIN_BAL 500
class InsufficientFunds{};

// Account Class

class Account{
private:
    long accountNum;
    string firstName;
    string lastName;
    float balance;
    static long nextAccountNum;

public:
    // Constructors
    Account(){};
    Account(string firstName, string lastName, float balance);

    // Accessor Functions (All of them are defined here)
    long getAccountNum(){return accountNum;} 
    string getFirstName(){return firstName;}
    string getLastName(){return lastName;}
    float getBalance(){return balance;}
    static long getLastAccountNum(){return nextAccountNum;}

    // Mutator Functions
    void deposit(float amount);
    void withdraw(float amount);
    static void setLastAccountNum(long accountNum);

    // File operator functions
    friend ofstream & operator<<(ofstream &ofs, Account &acc);
    friend ifstream & operator>>(ifstream &ifs, Account &acc);
    friend ostream & operator<<(ostream &os, Account &acc);
};

long Account::nextAccountNum = 0; // Setting initial account number

// Bank Class

class Bank{
private:
    map<long, Account> accounts;

public:
    // Bank Constructor
    Bank();

    // Functions on Accounts
    Account openAccount(string fname,string lname,float balance);
    Account balanceEnquiry(long accountNumber);
    Account deposit(long accountNum,float amount);
    Account withdraw(long accountNum,float amount);
    void closeAccount(long accountNum);
    void showAllAccounts();

    // Destructor
    ~Bank();

};


// Account Constructor
Account::Account(string firstName, string lastName, float balance){
    nextAccountNum++;
    accountNum = nextAccountNum;
    this->firstName = firstName;
    this->lastName = lastName;
    this->balance = balance;
}

// Account Functions

void Account::deposit(float amount){ // Deposit Money to Bank Account
    this->balance += amount;
} 

void Account::withdraw(float amount){ // Withdraw Money from Bank Account
    if(this->balance - amount < MIN_BAL) 
        throw InsufficientFunds();
    this->balance -= amount;
}

void Account::setLastAccountNum(long accountNum){ // Updating Account Numbers
    nextAccountNum = accountNum;
}

ofstream & operator<<(ofstream &ofs, Account &acc){ // Writes account details to file
    ofs<< acc.accountNum<<endl;
    ofs<< acc.firstName<<endl;
    ofs<< acc.lastName<<endl;
    ofs<< acc.balance<<endl;
    return ofs;
}

ifstream & operator>>(ifstream &ifs, Account &acc){ // Accesses account details from file
    ifs>> acc.accountNum;
    ifs>> acc.firstName;
    ifs>> acc.lastName;
    ifs>> acc.balance;
    return ifs;
}

ostream & operator<<(ostream &os, Account &acc){ // Shows Account details on terminal
    os<< "Account Number: "<< acc.accountNum << endl;
    os<< "First Name: "<< acc.firstName << endl;
    os<< "Last Name: "<< acc.lastName << endl;
    os<< "Balance: "<< acc.balance << endl;
    return os;
}


// Bank Constructor
Bank::Bank(){
    Account account;
    ifstream infile;
    infile.open("Bank.data");
    if(!infile){
        cout<<"Error in opening file. File not found."<<endl;
        return;
    }
    while(!infile.eof()){
        infile >> account;
        this->accounts.insert(pair<long, Account>(account.getAccountNum(), account));
    }
    Account::setLastAccountNum(account.getAccountNum());

    infile.close();
}

// Bank Functions

Account Bank::openAccount(string firstName, string lastName, float balance){ // Open account in bank
    ofstream outfile;
    Account account(firstName, lastName, balance);
    this->accounts.insert(pair<long, Account>(account.getAccountNum(), account));

    outfile.open("Bank.data", ios::trunc);

    map<long, Account>::iterator itr;
    for(itr = accounts.begin(); itr != accounts.end(); itr++){
        outfile << itr->second;
    }
    outfile.close();
    return account;
}

Account Bank::balanceEnquiry(long accountNum){ // Return account balance
    map<long, Account>::iterator itr = accounts.find(accountNum);
    return itr->second;
}

Account Bank::deposit(long accountNum, float amount){ // Deposit money to account
    map<long, Account>::iterator itr = accounts.find(accountNum);
    itr->second.deposit(amount);
    return itr->second;
}

Account Bank::withdraw(long accountNum, float amount){ // Withdraw money from account
    map<long, Account>::iterator itr = accounts.find(accountNum);
    itr->second.withdraw(amount);
    return itr->second;
}

void Bank::closeAccount(long accountNum){ // Close account
    map<long, Account>::iterator itr = accounts.find(accountNum);
    cout<<"Account "<< itr->second << " Deleted";
    accounts.erase(accountNum);
}

void Bank::showAllAccounts(){ // Show all accounts
    map<long, Account>::iterator itr;
    for(itr = accounts.begin(); itr != accounts.end(); itr++){
        cout<<"Account "<< itr->first << endl << itr->second << endl;
    }
}

// Bank Destructor
Bank::~Bank(){
    ofstream outfile;
    outfile.open("Bank.data", ios::trunc);
    map<long, Account>::iterator itr;

    for(itr = accounts.begin(); itr != accounts.end(); itr++){
        outfile<< itr->second;
    }

    outfile.close();
}

// Main Function bagins here
int main(){
    Bank sbi;
    Account acc;

    int choice; // User input
    string firstName, lastName;
    long accountNum;
    float balance;
    float amount;

    cout<< "Welcome to SBI" << endl;

    do{
        cout<< "\nSelect one option below ";
        cout<< "\n1. Open an Account";
        cout<< "\n2. Balance Enquiry";
        cout<< "\n3. Deposit";
        cout<< "\n4. Withdrawal";
        cout<< "\n5. Close an Account";
        cout<< "\n6. Show All Accounts";
        cout<< "\n7. Quit";
        cout<< "\nEnter your choice: ";
        cin >> choice;
        switch(choice){
            case 1:
                cout<< "Enter First Name: ";
                cin>> firstName;
                cout<< "Enter Last Name: ";
                cin>> lastName;
                cout<< "Enter Initial Amount: ";
                cin>> balance;
                acc = sbi.openAccount(firstName, lastName, balance);
                cout<< endl<< "Your Account Details:"<< endl;
                cout<< acc;
                break;
            case 2:
                cout<< "Enter Account Number: ";
                cin>> accountNum;
                acc = sbi.balanceEnquiry(accountNum);
                cout<< endl<< "Your Account Details:"<< endl;
                cout<< acc;
                break;
            case 3:
                cout<< "Enter Account Number: ";
                cin>> accountNum;
                cout<< "Enter Amount to Deposit: ";
                cin>> amount;
                acc= sbi.deposit(accountNum, amount);
                cout<< endl<< "Amount is Deposited"<< endl;
                cout<< acc;
                break;
            case 4:
                cout<< "Enter Account Number: ";
                cin>> accountNum;
                cout<< "Enter Amount to Withdraw: ";
                cin>> amount;
                acc= sbi.withdraw(accountNum, amount);
                cout<< endl<< "Amount Withdrawn"<< endl;
                cout<< acc;
                break;
            case 5:
                cout<< "Enter Account Number: ";
                cin>> accountNum;
                sbi.closeAccount(accountNum);
                cout<< endl<< "Account has been closed"<< endl;
                cout<< acc;
                break;
            case 6:
                sbi.showAllAccounts();
                break;
            case 7: 
                break;
            
            default:
                cout<< "\nEnter Correct Choice";
                exit(0);

        }

    }while(choice != 7);
    return 0;
}