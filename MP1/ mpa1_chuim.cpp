//We hereby declare, upon our honor, that we made this MPA by ourselves
/*
 * Bojos, Ena Margarita
 * Chu Im, Micah Nicole
 * Lim, Alyssa Mae
 * Marturillas, Jeza Marie
*/
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <stack>
#include <algorithm>
using namespace std;

vector <string>existingVarNames; //cppreference for vectors = http://www.cplusplus.com/reference/vector/vector/
void setMatrix(bool[10][10],int[13][5], bool[10][10]);
bool checkValidVarName(string);
char checkValidVarContent(string,vector <string>,int[13][5]);
char checkValidVarContentSimple(string);
int checkValidVarDec(string, bool[10][10],int[13][5], bool[10][10],int);//case1, matrix, call from main or call from func def
int checkFuncDec(string, bool[10][10], bool[10][10],int[13][5],int); //odd is valid, even is invalid. 1 - 2 var dec, 3 - 4 func dec, 5 - 6 func def;
int checkFuncDef(string, bool[10][10], bool[10][10],int[13][5]);

int main(){
	bool varMatrix[10][10]={false};
    int conMatrix[13][5]={0};
    bool funcDecMatrix[10][10] = {false};
    setMatrix(varMatrix, conMatrix, funcDecMatrix);
    int type;
	bool flag = true;
	string name;
	while (flag){
		cout << "What is the name of the file? (Please include the file extension.)" << endl;
		cin >> name;

		ifstream input;
  		input.open(name);
  		if (input.is_open()){
  			flag = false;
   			cout << "File successfully opened." << endl;
		}
		else{
    		cout << "Error opening file. Please try again." << endl;
    		input.close();
 		}
   	};

	ifstream input(name);
	ofstream output;
   	string numOfTests, convert, arr;
   	char temp;

	output.open("chuim1.out");

   	getline(input, numOfTests); //get first line with num of test cases: http://www.cplusplus.com/reference/istream/istream/getline/
   	input >> noskipws;
   	while (input >> temp) { //character by character reading of file: https://www.youtube.com/watch?v=qI-tUsujrCU
   		convert = temp; arr.append(convert); //append to the final test case array: http://www.cplusplus.com/forum/beginner/20409/
   		if (temp == '{') {
   			while (temp != '}') {
   				input >> temp; convert = temp; arr.append(convert);
   			}
   			type = checkValidVarDec(arr, varMatrix, conMatrix, funcDecMatrix, 1);
   			if(type==1){
				output<<"VALID VARIABLE DECLARATION"<<endl;
			}else if(type==2){
				output<<"INVALID VARIABLE DECLARATION"<<endl;
			}else if(type==3){
				output<<"VALID FUNCTION DECLARATION"<<endl;
			}else if(type==4){
				output<<"INVALID FUNCTION DECLARATION"<<endl;
			}else if(type==5){
				output<<"VALID FUNCTION DEFINITION"<<endl;
			}else if(type==6){
				output<<"INVALID FUNCTION DEFINITION"<<endl;
			}else{}
   			arr = "";
   			existingVarNames.clear();

   		} else if (temp == ';') {
   			type = checkValidVarDec(arr, varMatrix, conMatrix, funcDecMatrix, 1);
   			if(type==1){
				output<<"VALID VARIABLE DECLARATION"<<endl;
			}else if(type==2){
				output<<"INVALID VARIABLE DECLARATION"<<endl;
			}else if(type==3){
				output<<"VALID FUNCTION DECLARATION"<<endl;
			}else if(type==4){
				output<<"INVALID FUNCTION DECLARATION"<<endl;
			}else if(type==5){
				output<<"VALID FUNCTION DEFINITION"<<endl;
			}else if(type==6){
				output<<"INVALID FUNCTION DEFINITION"<<endl;
			}else{}
   			arr = "";
   			existingVarNames.clear();
   		}

   	}

	input.close();
	output.close();
    return 0;
}

