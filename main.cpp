#include <iostream>
#include <istream>
#include <string.h>
#include <stdio.h>
#include <fstream>
#include <algorithm>

using namespace std;

string readKey();
string readFilename();
void readFile(string fileName);
void encrypt(char* plainText);

int main(){
    unsigned char pArray[]={0x49fe,0xd3c6,0x7326,0x1234,0xdefb,0x5a8b,0x1e61,0x77ad,0x94b2,0x5731};
    string key = readKey();
    string fileName = readFileName();
    readFile(fileName);

    return 0;
}


void readFile(string fileName){

    FILE *file;
    char buffer[4];
    file = fopen(fileName.c_str(), "r");
    if(file == NULL){
        cout << "error opening file"<< endl;
    }else{
        int counter = 0;
        while(feof(file)){
            
            if(counter == 4){
                char* toEncrypt = (char*)malloc(4 * sizeof(char));
                copy(buffer, buffer + 4, toEncrypt.begin());
                counter = 0;
                encrypt(toEncrypt);

            } else{
                char temp = fgetc(file);
                counter++;
            }
            
        }
    }

}

void encrypt(char* plainText){




}

string readFileName(){
    
    char* fileName;
    cout << "Please enter file name: ";
    cin >> fileName;
    cout << endl << endl;
    return fileName;
}

string readKey(){

    char* key;
    cout << "Please enter the key: ";
    cin >> key;
    cout << endl << endl;

    return key;
}


