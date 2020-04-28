#include <iostream>
#include <istream>
#include <string.h>
#include <stdio.h>
#include <fstream>
#include <algorithm>
#include <bitset>
#include <string>
#include <sstream>

typedef unsigned char u_char;

using namespace std;

string readKey();
string readFileName();
string readOutputChoice();
int* keyExpansion(string key);
void readFile(string fileName, int* expandedKey);
void encrypt(char* plainText, int* expandedKey,int (*sValues)[8][4],string choice);
u_char* charsToHex(string byteString);
bitset <16> fBox(bitset<16> input, int (*sValues)[8][4]);

template <size_t N1, size_t N2 >
bitset <N1 + N2> concat( const bitset <N1> & b1, const bitset <N2> & b2 ) {
    string s1 = b1.to_string();
    string s2 = b2.to_string();
    return bitset <N1 + N2>( s1 + s2 );
}

int main(){
    //unsigned char pArray[]={0x49fe,0xd3c6,0x7326,0x1234,0xdefb,0x5a8b,0x1e61,0x77ad,0x94b2,0x5731};
    remove ("Encrypted.txt");
    string key = readKey();
    string fileName = readFileName();
    int* expandedKey = keyExpansion(key);
    readFile(fileName, expandedKey);
    

    return 0;
}


void readFile(string fileName, int* expandedKey){
    int sBox[8][4] = {{0xabcb,0x7386,0x6ea3,0x4128},
    {0x689b,0x22c7,0x532d,0xa42b},
    {0x0e52,0xf025,0x8724,0x9ac0},
    {0x776a,0x629f,0x4510,0x3188},
    {0x1111,0xda99,0x52bc,0xdead},
    {0xdeaf,0xbeef,0x5810,0x7219},
    {0x733b,0x5ac7,0x2468,0x9753},
    {0x8263,0x820d,0xe72f,0x410c}};
    string choice=readOutputChoice();
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
                encrypt(toEncrypt, expandedKey,&sBox,choice);

            } else{
                buffer[counter] = fgetc(file);
                counter++;
            }
            
        }
    }
    // FILE* file;
    // file = fopen(fileName.c_str(), "r");
    // char buffer[4];
    // string choice=readOutputChoice();
    // if(file == NULL){
    //     cout << "Error, unable to open file" << endl;
    // }else{
    //     while(!feof(file)){
    //         fgets(buffer, sizeof(char) + 4, file);
    //         char* toEncrypt = (char*) malloc(4 * sizeof(char));
    //         copy(buffer, buffer + 4, toEncrypt);
    //         encrypt(toEncrypt, expandedKey,&sBox,choice);
    //     }

    // }

}

int* keyExpansion(string key) {

    int pArray[10] = {0x49fe, 0xd3c6, 0x7326, 0x1234, 0xdefb, 0x5a8b, 0x1e61, 0x77ad, 0x94b2, 0x5731};

    if (key.length() < 20) {//check to see if key needs padding

        int lengths = key.length();//counter to keep track of length
        int counter = 0;

        while (lengths < 20) {//if key is too short, repeat the key
            key.push_back(key[counter]);
            lengths++;
            counter++;
        }
    }

    int* newKey = (int*) malloc(10 * sizeof(int));

    for (int i = 0; i < key.length() - 1 ; i++) {
        //cout << "Key: " << (int)key[i] << endl;

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
        //cout << "newKey: " << newKey[i] << endl;
    }

    return newKey;
}

string charToBinary(u_char c){
    char result[sizeof(u_char) * 8];
    unsigned index = sizeof(u_char) * 8;
    for(int i = 0; i < 8; i++){
        result[i] = '0';
    }
    result[index] = '\0';
    do result[--index] = '0' + (c & 1);
    while (c >>= 1);

    return string(result);
}

void encrypt(char* plainText, int* expandedKey,int (*sValues)[8][4],string choice){
    //Declared pArray here not sure if it would be better to be passed in or not
    //cout << plainText << endl;
    //Split plainText into 16 bits for left and right sides
    char cRight [2];
    cRight[0]=plainText[2];
    cRight[1]=plainText[3]; 

    string sLeft(plainText);
    sLeft = sLeft.substr(0, 2);
    string sRight(cRight);

    
    u_char* rightByteString = charsToHex(sRight);
    u_char* leftByteString = charsToHex(sLeft);
   
    bitset<8> left1(charToBinary(leftByteString[0]));
    bitset<8> left2 (charToBinary(leftByteString[1]));
    bitset<16> left = concat(left1,left2);
 

    bitset<8> right1(rightByteString[0]);
    bitset<8> right2 (rightByteString[1]);
    bitset<16> right = concat(right1,right2);
    //cout << "Left string: " << sLeft << endl;
    //cout << "Right string: " << sRight << endl;
    //cout << "Left bit string: " << left.to_string() <<endl;
    //cout << "Right bit string: " << right.to_string() <<endl;

    //for loop to run through encryption steps 0,9
    for(int counter=0; counter<=9; counter++){
        //placeHolder=left exclusive or with p[counter]
        bitset<16> pBits(expandedKey[counter]); //Will this work with INTS
        bitset<16> placeHolder = (left ^= pBits); 
        bitset<16> value;
        value=fBox(left,sValues); //fBox-returns 16 bits
        left = (value ^= right); //left=value exclusive or right
        right = placeHolder; 
        counter++;
    }
    
    //Encryption done, combine left and right into 32 bits
    bitset<32> combine = concat(left,right);
    //open and print to file 
    if(choice!="1"){
        ofstream output;
        //app is used to append to a file
        output.open(choice, ios_base::app);
        //What is supposed to be output? bits? 
        //output << plainText << " ";
        output << hex <<combine.to_ulong() <<" "; //to_ulong() -for hex
    }else{
        cout << plainText << " ";
        //cout << hex << combine.to_ulong() << " "; 
    }
    //Encryption done 


}

u_char* charsToHex(string data){
    
    u_char* byteArray = (u_char*)malloc(data.length() * sizeof(u_char));
    for(int i = 0; i < data.length(); i++){
        byteArray[i] = (u_char)data[i];
    }
    return byteArray;
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

bitset <16> fBox(bitset<16> input, int (*sValues)[8][4]){
    int sum=0;
    bitset<2> twoBits[8];
    int rowNum=0;
    int bitCount=1;
    //This for loop takes in 16 bits and pushes the bits onto 8 bitset<2> starting with the leftmost bits
    for(int i=15; i>=0; i--){
        twoBits[rowNum][bitCount]=input[i];
        bitCount--;
        if(i%2==0){
            rowNum++;
        }
        if(bitCount==-1){
            bitCount=1;
        }
       
   }
   //This for loop walks through the 8 bitsets and finds the appropriate sValue and adds them to a total sum
   int column;
    for (int row = 0; row < 8; row++)  { 
        column = (int)(twoBits[row].to_ulong()); //convert bits to int, which will be used to find the sValue column
        //cout << "Value: " << hex << (*sValues)[row][column] << " Row: " <<row << " Column: " << column <<endl;
        sum+=(*sValues)[row][column]; //Not sure if this counts as two's compliment addition??
    }
    bitset<16>sSum (sum);
    //cout << "sSum: " << sSum <<endl;
    return sSum;
    

}

string readOutputChoice(){
    cout << "If you would like to print to screen enter '1', else type output filename with a 'txt' extension at the end. " << endl;
    string choice;
    cin >> choice;
    return choice;
}