void setMatrix(bool varMatrix[10][10], int conMatrix[13][5], bool funcDecMatrix[10][10]){ 	//dfa  	=https://www.youtube.com/watch?v=-8jIzns9j1o&t=2s
											//https://www.youtube.com/watch?v=6yh3FLKgAGQ
    varMatrix[1][5] = true; varMatrix[2][5] = true; varMatrix[3][5] = true; varMatrix[4][5] = true;
    varMatrix[6][5] = true; varMatrix[5][6] = true; varMatrix[5][7] = true; varMatrix[5][8] = true;
    varMatrix[7][9] = true; varMatrix[9][6] = true; varMatrix[9][8] = true;

    conMatrix[1][2]=5; conMatrix[1][4]=6; conMatrix[2][1]=7; conMatrix[2][3]=8; conMatrix[3][1]=9;
    conMatrix[3][3]=10; conMatrix[4][1]=2; conMatrix[4][2]=11; conMatrix[4][4]=12; conMatrix[5][1]=7;
    conMatrix[5][3]=8; conMatrix[6][2]=11; conMatrix[6][4]=12; conMatrix[7][2]=5; conMatrix[7][4]=6;
    conMatrix[8][1]=9; conMatrix[8][3]=10; conMatrix[9][2]=5; conMatrix[9][4]=6; conMatrix[10][1]=9;
    conMatrix[10][3]=10; conMatrix[11][1]=7; conMatrix[11][3]=8; conMatrix[12][2]=11; conMatrix[12][4]=12;

    funcDecMatrix[0][5] = true; funcDecMatrix[0][8] = true; funcDecMatrix[1][5] = true; funcDecMatrix[1][8] = true; //old code
	funcDecMatrix[2][5] = true; funcDecMatrix[2][8] = true; funcDecMatrix[3][5] = true; funcDecMatrix[3][8] = true;
	funcDecMatrix[4][5] = true; funcDecMatrix[4][8] = true; funcDecMatrix[5][6] = true; funcDecMatrix[5][7] = true;
	funcDecMatrix[6][0] = true; funcDecMatrix[6][1] = true; funcDecMatrix[6][2] = true; funcDecMatrix[6][3] = true;
	funcDecMatrix[6][4] = true; funcDecMatrix[6][8] = true; funcDecMatrix[7][0] = true; funcDecMatrix[7][1] = true;
	funcDecMatrix[0][7] = true; funcDecMatrix[1][7] = true; funcDecMatrix[2][7] = true; funcDecMatrix[3][7] = true;
	funcDecMatrix[7][2] = true; funcDecMatrix[7][3] = true; funcDecMatrix[8][9] = true; funcDecMatrix[5][8] = true;

    return;
}

bool checkValidVarName(string case1){
	int arrI = 0;
	if(case1[arrI]=='i' && case1[arrI+1]=='n' && case1[arrI+2]=='t'){//if var type is int
        return false;
    }else if(case1[arrI]=='c' && case1[arrI+1]=='h' && case1[arrI+2]=='a' && case1[arrI+3]=='r'){//if var type is char
        return false;
    }else if(case1[arrI]=='f' && case1[arrI+1]=='l' && case1[arrI+2]=='o' && case1[arrI+3]=='a' && case1[arrI+4]=='t'){//if var type is float
        return false;
    }else if(case1[arrI]=='d' && case1[arrI+1]=='o' && case1[arrI+2]=='u' && case1[arrI+3]=='b' && case1[arrI+4]=='l' && case1[arrI+5]=='e'){//if var type is double
        return false;
    }
    for(int i=0; (case1[i]!='\0') && i<case1.size();i++){
        if(i==0){
            if(case1[i]!='_' && !(case1[i]>64 && case1[i]<91) && !(case1[i]>96 && case1[i]<123)){
                return false;
            }
        }else if(case1[i]!='_' && !(case1[i]>64 && case1[i]<91) && !(case1[i]>96 && case1[i]<123) && !(case1[i]>='0' && case1[i]<='9')){
            return false;
        }else if(case1[i]==10){
			return false;
        }else{}
    }
    return true;
}

