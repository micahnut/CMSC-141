//We hereby declare, upon our honor, that we made this MPA by ourselves.

/*
 * Bojos, Ena Margarita 
 * Chu Im, Micah Nicole
 * Lim, Alyssa Mae
 * Marturillas, Jeza Marie
 * 
*/

#define _WIN32_WINNT 0x0601
#include <iostream>
#include <cstring>
#include <list>
#include <windows.h>
#include <conio.h>
#include <stdlib.h>

using namespace std;

class cell{
public:
	string content;
	cell *left;
	cell *right;
	
	cell(){
		content = "b";
		left = right = NULL;
	}
	
	cell(string in){
		content = in;
		left = right = NULL; 
	}
};

class tape{
public:
	cell *current, *head;
	list<cell*> turingMachine;
	int index;
	
	tape(){
		current = new cell(">");
		head = current;
		turingMachine.push_back(current);
		index = 0;
	}
	
	void addBase(int base){	//adds the base
		for(int i = 0; i < base; i++){
			insertCell("1", current);
		}
		insertCell("#", current);
	}
	
	void addExp(int exp){	//adds the exponent
		for(int i = 0; i < exp; i++){
			insertCell("1", current);
		}
		insertCell("b", current);
	}
	
	void insertCell(string in, cell *tail){	//adds a new cell to the end of the tape
		cell *temp = new cell(in);
		temp->left = tail;
		tail->right = temp;
		turingMachine.push_back(temp);
		index++;
		tail = tail->right;
		current = tail;
	}
	
	void setColor(int ForgC){
		WORD wColor;

		HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
		CONSOLE_SCREEN_BUFFER_INFO csbi;
		if(GetConsoleScreenBufferInfo(hStdOut, &csbi)){
			wColor = (csbi.wAttributes & 0xF0) + (ForgC & 0x0F);
			SetConsoleTextAttribute(hStdOut, wColor);
		}
		return;
	}
	
	void printTape(int position){	//prints the tape.
		cell *temp = head;
		Sleep(100);
		system("cls");
		for(int i = 0; i <= index; i++){		
			if(i == position){
				setColor(4);
				cout<<temp->content;
				temp = temp->right;
				setColor(15);
			}else{
				cout<<temp->content;
				temp = temp->right;
			}
		}
	}
	
	void baseOne(){
		int pointer = 0;
		cell *temp = head;
		while(temp->content != "b"){
			temp = temp->right;
			pointer++;
			printTape(pointer);
		}
		temp->content = "a";
		insertCell("b",temp);
		pointer++;
		printTape(pointer);	
		temp = temp->right;
		printTape(pointer);
		temp->content = "1";
		insertCell("b",temp);
		pointer++;
		printTape(pointer);
			
		cout<<endl<<"Printing the answer..."<<endl;
		temp = temp->left;
		Sleep(500);
		pointer--;
		printTape(pointer);
		cout<<endl<<"Final answer: 1"<<endl;
	}
	
