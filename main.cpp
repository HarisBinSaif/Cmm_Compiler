#include <iostream>
#include <stdio.h>
#include <fstream>
#include <vector>
#include <cstring>
#include <string>
#include <map>
using namespace std;

ofstream words,symboltable;
const char *datatypes[3] = {"int", "char","print"};
const char *keywords[7] = {"if","else","for","while","read","write","return"};

void writeToFile(char temp_word[]){
    words<<"< "<<temp_word<<", ^ >"<<endl;
}

bool checkIfKeyword(char temp_word[]){
    int i=0;
    int j=0;
    bool flag = false;
    while(i<6 && flag==false){
        if(strlen(temp_word) != strlen(keywords[i])){
            i++;
        }
        else if(temp_word[j] != keywords[i][j]){
            i++;
        }
        else{
            while(temp_word[j] == keywords[i][j] && j<strlen(temp_word)){
                j++;
            }
            if(j>=strlen(temp_word)){
                flag=true;
            }
            i++;
        }
        j=0;
    }
    return flag;
}

bool checkIfDataType(char temp_word[]){
    int i=0;
    int j=0;
    bool flag = false;
    while(i<3 && flag==false){
        if(strlen(temp_word) != strlen(datatypes[i])){
            i++;
        }
        else if(temp_word[j] != datatypes[i][j]){
            i++;
        }
        else{
            while(temp_word[j] == datatypes[i][j] && j<strlen(temp_word)){
                j++;
            }
            if(j>=strlen(temp_word)){
                flag=true;
            }
            i++;
        }
        j=0;
    }
    return flag;
}

bool cmpString(char arr[][100], int row, char string[]){
    bool flag = true;
    int j = 0;
    if(row==0){
        return false;
    }
    else{
        for(int i=0;i<row;i++){
            j = 0;
            if(strlen(arr[i])==strlen(string)){
                while(j<strlen(string)){
                    if(arr[i][j]!=string[j]){
                        flag=false;
                        break;
                    }        
                    j++;
                }
                if(j>=strlen(string)){
                    return true;
                }
            }        
        }
    }
    return false;
}


string getFilePath(){
    string file_path;
    cout<<"Please Enter the file path: ";
    getline(cin,file_path);
    return file_path;
}

ifstream readFile(string file_path){
    ifstream code;
    code.open(file_path);
    return code;
}