char checkValidVarContent(string case1, vector <string> validVarNames, int conMatrix[13][5]){ //returns J if valid variable content,N otherwise // ex. 6.75+  (4+3*(apple+2))
    int arrI=0;

    stack<int> pStack;
    for(int i=0; i<case1.size();i++){//check matching parenthesis
        if(case1[i]=='('){
            pStack.push(1);
        }else if(case1[i]==')'){
            pStack.pop();
        }else{
        }
    }
    if(pStack.size()>0){
        return 'N';
    }

    string codeStringContent;
    vector <string>::iterator it;
    for(;case1[arrI]!='\0';){
        if((case1[arrI]=='\'' || (case1[arrI]>='0' && case1[arrI]<='9') || case1[arrI]=='.')){//simple case
            string varContent;//string variable to be sent for checking SIMPLE
            for(;(case1[arrI]!=',' && case1[arrI]!='='&& case1[arrI]!=';' && case1[arrI]!=' ' &&
                case1[arrI] != '+' && case1[arrI] != '-' && case1[arrI] != '/' && case1[arrI] != '*' &&
                case1[arrI] != '(' && case1[arrI] != ')') && arrI<case1.size();arrI++){//check until all valid varname chars are exhausted
                varContent+=case1[arrI];//copy to temp string for checking
            }

            char varC = checkValidVarContentSimple(varContent); //check for simple terms
            if(varC != 'J'){//invalid
                return 'N';
                //codeStringContent+='0';//invalid var content, used in 2nd matrix
            }else{
                codeStringContent+='1';//valid var content
            }
            for(;case1[arrI] == ' ';arrI++); //skip spaces
        }else if(case1[arrI]=='_' || (case1[arrI]>64 && case1[arrI]<91) || (case1[arrI]>96 && case1[arrI]<123)){//if existing variable name
            string varName;//string variable to be sent for checking
            for(;(case1[arrI]!=' ' && case1[arrI]!=',' && case1[arrI]!='='&& case1[arrI]!=';' &&
                case1[arrI] != '+' && case1[arrI] != '-' && case1[arrI] != '/' && case1[arrI] != '*' &&
                case1[arrI] != '(' && case1[arrI] != ')') && arrI<case1.size();arrI++){//check until all valid varname chars are exhausted
                varName+=case1[arrI];//copy to temp string for checking
            }

            bool checkVal = checkValidVarName(varName);//check if name is valid
            if(checkVal){
                it = find(validVarNames.begin(), validVarNames.end(), varName);
                if (it != validVarNames.end()){ //if name exists, can be used for arithmetic
                    codeStringContent+='1';//valid var content
                }else{ //if name doesn't exist
                    return 'N'; //invalid because used unexisting var name
                }
            }else{
                return 'N';
            }
            for(;case1[arrI] == ' ';arrI++);
        }else if(case1[arrI] == '+' || case1[arrI] == '-' || case1[arrI] == '/' || case1[arrI] == '*'){//if operations
            codeStringContent+='2';//valid operation
            arrI++;
            for(;case1[arrI] == ' ';arrI++);
        }else if(case1[arrI] == '('){//if parentheses
            codeStringContent+='3';
            arrI++;
            for(;case1[arrI] == ' ';arrI++);
        }else if(case1[arrI] == ')'){//if parentheses
            codeStringContent+='4';
            arrI++;
            for(;case1[arrI] == ' ';arrI++);
        }else{
            return 'N';
        }
    }

    int conFlag=codeStringContent[0]-48;
    for(int i=1; i<codeStringContent.size();i++){
        conFlag = conMatrix[conFlag][codeStringContent[i]-48];
    }

    if(conFlag == 0){
        return 'N';
    }else{
        return 'J';
    }
}

char checkValidVarContentSimple(string case1){
    bool foundD=false; //flag
    for(int i=0; (case1[i]!='\0') && i<case1.size();i++){
        if(i==0 && (case1[i]=='\'' && case1[i+2]=='\'') && ((i+3)==case1.size())){//if '' and after second 'is end
            return 'J';
        }else if(case1[i] == '.' && !foundD){ //flag first decimal point
            foundD=true;
        }else if(case1[i] == 'f' && (i+1)==case1.size()){ //if f is found and f is last
            return 'J';
        }else if(case1[i]>='0' && case1[i]<='9'){//if valid input digits
        }else{//two decimal points are found;f is not found in last; characters other than digits,decimal,f is found
            return 'N';
        }
    }
    return 'J';
}