	void compute(){	//does the computation of power and shows how it looks like on the tape
		int pointer = 0;
		cell *temp = head;
		int state = 0;
		printTape(pointer);
		while(state!=23){
			while(state == 0){
				if(temp->content == ">" || temp->content == "1" || temp->content == "#" ){
					temp = temp->right;
					pointer++;
					printTape(pointer);
				}else if(temp->content == "b"){
					temp->content = "a";
					insertCell("b", temp);
					printTape(pointer);
					temp = temp->left;
					pointer--;
					printTape(pointer);
					state = 1;
				}
				 
			}
			while(state == 1){
				if(temp->content == "1"){
					temp = temp->left;
					pointer--;
					printTape(pointer);
				}else if(temp->content == "#"){
					temp = temp->right;
					pointer++;
					printTape(pointer);
					state = 2;
				}
				 
			}
			while(state == 2){
				if(temp->content == "1"){
					temp->content = "y";
					printTape(pointer);
					temp = temp->left;
					pointer--;
					printTape(pointer);
					state = 3;
				}else if(temp->content == "y"){
					temp = temp->right;
					pointer++;
					printTape(pointer);
				}else if(temp->content == "a"){
					temp = temp->left;
					pointer--;
					printTape(pointer);
					state = 21;
				}
				 
			}
			while(state == 3){
				if(temp->content =="1"){
					temp->content = "x";
					printTape(pointer);
					temp = temp->right;
					pointer++;
					printTape(pointer);
					state = 4;
				}else if(temp->content == "#" || temp->content == "x"){
					temp = temp->left;
					pointer--;
					printTape(pointer);
				}else if(temp->content == ">"){
					temp = temp->right;
					pointer++;
					printTape(pointer);
					state = 6;
				}
				 
			}
			while(state == 4){
				if(temp->content == "a" || temp->content == "s" || temp->content == "x" || temp->content == "#" || temp->content == "y" || temp->content == "1"){
					temp = temp->right;
					pointer++;
					printTape(pointer);
				}else if(temp->content == "b"){
					temp->content = "s";
					insertCell("b", temp);
					printTape(pointer);
					temp = temp->left;
					pointer--;
					printTape(pointer);
					state = 5;
				}
				 
			}
			while(state == 5){
				if(temp->content == "a" || temp->content == "y" || temp->content == "1" || temp->content == "s"){
					temp = temp->left;
					pointer--;
					printTape(pointer);
				}else if(temp->content == "#"){
					temp = temp->left;
					pointer--;
					printTape(pointer);
					state = 3;
				}
				 
			}
			while(state == 6){
				if(temp->content == "x"){
					temp->content = "1";
					printTape(pointer);
					temp = temp->right;
					pointer++;
					printTape(pointer);
				}else if(temp->content == "#"){
					temp = temp->left;
					pointer--;
					printTape(pointer);
					state = 7;
				}
				 
			}
			while(state == 7){
				if(temp->content == "1"){
					temp->content = "x";
					printTape(pointer);
					temp = temp->right;
					pointer++;
					printTape(pointer);
					state = 8;
				}
				 
			}
			while(state == 8){
				if(temp->content == "#"){
					temp = temp->right;
					pointer++;
					printTape(pointer);
					state = 9;
				}
				 
			}
			while(state == 9){
				if(temp->content == "y"){
					temp = temp->right;
					pointer++;
					printTape(pointer);
				}else if(temp->content == "1"){
					temp->content = "y";
					printTape(pointer);
					temp = temp->left;
					pointer--;
					printTape(pointer);
					state = 10;
				}else if(temp->content == "a"){
					temp = temp->left;
					pointer--;
					printTape(pointer);
					state = 21;
				}
				 
			}
			while(state == 10){
				if(temp->content == "y" || temp->content == "#"){
					temp = temp->left;
					pointer--;
					printTape(pointer);
				}else if(temp->content == "x"){
					temp = temp->left;
					pointer--;
					printTape(pointer);
					state = 11;
				}
				 
			}
			while(state == 11){
				if(temp->content == "z" || temp->content == "a" || temp->content == "s" || temp->content == "x" || temp->content == "#" || temp->content == "y"){
					temp = temp->left;
					pointer--;
					printTape(pointer);
				}else if(temp->content == "1" ){
					temp->content = "z";
					printTape(pointer);
					temp = temp->right;
					pointer++;
					printTape(pointer);
					state = 12;
				}else if(temp->content == ">"){
					temp = temp->right;
					pointer++;
					printTape(pointer);
					state = 17;
				}		
				 
			}
			while(state == 12){
				if(temp->content == "a" || temp->content == "s" || temp->content == "x" || temp->content == "#" || temp->content == "y" || temp->content == "1" || temp->content == "z"){
					temp = temp->right;
					pointer++;
					printTape(pointer);
				}else if(temp->content == "b"){
					temp = temp->left;
					pointer--;
					printTape(pointer);
					state = 13;
				}
				 
			}
			while(state == 13){
				if(temp->content == "z" || temp->content == "1"){
					temp = temp->left;
					pointer--;
					printTape(pointer);
				}else if(temp->content == "s"){
					temp->content = "z";
					printTape(pointer);
					temp = temp->right;
					pointer++;
					printTape(pointer);
					state = 14;
				}else if(temp->content == "a"){
					temp = temp->right;
					pointer++;
					printTape(pointer);
					state = 16;
				}
				 
			}
			while(state == 14){
				if(temp->content == "z" || temp->content == "1"){
					temp = temp->right;
					pointer++; 
					printTape(pointer);
				}else if(temp->content == "b"){
					temp->content = "1";
					insertCell("b",temp);
					printTape(pointer);
					temp = temp->left;
					pointer--;
					printTape(pointer);
					state = 15;
				}
				 
			}
			while(state == 15){
				if(temp->content == "1"){
					temp = temp->left;
					pointer--; 
					printTape(pointer);
				}else if(temp->content == "z"){
					temp = temp->left;
					pointer--;
					printTape(pointer);
					state = 13;
				}
				 
			}
			while(state == 16){
				if(temp->content == "z"){
					temp->content = "s";
					printTape(pointer);
					temp = temp->right;
					pointer++;
					printTape(pointer);
				}else if(temp->content == "a" || temp->content == "s" || temp->content == "1" || temp->content == "#" || temp->content == "y"){
					temp = temp->left;
					pointer--;
					printTape(pointer);
				}else if(temp->content == "x"){
					temp = temp->left;
					pointer--;
					printTape(pointer);
					state = 11;
				}
				 
			}
			while(state == 17){
				if(temp->content == "x" || temp->content == "y" || temp->content == "#" || temp->content == "1"){
					temp = temp->right;
					pointer++;
					printTape(pointer);
				}else if(temp->content == "z"){
					temp->content = "1";
					printTape(pointer);
					temp = temp->right;
					pointer++;
					printTape(pointer);
				}else if(temp->content == "a"){
					temp = temp->right;
					pointer++;
					printTape(pointer);
					state = 18;
				}
				 
			}
			while(state == 18){
				if(temp->content == "s"){
					temp = temp->right;
					pointer++;
					printTape(pointer);
				}else if(temp->content == "1"){
					temp->content = "s";
					printTape(pointer);
					temp = temp->right;
					pointer++;
					printTape(pointer);
					state = 19;
				}
				 
			}
			while(state == 19){
				if(temp->content == "1"){
					temp->content = "s";
					printTape(pointer);
					temp = temp->right;
					printTape(pointer);
					pointer++;
				}else if(temp->content == "b"){
					temp = temp->left;
					pointer--;
					printTape(pointer);
					state = 20;
				}
				 
			}
			while(state == 20){
				if(temp->content == "1" || temp->content == "y" || temp->content == "a" || temp->content == "s" || temp->content == "z" ||temp->content == "x"){
					temp = temp->left;
					pointer--;
					printTape(pointer);
				}else if(temp->content == "#"){
					temp = temp->right;
					pointer++;
					printTape(pointer);
					state = 9;
				}
				 
			}
			while(state == 21){
				if(temp->content == "x" || temp->content == "y" || temp->content == "1"){
					temp->content = "1";
					printTape(pointer);
					temp = temp->left;
					pointer--;
					printTape(pointer);
				}else if(temp->content == "#"){
					temp = temp->left;
					pointer--;
					printTape(pointer);
				}else if(temp->content == ">"){
					temp = temp->right;
					pointer++;
					printTape(pointer);
					state = 22;
				}
				 
			}
			while(state == 22){
				if(temp->content == "a" || temp->content == "#" || temp->content == "1"){
					temp = temp->right;
					pointer++;
					printTape(pointer);
				}else if(temp->content == "s"){
					temp->content = "1";
					printTape(pointer);
					temp = temp->right;
					pointer++;
					printTape(pointer);
				}else if(temp->content == "b"){
					temp = temp->left;
					pointer--;
					printTape(pointer);
					state = 23;
				}
				 
			}
		}
		cout<<endl<<"Printing the answer...";
		Sleep(300);
		printAnswer(); //prints the answer
	}
	