void lexical_analyzer(ifstream & file){
    string line;
    int line_number = 1;
    int char_index=0;
    int index=0;
    char temp_word[50];
    char symbol_Table[1024][100];;
    bool isComment = false;
    memset(temp_word, '\0',50);
    
    while(getline(file, line)){
        while(char_index<line.length()){
            if(isComment == true){
                if(line[char_index]=='*' && line[char_index+1] == '/'){
                    isComment = false;
                }
            }

            else{
                if(isalpha(line[char_index])){
                    int temp_word_counter=0;
                    while(isalpha(line[char_index]) || isdigit(line[char_index])){
                        temp_word[temp_word_counter] = line[char_index];
                        char_index++;
                        temp_word_counter++;
                    }
                    if(checkIfDataType(temp_word)){
                        // data type
                        if(line[char_index]==':'){
                            writeToFile(temp_word);
                            char_index++;
                        }
                        else{
                            if(cmpString(symbol_Table,index, temp_word)==false){
                                for(int ind=0;ind<strlen(temp_word);ind++){
                                    symbol_Table[index][ind] = temp_word[ind];
                                }
                                index++;
                                cout<<temp_word<<endl;
                                symboltable<<temp_word<<endl;
                            }
                        }
                    }
                    else if(checkIfKeyword(temp_word)){
                        // keyword
                        writeToFile(temp_word);
                        
                    }
                    else{
                        // identifier
                        words<<"< ID, "<<temp_word<<" >"<<endl;
                        if(cmpString(symbol_Table,index, temp_word)==false){
                                for(int ind=0;ind<strlen(temp_word);ind++){
                                    symbol_Table[index][ind] = temp_word[ind];
                                }
                                index++;
                                cout<<temp_word<<endl;
                                symboltable<<temp_word<<endl;
                            }  
                    }
                    memset(temp_word,'\0',50);
                    char_index--;
                }
                // check for a comment
                else if(line[char_index]=='/'){
                    if(line[char_index+1]=='*'){
                        isComment=true;
                        char_index++;
                    }
                }
                // relational operators
                else if(line[char_index]=='>'){
                    if (line[char_index+1]=='='){
                        words<<"< RO, GE >"<<endl;
                        char_index++;
                    }
                    else{
                        words<<"< RO, GT >"<<endl;
                    }
                }
                else if(line[char_index]=='<'){
                    if (line[char_index+1]=='='){
                        words<<"< RO, LE >"<<endl;
                        char_index++;
                    }
                    else{
                        words<<"< RO, LT >"<<endl;
                    }
                }
                else if(line[char_index]=='!'){
                    if (line[char_index+1]=='='){
                        words<<"< RO, NE >"<<endl;
                        char_index++;
                    }
                    else{
                        cout<<"Unidentified symbol '!' at line number "<<line_number<<endl;
                    }
                }
                else if(line[char_index]=='='){
                    if (line[char_index+1]=='='){
                        words<<"< RO, EQ >"<<endl;
                    }
                    else{
                        cout<<"Unidentified symbol '=' at line number "<<line_number<<endl;
                    }
                }
                // arithematic operators
                else if(line[char_index]=='+'){
                    words<<"< +, ^ >"<<endl;
                }
                else if(line[char_index]=='-'){
                    words<<"< -, ^ >"<<endl;
                }
                else if(line[char_index]=='*'){
                    words<<"< *, ^ >"<<endl;
                }
                else if(line[char_index]=='/'){
                    words<<"< /, ^ >"<<endl;
                }
                else if(line[char_index]==':'){
                    if (line[char_index+1]=='='){
                        words<<"< :=, ^ >"<<endl;
                        char_index++;
                    }
                    else{
                        cout<<"Unidentified symbol ':' at line number "<<line_number<<endl;
                    }
                }
                else if(isdigit(line[char_index])){
                    int temp_word_counter=0;
                    while(isdigit(line[char_index])){
                        temp_word[temp_word_counter]=line[char_index];
                        temp_word_counter++;
                        char_index++;
                    }
                    if(isalpha(line[char_index])){
                        cout<<"Idetification error in line number "<<line_number<<endl;
                    }
                    else{
                        words<<"< NUM, "<<temp_word<<" >"<<endl;
                    }
                }
                //literal constants
                else if(line[char_index]=='\''){
                    if(isalpha(line[char_index+1])){
                        if(line[char_index+2]=='\''){
                            words<<"< LC, "<<line[char_index]<<line[char_index+1]<<line[char_index+2]<<" >"<<endl;
                            char_index+=2;
                        }
                    }
                }
                // strings
                else if (line[char_index]=='"'){
                    char_index++;
                    int temp_word_counter=0;
                    while(line[char_index]!='"'){
                        temp_word[temp_word_counter]=line[char_index];
                        temp_word_counter++;
                        char_index++;
                    }
                    words<<"< STR, "<<temp_word<<" >"<<endl;
                    memset(temp_word,'\0',50);
                }
                else if(line[char_index] == '(' || line[char_index] == ')' || line[char_index] == '[' 
                    || line[char_index] == ']' 
                    || line[char_index] == '{'
                    || line[char_index] == '}'
                    || line[char_index] == ';'
                    || line[char_index] == ':'
                    || line[char_index] == ','){
                        words<<"< "<<line[char_index]<<", ^ >"<<endl;
                }
                else if(line[char_index]!='\n' && line[char_index]!=' ' 
                    && line[char_index]!='\t' 
                    && line[char_index] != '\''
                    && line[char_index] != '"'
                    && line[char_index]!=-96 
                    && line[char_index]!=-62){
                        cout<<"Unidentified symbol "<<line[char_index]<<" at line number "<<line_number<<endl;
                }


            }
            char_index++;
        }
        char_index=0;
        line_number++;
    }
    words.close();
    symboltable.close();
}

int main(){
    string file_path = getFilePath();  
    ifstream file = readFile(file_path);
    words.open("words.txt");
    symboltable.open("symboltable.txt");
    lexical_analyzer(file);
    return 0;
}