int checkValidVarDec(string case1, bool varMatrix[10][10], int conMatrix[13][5], bool funcDecMatrix[10][10],int tag){//tag=1- if from main, 2, if from external function
    int arrI=0; //general index for traversing
    string codeString; //string that receives every token's state/type http://www.cplusplus.com/reference/string/string/
    vector <string>validVarNames;
    vector <string>::iterator it;

    for(; case1[arrI]==' '; arrI++);

    if(case1[arrI]==10){arrI++;}

    //determining var type
    if(case1[arrI]=='i' && case1[arrI+1]=='n' && case1[arrI+2]=='t'){//if var type is int
        codeString+='1'; //'A' for int
        arrI+=3;
    }else if(case1[arrI]=='c' && case1[arrI+1]=='h' && case1[arrI+2]=='a' && case1[arrI+3]=='r'){//if var type is char
        codeString+='2'; //'B' for char
        arrI+=4;
    }else if(case1[arrI]=='f' && case1[arrI+1]=='l' && case1[arrI+2]=='o' && case1[arrI+3]=='a' && case1[arrI+4]=='t'){//if var type is float
        codeString+='3'; //'C' for float
        arrI+=5;
    }else if(case1[arrI]=='d' && case1[arrI+1]=='o' && case1[arrI+2]=='u' && case1[arrI+3]=='b' && case1[arrI+4]=='l' && case1[arrI+5]=='e'){//if var type is double
        codeString+='4'; //'D' for double
        arrI+=6;
    }else{
        if(tag==1){//if called from main
            int type = checkFuncDec(case1, funcDecMatrix, varMatrix, conMatrix,1);
            return type;
        }else{//if called from function definition
            return 2;
        }
    }

    if(case1[arrI] != ' '){//bec there should be space/s between var type and var name
        return 2;
    }

    for(;case1[arrI] == ' ';arrI++); //skip spaces

    for(;case1[arrI]!='\0';){//big loop; no increment bec inner if statements determine new I
		if(case1[arrI]==10){arrI++;}
        if((case1[arrI]=='_' || (case1[arrI]>64 && case1[arrI]<91) || (case1[arrI]>96 && case1[arrI]<123)) && case1[arrI-1]!='='){ //varName valid characters
            string varName;//string variable to be sent for checking
            for(;(case1[arrI]!=' ' && case1[arrI]!=',' && case1[arrI]!='='&& case1[arrI]!=';' && case1[arrI]!='(' && case1[arrI]!=10) && arrI<case1.size();arrI++){//check until all valid varname chars are exhausted
                varName+=case1[arrI];//copy to temp string for checking
            }//by end of loop, 'i' will be index of character after valid varName character

            bool checkVal = checkValidVarName(varName);//check if name is valid
            if(checkVal){
                it = find(validVarNames.begin(), validVarNames.end(), varName);
                if (it != validVarNames.end()){
                    return 2; //INVALID bec repeated name
                }else{
                    codeString+='5'; //'E' if valid name
                    validVarNames.push_back(varName);
                    if(tag==2){  //katong tag**********
					existingVarNames.push_back(varName);
					}

                }
            }else if(case1[arrI]=='('){
				if(tag==1){//if called from main

					int type = checkFuncDec(case1, funcDecMatrix, varMatrix, conMatrix,1);
					return type;
				}else{//if called from function definition
					return 2;
				}
            }else{ return 2;}

            for(;case1[arrI] == ' ';arrI++); //skip spaces
        }else if(((codeString[0]=='1' || codeString[0]=='2' || codeString[0]=='3' || codeString[0]=='4') && codeString[1]=='5' && codeString.size()==2) && case1[arrI] == '('){//if function definition
            if(tag==1){//if called from main
                int type = checkFuncDec(case1, funcDecMatrix, varMatrix, conMatrix,1);
                return type;
            }else{//if called from function definition
                return 2;
            }
        }else if(case1[arrI]==','){//if comma
            codeString+='6'; //'F' is comma
            arrI++;
            for(;case1[arrI] == ' ';arrI++); //skip spaces
        }else if(case1[arrI]=='='){//if equals
            codeString+='7'; //'G' is equals
            arrI++;
            for(;case1[arrI] == ' ';arrI++); //skip spaces
        }else if(case1[arrI]==';'){//if semicolon
            codeString+='8'; //'H' is semicolon
            arrI++;
            for(;case1[arrI] == ' ';arrI++); //skip spaces
        }else if((case1[arrI]=='\'' || (case1[arrI]>='0' && case1[arrI]<='9') || case1[arrI]=='.' || case1[arrI]=='(') || /* if any or the characters \ 0 9 .*/
                 (((case1[arrI]>64 && case1[arrI]<91) || (case1[arrI]>96 && case1[arrI]<123)) && case1[arrI-1]=='=')){//if variable
            string varContent;//string variable to be sent for checking
            for(;(case1[arrI]!=',' && case1[arrI]!='='&& case1[arrI]!=';') && arrI<case1.size();arrI++){//check until all valid varname chars are exhausted
                varContent+=case1[arrI];//copy to temp string for checking
            }//by end of loop, 'i' will be index of character after valid varName character

            char varC = checkValidVarContent(varContent, validVarNames, conMatrix);
            if(varC != 'J'){
                return 2; //indicator to write invalid var dec to file
            }else{
                codeString+='9';//valid var content
            }
            for(;case1[arrI] == ' ';arrI++); //skip spaces
        }else{
            return 2; //invalid variable declaration
        }
        if(case1[arrI]==10){arrI++;}
    }

    bool varFlag=false; //check if variable declaration is valid
    for(int i=0; i<codeString.size()-1;i++){
        if(varMatrix[codeString[i]-'0'][codeString[i+1]-'0']){
            varFlag=true;
        }else{
            varFlag=false;
            break;
        }
    }

    if(varFlag){
        return 1;
    }else{
        return 2;
    }
}