	void printAnswer(){
		cell *temp = head;
		int pointer = 0;
		while(temp->content != "a"){
			temp = temp->right;
			pointer++;
		}
		int i = 0;
		for(; temp->right->content != "b";i++){
			temp = temp->right;
			pointer++;
			printTape(pointer);
			cout<<endl<<"Current count: "<<i+1;
		}
		printTape(pointer);
		cout<<endl<<"Final answer: "<<i<<endl;
	}	
};

void tmPalindrome();
void SetColor(int ForgC);
void printPalin(int i, string word);

int main(){
	int choice = 0;
		
	do{
		system("cls");
		cout<<endl<<"\tTuring Machine Simulator"<<endl;
		cout<<"\t1. Power"<<endl;
		cout<<"\t2. Palindrome"<<endl;
		cout<<"\t3. Exit"<<endl;
		cout<<endl<<"\tWhat do you want to do?"<<endl;
		cout<<"\tInput 1 for Power or 2 for Palindrome: ";
		cin>>choice;
		
		if(choice == 1){
			int base, exp;
			tape TM;// = new tape();
			system("cls");
			do{
				system("cls");
				cout<<"\tInput the base (positive integers only): ";
				cin>>base;
				cout<<endl<<"\tInput the exponent (positive integers only): ";
				cin>>exp;
			}while(base<1 && exp<1);
				TM.addBase(base);
				TM.addExp(exp);
			if(base == 1){
				TM.baseOne();
			}else{		
				TM.compute();
			}
			system("pause");
			system("cls");	
			choice = 0;
		}else if(choice == 2){
			system("cls");
			cout<<endl<<"\tThis machine only accepts even-length palindromes of the alphabet {a, b};"<<endl;
			tmPalindrome();
			system("pause");
			system("cls");	
			choice = 0;
		}else if(choice == 3){
			cout<<endl<<"\tProgram has been ended..."<<endl;
			break;
		}
	}while(choice<1 || choice >3);
	
	return 0;
}

