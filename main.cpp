#include <iostream>
#include <istream>
#include <string.h>
#include <stdio.h>
#include <fstream>
#include <algorithm>
#include <bitset>
#include <string>
#include <sstream>


using namespace std;

string readKey();
string readFileName();
int* keyExpansion(string key);
void readFile(string fileName, int* expandedKey);
void encrypt(char* plainText, int* expandedKey);

template <size_t N1, size_t N2 >
bitset <N1 + N2> concat( const bitset <N1> & b1, const bitset <N2> & b2 ) {
    string s1 = b1.to_string();
    string s2 = b2.to_string();
    return bitset <N1 + N2>( s1 + s2 );
}

int main(){
    //unsigned char pArray[]={0x49fe,0xd3c6,0x7326,0x1234,0xdefb,0x5a8b,0x1e61,0x77ad,0x94b2,0x5731};
    string key = readKey();
    string fileName = readFileName();
    int* expandedKey = keyExpansion(key);
    readFile(fileName, expandedKey);
    

    return 0;
}


void readFile(string fileName, int* expandedKey){
    FILE *file;
    char buffer[4];
    file = fopen(fileName.c_str(), "r");
    if(file == NULL){
        cout << "error opening file"<< endl;
    }else{
        int counter = 0;
        while(!feof(file)){
            if(counter == 4){
                char* toEncrypt = (char*)malloc(4 * sizeof(char));
                copy(buffer, buffer + 4, toEncrypt);
                counter = 0;
                encrypt(toEncrypt, expandedKey);

            } else{
                buffer[counter] = fgetc(file);
                counter++;
            }
            
        }
    }

}

int* keyExpansion(string key) {

    int pArray[10] = {0x49fe, 0xd3c6, 0x7326, 0x1234, 0xdefb, 0x5a8b, 0x1e61, 0x77ad, 0x94b2, 0x5731};

    if (key.length() < 20) {//check to see if key needs padding

        int legnths = key.length();//counter to keep track of length
        int counter = 0;

        while (legnths < 20) {//if key is too short, repeat the key
            key.push_back(key[counter]);
            legnths++;
            counter++;
        }
    }

    int* newKey = (int*) malloc(10 * sizeof(int));

    for (int i = 0; i < key.length() - 1 ; i++) {
        cout << "Key: " << (int)key[i] << endl;

        switch(i){
            case 0: newKey[0] += key[i]; break;
            case 1: newKey[0] += key[i]; break;
            case 2: newKey[1] += key[i]; break;
            case 3: newKey[1] += key[i]; break;
            case 4: newKey[2] += key[i]; break;
            case 5: newKey[2] += key[i]; break;
            case 6: newKey[3] += key[i]; break;
            case 7: newKey[3] += key[i]; break;
            case 8: newKey[4] += key[i]; break;
            case 9: newKey[4] += key[i]; break;
            case 10: newKey[5] += key[i]; break;
            case 11: newKey[5] += key[i]; break;
            case 12: newKey[6] += key[i]; break;
            case 13: newKey[6] += key[i]; break;
            case 14: newKey[7] += key[i]; break;
            case 15: newKey[7] += key[i]; break;
            case 16: newKey[8] += key[i]; break;
            case 17: newKey[8] += key[i]; break;
            case 18: newKey[9] += key[i]; break;
            case 19: newKey[9] += key[i]; break;
        }
    }

    for (int i = 0; i < 9; i++) {
        newKey[i] = newKey[i] ^ pArray[i];
        cout << "newKey: " << newKey[i] << endl;
    }

    return newKey;
}

void encrypt(char* plainText, int* expandedKey){
    //Declared pArray here not sure if it would be better to be passed in or not
    cout << plainText << endl;
    //Split plainText into 16 bits for left and right sides
    char cRight [2];
    cRight[1]=plainText[3];
    cRight[2]=plainText[4];
    string sLeft(plainText);
    sLeft = sLeft.substr(0, 2);
    string sRight(cRight);

    string iString = sLeft, hString;
    int iTemp = 0;
    stringstream is, hs;
    is << iString; 
    is >> iTemp; // convert string to int
    hs << hex << iTemp; // push int through hex manipulator
    hString = hs.str(); // store hex string
    bitset<16> left(hString);

    string stringRight, lString;
    int lTemp = 0;
    stringstream ls, lhs;
    ls << lString; 
    ls >> lTemp; // convert string to int
    lhs << hex << lTemp; // push int through hex manipulator
    lString = lhs.str(); // store hex string

    bitset<16> right(lString);

    cout << "MADE IT!" <<endl;
    //for loop to run through encryption steps 0,9
    for(int counter=0; counter<=9; counter++){
        //placeHolder=left exclusive or with p[counter]
        bitset<16> pBits(expandedKey[counter]); //Will this work with INTS
        bitset<16> placeHolder = (left ^= pBits); 
        bitset<16> value;
        //value=fBox(placeHolder, Reference S box array); //fBox-returns 16 bits
        left = (value ^= right); //left=value exclusive or right
        right = placeHolder; 
        counter++;
    }
    
    //Encryption done, combine left and right into 32 bits
    bitset<32> combine = concat(left,right);
    //open and print to file 
    ofstream output;
    //app is used to append to a file
    output.open("Encrypted.txt", ios_base::app);
    //What is supposed to be output? bits? 
    output << combine;
    //Encryption done 

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