int checkFuncDec(string array, bool state[10][10], bool varMatrix[10][10], int conMatrix[13][5],int tag){
	int index = 0, type=0;
	string codeString = "";
	string copy;
	vector <string>validVarNames;
    vector <string>::iterator it;

    for(;array[index]==' ';index++);

	for(int i=0; i< array.size();i++){
		copy+= array[i];
	}
	if(array[index]==10){index++;}
	if(array[index] == 'i' && array[index+1] == 'n' && array[index+2] == 't'){ //int
		index+=3;
		codeString+='0';
	}else if(array[index] == 'c' && array[index+1] == 'h' && array[index+2] == 'a' && array[index+3] == 'r'){ //char
		index+=4;
		codeString+='1';
	}else if(array[index] == 'f' && array[index+1] == 'l' && array[index+2] == 'o' && array[index+3] == 'a' && array[index+4] == 't'){ //float
		index+=5;
		codeString+='2';
	}else if(array[index] == 'd' && array[index+1] == 'o' && array[index+2] == 'u' && array[index+3] == 'b' && array[index+4] == 'l' && array[index+5] == 'e'){ //double
		index+=6;
		codeString+='3';
	}else if(array[index] == 'v' && array[index+1] == 'o' && array[index+2] == 'i' && array[index+3] == 'd'){ //void
		index+=4;
		codeString+='4';
	}else{ //this is the part where it checks if INT etc.
		if(array.find("(") == string::npos && array.find(")") == string::npos){
			return 2;
		}else{ //invalid func dec
			return 4;
		}
	}
	if(array[index]!= ' '){
		if(array.find("()") == string::npos){
			return type; //this returns the value that Jeza passed
		}else{ //invalid func dec
			return 4;
		}
	}

	for(;array[index] == ' '; index++); //skip spaces

	for(;array[index] != '\0';){
		if(array[index]==10){index++;}
		if(array[index]=='_' || (array[index]>64 && array[index]<91) || (array[index]>96 && array[index]<123)){ //varName valid characters
            string name; //string variable to be sent for checking
            for(;(array[index]!=' ' && array[index]!='(' && array[index]!=','&& array[index]!=')'&& array[index]!=';') && index<array.size();index++){//check until all valid varname chars are exhausted
                name+=array[index]; //copy to temp string for checking
            } //by end of loop, 'i' will be index of character after valid varName character

            bool checkVal = checkValidVarName(name);//check if name is valid
            if(checkVal){
                codeString+='5'; //'E' if valid name
            }else{
                return 4;
            }
            for(;array[index] == ' ';index++); //skip spaces
        }else if(array[index] == '('){ //this big if checks whether the parameters are valid or not
			codeString+="6";
			index++;
			for(;array[index] == ' ';index++); //skip spaces

			bool hasName = true;

			while(array[index]!= ')'){ //this part checks whether parameters are valid or not
				if(array[index] == 'i' && array[index+1] == 'n' && array[index+2] == 't'){ //int
					index+=3;
					codeString+='0';
				}else if(array[index] == 'c' && array[index+1] == 'h' && array[index+2] == 'a' && array[index+3] == 'r'){ //char
					index+=4;
					codeString+='1';
				}else if(array[index] == 'f' && array[index+1] == 'l' && array[index+2] == 'o' && array[index+3] == 'a' && array[index+4] == 't'){ //float
					index+=5;
					codeString+='2';
				}else if(array[index] == 'd' && array[index+1] == 'o' && array[index+2] == 'u' && array[index+3] == 'b' && array[index+4] == 'l' && array[index+5] == 'e'){ //double
					index+=6;
					codeString+='3';
				}else if(array[index] == 'v' && array[index+1] == 'o' && array[index+2] == 'i' && array[index+3] == 'd'){ //void
					index+=4;
					codeString+='4';
					hasName = false;
				}

				if(array[index]!=' ' && array[index] != ',' && array[index]!=')'){
					return 4;
				}

				for(;array[index] == ' ';index++);

				if(array[index]=='_' || (array[index]>64 && array[index]<91) || (array[index]>96 && array[index]<123)){ //varName valid characters
					if(!hasName){
						return 4;
					}
					string name;//string variable to be sent for checking
					for(;(array[index]!=' ' && array[index]!='(' && array[index]!=','&& array[index]!=')'&& array[index]!=';') && index<array.size();index++){//check until all valid varname chars are exhausted
						name+=array[index];//copy to temp string for checking
					}//by end of loop, 'i' will be index of character after valid varName character

					bool checkVal = checkValidVarName(name);//check if name is valid
					if(checkVal){
						codeString+='5'; //'E' if valid name
						if(tag==2){
							validVarNames.push_back(name);
						}
					}else{
						return 4;
					}
					for(;array[index] == ' ';index++); //skip spaces
				}

				if(array[index] == ','){
					index++;
					codeString+='7';
				}
			}
		}else if(array[index] == ')'){
			codeString+='8';
			index++;
		}else if(array[index] == ';'){
			codeString+='9';
			index++;
		}else if(array[index] == '{'){
			//pass to func def
			bool varFlag=false;
		    for(int i=0; i<codeString.size()-1;i++){
		        if(state[codeString[i]-'0'][codeString[i+1]-'0']){
		            varFlag=true;
		        }else{
		            varFlag=false;
		            return 4;
		        }
		    }

			index++;
			int type = checkFuncDef(copy, state, varMatrix, conMatrix);
			return type;
		}else{
			return 4;
		}
	}
	if(array[index]==10){index++;}

//check if variable declaration is valid
    bool varFlag=false;
    for(int i=0; i<codeString.size()-1;i++){
		if(state[codeString[i]-'0'][codeString[i+1]-'0'] && codeString[codeString.length()-1] == '9'){
            varFlag=true;
        }else{
            varFlag=false;
            break;
        }
    }

     if(varFlag){
		if(tag==2){//check if all variables have a varname
			bool varPair = true;
			for(int i = 0; i<codeString.size()-1 && varPair; i++){
				if(codeString[i] == '6'){
					for(int j = i+1; (codeString[j]!='8') && varPair;){
						if((codeString[j]=='0' && codeString[j+1]=='5') || (codeString[j]=='1' && codeString[j+1]=='5') || (codeString[j]=='2' && codeString[j+1]=='5') || (codeString[j]=='3' && codeString[j+1]=='5')){
							varPair = true;
							j+=2;
						}else{
							varPair = false;
						}
					}
				}
			}
			if(varPair){
				for(it = validVarNames.begin(); it!=validVarNames.end(); ++it){
					existingVarNames.push_back(*it);
				}
			}else{
				return 6;
			}
		}
        type = 3;
    }else{
        type = 4;
    }

	return type;
}

  int checkFuncDef(string array, bool state[10][10], bool varMatrix[10][10], int conMatrix[13][5]){
  	int index = 0, type=0;
	string codeString = "";
	vector <string>::iterator it;
	for(;array[index]==' ';index++);

  	string copy;
  	for(;array[index]!='{';index++){
		copy+= array[index];

	  }
  		copy+=';';

  	if(array[index]==10){index++;}
  	type = checkFuncDec(copy,state,varMatrix,conMatrix,2);

  	if(type==6){
  		return type;
	}
  	index++;
  	if(array[index]==10){index++;}
  	for(;array[index] == ' ';index++); //skip spaces

  	for(;array[index]!='\0';){
  		if(array[index]=='}'){ //not sure how to check if only {}
  			return 5;
		  }

  		if((array[index]=='i' && array[index+1]=='n' && array[index+2]=='t')||(array[index]=='c' && array[index+1]=='h' && array[index+2]=='a' && array[index+3]=='r')||
		  (array[index] == 'f' && array[index+1] == 'l' && array[index+2] == 'o' && array[index+3] == 'a' && array[index+4] == 't')||
		  (array[index] == 'd' && array[index+1] == 'o' && array[index+2] == 'u' && array[index+3] == 'b' && array[index+4] == 'l' && array[index+5] == 'e')||
		  (array[index] == 'v' && array[index+1] == 'o' && array[index+2] == 'i' && array[index+3] == 'd')){

		string copy;
  		string w;
  		for(;array[index]!=';';index++){
  			copy+= array[index];

	  	}
  			copy+=';';

			type = checkValidVarDec(copy,varMatrix,conMatrix,state,2);


		 if(type==4){
		  	return 6;
		  }
		  w = array[index];

		  for(;array[index] == ' ';index++); //skip spaces

		  w = array[index];

		  }

		else if(array[index]=='r' && array[index+1]=='e' && array[index+2]=='t' && array[index+3]=='u' && array[index+4]=='r' && array[index+5]=='n'){
			index+=6;
        	if(array[index]!=' '){
        		return 6;
			}
			for(;array[index] == ' ';index++); //skip spaces

			string w;
			w = array[index];
			string temp;


			for(;array[index]!='=';index++){
				if(array[index]==59){
					break;
				}else if(array[index]=='\0'){
					break;
				}
				temp+= array[index];
			}

			for(int i=0; i<existingVarNames.size(); ++i)

			if(array[index]=='='){
				if(checkValidVarName(temp)){
					it = find(existingVarNames.begin(), existingVarNames.end(), temp);
					if (it == existingVarNames.end()){
						//varname used doesn't exist
						return 6;
					}else{
						index++; //skip equals
						for(;array[index] == ' ';index++); //skip spaces
						string temp2;
						for(;array[index]!=59;index++){
							temp2+=array[index];
						}
						if(array[index]!=';') return 6; //no semicolon
						char t = checkValidVarContent(temp2,existingVarNames, conMatrix);

						if(t=='N'){
							return 6;
						}
						index++; //to skip semicolon
					}
				}
			}else if(array[index]==59){
				int t = checkValidVarContent(temp,existingVarNames, conMatrix);
				if(t=='N'){
					return 6;
				}else{
					return 5;
				}
				index++;
			}else{
				return 6;
			}
		}

		else if((array[index]=='_' || (array[index]>64 && array[index]<91) || (array[index]>96 && array[index]<123)) && array[index-1]!='='){ //varName valid characters
			string varName;//string variable to be sent for checking
            for(;(array[index]!=' ' && array[index]!=',' && array[index]!='='&& array[index]!=';' && array[index]!='(') && index<array.size();index++){//check until all valid varname chars are exhausted
                varName+=array[index];//copy to temp string for checking
            }//by end of loop, 'i' will be index of character after valid varName character

            bool checkVal = checkValidVarName(varName);//check if name is valid

		for(int i=0; i<existingVarNames.size(); ++i)
            if(checkVal){
                it = find(existingVarNames.begin(), existingVarNames.end(), varName);

                if(it != existingVarNames.end()){
                	index++;
				}

				if (it == existingVarNames.end()){
					return 6; //INVALID bec repeated name
                }

            }else{
                return 6;
            }
            for(;array[index] == ' ';index++); //skip spaces
        }else if(array[index] == 10){
			index++;
		}
		index++;
	}
	  return 5;
  }
