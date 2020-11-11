#include<iostream>
#include<conio.h>
#include<fstream>
#include"header.h"
#include<cctype>
#include<string>
#include<windows.h>
#include<iomanip>
using namespace std;

void account::create_account()
{
	cout<<"\nEnter The account No. : ";
	cin>>acno;
	cout<<"\n\nEnter The Name of The account Holder : ";
	cin.ignore();
	cin.getline(name,50);
	cout<<"\nEnter Type of The account (C/S) : ";
	cin>>type;
	type=toupper(type);    // replaces any lowercase characters to uppercase   cctype 
	cout<<"\nEnter The Initial amount(>=500 for Saving and >=1000 for current ) : ";
	cin>>deposit;
	cout<<"\n\n\nAccount Created..";
}

void account::show_account() const
{
	cout<<"\nAccount No. : "<<acno;
	cout<<"\nAccount Holder Name : ";
	cout<<name;
	cout<<"\nType of Account : "<<type;
	cout<<"\nBalance amount : "<<deposit;
}


void account::modify()
{
	cout<<"\nAccount No. : "<<acno;
	cout<<"\nModify Account Holder Name : ";
	cin.ignore();
	cin.getline(name,50);
	cout<<"\nModify Type of Account : ";
	cin>>type;
type=toupper(type);       // replaces any lowercase characters to uppercase   cctype 
	cout<<"\nModify Balance amount : ";
	cin>>deposit;
}

	
void account::dep(int x)
{
	deposit+=x;
}
	
void account::draw(int x)
{
	deposit-=x;
}
	
void account::report() const
{
	cout<<acno<<setw(10)<<" "<<name<<setw(10)<<" "<<type<<setw(6)<<deposit<<endl;  //setw  use library iomanip
}
	
int account::retacno() const
{
	return acno;
}

int account::retdeposit() const
{
	return deposit;
}

char account::rettype() const
{
	return type;
}
//***************************************************************
//    	function to write in file
//****************************************************************

void write_account()
{
	account ac;
	ofstream outFile;
	outFile.open("account.txt");
	ac.create_account();
	outFile.write(reinterpret_cast<char *> (&ac), sizeof(account));   /////not
	outFile.close();
}

//***************************************************************
//    	function to read specific record from file
//****************************************************************

void display_sp(int n)
{
	account ac;
	bool flag=false;
	ifstream inFile;
	inFile.open("account.txt",ios::in);
	if(!inFile)
	{
		cout<<"File could not be open !! Press any Key...";
		return;
	}
	cout<<"\nBALANCE DETAILS\n";
    	while(inFile.read(reinterpret_cast<char *> (&ac), sizeof(account)))        //////not
	{
		if(ac.retacno()==n)
		{
			ac.show_account();
			flag=true;
		}
	}
    inFile.close();
	if(flag==false)
		cout<<"\n\nAccount number does not exist";
}


//***************************************************************
//    	function to modify record of file
//****************************************************************

void modify_account(int n)
{
	bool found=false;
	account ac;
	fstream File;
    File.open("account.txt",ios::in|ios::out);
	if(!File)
	{
		cout<<"File could not be open !! Press any Key...";
		return;
	}
	while(!File.eof() && found==false)
	{
		File.read(reinterpret_cast<char *> (&ac), sizeof(account));    ////not
		if(ac.retacno()==n)
		{
			ac.show_account();
			cout<<"\n\nEnter The New Details of account"<<endl;
			ac.modify();
			int pos=(-1)*static_cast<int>(sizeof(account));  ///  not
			File.seekp(pos,ios::cur);
		    File.write(reinterpret_cast<char *> (&ac), sizeof(account));   /// not
		    cout<<"\n\n\t Record Updated";
		    found=true;
		  }
	}
	File.close();
	if(found==false)
		cout<<"\n\n Record Not Found ";
}

//***************************************************************
//    	function to delete record of file
//****************************************************************


void delete_account(int n)
{
	account ac;
	ifstream inFile;
	ofstream outFile;
	inFile.open("account.txt",ios::in);
	if(!inFile)
	{
		cout<<"File could not be open !! Press any Key...";
		return;
	}
	outFile.open("Temp.txt",ios::out);
	inFile.seekg(0,ios::beg);
	while(inFile.read(reinterpret_cast<char *> (&ac), sizeof(account)))   ///not
	{
		if(ac.retacno()!=n)
		{
			outFile.write(reinterpret_cast<char *> (&ac), sizeof(account));  ///not
		}
	}
    inFile.close();
	outFile.close();
	remove("account.txt");
	rename("Temp.txt","account.txt");
	cout<<"\n\n\tRecord Deleted ..";
}


void display_all()
{
	account ac;
	ifstream inFile;
	inFile.open("account.txt",ios::in);
	if(!inFile)
	{
		cout<<"File could not be open !! Press any Key...";
		return;
	}
	cout<<"\n\n\t\tACCOUNT HOLDER LIST\n\n";
	cout<<"====================================================\n";
	cout<<"A/c no.      NAME           Type    Balance\n";
	cout<<"====================================================\n";
	while(inFile.read(reinterpret_cast<char *> (&ac), sizeof(account)))
	{
		ac.report();
	}
	inFile.close();
}


//****************************************************************

void deposit_withdraw(int n, int option)
{
	int amt;
	bool found=false;
	account ac;
	fstream File;
    File.open("account.dat", ios::in|ios::out);
	if(!File)
	{
		cout<<"File could not be open !! Press any Key...";
		return;
	}
	while(!File.eof() && found==false)
	{
		File.read(reinterpret_cast<char *> (&ac), sizeof(account));
		if(ac.retacno()==n)
		{
			ac.show_account();
			if(option==1)
			{
				cout<<"\n\n\tTO DEPOSITE AMOUNT ";
				cout<<"\n\nEnter The amount to be deposited";
				cin>>amt;
				ac.dep(amt);
			}
		    if(option==2)
			{
				cout<<"\n\n\tTO WITHDRAW AMOUNT ";
				cout<<"\n\nEnter The amount to be withdraw";
				cin>>amt;
				int bal=ac.retdeposit()-amt;
				if((bal<500 && ac.rettype()=='S') || (bal<1000 && ac.rettype()=='C'))
					cout<<"Insufficience balance";
				else
					ac.draw(amt);
		      }
			int pos=(-1)*static_cast<int>(sizeof(ac));
			File.seekp(pos,ios::cur);
			File.write(reinterpret_cast<char *> (&ac), sizeof(account));
			cout<<"\n\n\t Record Updated";
			found=true;
	       }
         }
    File.close();
	if(found==false)
		cout<<"\n\n Record Not Found ";
}










void intro()
{
	cout<<"\n\n\n\t\t\t\t\t ";
char w[]={'B','A','N','K', '  ','M','A','N','A','G','E','M','E','N','T','  ','S','Y','S','T','E','M'};
for(int i=0;i<=21;i++){
	Sleep(200);
	cout<<w[i];
	
}
	cout<<"\n\n\n\nMADE BY :Ahmed E. Rizk && Ibrahim ELsayed && Ahmed Aksam && Ahmed Ezz &&  Ahmed Gamal  ";

	cout<<"\n\n Menoufia university";
	cin.get();
}