void printPalin(int i, string word) {
	
		for (int j = 0; j < word.length(); j++) {
		if (j == i) {
			SetColor(6);
			cout << word[j];
		} else {
			SetColor(15);
			cout << word[j];
		}
	}
	Sleep(300);
	system("cls");
	SetColor(15);
	
}

int notInAlpha(string word) {
	
	int check = 0;
	for (int i = 0; i < word.length(); i++) {
		if (word[i] == 'a' || word[i] == 'b') {
			check = 0;
		}else { return 1; }
	}
	return check;
	
}

void tmPalindrome(){
	string input, word = ">", state = "loop";
	long transition = 0;
	int wordCheck;
	
	cout << "\tInput even-length palindrome: ";
	cin >> input;
	
	wordCheck = notInAlpha(input);
	
	if (wordCheck == 1) {state = "wrong";}
	else {state == "loop";}
	
	for(int i = 1; i <= (word.length() +1); ++i){
        transition += i;
    }
    word += input;
    word += '#';
    
    system("cls");
	
	for (int i = 1; state == "loop"; i++) {
		if (word[i] == 'a') {
			printPalin(i, word);
			word[i] = 'x';
			
			printPalin(i, word);		
			i++;
			printPalin(i, word);
		
			while (word[i] == 'a' || word[i] == 'b') {
				i++; 
				printPalin(i, word);
			}
			
			i--;
			printPalin(i, word);
			
			if (word[i] == 'a'){
				word[i] = 'x';
				
				printPalin(i, word);
				i--;
				printPalin(i, word);

				while (word[i] == 'a' || word[i] == 'b') {
					i--;
					printPalin(i, word);
				}
			} else { state = "reject"; }
		}
		else if (word[i] == 'b') {
			printPalin(i, word);
			word[i] = 'x';
			
			printPalin(i, word);		
			i++;
			printPalin(i, word);
						
			while (word[i] == 'a' || word[i] == 'b') {
				i++; 
				printPalin(i, word);
			}
			i--;
			printPalin(i, word);
			
			if (word[i] == 'b'){
				word[i] = 'x';
				
				printPalin(i, word);
				i--;				
				printPalin(i, word);

				while (word[i] == 'a' || word[i] == 'b') {
					i--;
					printPalin(i, word);
				}
			} else { state = "rejected"; }
		} else if (word[i] == 'x') {state = "accepted";}
	}
	if(state == "rejected"){
		cout<<endl<<"\tThis string is NOT an even-length palindrome."<<endl;
	}else if (state == "accepted"){
		cout<<endl<<"\tThis string is an even-length palindrome"<<endl;
	}else if (state == "wrong") {
		cout<<endl<<"\tThe string has values not included in the {a,b} alphabet. Try again..."<<endl<<endl;
		tmPalindrome();
	}
}

void SetColor(int ForgC){
	WORD wColor;

	HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	if(GetConsoleScreenBufferInfo(hStdOut, &csbi)){
		wColor = (csbi.wAttributes & 0xF0) + (ForgC & 0x0F);
		SetConsoleTextAttribute(hStdOut, wColor);
	}
	return;
}
