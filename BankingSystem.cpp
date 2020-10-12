#include <iostream>
#include <cstdlib>
#include <vector>
#include <map>
#include <fstream>
#include <string>

using namespace std;

#define MIN_BALANCE 500

class InsufficientFunds{};

//Class Account starts
class Account
{
    private:
    long AccNumber;
    string firstName;
    string lastName;
    float Balance;
    static long nextAccNumber;

    public:
    Account(){}
    Account(string fname, string lname, float bal)
    {
        nextAccNumber++;
        AccNumber = nextAccNumber;
        firstName = fname;
        lastName = lname;
        this->Balance = bal;   
    }
    long getAccnumber(){return AccNumber;}
    string getfname(){return firstName;}
    string getlname(){return lastName;}
    float getbalance(){return Balance;}

    void Deposit(float amount)
    {
        Balance = Balance + amount;
    }
    void Withdraw(float amount)
    {
        if(Balance-amount<MIN_BALANCE)
        {
            throw InsufficientFunds();
        }
        Balance=Balance-amount;
    }
    static void setLastAccNum(long accNum)
    {
        nextAccNumber = accNum;
    }
    static long getLastAccNum()
    {
        return nextAccNumber;
    }
    friend ofstream & operator<<(ofstream &ofs,Account &acc);
    friend ifstream & operator>>(ifstream &ifs,Account &acc);
    friend ostream & operator<<(ostream &os,Account &acc);

};

long Account::nextAccNumber=0;

ofstream & operator<<(ofstream &ofs,Account &acc)
{
    ofs<<acc.AccNumber<<endl;
    ofs<<acc.firstName<<endl;
    ofs<<acc.lastName<<endl;
    ofs<<acc.Balance<<endl;
    return ofs;
}
ifstream & operator>>(ifstream &ifs,Account &acc)
{
    ifs>>acc.AccNumber;
    ifs>>acc.firstName;
    ifs>>acc.lastName;
    ifs>>acc.Balance;
    return ifs;
}
ostream & operator<<(ostream &os,Account &acc)
{
    os<<"First Name: "<<acc.getfname()<<endl;
    os<<"Last Name: "<<acc.getlname()<<endl;
    os<<"Account Number: "<<acc.getAccnumber()<<endl;
    os<<"Balance: "<<acc.getbalance()<<endl;
    return os;
}
//Class Account ends

//Class Bank starts
class Bank
{
    private:
    map<long, Account> accounts;

    public:
    Bank()
    {
        Account account;
        ifstream infile("Bank.data");
        if(!infile)
        {
            cout << "Error in opening file! File not found!" << endl;
            return; 
        }
        while(!infile.eof())
        {
            infile >> account;
            accounts.insert(pair<long, Account>(account.getAccnumber(), account));
        }
        Account::setLastAccNum(account.getAccnumber());
        infile.close();
    }
    Account OpenAcc(string fname, string lname, float bal)
    {
        ofstream outfile;
        Account account(fname, lname, bal);
        accounts.insert(pair<long, Account>(account.getAccnumber(),account));

        outfile.open("Bank.data", ios::trunc);

        map<long, Account> :: iterator itr;
        for(itr=accounts.begin();itr!=accounts.end();itr++)
        {
            outfile<<itr->second;
        }
        outfile.close();
        
        return account;
    }
    Account balanceEnquiry(long accNum)
    {
        map<long, Account> :: iterator itr=accounts.find(accNum);
        return itr->second;
    }
    Account Deposit(long accNum, float amount)
    {
        map<long, Account> :: iterator itr=accounts.find(accNum);
        itr->second.Deposit(amount);
        return itr->second;
    }
    Account Withdraw(long accNum, float amount)
    {
        map<long, Account> :: iterator itr=accounts.find(accNum);
        itr->second.Withdraw(amount);
        return itr->second;
    }
    void CloseAcc(long accNum)
    {
        map<long, Account> :: iterator itr=accounts.find(accNum);
        cout << "ACCOUNT DELETED\n" << itr->second;
        cout << endl;
        accounts.erase(accNum);
    }
    void ShowAllAcc()
    {
        map<long, Account> :: iterator itr;
        for(itr=accounts.begin();itr!=accounts.end();itr++)
        {
            cout << "Account: " << itr->first << endl << itr->second << endl;
        }
    }
    ~Bank()
    {
        ofstream outfile("Bank.data", ios::trunc);
        map<long, Account> :: iterator itr;
        for(itr=accounts.begin();itr!=accounts.end();itr++)
        {
            outfile << itr->second;
        }
        outfile.close();
    }

};
//Bank class ends

int main()
{
    Bank b;
    Account acc;
    int choice;
    string fname,lname;
    long accountNumber;
    float balance;
    float amount;
    cout<<"<< BANKING SYSTEM by SMJI >>"<<endl;
    do
    {
        cout<<"\n\tSelect one option below ";
        cout<<"\n\t1 Open an Account";
        cout<<"\n\t2 Balance Enquiry";
        cout<<"\n\t3 Deposit";
        cout<<"\n\t4 Withdrawal";
        cout<<"\n\t5 Close an Account";
        cout<<"\n\t6 Show All Accounts";
        cout<<"\n\t7 Quit";
        cout << endl;
        cout<<"\nEnter your choice: ";
        cin>>choice;
        cout << endl;
        switch(choice)
        {
            case 1:
            {
                cout << "==========OPEN ACCOUNT==========" << endl;
                cout << endl;
                cout << "Enter first name: ";
                cin >> fname;
                cout << "Enter last name: ";
                cin >> lname;
                cout << "Enter initial balance: ";
                cin >> balance;
                acc=b.OpenAcc(fname, lname, balance);
                cout << endl << "Congratulations, your account is created: " << endl;
                cout << acc;
                break;
            }
            case 2:
            {
                cout << "==========BALANCE ENQUIRY==========" << endl;
                cout << endl;
                cout<<"Enter Account Number: ";
                cin>>accountNumber;
                acc=b.balanceEnquiry(accountNumber);
                cout<<endl<<"Your Account Details: "<<endl;
                cout<<acc;
                break;
            }
            case 3:
            {
                cout << "==========DEPOSIT AMOUNT==========" << endl;
                cout << endl;
                cout<<"Enter Account Number: ";
                cin>>accountNumber;
                cout<<"Enter Balance: ";
                cin>>amount;
                acc=b.Deposit(accountNumber, amount);
                cout<<endl<<"Amount is Deposited"<<endl;
                cout<<acc;
                break;
            }
            case 4:
            {
                cout << "==========WITHDRAW AMOUNT==========" << endl;
                cout << endl;
                cout<<"Enter Account Number: ";
                cin>>accountNumber;
                cout<<"Enter Balance: ";
                cin>>amount;
                acc=b.Withdraw(accountNumber, amount);
                cout<<endl<<"Amount Withdrawn"<<endl;
                cout<<acc;
                break;
            }
            case 5:
            {
                cout << "==========CLOSE ACCOUNT==========" << endl;
                cout << endl;
                cout<<"Enter Account Number: ";
                cin>>accountNumber;
                b.CloseAcc(accountNumber);
                cout<<endl<<"Account is Closed"<<endl;
                break;
            }
            case 6:
            {
                cout << "==========ALL ACCOUNTS==========" << endl;
                cout << endl;
                b.ShowAllAcc();
                break;
            }
            case 7:
            {
                break;
            }
            default:
            {
                cout << "Enter the correct choice";
                exit(0);
            }
        }

    }while(choice!=7);

    return 0;
}