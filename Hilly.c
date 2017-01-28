/** Hilly Program
Author: Yahya Almardeny
2016
Last Update: 01/10/2016
*/
#include<stdio.h>
#include<stdbool.h>
#include<string.h>
#include<Shlobj.h>
#include<math.h>
#include "cMenu.h"
#include "matrix.h"



/*Global Vars*/
typedef double** M;
M key;
M extensionMatrix;
M keyInverse;
M extensionMatrixInverse;

double diagonalSum=0;
double secondaryDiagonalSum = 0;
double *determinant;

double xEncDecryption[1000000];
double cipherLettersDecryption[1000000];
char cipher_Text[10000000]; // for Decryption
char plainText[1000000]; // for Decryption

char plain_Text[1000000]; // for Encryption
char cipherText[10000000]; // for Encryption

double substition_Table[74];

char symbolsSubstitutionTable[]={'*','#','&','%','$','+','?','!','@','^' ,'-'}; // default without shift yet

char padding[2];

/*Functions*/

/*INTRO*/
int intro();
int mainMenu();
int findConsoleScreenWidth();
void centerText(char *text, size_t color, size_t textWidth);
void printCharByChar(char *text, size_t delay, size_t color, char *newline);


/*MIS. SHARED FUNCTIONS */
M create_key();
void create_extensionMatrix();
void delete_matrix(M matrix);
void deleteColumnMatrix(M columnMatrix);
void emptyArray(char *theArray);
void resetSymbolsSubstitutionTable();
void resetSubstitutionTable();
int gcd(int a);


/*ENCRYPTION*/
int keyMatrixEnc();
int generateKey();
int insertKey(M key);
int loadKey();
int saveKey(M key);
int saveInsertedKey(M key);
int validateKey(M key);
int drawKey(int vpos,  M key);
int drawKeyFromFile(int indent, int vpos, M key);
double controlledRandom();

int symbols();
int shiftSymbols(char *arrayOfSymbols);
int mod(double x , int y);
double sum_t();
int loadSymbols();
int insertSymbols();
int saveInsertedSymbols();

int plainTextFun();
int typePlainText();
int loadPlainText();
int plainTextLength();

int encrypt();
int saveCipherText();
int encryption();

int addPadding();
double roundExtentionX(double extentionX);

/*DECRYPTION*/
int keyMatrixDec();
int cipherTextFun();
int typeCipherText();
int loadCipherText();
int savePlainText();
int cipherTextLength();
int find_NumberOfAddedChars();
int decrypt(char *cipher_Text);
double convertCharToDouble(char *arrayOfChars, double arrayOfDoubles[]);
void removePadding();
double roundP(double p);
void findExtensionMatrixInverse();
void findKeyInverse();
void emptyDoubleArray();
int decryption();


/*How To Use & about*/
void howToUse();
void about();

int main(){


intro();
int option =0;

while(option != 5){
     system("cls");
     option = mainMenu();
     switch(option){
        case 1:
            if(encryption()){printf("\n\n  Press Enter To Continue..."); while(getchar()!='\n'); break;}
            else {break;}

        case 2:
            if(decryption()){printf("\n\n  Press Enter To Continue..."); while(getchar()!='\n'); break;}
            else {break;}
            break;

        case 3:
            howToUse();
            break;

        case 4:
            about();
            break;

        case 5:
            exit(0);
    }
}

return 0;}






/*HILLY INTRO & MAIN MENU*/

int mainMenu(){
HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
CONSOLE_CURSOR_INFO info;
info.dwSize = 100;
info.bVisible = FALSE;
SetConsoleCursorInfo(hOut, &info);
system("cls");
printf("\n\n\n");
centerText("",0, 5);
printCharByChar("HILLY", 0, 5,"\n");
centerText("",0, 34);
printCharByChar("An Improved Version Of Hill Cipher", 0, 5,"\n");
centerText("",0, 34);
printCharByChar("----------------------------------", 0, 15,"\n\n");
int option = VMenu((findConsoleScreenWidth()/2)-11, -1,2,">",15,"",0,15,3,5, "Encryption", "Decryption", "How To Use This Program", "About", "Exit");
switch(option){
    case 1:
        return 1;
    case 2:
        return 2;
    case 3:
        return 3;
    case 4:
        return 4;
    case 5:
        exit(0);
}

}

int intro(){
HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
CONSOLE_CURSOR_INFO info;
info.dwSize = 100;
info.bVisible = FALSE;
SetConsoleCursorInfo(hOut, &info);
printf("\n\n\n");
centerText("",0, 5);
printCharByChar("HILLY", 80, 15,"\n");
centerText("",0, 34);
printCharByChar("An Improved Version Of Hill Cipher", 60, 8,"\n\n\n");


CONSOLE_SCREEN_BUFFER_INFO SBInfo;
GetConsoleScreenBufferInfo(hOut, &SBInfo);

COORD c;
c.Y = SBInfo.dwCursorPosition.Y;
c.X = SBInfo.dwCursorPosition.X;

SetConsoleCursorPosition(hOut, c);
Sleep(500);

int x=0;
for(;x<2;x++){
    SetConsoleCursorPosition(hOut, c);
    centerText("Loading   ",10, 7);
    Sleep(500);

    SetConsoleCursorPosition(hOut, c);
    centerText("Loading.  ",11, 7);

    Sleep(500);
    SetConsoleCursorPosition(hOut, c);
    centerText("Loading.. ",13, 7);

    Sleep(500);
    SetConsoleCursorPosition(hOut, c);
    centerText("Loading...",12, 7);
    Sleep(500);
}

return 0;}

int findConsoleScreenWidth(){
HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
CONSOLE_SCREEN_BUFFER_INFO SBInfo;
GetConsoleScreenBufferInfo(hOut, &SBInfo);
int pos = SBInfo.dwCursorPosition.Y;
if (pos==0){pos++;printf("\n");}
int theEnd =pos+1, width=0;

while (pos<theEnd){
       printf(" ");
       GetConsoleScreenBufferInfo(hOut, &SBInfo);
       pos = SBInfo.dwCursorPosition.Y;
       width++;
}
for (pos=0;pos<width;pos++){printf("\b");}
return width-10; // I liked this position more :D
}

void centerText(char *text, size_t color, size_t textWidth){
HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
CONSOLE_CURSOR_INFO info;
info.dwSize = 100;
info.bVisible = FALSE;
SetConsoleCursorInfo(hOut, &info);
int width = findConsoleScreenWidth();
int indent;
if (textWidth){
    double even;
    if((even = ((double)textWidth-((double)textWidth/2)))>0){textWidth++;}
    for (indent=0; indent<((width/2)-(textWidth/2)); indent++){printf(" ");}
}
else {
    int len = strlen(text);
    for (indent=0; indent<((width/2)-(len/2)); indent++){printf(" ");}
}
SetConsoleTextAttribute(hOut, color);
printf(text);
SetConsoleTextAttribute(hOut, 15);
}

void printCharByChar(char *text, size_t delay, size_t color, char *newline){
HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
SetConsoleTextAttribute(hOut, color);
while(*text){
      printf("%c", *text);
      Sleep(delay);
      text++;
}
SetConsoleTextAttribute(hOut, 15);
printf(newline);
}









/*MIS. FUNCTIONS SHARED BETWEEN ENCRYPTION & DECRYPTION*/

M create_key(){
int x;
M key = calloc(6,sizeof(double*));
for (x=0;x<6;x++){key[x] = calloc(6, sizeof(double));}

return key;
}

void create_extensionMatrix(){
int col,row;
extensionMatrix = calloc(6,sizeof(double*));
for (col=0;col<6;col++){extensionMatrix[col] = calloc(6, sizeof(double));}
if (diagonalSum!=0){
    for (col=0;col<6;col++){
        for (row=0;row<6;row++){
           extensionMatrix[col][row] = key[col][row] + diagonalSum;
        }
    }
}
else{
    for (col=0;col<6;col++){
        for (row=0;row<6;row++){
           extensionMatrix[col][row] = key[col][row] + secondaryDiagonalSum;
        }
    }
}
}

void delete_matrix(M matrix){
int x;
for (x=0;x<6;x++){free(matrix[x]);}
free(matrix);
}

void deleteColumnMatrix(M columnMatrix){
int i;
for(i=0;i<6;i++){free(columnMatrix[i]);}
free(columnMatrix);
}

void emptyArray(char *theArray){
    int i=0;
    while(theArray[i]){
        theArray[i]=0;
        i++;
    }
}

void resetSymbolsSubstitutionTable(){
    symbolsSubstitutionTable[0]= '*';
    symbolsSubstitutionTable[1]= '#';
    symbolsSubstitutionTable[2]= '&';
    symbolsSubstitutionTable[3]= '%';
    symbolsSubstitutionTable[4]= '$';
    symbolsSubstitutionTable[5]= '+';
    symbolsSubstitutionTable[6]= '?';
    symbolsSubstitutionTable[7]= '!';
    symbolsSubstitutionTable[8]= '@';
    symbolsSubstitutionTable[9]= '^';
    symbolsSubstitutionTable[10]= '-';

}

void resetSubstitutionTable(){
int i;
for (i=0;i<74;i++){
    substition_Table[i]=0;
}

}








/*ENCRYPTION*/

/*THE KEY*/
int keyMatrixEnc(){ // The Key Matrix For Encryption

HANDLE hOutput = GetStdHandle(STD_OUTPUT_HANDLE);
int doneKey =0;

while (!doneKey){

        system("cls"); // when choose to Cancel
        printf("\n");
        SetConsoleTextAttribute(hOutput, 3);
        printf("==================================================================\n");
        printf("\t\t\t     KEY");
        printf("\n==================================================================\n\n");

        int x=0,y=0,option=0;
        key = create_key();

        option= HMenu(1, -1, 5, ">", 15, "|", 9, 15, 128, 4, "Generate Key", "Type Key", "Load Key From File", "Main Menu");

        switch (option){
                case 1:
                    doneKey=generateKey();
                    if(doneKey==1){return 1;}
                    break;

                case 2:
                    doneKey=insertKey(key);
                    if(doneKey==1){return 1;}
                    else{delete_matrix(key);}
                    break;

                case 3:
                    doneKey=loadKey();
                    if(doneKey==1){return 1;}
                    else {delete_matrix(key);}
                    break;

                case 4:
                    return -1;
        }
}

return 0;
}

int generateKey(){ // Generate Key Randomly for the User

printf("\n");
determinant = calloc(1, sizeof(double));
int option = HMenu(1, -1, 4, ">", 15, "|", 9, 15, 128, 4, "Positive Integers", "Negative Integers", "Pos. & Neg.", "Cancel");
int x,y, option1 = 1, vpos = -1;
switch (option){
    case 1:
        while(option1 == 1){ // while Re-generate is chosen
              diagonalSum=0; *determinant=0;
              while(diagonalSum==0||*determinant==0||gcd((int)*determinant)==1){ // secondaryDiagonalSum is ignored here because we have plenty of possibilities before taking it into account
                    for(x=0;x<6;x++){
                        for(y=0;y<6;y++){
                            key[x][y] = controlledRandom();
                            if(x==y){diagonalSum += key[x][y];}
                        }
                    }
                    determinant = mDet(6,"lf", key);
              }

              vpos = drawKey(vpos, key);
              printf("\n");
              option1 = HMenu(1, -1, 5, ">", 15, "|", 9, 15, 128,4, "Re-generate", "Save to File", "Continue Without Save", "Cancel");
        }

        if(option1 == 2){if(saveKey(key)){return 1;} else {return 0;}}
        else if(option1 == 3){return 1;}
        else if (option1 == 4){ delete_matrix(key); free(determinant); diagonalSum=0; return 0;}

    case 2:
        while(option1 == 1){
              diagonalSum=0; *determinant=0;
              while(diagonalSum==0 || *determinant==0 || gcd((int)*determinant)==1){
                    for(x=0;x<6;x++){
                        for(y=0;y<6;y++){
                            key[x][y] = controlledRandom() * -1;
                            if(x==y){diagonalSum += key[x][y];}
                        }
                    }
                    determinant = mDet(6,"lf", key);
              }
              vpos = drawKey(vpos, key);
              printf("\n");
              option1 = HMenu(1, -1, 5, ">", 15, "|", 9, 15, 128,4, "Re-generate", "Save to File", "Continue Without Save", "Cancel");
        }

        if(option1 == 2){if(saveKey(key)){return 1;} else {return 0;}}
        else if(option1 == 3){return 1;}
        else if (option1 == 4){ delete_matrix(key); free(determinant); diagonalSum=0; return 0;}

    case 3:
        while(option1 == 1){
              diagonalSum=0; *determinant=0;
              while(diagonalSum==0 || *determinant==0|| gcd((int)*determinant)==1){
                  for(x=0;x<6;x++){
                    for(y=0;y<6;y++){
                        key[x][y] = controlledRandom() - controlledRandom();
                        if(x==y){diagonalSum += key[x][y];}
                    }
                  }
                  determinant = mDet(6,"lf", key);
              }
              vpos = drawKey(vpos, key);
              printf("\n");
              option1 = HMenu(1, -1, 5, ">", 15, "|", 9, 15, 128,4, "Re-generate", "Save to File", "Continue Without Save", "Cancel");
            }
        if(option1 == 2){if(saveKey(key)){return 1;} else {return 0;}}
        else if(option1 == 3){return 1;}
        else if (option1 == 4){ delete_matrix(key); free(determinant); diagonalSum=0; return 0;}

    case 4:
        return 0;
}
return 0;
}

int insertKey(M key){ // When The User Inserts The Key Typing on The Keyboard (Important: Shared between Encryption & Decryption!)

HANDLE hOutput = GetStdHandle(STD_OUTPUT_HANDLE);
int attempts=0;
while(attempts<6){ // number of attempts
    printf("\n Insert Each Integer Separated by a Comma or a Space then Press Enter:\n\n");
    bool detect_non_digit = false;
    char line[256];
    char *line_pointer = line;
    char *ptr = line;
    long numbers[37]; // should hold the 36 integers for the key
    int i=0; // index for number array
    printf(" ");
    SetConsoleTextAttribute(hOutput,8);
    if (fgets(line, sizeof(line), stdin)){ // reads the entire line and check for invalid input (non-digit except comma, space and minus sign)
    SetConsoleTextAttribute(hOutput,15);
        while (*ptr != '\n'){
              if (!(isdigit(*ptr))){
                 if (*ptr != ' ' && *ptr != ',' && *ptr != '-'){ // only allowed from "Non-digits"
                    detect_non_digit = true;
                    break;
                 }
              }
              ptr++;
        }

        if (detect_non_digit == false){
            *ptr = ',';
             ptr++;
            *ptr= '\n';
            while (*line_pointer!='\n'){
                    int x =0;
                    while(*line_pointer!=',' && *line_pointer!=' '){
                            char eachInput[25];
                            eachInput[x] = *line_pointer;
                            x++;
                            line_pointer++;
                            if (*line_pointer==',' || *line_pointer==' ' || *line_pointer =='-'){ // that means it's the end of all digits of each number
                            eachInput[x] = '\0';
                            numbers[i]= strtol(eachInput , NULL, 10);
                            i++;
                        }
                }
                line_pointer++;
            }
            if (i==0) {return 0;} // that means user did not enter any input
            else if (i==1) {SetConsoleTextAttribute(hOutput,1);
                            printf("\n   /You Have Inserted Only 1 Integer, Please Insert 36 Integers!/\n");
                            SetConsoleTextAttribute(hOutput, 15);}
            else if (i<36){ SetConsoleTextAttribute(hOutput, 1);
                            printf("\n   /You Have Inserted Only %d Integers, Please Insert 36 Integers!/\n", i);
                            SetConsoleTextAttribute(hOutput, 15);}
            else if (i>36) {SetConsoleTextAttribute(hOutput, 1);
                            printf("\n   /You Have Inserted %d Integers, Please Insert Only 36 Integers!/\n", i);
                            SetConsoleTextAttribute(hOutput, 15);}
            else if (i==36){

                    int col,row, y;
                    for (col=0,y=0;col<6;col++){
                        for(row=0;row<6;row++,y++){
                            key[col][row] = (double)numbers[y]; // fill the Key
                        }
                    }
                    if (validateKey(key)){SetConsoleTextAttribute(hOutput,3); // then validate for determinant, DiagonalSum, and secondaryDiagonalSum
                                            printf("\n");
                                           for (col=0;col<6;col++,printf("\n")){
                                            for(row=0;row<6;row++){
                                                printf("   %.0lf", key[col][row]);
                                            }
                                        }
                                        if(saveInsertedKey(key)){return 1;}
                                        else{return 0;}
                    }
                    else {SetConsoleTextAttribute(hOutput,12);
                          printf("\n\tInvalid Key!\n");
                          SetConsoleTextAttribute(hOutput, 15);}
            }
      }

      else if (detect_non_digit == true) {SetConsoleTextAttribute(hOutput,12); printf("\n\tInvalid Input!\n"); SetConsoleTextAttribute(hOutput, 15);}

    }


attempts++;
if(attempts==6){SetConsoleTextAttribute(hOutput, 12); printf("\n\t***Failure to Receive Valid Input, No More Retries***\n"); SetConsoleTextAttribute(hOutput, 15); Sleep(1600); return 0;}
}
}

int loadKey(){ // When The User Choose To Load The Key from a File (Important: Shared between Encryption & Decryption!)

HANDLE hOutput = GetStdHandle(STD_OUTPUT_HANDLE);
int attempts=0;
while(attempts<6){ // number of attempts allowed
      printf("\n");
      bool enter = true; // reference if the "Enter" Key has been pressed without any input
      bool not36 = false; // reference for the number of the Key elements
      int option = HMenu(7, -1, 4, ">", 15, "|", 9, 15, 128, 3, "Load from Desktop", "Load from Directory", "Cancel");
      char line[256];
      char *line_ptr = line;

      switch(option){
            case 1:
                printf("\n\tInsert The File Name: ");
                if(scanf("%256[0-9a-zA-Z ,.'!@#$%^&()-_=+~{[]}; ]", line)){
                    int x,y;
                    FILE *fp;
                    /*Works on Windows Only!*/
                    TCHAR szPath[MAX_PATH];
                    SHGetFolderPath(NULL,CSIDL_DESKTOPDIRECTORY,NULL,0,szPath);
                    strcat(szPath,"\\");strcat(szPath, line);strcat(szPath,".txt"); // concatenate to form a proper directory
                    fp = fopen(szPath, "r");
                    if (fp == NULL) {SetConsoleTextAttribute(hOutput,1);
                                    printf("\n\tCould not Find the File!\n");
                                    SetConsoleTextAttribute(hOutput,15);
                                    /*free the stdin buffer*/
                                    while(getchar() != '\n');  enter=false; break;}
                    else {
                            for(x=0;x<6;x++){ // attempt to read the file and fill the Key
                                for(y=0;y<6;y++){
                                    if(fscanf(fp, "%lf", &key[x][y])<1){ // if could not read
                                            fclose(fp);
                                            not36 = true; // report that could not read 36 integers
                                            x=6;
                                            break;

                                    }
                                }
                             }

                            if(validateKey(key) && (!not36)){ // if it's o.k
                                    printf("\n");
                                    fclose(fp);
                                    int option1 = HMenu(12, -1, 5, ">", 15, "|", 9, 15, 128, 3, "Accept", "Load Different Key", "Cancel");
                                    switch(option1){
                                        case 1:
                                            while(getchar()!='\n');
                                            drawKeyFromFile(4, -1,key);
                                            return 1;
                                        case 2:
                                            while(getchar()!='\n');
                                            delete_matrix(key);
                                            key = create_key();
                                            loadKey();
                                            return 1;
                                        case 3:
                                            while(getchar()!='\n');
                                            return 0;
                                    }
                            }
                            else {
                                    fclose(fp);
                                    SetConsoleTextAttribute(hOutput,12);
                                    printf("\n\tInvalid Key!\n");
                                    SetConsoleTextAttribute(hOutput,15);
                                    break;
                            }

                   }
                }
                else {while(getchar() != '\n'); enter=false; break;} // if the user hit Enter without inserting anything

                break;

            case 2:
                printf("\n\tInsert The Directory: ");
                if(scanf("%256[0-9a-zA-Z:.\\/ ~$@!""£%^&*()_-+=|?<>'#;={[]}]", line)){ // try to be able to read everything, don't judge now
                        int x,y;
                        FILE *fp;
                        bool dir = false; // reference if the input looks to be valid directory
                        while (*line_ptr!='\0'){
                              if(*line_ptr == '\\' || *line_ptr == '/' || *line_ptr == ':'){ // indicates that a directory may has been inserted
                                 dir = true;
                                 break;
                               }

                              line_ptr++;
                        }
                        if (dir == true){
                            fp = fopen(line, "r");
                            if (fp == NULL) {
                                    SetConsoleTextAttribute(hOutput,1);
                                    printf("\n\tCould not Find the File!\n");
                                    SetConsoleTextAttribute(hOutput,15);
                                    while(getchar() != '\n'); enter=false; break;}
                            else {
                                    for (x=0;x<6;x++){
                                        for(y=0;y<6;y++){
                                            if(fscanf(fp, "%lf", &key[x][y])<1){ // because when it reads it returns 1
                                                    not36 = true;
                                                    fclose(fp);
                                                    x=6;
                                                    break;
                                            }
                                        }
                                    }

                                    if(validateKey(key) && (!not36)){ // check validity of the Key
                                                printf("\n");
                                                fclose(fp);
                                                int option1 = HMenu(12, -1, 5, ">", 15, "|", 9, 15, 128, 3, "Accept", "Load Different Key", "Cancel");
                                                switch(option1){
                                                    case 1:
                                                        while(getchar()!='\n');
                                                        drawKeyFromFile(4, -1,key);
                                                        return 1;
                                                    case 2:
                                                        while(getchar()!='\n');
                                                        delete_matrix(key);
                                                        key = create_key();
                                                        loadKey();
                                                        return 1;

                                                    case 3:
                                                        while(getchar()!='\n');
                                                        return 0;
                                                }

                                        }
                                    else {
                                            fclose(fp);
                                            SetConsoleTextAttribute(hOutput,12);
                                            printf("\n\tInvalid Key!\n");
                                            SetConsoleTextAttribute(hOutput,15);
                                            break;
                                    }
                            }
                        }

                        else {SetConsoleTextAttribute(hOutput,12); printf("\n\tInvalid Directory!\n"); SetConsoleTextAttribute(hOutput,15); break;}
                }
                else {/*free Stdin*/ while(getchar() != '\n'); enter=false; break;} // if could not read any input
                break;

            case 3:
                return 0;
    }

if(enter){while(getchar()!='\n');}

attempts++;
if (attempts==6){SetConsoleTextAttribute(hOutput,12);printf("\n\t***Failure to receive any valid input, No More Retries***\n");SetConsoleTextAttribute(hOutput,15);Sleep(1600); return 0;}
}
}

int saveKey(M key){ // To Save The Key Generated By The Program on the User's Computer

HANDLE hOutput = GetStdHandle(STD_OUTPUT_HANDLE);
int attempts=0;
while (attempts<6){ //number of attempts allowed
    printf("\n");
    bool enter = true;
    char line[256];
    int option = HMenu(6, -1, 5, ">", 15, "|", 9, 15, 128, 3, "Save on Desktop", "Save to Directory", "Cancel");

    switch (option){
          case 1:
                printf("\n\tInsert The Key File Name: ");
                if(scanf("%256[0-9a-zA-Z ,.'!@#$%^&()-_=+~{[]}; ]", line)){
                        int x,y;
                        FILE *fp;
                        TCHAR szPath[MAX_PATH];
                        SHGetFolderPath(NULL,CSIDL_DESKTOPDIRECTORY,NULL,0,szPath);
                        strcat(szPath,"\\");strcat(szPath, line);strcat(szPath,".txt");
                        fp = fopen(szPath, "w");
                        if (fp == NULL) {SetConsoleTextAttribute(hOutput, 1);
                                         printf("\n\tCould not Create the File!\n");
                                         SetConsoleTextAttribute(hOutput, 15);
                                         while(getchar() != '\n'); enter=false; break;}
                        else {
                        for (x=0;x<6;x++,fprintf(fp,"\n")){
                             for(y=0;y<6;y++){
                                 fprintf(fp, "%.0lf\t", key[x][y]);
                             }
                        }

                        fclose(fp);
                        printf("\n\tDONE!\n");
                        while(getchar() != '\n'); // to free Stdin
                        return 1;
                        }
                }
                else {SetConsoleTextAttribute(hOutput, 12);
                      printf("\n\tInvalid File Name!\n");
                      SetConsoleTextAttribute(hOutput, 15);
                      while(getchar() != '\n'); enter=false; break;}


        case 2:
                printf("\n\tInsert The Directory: ");
                if(scanf("%256[0-9a-zA-Z :.\\/~$@!""£%^&*()_-+=|?<>'#;{[]} ]", line)){
                        char *line_ptr = line;
                        bool dir = false;
                        while (*line_ptr!='\0'){
                        if(*line_ptr == '\\' || *line_ptr == '/' || *line_ptr == ':'){ // indicates that a directory may has been inserted
                            dir = true;
                            break;
                        }

                        line_ptr++;
                        }
                        if (dir==true){
                            int x,y;
                            FILE *fp;
                            fp = fopen(line, "w");
                            if (fp == NULL) {SetConsoleTextAttribute(hOutput, 1);
                                             printf("\n Could not Create the File!\n");
                                             SetConsoleTextAttribute(hOutput, 15);
                                             while(getchar() != '\n'); enter=false; break;}
                            else{
                                for (x=0;x<6;x++,fprintf(fp,"\n")){
                                     for(y=0;y<6;y++){
                                         fprintf(fp, "%.0lf\t", key[x][y]);
                                    }
                                }
                                fclose(fp);
                                printf("\n\tDONE!\n");
                                while(getchar() != '\n'); // to free Stdin
                                return 1;
                            }
                        }
                      else {SetConsoleTextAttribute(hOutput, 12); printf("\n\tInvalid Directory\n"); SetConsoleTextAttribute(hOutput, 15); break;}
              }
              else {while(getchar() != '\n'); enter=false; break;}

        case 3:
            return 0;
    }
    if(enter) {while(getchar()!='\n');}
    attempts++;
    if (attempts==6){SetConsoleTextAttribute(hOutput, 12);printf("\n***Failure To Receive Valid Input, No More Retries***\n");SetConsoleTextAttribute(hOutput, 15);Sleep(1600); return 0;}
}
}

int saveInsertedKey(M key){ // To Save The Key Inserted By User on the User's Computer

HANDLE hOutput = GetStdHandle(STD_OUTPUT_HANDLE);
int attempts=0;
while (attempts<6){ //number of attempts allowed
       printf("\n");
       bool enter = true;
       int option = HMenu(1, -1, 4, ">", 15, "|", 9, 15, 128, 4, "Save on Desktop", "Save to Directory", "Continue Without Save", "Cancel");
       char line[256];
       switch (option){
          case 1:
                printf("\n\tInsert The Key File Name: ");
                if(scanf("%256[0-9a-zA-Z ,.'!@#$%^&()-_=+~{[]}; ]", line)){
                        int x,y;
                        FILE *fp;
                        TCHAR szPath[MAX_PATH];
                        SHGetFolderPath(NULL,CSIDL_DESKTOPDIRECTORY,NULL,0,szPath);
                        strcat(szPath,"\\");strcat(szPath, line);strcat(szPath,".txt");
                        fp = fopen(szPath, "w");
                        if (fp == NULL) {SetConsoleTextAttribute(hOutput, 1);
                                         printf("\n\tCould not Create the File!\n");
                                         SetConsoleTextAttribute(hOutput, 15);
                                         while(getchar() != '\n'); enter=false; break;}
                        else {
                        for (x=0;x<6;x++,fprintf(fp,"\n")){
                             for(y=0;y<6;y++){
                                 fprintf(fp, "%.0lf\t", key[x][y]);
                             }
                        }

                        fclose(fp);
                        printf("\n\tDONE!\n");
                        while(getchar() != '\n'); // to free Stdin
                        return 1;
                        }
                }
                else {SetConsoleTextAttribute(hOutput, 12);
                      printf("\n\tInvalid File Name!\n");
                      SetConsoleTextAttribute(hOutput, 15);
                      while(getchar() != '\n'); enter=false; break;}


        case 2:
                printf("\n\tInsert The Directory: ");
                if(scanf("%256[0-9a-zA-Z:.\\/~$@!""£%^&*()_-+=|?<>'#;{[]} ]", line)){
                        char *line_ptr = line;
                        bool dir = false;
                        while (*line_ptr!='\0'){
                        if(*line_ptr == '\\' || *line_ptr == '/' || *line_ptr == ':'){ // indicates that a directory may has been inserted
                            dir = true;
                            break;
                        }

                        line_ptr++;
                        }
                        if (dir==true){
                            int x,y;
                            FILE *fp;
                            fp = fopen(line, "w");
                            if (fp == NULL) {SetConsoleTextAttribute(hOutput, 1);
                                             printf("\n Could not Create the File!\n");
                                             SetConsoleTextAttribute(hOutput, 15);
                                             while(getchar() != '\n'); enter=false; break;}
                            else{
                                for (x=0;x<6;x++,fprintf(fp,"\n")){
                                     for(y=0;y<6;y++){
                                         fprintf(fp, "%.0lf\t", key[x][y]);
                                    }
                                }
                                fclose(fp);
                                printf("\n\tDONE!\n");
                                while(getchar() != '\n'); // to free Stdin
                                return 1;
                            }
                        }
                      else {SetConsoleTextAttribute(hOutput, 12); printf("\n\tInvalid Directory\n"); SetConsoleTextAttribute(hOutput, 15); break;}
              }
              else {while(getchar() != '\n'); enter=false; break;}

        case 3:
            return 1;

        case 4:
            return 0;
    }
    if(enter) {while(getchar()!='\n');}
    attempts++;
    if (attempts==6){SetConsoleTextAttribute(hOutput, 12); printf("\n***Failure To Receive Valid Input, No More Retries***\n");SetConsoleTextAttribute(hOutput, 15);Sleep(1600);return 0;}
}
}

int validateKey(M key){ // Validate Inserted Key By User or Uploaded From File (Important: Shared between Encryption & Decryption!)
int x, y;
determinant = calloc(1, sizeof(double));
diagonalSum = 0, secondaryDiagonalSum=0;
for (x=0;x<6;x++){
    for(y=0;y<6;y++){
          if (x==y){diagonalSum += key[x][y];}
          if ((y+x)==5){secondaryDiagonalSum += key[x][y];}
    }
}
determinant = mDet(6, "lf", key);
int greaterCommonDivisor = gcd(*determinant);

if (*determinant!=0 && diagonalSum!=0 && greaterCommonDivisor!=1){free(determinant); return 1;}
else if (*determinant!=0 && secondaryDiagonalSum!=0 && greaterCommonDivisor!=1){free(determinant); return 1;}
else {free(determinant); return 0;}
}

int drawKey(int vpos,  M key){ // Draw The Key that generated by the program on the Console
int x,y,i=3,j=1;
HANDLE hOutput = GetStdHandle(STD_OUTPUT_HANDLE); // general STDout Handle
if(vpos<0){
    /*Find The Cursor Position*/
    CONSOLE_SCREEN_BUFFER_INFO SBInfo;
    GetConsoleScreenBufferInfo(hOutput, &SBInfo);
    vpos = SBInfo.dwCursorPosition.Y;
}

COORD cursor;
for(x=0;x<6;x++,i=3,j+=2){
    for(y=0; y<6; y++){
        cursor.X =i;
        cursor.Y = vpos+j;
        SetConsoleCursorPosition(hOutput, cursor);
        SetConsoleTextAttribute(hOutput, 3);
        char numberLength[10];
        sprintf(numberLength, "%.0lf", key[x][y]);
        WriteConsoleA(hOutput, numberLength, strlen(numberLength), NULL, NULL);
        i+=7;
        WriteConsoleA(hOutput, "      ", 6, NULL, NULL);
    }
}
printf("\n\n");
return vpos;
}

int drawKeyFromFile(int indent, int vpos, M key){ // Draw The Key that loaded from a file on the Console (Important: Shared between Encryption & Decryption!)
printf("\n");
int x,y,i=3,j=1;
HANDLE hOutput = GetStdHandle(STD_OUTPUT_HANDLE); // general Stdout Handle
if(vpos<0){
    /*Find The Cursor Position*/
    CONSOLE_SCREEN_BUFFER_INFO SBInfo;
    GetConsoleScreenBufferInfo(hOutput, &SBInfo);
    vpos = SBInfo.dwCursorPosition.Y;
}

COORD cursor;
cursor.Y = vpos;
SetConsoleCursorPosition(hOutput, cursor);
SetConsoleTextAttribute(hOutput, 3);
        printf("\n\t");
        for (x=0;x<6;x++, printf("\n\t")){
            for(y=0;y<6;y++){
                printf("%.0lf\t", key[x][y]);
            }
        }
SetConsoleTextAttribute(hOutput,  15);
printf("\n\t  DONE!\n");
return vpos;
}

double controlledRandom(){
int upperlimits[300]; // 300 different upper limits
int x;
for (x=0;x<300;x++){ // generate upper limits
    upperlimits[x]= x+1;
}
double result = (double) (rand()%(upperlimits[rand()%300]));
return result;
}


/*THE SYMBOLS (IMPORTANT: ALL FUNCTIONS ARE USED IN BOTH ENCRYPTION & DECRYPTION!)*/
int symbols(){

HANDLE hOutput = GetStdHandle(STD_OUTPUT_HANDLE);
SetConsoleTextAttribute(hOutput, 2);
printf("\n==================================================================\n");
printf("\t\t\t     SYMBOLS");
printf("\n==================================================================\n\n");
SetConsoleTextAttribute(hOutput, 15);
int attempts =0;
while (attempts<6){
    int option = HMenu(8, -1, 5, ">", 15, "|", 9, 15, 128, 3, "Use Default", "Define New Symbols", "Cancel");
    switch(option){
        case 1:
            if(shiftSymbols(symbolsSubstitutionTable)){
                    printf("\t\t       ");
                    SetConsoleTextAttribute(hOutput, 2);
                    int i=0;
                    for(; i<11;i++){printf("%c ", symbolsSubstitutionTable[i]);}
                    SetConsoleTextAttribute(hOutput, 15);
                    printf("\n\n\tDONE!\n");
                    return 1;
            }
            break;

        case 2:
            printf("\n");
            int option1 = HMenu(9, -1, 5, ">", 15, "|", 9, 15, 128, 3, "Insert Symbols", "Load From File", "Back");
            switch(option1){
                case 1:
                    if(insertSymbols()){return 1;}
                    break;

                case 2:
                    if(loadSymbols()){return 1;}
                    break;

                case 3:
                    printf("\n");
                    break;
            }
            break;

        case 3:
            return 0;
}

attempts++;
if(attempts==6){SetConsoleTextAttribute(hOutput, 12);printf("\n\t**Failure to Receive Valid Input, No More Retries**\n\n");SetConsoleTextAttribute(hOutput, 15);Sleep(1600);return 0;}
}
}

int shiftSymbols(char *arrayOfSymbols){

printf("\n");
int i,repeat=0;
int shift = mod(sum_t(), 11);
for (i=0;i<11;i++){symbolsSubstitutionTable[i]=arrayOfSymbols[i];} // copy the arrayOfSymbols to symbolsSubstitutionTable
char lastChar = symbolsSubstitutionTable[10]; // to hold the last symbol in the array in every round/loop while doing the shift
while(repeat<shift){
        for(i=10; i>0; i--){
            if (i==10){lastChar = symbolsSubstitutionTable[10];} // hold the last char
            symbolsSubstitutionTable[i] = symbolsSubstitutionTable[i-1]; // do the shift
            if (i==1){symbolsSubstitutionTable[0] = lastChar;} // copy last char to the first index in the table
        }
        repeat++; // repeat
}
return 1;
}

int mod(double x , int y){

int result =  ((((((int)x)%(y))+(y))%(y)));
return result;
}

double sum_t(){ // Find diagonalSum or secondaryDiagonalSum
int x,y;
double sum=0;
for (x=0;x<6;x++){ // Find DiagonalSum
    for(y=0;y<6;y++){
        sum+=key[x][y];
    }
}
if(sum==0){ // then find the secondaryDiagonalSum
  for (x=0;x<6;x++){
    for(y=0;y<6;y++){
        if((x+y)==5){sum+=key[x][y];}
    }
}
}
return sum;
}

int loadSymbols(){

HANDLE hOutput = GetStdHandle(STD_OUTPUT_HANDLE);
int attempts =0;
while(attempts<6){ // number of attempts
    bool enter = true;
    char line[256];
    char *line_ptr = line;
    char symbolsFromFile[100];
    printf("\n");
    int option = HMenu(6, -1, 5, ">", 15, "|", 9, 15, 128, 3, "Load from Desktop", "Load from Directory", "Back");

    switch(option){
        case 1:
            printf("\n\tInsert The File Name: ");
            if(scanf("%256[0-9a-zA-Z ,.'!@#$%^&()-_=+~{[]}; ]", line)){ // for reading the file name to be found on the desktop
                FILE *fp;
                TCHAR szPath[MAX_PATH];
                SHGetFolderPath(NULL,CSIDL_DESKTOPDIRECTORY,NULL,0,szPath);
                strcat(szPath,"\\");strcat(szPath, line);strcat(szPath,".txt");
                fp = fopen(szPath, "r");
                if (fp == NULL) {
                    SetConsoleTextAttribute(hOutput, 1);
                    printf("\n\tCould not Find the File!\n");
                    SetConsoleTextAttribute(hOutput, 15);
                    break;
                }
                else {
                      fseek(fp, 0, SEEK_END);
                      long fsize = ftell(fp); // returns the current value of position indicator
                      if(fsize>0){
                              rewind(fp);
                              fread(symbolsFromFile, fsize, 1, fp);
                              fclose(fp);
                              symbolsFromFile[fsize] = 0; // nul terminator
                              int x=0,y=0;
                              while(symbolsFromFile[y]){
                                    if (symbolsFromFile[y]=='\t'){
                                        symbolsFromFile[x]= ' ';
                                        x++;
                                    }
                                    else if(symbolsFromFile[y]!='\n'){
                                        symbolsFromFile[x]=symbolsFromFile[y];
                                        x++;
                                    }
                                    else if(symbolsFromFile[y]=='\n'){
                                            symbolsFromFile[x] = ' ';
                                            x++;
                                    }
                                    y++;
                              }

                              if(validateSymbolsFromFile(symbolsFromFile)){shiftSymbols(symbolsSubstitutionTable); while(getchar()!='\n');return 1;}
                      }

                      else{
                            SetConsoleTextAttribute(hOutput, 1);
                            printf("\n\tEmpty File!\n");
                            SetConsoleTextAttribute(hOutput, 15);
                            break;
                      }
                }
            }


            else {while(getchar() != '\n'); enter=false; break;} // if no input
            break;

    case 2:
        printf("\n\tInsert The Directory: ");
        if(scanf("%256[0-9a-zA-Z:.\\/ ~$@!""£%^&*()_-+=|?<>'#;{[]}]", line)){ // try to be able to read everything, don't judge now
                int x,y;
                FILE *fp;
                bool dir = false;
                while (*line_ptr!='\0'){
                      if(*line_ptr == '\\' || *line_ptr == '/' || *line_ptr == ':'){ // indicates that a proper directory may has been inserted
                         dir = true;
                         break;
                       }
                       line_ptr++;
                }

                if (dir == true){
                    fp = fopen(line, "r");
                    if (fp == NULL) {
                            SetConsoleTextAttribute(hOutput, 1);
                            printf("\n\tCould not Find the File!\n");
                            SetConsoleTextAttribute(hOutput, 15);
                            break;
                    }
                    else {
                          fseek(fp, 0, SEEK_END);
                          long fsize = ftell(fp); // returns the current value of position indicator
                          if(fsize>0){
                              rewind(fp);
                              fread(symbolsFromFile, fsize, 1, fp);
                              fclose(fp);
                              symbolsFromFile[fsize] = 0; // nul terminator
                              int x=0,y=0;
                              while(symbolsFromFile[y]){
                                    if (symbolsFromFile[y]=='\t'){
                                        symbolsFromFile[x]= ' ';
                                        x++;
                                    }
                                    else if(symbolsFromFile[y]!='\n'){
                                        symbolsFromFile[x]=symbolsFromFile[y];
                                        x++;
                                    }
                                    else if(symbolsFromFile[y]=='\n'){
                                            symbolsFromFile[x] = ' ';
                                            x++;
                                    }
                                    y++;
                              }
                              if(validateSymbolsFromFile(symbolsFromFile)){shiftSymbols(symbolsSubstitutionTable); while(getchar()!='\n'); return 1;}
                          }
                          else{
                                SetConsoleTextAttribute(hOutput, 1);
                                printf("\n\tEmpty File!\n");
                                SetConsoleTextAttribute(hOutput, 15);
                                break;
                          }
                    }
                }
                else {
                        SetConsoleTextAttribute(hOutput, 12);
                        printf("\n\tInvalid Directory!\n");
                        SetConsoleTextAttribute(hOutput, 15);
                        break;
                }
          }

        else {while(getchar() != '\n'); enter=false; break; } // if not entry/input
        break;

    case 3:
           printf("\n");
           return 0;
    }

if(enter){while(getchar()!='\n');}
attempts++;
if(attempts==6){SetConsoleTextAttribute(hOutput, 12);printf("\n\t***Failure to receive any valid input, No More Retries***\n\n");SetConsoleTextAttribute(hOutput, 15);Sleep(1600);return 0;}
}
}

int validateSymbolsFromFile(char *symbolsFromFile){ // Validate and print out the Symbols loaded from a file

HANDLE hOutput = GetStdHandle(STD_OUTPUT_HANDLE);
bool detect_invalid = false; // reference for invalid input (i.e letter)
bool detect_duplicate = false;
int numberOfSymbols =0 ,noOfSymbIncludeSpaces=0, i=0, j=0;

while (symbolsFromFile[i]){
        if (isalpha(symbolsFromFile[i])){
             detect_invalid = true;
              break;
        }
        if(symbolsFromFile[i]!=' '){numberOfSymbols++;} // move around the spaces if found
        i++;
        noOfSymbIncludeSpaces++;
}

i=0;
for (j=0;j<noOfSymbIncludeSpaces;j++){
    for (i=0;i<noOfSymbIncludeSpaces;i++){
        if (j!=i && symbolsFromFile[i]!=0 && symbolsFromFile[j]!=0 && symbolsFromFile[i]!=' ' && symbolsFromFile[j]!=' '){
            if (symbolsFromFile[j]==symbolsFromFile[i]){
                detect_duplicate = true;
                j=noOfSymbIncludeSpaces;
                break;
            }
        }
    }
}

if (detect_invalid == false && numberOfSymbols==11 && detect_duplicate == false){ // if everything seems to be o.k
    SetConsoleTextAttribute(hOutput, 2);
    int x =0;
    printf("\n\t\t");
    while(symbolsFromFile[x]){// cycle through the entire array
          if (symbolsFromFile[x]!=' '){ // if it is not a space then pick it up
              symbolsSubstitutionTable[x]=symbolsFromFile[x];
              printf("%c ",symbolsSubstitutionTable[x]);
          }
          x++;
    }
    symbolsSubstitutionTable[x]=0; // ad nul terminator
    SetConsoleTextAttribute(hOutput, 15);
    printf("\n\n\tDONE!\n");
    return 1;
}

else if (detect_invalid == true) {
         SetConsoleTextAttribute(hOutput, 12);
         printf("\n  Invalid Input! Letters are not Allowed!\n");
         SetConsoleTextAttribute(hOutput, 15);
         return 0;
}
else if (detect_duplicate == true){
         SetConsoleTextAttribute(hOutput, 12);
         printf("\n  Invalid Input! Duplicate Symbol Detected!\n");
         SetConsoleTextAttribute(hOutput, 15);
         return 0;
}
else if (numberOfSymbols==0) {return 0;} // that means user did not enter anything and user wants to cancel
else if (numberOfSymbols==1) {
         SetConsoleTextAttribute(hOutput, 1);
         printf("\n  The File Contains Only 1 Symbol, 11 Symbols are Required!\n");
         SetConsoleTextAttribute(hOutput, 15);
         return 0;
}
else if (numberOfSymbols<11){
         SetConsoleTextAttribute(hOutput, 1);
         printf("\n  The File Contains Only %d Symbols, 11 Symbols are Required!\n", numberOfSymbols);
         SetConsoleTextAttribute(hOutput, 15);
         return 0;
}
else if (numberOfSymbols>11){
         SetConsoleTextAttribute(hOutput, 1);
         printf("\n  The File Contains %d Symbols, Only 11 Symbols are Required!\n", numberOfSymbols);
         SetConsoleTextAttribute(hOutput, 15);
         return 0;
}
}

int insertSymbols(){

HANDLE hOutput = GetStdHandle(STD_OUTPUT_HANDLE);
int attempts=0;
while(attempts<6){
    bool detect_invalid = false;
    bool detect_duplicate = false;
    char line[256];
    char *line_pointer = line;
    char *ptr = line;
    int numberOfSymbols =0, noOfSymbIncludeSpaces =0;

    printf("\n\t Insert the Symbols. When You Finish Press Enter:\n\t  ");
    SetConsoleTextAttribute(hOutput,8);
    if (fgets(line, sizeof(line), stdin)){ // reads the entire line and check for invalid input (letters, minus sign)
        SetConsoleTextAttribute(hOutput,15);
        while (*ptr != '\n'){
              if ((isalpha(*ptr))){
                    detect_invalid = true;
                    break;
                 }

              else if(*ptr != ' ' && *ptr != '\t'){numberOfSymbols++;} // in case user separated them by spaces or commas
              ptr++;
              noOfSymbIncludeSpaces++;

        }

        *ptr = '\n';
        int i=0, j=0;
        for (j=0;j<noOfSymbIncludeSpaces;j++){
            for (i=0;i<noOfSymbIncludeSpaces;i++){
                if (j!=i && line[i]!=0 && line[j]!=0 && line[i]!=' ' && line[j]!=' '){
                    if (line[j]==line[i]){
                        detect_duplicate = true;
                        j=noOfSymbIncludeSpaces;
                        break;
                    }
                }
            }
        }

        if (detect_invalid == false && numberOfSymbols==11 && detect_duplicate == false){ // if everything seems to be o.k
            int i =0;
            while (*line_pointer!='\n'){
                  if(*line_pointer!=' ' && *line_pointer != '\t'){ // in case user separated them by spaces or commas
                        symbolsSubstitutionTable[i] = *line_pointer;
                        i++;
                        line_pointer++;
                  }
                  else{line_pointer++;}
            }
            if(saveInsertedSymbols()){shiftSymbols(symbolsSubstitutionTable); return 1;}
        }
        else if (detect_invalid == true) {
                 SetConsoleTextAttribute(hOutput, 12);
                 printf("\n   Invalid Input, Letters are not Allowed!\n");
                 SetConsoleTextAttribute(hOutput, 15);
        }
        else if (detect_duplicate == true){
                 SetConsoleTextAttribute(hOutput, 12);
                 printf("\n  Invalid Input! Duplicate Symbol Detected!\n");
                 SetConsoleTextAttribute(hOutput, 15);
        }
        else if (numberOfSymbols==0) {return 0;} // that means user did not insert anything
        else if (numberOfSymbols==1) {
                 SetConsoleTextAttribute(hOutput, 1);
                 printf("\n   You Have Inserted Only 1 Symbol, Please Insert 11 Symbols\n");
                 SetConsoleTextAttribute(hOutput, 15);
        }
        else if (numberOfSymbols<11){
                 SetConsoleTextAttribute(hOutput, 1);
                 printf("\n   You Have Inserted Only %d Symbols, Please Insert 11 Symbols\n", numberOfSymbols);
                 SetConsoleTextAttribute(hOutput, 15);
        }
        else if (numberOfSymbols>11){
                 SetConsoleTextAttribute(hOutput, 1);
                 printf("\n   You Have Inserted %d Symbols, Please Insert Only 11 Symbols\n", numberOfSymbols);
                 SetConsoleTextAttribute(hOutput, 15);
        }
   }
attempts++;
if(attempts==6){SetConsoleTextAttribute(hOutput, 12);printf("\n  **Failure to Receive Valid Input, No More Retries**\n\n");SetConsoleTextAttribute(hOutput, 15);Sleep(1600);return 0;}
}
}

int saveInsertedSymbols(){

HANDLE hOutput = GetStdHandle(STD_OUTPUT_HANDLE);
int attempts=0;
while (attempts<6){
    bool enter = true;
    printf("\n");
    int option = HMenu(1, -1, 4, ">", 15, "|", 9, 15, 128, 4, "Save on Desktop", "Save to Directory", "Continue Without Save", "Cancel");
    char line[256];
    switch (option){
          case 1:
                printf("\n\tInsert The Symbols File Name: ");
                if(scanf("%256[0-9a-zA-Z ,.'!@#$%^&()-_=+~{[]}; ]", line)){
                        int x,y;
                        FILE *fp;
                        TCHAR szPath[MAX_PATH];
                        SHGetFolderPath(NULL,CSIDL_DESKTOPDIRECTORY,NULL,0,szPath);
                        strcat(szPath,"\\");strcat(szPath, line);strcat(szPath,".txt");
                        fp = fopen(szPath, "w");
                        if (fp == NULL) {SetConsoleTextAttribute(hOutput, 1);
                                         printf("\n\tCould not Create the File!\n");
                                         SetConsoleTextAttribute(hOutput, 15);
                                         while(getchar() != '\n'); enter=false; break;}
                        else {
                              fprintf(fp, "%s", symbolsSubstitutionTable);
                              fclose(fp);
                              printf("\n\tDONE!\n");
                              while(getchar() != '\n'); // to free Stdin
                              return 1;
                        }
                }
                else {SetConsoleTextAttribute(hOutput, 12);
                      printf("\n\tInvalid File Name!\n");
                      SetConsoleTextAttribute(hOutput, 15);
                      while(getchar() != '\n'); enter=false; break;}


        case 2:
                printf("\n\tInsert The Directory: ");
                if(scanf("%256[0-9a-zA-Z:.\\/~$@!""£%^&*()_-+=|?<>'#;{[]} ]", line)){
                        char *line_ptr = line;
                        bool dir = false;
                        while (*line_ptr!='\0'){
                        if(*line_ptr == '\\' || *line_ptr == '/' || *line_ptr == ':'){ // indicates that a directory may has been inserted
                            dir = true;
                            break;
                        }

                        line_ptr++;
                        }
                        if (dir==true){
                            int x,y;
                            FILE *fp;
                            fp = fopen(line, "w");
                            if (fp == NULL) {SetConsoleTextAttribute(hOutput, 1);
                                             printf("\n\tCould not Create the File!\n");
                                             SetConsoleTextAttribute(hOutput, 15);
                                             while(getchar() != '\n'); enter=false; break;}
                            else{
                                fprintf(fp, "%s", symbolsSubstitutionTable);
                                fclose(fp);
                                printf("\n\tDONE!\n");
                                while(getchar() != '\n'); // to free Stdin
                                return 1;
                            }
                        }
                      else {SetConsoleTextAttribute(hOutput, 12); printf("\n\tInvalid Directory\n"); SetConsoleTextAttribute(hOutput, 15); break;}
              }
              else {while(getchar() != '\n'); enter=false; break;}

        case 3:
            return 1;

        case 4:
            return 0;
    }
    if(enter) {while(getchar()!='\n');}
    attempts++;
    if (attempts==6){SetConsoleTextAttribute(hOutput, 12); printf("\n\t***Failure To Receive Valid Input, No More Retries***\n"); SetConsoleTextAttribute(hOutput, 15); Sleep(1600); return 0;}
}
}


/*PLAIN TEXT IN ENCRYPTION*/
int plainTextFun(){

HANDLE hOutput = GetStdHandle(STD_OUTPUT_HANDLE);
SetConsoleTextAttribute(hOutput, 11);
printf("\n========================================================================\n");
printf("\t\t\t  PLAIN TEXT\t\t\t\t\t");
printf("\n========================================================================\n\n");
SetConsoleTextAttribute(hOutput, 15);
int option=0;
while(option!=3){
    option = HMenu(6, -1, 5, ">", 15, "|", 9, 15, 128, 3, "Type Plain-Text", "Load From a File", "Cancel");
    switch(option){
        case 1:
            if(typePlainText()){return 1;}
            printf("\n");
            break;

        case 2:
            if(loadPlainText()){return 1;}
            printf("\n");
            break;

        case 3:
            return 0;
    }
}
}

int typePlainText(){ // when user choose to type the plain text on the console
HANDLE hOutput = GetStdHandle(STD_OUTPUT_HANDLE);
int option =2, i=0;
while (option==2){
    printf("\n     Type The Plain-Text. When You Finish Press Enter:\n\n ");
    SetConsoleTextAttribute(hOutput, 11);
    if(fgets(plain_Text, sizeof(plain_Text),stdin)){
            SetConsoleTextAttribute(hOutput, 15);
            i = plainTextLength();
            if (i>0){
                int y=0;
                          while(plain_Text[y]){
                                if(plain_Text[y]=='\n'){
                                    plain_Text[y]=0;
                                }
                                else if (plain_Text[y]=='\t'){ //replace every tab with a space
                                    plain_Text[y] = ' ';
                                }
                                y++;
                          }
                printf("\n");
                if (y>1){
                option = HMenu(12, -1, 5, ">", 15, "|", 9, 15, 128, 3, "Encrypt", "Change", "Back");
                switch(option){

                case 1:
                    return 1;

                case 2:
                    break;

                case 3:
                    return 0;
                }
              }
              else {SetConsoleTextAttribute(hOutput, 1);printf("\tEmpty Input!\n"); SetConsoleTextAttribute(hOutput, 15);return 0;}
            }
    }
}
}

int loadPlainText(){
HANDLE hOutput = GetStdHandle(STD_OUTPUT_HANDLE);
int attempts =0;
while(attempts<6){ // number of attempts
    bool enter = true;
    char line[256];
    char *line_ptr = line;

    printf("\n");
    int option = HMenu(5, -1, 5, ">", 15, "|", 9, 15, 128, 3, "Load from Desktop", "Load from Directory", "Back");

    switch(option){
        case 1:
            printf("\n\tInsert The File Name: ");
            if(scanf("%256[0-9a-zA-Z ,.'!@#$%^&()-_=+~{[]}; ]", line)){ // for reading the file name to be found on the desktop
                FILE *fp;
                TCHAR szPath[MAX_PATH];
                SHGetFolderPath(NULL,CSIDL_DESKTOPDIRECTORY,NULL,0,szPath);
                strcat(szPath,"\\");strcat(szPath, line);strcat(szPath,".txt");
                fp = fopen(szPath, "r");
                if (fp == NULL) {
                                SetConsoleTextAttribute(hOutput, 1);
                                printf("\n\tCould not Find the File!\n");
                                SetConsoleTextAttribute(hOutput, 15);
                                break;
                }
                else {
                      fseek(fp, 0, SEEK_END);
                      long fsize = ftell(fp); // returns the current value of position indicator
                      if(fsize>0){
                          rewind(fp);
                          fread(plain_Text, fsize, 1, fp);
                          fclose(fp);
                          plain_Text[fsize] = 0; // nul terminator
                          int x=0,y=0;
                          while(plain_Text[y]){ // this handle the file content if the user is playing with me :D
                                if(plain_Text[y]=='\t'){
                                        plain_Text[x] = ' ';
                                        x++;
                                }
                                else if(plain_Text[y]=='\n'){
                                        plain_Text[x] = ' ';
                                        x++;
                                }
                                else if(plain_Text[y]!='\n'){
                                    plain_Text[x]=plain_Text[y];
                                    x++;
                                }


                                y++;
                          }
                          SetConsoleTextAttribute(hOutput, 11);
                          printf("\n%s\n", plain_Text);
                          SetConsoleTextAttribute(hOutput, 15);
                          printf("\n");
                          option = HMenu(12, -1, 5, ">", 15, "|", 9, 15, 128, 3, "Encrypt", "Change", "Back");
                          switch(option){
                                case 1:
                                    while(getchar() != '\n');
                                    return 1;

                                case 2:
                                    while(getchar() != '\n');
                                    break;

                                case 3:
                                    while(getchar()!='\n');
                                    return 0;
                        }
                      }
                      else{SetConsoleTextAttribute(hOutput, 1);printf("\n\tEmpty File!\n"); SetConsoleTextAttribute(hOutput, 15);break;}
                }
           }


            else {while(getchar() != '\n');  enter=false; break;}
            break;

        case 2:
            printf("\n\tInsert The Directory: ");
            if(scanf("%256[0-9a-zA-Z:.\\/ ~$@!""£%^&*()_-+=|?<>'#;{[]}]", line)){
                    int x,y;
                    FILE *fp;
                    bool dir = false;
                    while (*line_ptr!='\0'){
                          if(*line_ptr == '\\' || *line_ptr == '/' || *line_ptr == ':'){ // indicates that a directory has been inserted
                             dir = true;
                             break;
                          }
                          line_ptr++;
                    }
                    if (dir == true){
                        fp = fopen(line, "r");
                        if (fp == NULL) {
                                SetConsoleTextAttribute(hOutput, 1);
                                printf("\n\tCould not Find the File!\n");
                                while(getchar() != '\n');
                                SetConsoleTextAttribute(hOutput, 15);
                                break;
                        }
                        else {
                              fseek(fp, 0, SEEK_END);
                              long fsize = ftell(fp); // returns the current value of position indicator
                              if(fsize>0){
                                  rewind(fp);
                                  fread(plain_Text, fsize, 1, fp);
                                  fclose(fp);
                                  plain_Text[fsize] = 0; // nul terminator
                                  int x=0,y=0;
                                  while(plain_Text[y]){
                                        if(plain_Text[y]=='\t'){
                                        plain_Text[x] = ' ';
                                        x++;
                                        }
                                        else if(plain_Text[y]!='\n'){
                                            plain_Text[x]=plain_Text[y];
                                            x++;
                                        }
                                        else if(plain_Text[y]=='\n'){
                                                plain_Text[x] = ' ';
                                                x++;
                                        }
                                        y++;
                                  }
                                  SetConsoleTextAttribute(hOutput, 11);
                                  printf("\n%s\n", plain_Text);
                                  SetConsoleTextAttribute(hOutput, 15);
                                   printf("\n");
                                  option = HMenu(12, -1, 5, ">", 15, "|", 9, 15, 128, 3, "Encrypt", "Change", "Back");
                                  switch(option){
                                        case 1:
                                            while(getchar() != '\n');
                                            return 1;

                                        case 2:
                                            while(getchar() != '\n');
                                            break;

                                        case 3:
                                            while(getchar()!='\n');
                                            return 0;
                                  }
                              }
                              else{SetConsoleTextAttribute(hOutput, 1);
                                   printf("\n\tEmpty File!\n");
                                   SetConsoleTextAttribute(hOutput, 15);
                                   break;
                              }
                      }
                   }
                   else { SetConsoleTextAttribute(hOutput, 12);
                          printf("\n\tInvalid Directory!\n");
                          SetConsoleTextAttribute(hOutput, 15);
                          break;
                   }
            }

            else {while(getchar() != '\n'); enter=false; break;}
            break;

        case 3:
               return 0;

    }

if(enter){while(getchar()!='\n');}
attempts++;
if (attempts==6){ SetConsoleTextAttribute(hOutput, 12);printf("\n\t***Failure to receive any valid input, No More Retries***\n"); SetConsoleTextAttribute(hOutput, 15);Sleep(1600); return 0;}
}
}

int plainTextLength(){ // plain text length including spaces
int len =0;
while(plain_Text[len]!= '\0'){
      len++;
}
return len;
}


/*ENCRYPT PLAIN TEXT*/
int encrypt(char *plain_Text, M key){

int x , cipherLettersIndex=0, cipherTextIndex=0;

/*First of all Add the Padding to the Plain Text if needed*/
int noOfAddedChars = addPadding();

/*Create Substitution Table of Chars*/
/*A B C	D E	F G	H I	J K	L M	N O	P Q	R S	T U	V W	X Y Z 0	1 2	3 4	5 6	7 8	9 ,	. ' ! ? : + - =  @ ; $ # & ( ) * " _  %  £ € / \ < > [ ] { } ^ ~  “ ” ‘ ’  ÷ | ¬ */
if(diagonalSum){
    for (x=0;x<74;x++){
         substition_Table[x] = diagonalSum * (x+1);
    }
}
else {
     for (x=0;x<74;x++){
         substition_Table[x] = secondaryDiagonalSum * (x+1);
    }
}

/*Create The ExtensionMatrix , the Columns Matrices and the Arrays Required for Encryption*/
M vectorP = calloc(6,sizeof(double*)); // it's required dynamic pointer to pointer for the function mMult()
for(x=0;x<6;x++){vectorP[x] = calloc(1, sizeof(double));}

M vectorC = calloc(6,sizeof(double*)); // it's required dynamic pointer to pointer for the function mMult()
for(x=0;x<6;x++){vectorC[x] = calloc(1, sizeof(double));}

M xEnc = calloc(6,sizeof(double*)); // it's required dynamic pointer to pointer for the function mMult()
for(x=0;x<6;x++){xEnc[x] = calloc(1, sizeof(double));}

char cipherLetters[plainTextLength()+6]; // the maximum size of the Cipher Text

int noOfspaces=0; x=0;
while(plain_Text[x]){ // know how many spaces there in the Plain Text
     if(plain_Text[x]==' '){noOfspaces++;}
     x++;
}
int *sp = calloc(noOfspaces , sizeof(int));
create_extensionMatrix();

/*remove leading spaces at the beginning of the Plain Text*/
x=0;
while (isspace(plain_Text[x])){
    x++;
}
int i =0;
while(plain_Text[i]){
      plain_Text[i] = plain_Text[x];
      i++;
      x++;
}
plain_Text[i] =0;
int y=0,v=0, textWithoutSpaces=0; x=0;

while(plain_Text[x]){ // until the end of the plain-text, substitute it with the chars' values
      switch(plain_Text[x]){
              case ' ': // if a space found
                        sp[v] = textWithoutSpaces;
                        v++;
                        y--;
                        textWithoutSpaces--;
                        break;

              case 'A':
              case 'a':
                    vectorP[y][0]=substition_Table[0];
                    break;

              case 'B':
              case 'b':
                    vectorP[y][0]=substition_Table[1];
                    break;

              case 'C':
              case 'c':
                    vectorP[y][0]=substition_Table[2];
                    break;

              case 'D':
              case 'd':
                    vectorP[y][0]=substition_Table[3];
                    break;

              case 'E':
              case 'e':
                    vectorP[y][0]=substition_Table[4];
                    break;

              case 'F':
              case 'f':
                    vectorP[y][0]=substition_Table[5];
                    break;

              case 'G':
              case 'g':
                      vectorP[y][0]=substition_Table[6];
                      break;

              case 'H':
              case 'h':
                    vectorP[y][0]=substition_Table[7];
                    break;

              case 'I':
              case 'i':
                    vectorP[y][0]=substition_Table[8];
                    break;

              case 'J':
              case 'j':
                    vectorP[y][0]=substition_Table[9];
                    break;

              case 'K':
              case 'k':
                    vectorP[y][0]=substition_Table[10];
                    break;

              case 'L':
              case 'l':
                    vectorP[y][0]=substition_Table[11];
                    break;

              case 'M':
              case 'm':
                    vectorP[y][0]=substition_Table[12];
                    break;

              case 'N':
              case 'n':
                    vectorP[y][0]=substition_Table[13];
                    break;

              case 'O':
              case 'o':
                    vectorP[y][0]=substition_Table[14];
                    break;

              case 'P':
              case 'p':
                    vectorP[y][0]=substition_Table[15];
                    break;

              case 'Q':
              case 'q':
                    vectorP[y][0]=substition_Table[16];
                    break;

              case 'R':
              case 'r':
                    vectorP[y][0]=substition_Table[17];
                    break;

              case 'S':
              case 's':
                    vectorP[y][0]=substition_Table[18];
                    break;

              case 'T':
              case 't':
                    vectorP[y][0]=substition_Table[19];
                    break;

              case 'U':
              case 'u':
                    vectorP[y][0]=substition_Table[20];
                    break;

              case 'V':
              case 'v':
                    vectorP[y][0]=substition_Table[21];
                    break;

              case 'W':
              case 'w':
                    vectorP[y][0]=substition_Table[22];
                    break;

              case 'X':
              case 'x':
                    vectorP[y][0]=substition_Table[23];
                    break;

              case 'Y':
              case 'y':
                    vectorP[y][0]=substition_Table[24];
                    break;

              case 'Z':
              case 'z':
                    vectorP[y][0]=substition_Table[25];
                    break;

              case '0':
                    vectorP[y][0]=substition_Table[26];
                    break;

              case '1':
                    vectorP[y][0]=substition_Table[27];
                    break;

              case '2':
                    vectorP[y][0]=substition_Table[28];
                    break;

              case '3':
                    vectorP[y][0]=substition_Table[29];
                    break;

              case '4':
                    vectorP[y][0]=substition_Table[30];
                    break;

              case '5':
                    vectorP[y][0]=substition_Table[31];
                    break;

              case '6':
                    vectorP[y][0]=substition_Table[32];
                    break;

              case '7':
                    vectorP[y][0]=substition_Table[33];
                    break;

              case '8':
                    vectorP[y][0]=substition_Table[34];
                    break;

              case '9':
                    vectorP[y][0]=substition_Table[35];
                    break;

              case ',':
                    vectorP[y][0]=substition_Table[36];
                    break;

              case '.':
                    vectorP[y][0]=substition_Table[37];
                    break;

              case '\'':
                    vectorP[y][0]=substition_Table[38];
                    break;

              case '!':
                    vectorP[y][0]=substition_Table[39];
                    break;

              case '?':
                    vectorP[y][0]=substition_Table[40];
                    break;

              case ':':
                    vectorP[y][0]=substition_Table[41];
                    break;

              case '+':
                    vectorP[y][0]=substition_Table[42];
                    break;

              case '-':
                    vectorP[y][0]=substition_Table[43];
                    break;

              case '=':
                    vectorP[y][0]=substition_Table[44];
                    break;

              case '@':
                    vectorP[y][0]=substition_Table[45];
                    break;

              case ';':
                    vectorP[y][0]=substition_Table[46];
                    break;

              case '$':
                    vectorP[y][0]=substition_Table[47];
                    break;

              case '#':
                    vectorP[y][0]=substition_Table[48];
                    break;

              case '&':
                    vectorP[y][0]=substition_Table[49];
                    break;

              case '(':
                    vectorP[y][0]=substition_Table[50];
                    break;

              case ')':
                    vectorP[y][0]=substition_Table[51];
                    break;

              case '*':
                    vectorP[y][0]=substition_Table[52];
                    break;

              case '"':
                    vectorP[y][0]=substition_Table[53];
                    break;

              case '_':
                    vectorP[y][0]=substition_Table[54];
                    break;

             case '%':
                    vectorP[y][0]=substition_Table[55];
                    break;

              case '£':
                    vectorP[y][0]=substition_Table[56];
                    break;

              case '€':
                    vectorP[y][0]=substition_Table[57];
                    break;

              case '/':
                    vectorP[y][0]=substition_Table[58];
                    break;

              case '\\':
                    vectorP[y][0]=substition_Table[59];
                    break;

              case '<':
                    vectorP[y][0]=substition_Table[60];
                    break;

              case '>':
                    vectorP[y][0]=substition_Table[61];
                    break;

              case '“':
                    vectorP[y][0]=substition_Table[62];
                    break;

              case '”':
                    vectorP[y][0]=substition_Table[63];
                    break;

              case '[':
                    vectorP[y][0]=substition_Table[64];
                    break;

              case ']':
                    vectorP[y][0]=substition_Table[65];
                    break;

              case '^':
                    vectorP[y][0]=substition_Table[66];
                    break;

              case '~':
                    vectorP[y][0]=substition_Table[67];
                    break;

              case '{':
                    vectorP[y][0]=substition_Table[68];
                    break;

              case '}':
                    vectorP[y][0]=substition_Table[69];
                    break;

              case '‘':
                    vectorP[y][0]=substition_Table[70];
                    break;

              case '’':
                    vectorP[y][0]=substition_Table[71];
                    break;

              case '÷':
                    vectorP[y][0]=substition_Table[72];
                    break;

              case '|':
                    vectorP[y][0]=substition_Table[73];
                    break;

              case '¬':
                    vectorP[y][0]=substition_Table[74];
                    break;
      }
y++;

if(y==6){// when complete a column matrix size
    y=0; // re-set Vector P

    /*C = (K * P) Mod 26 */
    vectorC = mMult(6,6,6,1,"lf", key,vectorP);

    int i;
    for(i=0;i<6;i++,cipherLettersIndex++){
            /*Cipher Letters*/
            int m = mod(vectorC[i][0], 26);
            switch(m){

                case 0:
                    cipherLetters[cipherLettersIndex] = 'z';
                    break;
                case 1:
                    cipherLetters[cipherLettersIndex] = 'a';
                    break;
                case 2:
                    cipherLetters[cipherLettersIndex] = 'b';
                    break;
                case 3:
                    cipherLetters[cipherLettersIndex] = 'c';
                    break;
                case 4:
                    cipherLetters[cipherLettersIndex] = 'd';
                    break;
                case 5:
                    cipherLetters[cipherLettersIndex] = 'e';
                    break;
                case 6:
                    cipherLetters[cipherLettersIndex] = 'f';
                    break;
                case 7:
                    cipherLetters[cipherLettersIndex] = 'g';
                    break;
                case 8:
                    cipherLetters[cipherLettersIndex] = 'h';
                    break;
                case 9:
                    cipherLetters[cipherLettersIndex] = 'i';
                    break;
                case 10:
                    cipherLetters[cipherLettersIndex] = 'j';
                    break;
                case 11:
                    cipherLetters[cipherLettersIndex] = 'k';
                    break;
                case 12:
                    cipherLetters[cipherLettersIndex] = 'l';
                    break;
                case 13:
                    cipherLetters[cipherLettersIndex] = 'm';
                    break;
                case 14:
                    cipherLetters[cipherLettersIndex] = 'n';
                    break;
                case 15:
                    cipherLetters[cipherLettersIndex] = 'o';
                    break;
                case 16:
                    cipherLetters[cipherLettersIndex] = 'p';
                    break;
                case 17:
                    cipherLetters[cipherLettersIndex] = 'q';
                    break;
                case 18:
                    cipherLetters[cipherLettersIndex] = 'r';
                    break;
                case 19:
                    cipherLetters[cipherLettersIndex] = 's';
                    break;
                case 20:
                    cipherLetters[cipherLettersIndex] = 't';
                    break;
                case 21:
                    cipherLetters[cipherLettersIndex] = 'u';
                    break;
                case 22:
                    cipherLetters[cipherLettersIndex] = 'v';
                    break;
                case 23:
                    cipherLetters[cipherLettersIndex] = 'w';
                    break;
                case 24:
                    cipherLetters[cipherLettersIndex] = 'x';
                    break;
                case 25:
                    cipherLetters[cipherLettersIndex] = 'y';
                    break;
            }

            /*Find The Extensions*/
            vectorC[i][0] = roundExtentionX((vectorC[i][0]/26));
    }

    /* Multiply Extension Matrix * Extension */
    xEnc = mMult(6,6,6,1,"lf", extensionMatrix, vectorC);

    /* Make the Cipher Text */
    int z; cipherLettersIndex-=6;
    for (z=0;z<6;z++,cipherLettersIndex++){
            /*pick up the cipher letter*/
            cipherText[cipherTextIndex] = cipherLetters[cipherLettersIndex];
            cipherTextIndex++;
            /*Split every Number into Digits*/
            long long div = 1;
            for(;div<=fabs(xEnc[z][0]); div*=10);
            if(xEnc[z][0]<0){cipherText[cipherTextIndex] = '-'; cipherTextIndex++;} //check if it's negative
            long long xEncValue = xEnc[z][0];
            while(((long long)fabs(xEnc[z][0]))>0){
                  div /= 10;
                  long long digit = (long long) (fabs(xEnc[z][0]) / div);
                  char s[2];
                  sprintf(s, "%lld", digit);
                  cipherText[cipherTextIndex] = s[0];
                  cipherTextIndex++;
                  xEnc[z][0] = ((long long)fabs(xEnc[z][0])) % div;
            }
            while (xEncValue){ /*in case there is 0s at the end of the number*/
                    if((xEncValue%10)==0){cipherText[cipherTextIndex] = '0'; cipherTextIndex++;}
                    else {break;}
                    xEncValue /= 10;
           }
    }
}

x++;
textWithoutSpaces++;
}

cipherText[cipherTextIndex] = 0; // add nul terminator at the end of the Cipher Text

/*Put Back the Spaces were exist in the Plain-Text in the Cipher-Text*/
char cipherArray[cipherTextIndex]; // to hold the cipher text temporarily
strcpy(cipherArray,cipherText);

x=0, y=1; int z=0, l=0;
if (sp[0]>0){
while(cipherArray[x]){
     if (isalpha(cipherArray[x])){// the space should after the cipher letter
         if(y==sp[l]+1){
                cipherText[z]=' ';
                l++;
                z++;
         }
         y++;
    }
    cipherText[z] = cipherArray[x]; // continue copying the extensions
    x++;
    z++;
}
cipherText[z] = 0; // add nul terminator
}
/*Substitute Extensions With Symbols from the SymbolsSubstitutionTable*/
i=0;
while (cipherText[i]){
    switch(cipherText[i]){
        case '-':
            cipherText[i]=symbolsSubstitutionTable[0];
            break;
        case '0':
            cipherText[i]=symbolsSubstitutionTable[1];
            break;
        case '1':
            cipherText[i]=symbolsSubstitutionTable[2];
            break;
        case '2':
            cipherText[i]=symbolsSubstitutionTable[3];
            break;
        case '3':
            cipherText[i]=symbolsSubstitutionTable[4];
            break;
        case '4':
            cipherText[i]=symbolsSubstitutionTable[5];
            break;
        case '5':
            cipherText[i]=symbolsSubstitutionTable[6];
            break;
        case '6':
            cipherText[i]=symbolsSubstitutionTable[7];
            break;
        case '7':
            cipherText[i]=symbolsSubstitutionTable[8];
            break;
        case '8':
            cipherText[i]=symbolsSubstitutionTable[9];
            break;
        case '9':
            cipherText[i]=symbolsSubstitutionTable[10];
            break;

    }
    i++;
}
/*Add noOfAddedChars*/
switch(noOfAddedChars){
case 0:
    cipherText[i]='"';
    i++;
    break;
case 1:
    cipherText[i]='\'';
    i++;
    break;
case 2:
    cipherText[i]=':';
    i++;
    break;
case 3:
    cipherText[i]=';';
    i++;
    break;
case 4:
    cipherText[i]=',';
    i++;
    break;
case 5:
    cipherText[i]='.';
    i++;
    break;


}
cipherText[i] =0;
HANDLE hOutput = GetStdHandle(STD_OUTPUT_HANDLE);
printf("\n CIPHER TEXT:\n");
SetConsoleTextAttribute(hOutput,8);
printf("%s\n", cipherText);
SetConsoleTextAttribute(hOutput, 15);
printf("\n");
while(1){
printf("\n");
int option = HMenu(9, -1, 5, ">", 15, "|", 9, 15, 128, 2, "Save To File", "Back To Main Menu");
    switch(option){
    case 1:
        if(saveCipherText()){
                    delete_matrix(key);
                    delete_matrix(extensionMatrix);
                    deleteColumnMatrix(vectorC);
                    deleteColumnMatrix(vectorP);
                    deleteColumnMatrix(xEnc);
                    free(determinant);
                    free(sp);
                    diagonalSum=0;
                    secondaryDiagonalSum=0;
                    resetSymbolsSubstitutionTable();
                    emptyArray(plain_Text);
                    emptyArray(cipherText);
                    resetSubstitutionTable();


                    return 1;
        }
        break;

    case 2:
        delete_matrix(key);
        delete_matrix(extensionMatrix);
        deleteColumnMatrix(vectorC);
        deleteColumnMatrix(vectorP);
        deleteColumnMatrix(xEnc);
        free(determinant);
        free(sp);
        diagonalSum=0;
        secondaryDiagonalSum=0;
        resetSymbolsSubstitutionTable();
        emptyArray(plain_Text);
        emptyArray(cipherText);
        resetSubstitutionTable();
        return 0;
        break;
    }
}

}

int saveCipherText(){ // To Save The Cipher Text on the User's Computer
HANDLE hOutput = GetStdHandle(STD_OUTPUT_HANDLE);
int attempts=0;
while (attempts<10){
bool enter = true;
printf("\n");
int option = HMenu(4, -1, 5, ">", 15, "|", 9, 15, 128, 3, "Save on Desktop", "Save to Directory", "Back");
char line[256];
switch (option){
      case 1:
            printf("\n\tInsert The CipherText File Name: ");
            if(scanf("%256[0-9a-zA-Z ,.'!@#$%^&()-_=+~{[]}; ]", line)){
                    int x,y;
                    FILE *fp;
                    TCHAR szPath[MAX_PATH];
                    SHGetFolderPath(NULL,CSIDL_DESKTOPDIRECTORY,NULL,0,szPath);
                    strcat(szPath,"\\");strcat(szPath, line);strcat(szPath,".txt");
                    fp = fopen(szPath, "w");
                    if (fp == NULL) {SetConsoleTextAttribute(hOutput, 1);
                                     printf("\n\tCould not Create the File!\n");
                                     SetConsoleTextAttribute(hOutput, 15);
                                     while(getchar() != '\n'); enter=false; break;}
                    else {
                        fprintf(fp,"%s", cipherText);
                        fclose(fp);
                        printf("\n\tDONE!\n");
                        while(getchar() != '\n'); // to free Stdin
                        return 1;
                    }
            }
            else {SetConsoleTextAttribute(hOutput, 12);
                  printf("\n\tInvalid File Name!\n\n");
                  SetConsoleTextAttribute(hOutput, 15);
                  while(getchar() != '\n'); enter=false; break;}


    case 2:
            printf("\n\tInsert The Directory: ");
            if(scanf("%256[0-9a-zA-Z :.\\/~$@!""£%^&*()_-+=|?<>'#;{[]} ]", line)){
                    char *line_ptr = line;
                    bool dir = false;
                    while (*line_ptr!='\0'){
                    if(*line_ptr == '\\' || *line_ptr == '/' || *line_ptr == ':'){ // indicates that a directory may has been inserted
                        dir = true;
                        break;
                    }

                    line_ptr++;
                    }
                    if (dir==true){
                        int x,y;
                        FILE *fp;
                        fp = fopen(line, "w");
                        if (fp == NULL) {SetConsoleTextAttribute(hOutput, 1);
                                         printf("\n Could not Create the File!\n");
                                         SetConsoleTextAttribute(hOutput, 15);
                                         while(getchar() != '\n'); enter=false; break;}
                        else{
                            fprintf(fp, "%s", cipherText);
                            fclose(fp);
                            printf("\n\tDONE!\n\n");
                            while(getchar() != '\n'); // to free Stdin
                            return 1;
                        }
                    }
                  else {SetConsoleTextAttribute(hOutput, 12); printf("\n\tInvalid Directory\n"); SetConsoleTextAttribute(hOutput, 15); break;}
          }
          else {while(getchar() != '\n'); enter=false; break;}

    case 3:
        return 0;
}
if(enter) {while(getchar()!='\n');}
attempts++;
if (attempts==10){SetConsoleTextAttribute(hOutput, 12); printf("\n***Failure To Receive Valid Input, No More Retries***\n"); SetConsoleTextAttribute(hOutput, 15); Sleep(1600); return 0;}
}
}

int encryption(){ /*Main Flow In Encryption*/
int x=0;
while(1){
    x=0;
    system("cls");
    if(x=keyMatrixEnc()){
            if(x==-1){return -1;}
            else if (x==1){
                    if (symbols()){
                        if(plainTextFun()){
                            if(encrypt(plain_Text, key)){
                                    return 1;}
                            else {return 0;}

                    }
                    }
            }
    }
}
return 0;}


/*MIS. IN ENCRYPTION*/
int addPadding(){
int lenWithoutSpaces=0, i=0;
while(plain_Text[i]!='\0'){
        if(plain_Text[i]!=' '){lenWithoutSpaces++;}
        i++;
}

int padding = (6-(lenWithoutSpaces%6));
int len = plainTextLength();

if (padding != 6){
    for (i=0; i<padding;i++, len++){
          plain_Text[len] = plain_Text[len-1];
}
plain_Text[len+padding] = 0;
return padding;
}
else if (padding==6){return 0;}
}

double roundExtentionX(double extentionX){ // if X>0 -> RoundUp. if X<0 -> RoundDown
int extensionInteger = extentionX;
if ((extentionX-extensionInteger)>0){extensionInteger+=1; extentionX=(double)extensionInteger;}
else if ((extentionX-extensionInteger)<0){extentionX=(double)extensionInteger;}

return extentionX;
}













/*DECRYPTION*/

/*KEY MATRIX IN DECRYPTION*/
int keyMatrixDec(){

HANDLE hOutput = GetStdHandle(STD_OUTPUT_HANDLE);
int doneKey =0;

while (!doneKey){
    system("cls"); // when choose to Cancel
    printf("\n");
    SetConsoleTextAttribute(hOutput, 3);
    printf("=====================================================================\n");
    printf("\t\t\t      KEY");
    printf("\n=====================================================================\n\n");
    SetConsoleTextAttribute(hOutput, 15);
    int x=0,y=0, option=0;
    key = create_key();
    option= HMenu(8, -1, 5, ">", 15, "|", 9, 15, 128, 3, "Type Key", "Load Key From File", "Main Menu");

    switch (option){

            case 1:
                doneKey=insertKey(key);
                if(doneKey==1){return 1;}
                else{delete_matrix(key);}
                break;

            case 2:
                doneKey=loadKey();
                if(doneKey==1){return 1;}
                else {delete_matrix(key);}
                break;

            case 3:
                return -1;
    }
}

return 0;
}


/*CIPHER TEXT IN THE DECRYPTION*/
int cipherTextFun(){
HANDLE hOutput = GetStdHandle(STD_OUTPUT_HANDLE);
SetConsoleTextAttribute(hOutput, 5);
printf("\n==================================================================\n");
printf("\t\t\t  CIPHER TEXT\t\t\t\t\t");
printf("\n==================================================================\n\n");
SetConsoleTextAttribute(hOutput, 15);
int option=0;
while(option!=3){
option = HMenu(8, -1, 5, ">", 15, "|", 9, 15, 128, 3, "Type Cipher-Text", "Load From File", "Cancel");
switch(option){
    case 1:
        if(typeCipherText()){return 1;}
        printf("\n");
        break;

    case 2:
        if(loadCipherText()){return 1;}
        printf("\n");
        break;

    case 3:
        return 0;
}
}
}

int typeCipherText(){ // when user choose to type the cipher text on the console
HANDLE hOutput = GetStdHandle(STD_OUTPUT_HANDLE);
int option =2, i=0;
while (option==2){
    printf("\n      Type The Cipher-Text. When You Finish Press Enter:\n\n ");
    SetConsoleTextAttribute(hOutput, 8);
    if(fgets(cipher_Text, sizeof(cipher_Text),stdin)){
            SetConsoleTextAttribute(hOutput, 15);
            i = cipherTextLength();
            if (i>1){
                int y=0;
                          while(cipher_Text[y]){
                                if(cipher_Text[y]=='\n'){
                                    cipher_Text[y]=0;
                                }
                                else if (cipher_Text[y]=='\t'){ //replace every tab with a space
                                    cipher_Text[y] = ' ';
                                }
                                y++;
                          }
                printf("\n");
                option = HMenu(16, -1, 5, ">", 15, "|", 9, 15, 128, 3, "Decrypt", "Change", "Back");
                switch(option){

                case 1:
                    return 1;

                case 2:
                    break;

                case 3:
                    return 0;
                }
            }
            else {SetConsoleTextAttribute(hOutput, 1); printf("\tEmpty Input!\n"); SetConsoleTextAttribute(hOutput, 15); return 0;}
    }
}
}

int loadCipherText(){
HANDLE hOutput = GetStdHandle(STD_OUTPUT_HANDLE);
int attempts =0;
while(attempts<6){ // number of attempts
    bool enter = true;
    char line[256];
    char *line_ptr = line;

    printf("\n");
    int option = HMenu(7, -1, 5, ">", 15, "|", 9, 15, 128, 3, "Load from Desktop", "Load from Directory", "Back");

    switch(option){
        case 1:
            printf("\n\tInsert The File Name: ");
            if(scanf("%256[0-9a-zA-Z ,.'!@#$%^&()-_=+~{[]}; ]", line)){ // for reading the file name to be found on the desktop
                FILE *fp;
                TCHAR szPath[MAX_PATH];
                SHGetFolderPath(NULL,CSIDL_DESKTOPDIRECTORY,NULL,0,szPath);
                strcat(szPath,"\\");strcat(szPath, line);strcat(szPath,".txt");
                fp = fopen(szPath, "r");
                if (fp == NULL) {
                                SetConsoleTextAttribute(hOutput, 1);
                                printf("\n\tCould not Find the File!\n");
                                SetConsoleTextAttribute(hOutput, 15);
                                break;
                }
                else {
                      fseek(fp, 0, SEEK_END);
                      long fsize = ftell(fp); // returns the current value of position indicator
                      if(fsize>0){
                          rewind(fp);
                          fread(cipher_Text, fsize, 1, fp);
                          fclose(fp);
                          cipher_Text[fsize] = 0; // nul terminator
                          int x=0,y=0;
                          while(cipher_Text[y]){ // this handle the file content if the user is playing with me :D
                                if(cipher_Text[y]=='\t'){
                                        cipher_Text[x] = ' ';
                                        x++;
                                }
                                else if(cipher_Text[y]=='\n'){
                                        cipher_Text[x] = ' ';
                                        x++;
                                }
                                else if(cipher_Text[y]!='\n'){
                                    cipher_Text[x]=cipher_Text[y];
                                    x++;
                                }


                                y++;
                          }
                          SetConsoleTextAttribute(hOutput, 11);
                          printf("\n %s\n", cipher_Text);
                          SetConsoleTextAttribute(hOutput, 15);
                          printf("\n");
                          option = HMenu(18, -1, 5, ">", 15, "|", 9, 15, 128, 3, "Decrypt", "Change", "Back");
                          switch(option){
                                case 1:
                                    while(getchar() != '\n');
                                    return 1;

                                case 2:
                                    break;

                                case 3:
                                    while(getchar()!='\n');
                                    return 0;
                          }
                      }
                      else{SetConsoleTextAttribute(hOutput, 1);printf("\n\tEmpty File!\n"); SetConsoleTextAttribute(hOutput, 15);break;}
                }
           }


            else {while(getchar() != '\n');  enter=false; break;}
            break;

        case 2:
            printf("\n\tInsert The Directory: ");
            if(scanf("%256[0-9a-zA-Z:.\\/ ~$@!""£%^&*()_-+=|?<>'#;{[]}]", line)){
                    int x,y;
                    FILE *fp;
                    bool dir = false;
                    while (*line_ptr!='\0'){
                          if(*line_ptr == '\\' || *line_ptr == '/' || *line_ptr == ':'){ // indicates that a directory has been inserted
                             dir = true;
                             break;
                          }
                          line_ptr++;
                    }
                    if (dir == true){
                        fp = fopen(line, "r");
                        if (fp == NULL) {
                                SetConsoleTextAttribute(hOutput, 1);
                                printf("\n\tCould not Find the File!\n");
                                SetConsoleTextAttribute(hOutput, 15);
                                break;
                        }
                        else {
                              fseek(fp, 0, SEEK_END);
                              long fsize = ftell(fp); // returns the current value of position indicator
                              if(fsize>0){
                                  rewind(fp);
                                  fread(cipher_Text, fsize, 1, fp);
                                  fclose(fp);
                                  cipher_Text[fsize] = 0; // nul terminator
                                  int x=0,y=0;
                                  while(cipher_Text[y]){
                                        if(cipher_Text[y]=='\t'){
                                        cipher_Text[x] = ' ';
                                        x++;
                                        }
                                        else if(cipher_Text[y]!='\n'){
                                            cipher_Text[x]=cipher_Text[y];
                                            x++;
                                        }
                                        else if(cipher_Text[y]=='\n'){
                                                cipher_Text[x] = ' ';
                                                x++;
                                        }
                                        y++;
                                  }
                                  SetConsoleTextAttribute(hOutput, 11);
                                  printf("\n %s\n", cipher_Text);
                                  SetConsoleTextAttribute(hOutput, 15);
                                   printf("\n");
                                  option = HMenu(18, -1, 5, ">", 15, "|", 9, 15, 128, 3, "Decrypt", "Change", "Back");
                                  switch(option){
                                        case 1:
                                            while(getchar() != '\n');
                                            return 1;

                                        case 2:
                                            break;

                                        case 3:
                                            while(getchar()!='\n');
                                            return 0;
                                  }
                              }
                              else{SetConsoleTextAttribute(hOutput, 1);
                                   printf("\n\tEmpty File!\n");
                                   SetConsoleTextAttribute(hOutput, 15);
                                   break;
                              }
                      }
                   }
                   else { SetConsoleTextAttribute(hOutput, 12);
                          printf("\n\tInvalid Directory!\n");
                          SetConsoleTextAttribute(hOutput, 15);
                          break;
                   }
            }

            else {while(getchar() != '\n'); enter=false; break;}
            break;

        case 3:
               return 0;

    }

if(enter){while(getchar()!='\n');}
attempts++;
if (attempts==6){ SetConsoleTextAttribute(hOutput, 12);printf("\n\t***Failure to receive any valid input, No More Retries***\n"); SetConsoleTextAttribute(hOutput, 15);Sleep(1600); return 0;}
}
}

int savePlainText(){
HANDLE hOutput = GetStdHandle(STD_OUTPUT_HANDLE);
int attempts=0;
while (attempts<10){ //number of attempts allowed
bool enter = true;
printf("\n");
int option = HMenu(7, -1, 5, ">", 15, "|", 9, 15, 128, 3, "Save on Desktop", "Save to Directory", "Back");
char line[256];
switch (option){
      case 1:
            printf("\n\tInsert The PlainText File Name: ");
            if(scanf("%256[0-9a-zA-Z ,.'!@#$%^&()-_=+~{[]}; ]", line)){
                    int x,y;
                    FILE *fp;
                    TCHAR szPath[MAX_PATH];
                    SHGetFolderPath(NULL,CSIDL_DESKTOPDIRECTORY,NULL,0,szPath);
                    strcat(szPath,"\\");strcat(szPath, line);strcat(szPath,".txt");
                    fp = fopen(szPath, "w");
                    if (fp == NULL) {SetConsoleTextAttribute(hOutput, 1);
                                     printf("\n\tCould not Create the File!\n");
                                     SetConsoleTextAttribute(hOutput, 15);
                                     while(getchar() != '\n'); enter=false; break;}
                    else {
                        fprintf(fp, "%s", plainText);
                        fclose(fp);
                        printf("\n\tDONE!\n");
                        while(getchar() != '\n'); // to free Stdin
                        return 1;
                    }
            }
            else {SetConsoleTextAttribute(hOutput, 12);
                  printf("\n\tInvalid File Name!\n\n");
                  SetConsoleTextAttribute(hOutput, 15);
                  while(getchar() != '\n'); enter=false; break;}


    case 2:
            printf("\n\tInsert The Directory: ");
            if(scanf("%256[0-9a-zA-Z:.\\/~$@!""£%^&*()_-+=|?<>'#;{[]} ]", line)){
                    char *line_ptr = line;
                    bool dir = false;
                    while (*line_ptr!='\0'){
                    if(*line_ptr == '\\' || *line_ptr == '/' || *line_ptr == ':'){ // indicates that a directory may has been inserted
                        dir = true;
                        break;
                    }

                    line_ptr++;
                    }
                    if (dir==true){
                        int x,y;
                        FILE *fp;
                        fp = fopen(line, "w");
                        if (fp == NULL) {SetConsoleTextAttribute(hOutput, 1);
                                         printf("\n Could not Create the File!\n");
                                         SetConsoleTextAttribute(hOutput, 15);
                                         while(getchar() != '\n'); enter=false; break;}
                        else{
                            fprintf(fp, "%s", plainText);
                            fclose(fp);
                            printf("\n\tDONE!\n\n");
                            while(getchar() != '\n'); // to free Stdin
                            return 1;
                        }
                    }
                  else {SetConsoleTextAttribute(hOutput, 12); printf("\n\tInvalid Directory\n"); SetConsoleTextAttribute(hOutput, 15); break;}
          }
          else {while(getchar() != '\n'); enter=false; break;}

    case 3:
        return 0;
}
if(enter) {while(getchar()!='\n');}
attempts++;
if (attempts==10){SetConsoleTextAttribute(hOutput, 12); printf("\n***Failure To Receive Valid Input, No More Retries***\n"); SetConsoleTextAttribute(hOutput, 15); Sleep(1600); return 0;}
}
}

int cipherTextLength(){ // cipher text length including spaces
int len =0;
while(cipher_Text[len]){
      len++;
}
return len;
}

int find_NumberOfAddedChars(){
switch(padding[0]){
case '"':
     return 0;

case '\'':
    return 1;

case ':':
    return 2;

case ';':
    return 3;

case ',':
    return 4;

case '.':
    return 5;
}
return 0; // in case it's deleted by mistake
}


/*DECRYPT CIPHER TEXT*/
int decrypt(char *cipher_Text){


int x , cipherLettersIndex=0;

/*Create Substitution Table of Chars*/
/*A B C	D E	F G	H I	J K	L M	N O	P Q	R S	T U	V W	X Y Z 0	1 2	3 4	5 6	7 8	9 ,	. ' ! ? : + - =  @ ; $ # & ( ) * " _  %  £ € / \ < > [ ] { } ^ ~  “ ” ‘ ’  ÷ | ¬ */
if(diagonalSum){
    for (x=0;x<74;x++){
         substition_Table[x] = diagonalSum * (x+1);
    }
}
else {
     for (x=0;x<74;x++){
         substition_Table[x] = secondaryDiagonalSum * (x+1);
    }
}

/*Create Extension Matrix*/
create_extensionMatrix();
findExtensionMatrixInverse();
findKeyInverse();

/*remove leading spaces at the beginning of the Cipher Text (if any!)*/
x=0;
while(isspace(cipher_Text[x])){
        x++;
}
int i =0;
while(cipher_Text[i]){
      cipher_Text[i] = cipher_Text[x];
      i++;
      x++;
}
padding[0] = cipher_Text[i-1];
cipher_Text[i-1]=0;



/*Find Encrypted Extensions. Substitute Symbols*/
x=0;
while(cipher_Text[x]){

     if(symbolsSubstitutionTable[0] == cipher_Text[x]){
        cipher_Text[x]= '-';
     }
     else if(symbolsSubstitutionTable[1] == cipher_Text[x]){
        cipher_Text[x]= '0';
     }
     else if(symbolsSubstitutionTable[2] == cipher_Text[x]){
        cipher_Text[x]= '1';
     }
     else if(symbolsSubstitutionTable[3] == cipher_Text[x]){
        cipher_Text[x]= '2';
     }
     else if(symbolsSubstitutionTable[4] == cipher_Text[x]){
        cipher_Text[x]= '3';
     }
     else if(symbolsSubstitutionTable[5] == cipher_Text[x]){
        cipher_Text[x]= '4';
     }
     else if(symbolsSubstitutionTable[6] == cipher_Text[x]){
        cipher_Text[x]= '5';
     }
     else if(symbolsSubstitutionTable[7] == cipher_Text[x]){
        cipher_Text[x]= '6';
     }
     else if(symbolsSubstitutionTable[8] == cipher_Text[x]){
        cipher_Text[x]= '7';
     }
     else if(symbolsSubstitutionTable[9] == cipher_Text[x]){
        cipher_Text[x]= '8';
     }
     else if(symbolsSubstitutionTable[10] == cipher_Text[x]){
        cipher_Text[x]= '9';
     }


    x++;
}

int lengthOfPlainText = convertCharToDouble(cipher_Text, xEncDecryption); // convert from char to double

/*Substitute Cipher Letters*/
x=0;
while (cipher_Text[x]){
     if(cipher_Text[x] == 'a'){
        cipherLettersDecryption[cipherLettersIndex]= 1;
        cipherLettersIndex++;
     }
     else if(cipher_Text[x] == 'b'){
            cipherLettersDecryption[cipherLettersIndex]= 2;
            cipherLettersIndex++;
     }
     else if(cipher_Text[x] == 'c'){
            cipherLettersDecryption[cipherLettersIndex]= 3;
            cipherLettersIndex++;
     }
     else if(cipher_Text[x] == 'd'){
            cipherLettersDecryption[cipherLettersIndex]= 4;
            cipherLettersIndex++;
     }
     else if(cipher_Text[x] == 'e'){
            cipherLettersDecryption[cipherLettersIndex]= 5;
            cipherLettersIndex++;
     }
     else if(cipher_Text[x] == 'f'){
            cipherLettersDecryption[cipherLettersIndex]= 6;
            cipherLettersIndex++;
     }
     else if(cipher_Text[x] == 'g'){
            cipherLettersDecryption[cipherLettersIndex]= 7;
            cipherLettersIndex++;
     }
     else if(cipher_Text[x] == 'h'){
            cipherLettersDecryption[cipherLettersIndex]= 8;
            cipherLettersIndex++;
     }
     else if(cipher_Text[x] == 'i'){
            cipherLettersDecryption[cipherLettersIndex]= 9;
            cipherLettersIndex++;
     }
     else if(cipher_Text[x] == 'j'){
            cipherLettersDecryption[cipherLettersIndex]= 10;
            cipherLettersIndex++;
     }
     else if(cipher_Text[x] == 'k'){
            cipherLettersDecryption[cipherLettersIndex]= 11;
            cipherLettersIndex++;
     }
     else if(cipher_Text[x] == 'l'){
            cipherLettersDecryption[cipherLettersIndex]= 12;
            cipherLettersIndex++;
     }
     else if(cipher_Text[x] == 'm'){
            cipherLettersDecryption[cipherLettersIndex]= 13;
            cipherLettersIndex++;
     }
     else if(cipher_Text[x] == 'n'){
            cipherLettersDecryption[cipherLettersIndex]= 14;
            cipherLettersIndex++;
     }
     else if(cipher_Text[x] == 'o'){
            cipherLettersDecryption[cipherLettersIndex]= 15;
            cipherLettersIndex++;
     }
     else if(cipher_Text[x] == 'p'){
            cipherLettersDecryption[cipherLettersIndex]= 16;
            cipherLettersIndex++;
     }
     else if(cipher_Text[x] == 'q'){
            cipherLettersDecryption[cipherLettersIndex]= 17;
            cipherLettersIndex++;
     }
     else if(cipher_Text[x] == 'r'){
            cipherLettersDecryption[cipherLettersIndex]= 18;
            cipherLettersIndex++;
     }
     else if(cipher_Text[x] == 's'){
            cipherLettersDecryption[cipherLettersIndex]= 19;
            cipherLettersIndex++;
     }
     else if(cipher_Text[x] == 't'){
            cipherLettersDecryption[cipherLettersIndex]= 20;
            cipherLettersIndex++;
     }
     else if(cipher_Text[x] == 'u'){
            cipherLettersDecryption[cipherLettersIndex]= 21;
            cipherLettersIndex++;
     }
     else if(cipher_Text[x] == 'v'){
            cipherLettersDecryption[cipherLettersIndex]= 22;
            cipherLettersIndex++;
     }
    else if(cipher_Text[x] == 'w'){
            cipherLettersDecryption[cipherLettersIndex]= 23;
            cipherLettersIndex++;
     }
     else if(cipher_Text[x] == 'x'){
            cipherLettersDecryption[cipherLettersIndex]= 24;
            cipherLettersIndex++;
     }
     else if(cipher_Text[x] == 'y'){
            cipherLettersDecryption[cipherLettersIndex]= 25;
            cipherLettersIndex++;
     }
      else if(cipher_Text[x] == 'z'){
            cipherLettersDecryption[cipherLettersIndex]= 26;
            cipherLettersIndex++;
     }

    x++;
}


M xEncColumnArray = calloc(6,sizeof(double*));
for(x=0;x<6;x++){xEncColumnArray[x]=calloc(1,sizeof(double));}

M extensionsX = calloc(6,sizeof(double*));
for(x=0;x<6;x++){extensionsX[x]=calloc(1,sizeof(double));}

M C1 = calloc(6,sizeof(double*));
for(x=0;x<6;x++){C1[x]=calloc(1,sizeof(double));}

M P = calloc(6,sizeof(double*));
for(x=0;x<6;x++){P[x]=calloc(1,sizeof(double));}

int y;
for(x=0,y=0; x<=lengthOfPlainText; x++,y++){

    if(y==6){
        int i,j;
        for(i=0,j=(x-6);i<6;i++,j++){ // copy every 6 elements to make the column array
            xEncColumnArray[i][0] = xEncDecryption[j];
        }
        extensionsX = mMult(6,6,6,1,"lf",extensionMatrixInverse,xEncColumnArray);

        for(i=0,j=(x-6);i<6;i++,j++){
            C1[i][0]= (extensionsX[i][0] * 26) - (26 - cipherLettersDecryption[j]);
        }
        P = mMult(6,6,6,1,"lf",keyInverse,C1);
        for(i=0,j=(x-6);i<6;i++,j++){
                if (roundP(P[i][0]) == substition_Table[0]){
                        plainText[j]='a';
                }
                else if (roundP(P[i][0]) == substition_Table[1]){
                        plainText[j]='b';
                }
                else if (roundP(P[i][0]) == substition_Table[2]){
                        plainText[j]='c';
                }
                else if (roundP(P[i][0]) == substition_Table[3]){
                        plainText[j]='d';
                }
                else if (roundP(P[i][0]) == substition_Table[4]){
                        plainText[j]='e';
                }
                else if (roundP(P[i][0]) == substition_Table[5]){
                        plainText[j]='f';
                }
                else if (roundP(P[i][0]) == substition_Table[6]){
                        plainText[j]='g';
                }
                else if (roundP(P[i][0]) == substition_Table[7]){
                        plainText[j]='h';
                }
                else if (roundP(P[i][0]) == substition_Table[8]){
                        plainText[j]='i';
                }
                else if (roundP(P[i][0]) == substition_Table[9]){
                        plainText[j]='j';
                }
                else if (roundP(P[i][0]) == substition_Table[10]){
                        plainText[j]='k';
                }
                else if (roundP(P[i][0]) == substition_Table[11]){
                        plainText[j]='l';
                }
                else if (roundP(P[i][0]) == substition_Table[12]){
                        plainText[j]='m';
                }
                else if (roundP(P[i][0]) == substition_Table[13]){
                        plainText[j]='n';
                }
                else if (roundP(P[i][0]) == substition_Table[14]){
                        plainText[j]='o';
                }
                else if (roundP(P[i][0]) == substition_Table[15]){
                        plainText[j]='p';
                }
                else if (roundP(P[i][0]) == substition_Table[16]){
                        plainText[j]='q';
                }
                else if (roundP(P[i][0]) == substition_Table[17]){
                        plainText[j]='r';
                }
                else if (roundP(P[i][0]) == substition_Table[18]){
                        plainText[j]='s';
                }
                else if (roundP(P[i][0]) == substition_Table[19]){
                        plainText[j]='t';
                }
                else if (roundP(P[i][0]) == substition_Table[20]){
                        plainText[j]='u';
                }
                else if (roundP(P[i][0]) == substition_Table[21]){
                        plainText[j]='v';
                }
                else if (roundP(P[i][0]) == substition_Table[22]){
                        plainText[j]='w';
                }
                else if (roundP(P[i][0]) == substition_Table[23]){
                        plainText[j]='x';
                }
                else if (roundP(P[i][0]) == substition_Table[24]){
                        plainText[j]='y';
                }
                else if (roundP(P[i][0]) == substition_Table[25]){
                        plainText[j]='z';
                }
                else if (roundP(P[i][0]) == substition_Table[26]){
                        plainText[j]='0';
                }
                else if (roundP(P[i][0]) == substition_Table[27]){
                        plainText[j]='1';
                }
                else if (roundP(P[i][0]) == substition_Table[28]){
                        plainText[j]='2';
                }
                else if (roundP(P[i][0]) == substition_Table[29]){
                        plainText[j]='3';
                }
                else if (roundP(P[i][0]) == substition_Table[30]){
                        plainText[j]='4';
                }
                else if (roundP(P[i][0]) == substition_Table[31]){
                        plainText[j]='5';
                }
                else if (roundP(P[i][0]) == substition_Table[32]){
                        plainText[j]='6';
                }
                else if (roundP(P[i][0]) == substition_Table[33]){
                        plainText[j]='7';
                }
                else if (roundP(P[i][0]) == substition_Table[34]){
                        plainText[j]='8';
                }
                else if (roundP(P[i][0]) == substition_Table[35]){
                        plainText[j]='9';
                }
                else if (roundP(P[i][0]) == substition_Table[36]){
                        plainText[j]=',';
                }
                else if (roundP(P[i][0]) == substition_Table[37]){
                        plainText[j]='.';
                }
                else if (roundP(P[i][0]) == substition_Table[38]){
                        plainText[j]='\'';
                }
                else if (roundP(P[i][0]) == substition_Table[39]){
                        plainText[j]='!';
                }
                else if (roundP(P[i][0]) == substition_Table[40]){
                        plainText[j]='?';
                }
                else if (roundP(P[i][0]) == substition_Table[41]){
                        plainText[j]=':';
                }
                else if (roundP(P[i][0]) == substition_Table[42]){
                        plainText[j]='+';
                }
                else if (roundP(P[i][0]) == substition_Table[43]){
                        plainText[j]='-';
                }
                else if (roundP(P[i][0]) == substition_Table[44]){
                        plainText[j]='=';
                }
                else if (roundP(P[i][0]) == substition_Table[45]){
                        plainText[j]='@';
                }
                else if (roundP(P[i][0]) == substition_Table[46]){
                        plainText[j]=';';
                }
                else if (roundP(P[i][0]) == substition_Table[47]){
                        plainText[j]='$';
                }
                else if (roundP(P[i][0]) == substition_Table[48]){
                        plainText[j]='#';
                }
                else if (roundP(P[i][0]) == substition_Table[49]){
                        plainText[j]='&';
                }
                else if (roundP(P[i][0]) == substition_Table[50]){
                        plainText[j]='(';
                }
                else if (roundP(P[i][0]) == substition_Table[51]){
                        plainText[j]=')';
                }
                else if (roundP(P[i][0]) == substition_Table[52]){
                        plainText[j]='*';
                }
                else if (roundP(P[i][0]) == substition_Table[53]){
                        plainText[j]='"';
                }
                else if (roundP(P[i][0]) == substition_Table[54]){
                        plainText[j]='_';
                }
                else if (roundP(P[i][0]) == substition_Table[55]){
                        plainText[j]='%';
                }
                else if (roundP(P[i][0]) == substition_Table[56]){
                        plainText[j]='£';
                }
                else if (roundP(P[i][0]) == substition_Table[57]){
                        plainText[j]='€';
                }
                else if (roundP(P[i][0]) == substition_Table[58]){
                        plainText[j]='/';
                }
                else if (roundP(P[i][0]) == substition_Table[59]){
                        plainText[j]='\\';
                }
                else if (roundP(P[i][0]) == substition_Table[60]){
                        plainText[j]='<';
                }
                else if (roundP(P[i][0]) == substition_Table[61]){
                        plainText[j]='>';
                }
                else if (roundP(P[i][0]) == substition_Table[62]){
                        plainText[j]='“';
                }
                else if (roundP(P[i][0]) == substition_Table[63]){
                        plainText[j]='”';
                }
                else if (roundP(P[i][0]) == substition_Table[64]){
                        plainText[j]='[';
                }
                else if (roundP(P[i][0]) == substition_Table[65]){
                        plainText[j]=']';
                }
                else if (roundP(P[i][0]) == substition_Table[66]){
                        plainText[j]='^';
                }
                else if (roundP(P[i][0]) == substition_Table[67]){
                        plainText[j]='~';
                }
                else if (roundP(P[i][0]) == substition_Table[68]){
                        plainText[j]='{';
                }
                else if (roundP(P[i][0]) == substition_Table[69]){
                        plainText[j]='}';
                }
                else if (roundP(P[i][0]) == substition_Table[70]){
                        plainText[j]='‘';
                }
                else if (roundP(P[i][0]) == substition_Table[71]){
                        plainText[j]='’';
                }
                else if (roundP(P[i][0]) == substition_Table[72]){
                        plainText[j]='÷';
                }
                else if (roundP(P[i][0]) == substition_Table[73]){
                        plainText[j]='|';
                }
                else if (roundP(P[i][0]) == substition_Table[74]){
                        plainText[j]='¬';
                }
            }
        y=0;
    }
}

removePadding();

int noOfspaces=0; x=0;
while(cipher_Text[x]){ // know how many spaces there in the Plain Text
     if(cipher_Text[x]==' '){noOfspaces++;}
     x++;
}

int *sp = calloc(noOfspaces , sizeof(int));

i=0; x=0;
int textWithoutSpaces=0;
while(cipher_Text[x]){
      if(cipher_Text[x]==' '){
            sp[i] = textWithoutSpaces;
            i++;
      }
      if(isalpha(cipher_Text[x])){
         textWithoutSpaces++;
      }
      x++;
}



/*Put Back the Spaces were exist in the plain-Text*/
int len=0;
while(plainText[len]){
    len++;
}

char plainArray[len*2]; // to hold the plain text temporarily
strcpy(plainArray,plainText);

x=0, i=0; int z=0, l=0;
while(plainArray[x]){
      if(x==sp[i]){
                plainText[z]=' ';
                i++;
                z++;
      }
      plainText[z]=plainArray[x];
      z++;
      x++;
}
plainText[z] = 0;


HANDLE hOutput = GetStdHandle(STD_OUTPUT_HANDLE);
printf("\n PLAIN TEXT:\n");
SetConsoleTextAttribute(hOutput,8);
printf("%s\n", plainText);
SetConsoleTextAttribute(hOutput, 15);
printf("\n");
while(1){
printf("\n");
int option = HMenu(12, -1, 5, ">", 15, "|", 9, 15, 128, 2, "Save To File", "Back To Main Menu");
    switch(option){
    case 1:
        if(savePlainText()){
                delete_matrix(key);
                delete_matrix(keyInverse);
                delete_matrix(extensionMatrix);
                delete_matrix(extensionMatrixInverse);
                deleteColumnMatrix(xEncColumnArray);
                deleteColumnMatrix(extensionsX);
                deleteColumnMatrix(C1);
                deleteColumnMatrix(P);
                free(determinant);
                free(sp);
                diagonalSum=0;
                secondaryDiagonalSum=0;
                resetSymbolsSubstitutionTable();
                emptyDoubleArray();
                emptyArray(plainText);
                emptyArray(cipher_Text);
                resetSubstitutionTable();
                padding[0] =0;
                return 1;
        }
        break;

    case 2:
        delete_matrix(key);
        delete_matrix(keyInverse);
        delete_matrix(extensionMatrix);
        delete_matrix(extensionMatrixInverse);
        deleteColumnMatrix(xEncColumnArray);
        deleteColumnMatrix(extensionsX);
        deleteColumnMatrix(C1);
        deleteColumnMatrix(P);
        free(determinant);
        free(sp);
        diagonalSum=0;
        secondaryDiagonalSum=0;
        resetSymbolsSubstitutionTable();
        emptyDoubleArray();
        emptyArray(plainText);
        emptyArray(cipher_Text);
        resetSubstitutionTable();

        padding[0] =0;
        return 0;
        break;
    }
}

}


/*MIS. FUNCTIONS IN DECRYPTION*/
double convertCharToDouble(char *arrayOfChars, double arrayOfDoubles[]){
int i=0, j=0,x=0;
bool negative = false;


while(arrayOfChars[i]){ // it should start with cipher letter or it may start with a space*/

     if((isdigit(arrayOfChars[i])||(arrayOfChars[i]== '-'))&&(!isspace(arrayOfChars[i]))){
                if(arrayOfChars[i]== '-'){
                    i++;
                    arrayOfDoubles[j]= (arrayOfDoubles[j]*10)+(((double)(arrayOfChars[i]-'0'))*-1);
                    negative = true;
                }
                else if(arrayOfChars[i]!= '-'){
                        if (negative == true){
                        arrayOfDoubles[j]=(arrayOfDoubles[j]*10)-((double)(arrayOfChars[i]-'0'));
                        }
                        else if (negative == false) {arrayOfDoubles[j]=(arrayOfDoubles[j]*10)+((double)(arrayOfChars[i]-'0'));}
                }
      }
        else if(isspace(arrayOfChars[i])){i++;}
        if (isalpha(arrayOfChars[i])&&(arrayOfChars[i])){
                if (i!=0){
                j++;
                negative = false;
                }
        }

        i++;
}
return (double)j+1;
}

void removePadding(){
int numberOfCharsToRemove = find_NumberOfAddedChars();
int plainTextLength=0, i=0;
while(plainText[plainTextLength]){
      plainTextLength++;
}

for (i=0; numberOfCharsToRemove>=0; numberOfCharsToRemove--,plainTextLength--){
     plainText[plainTextLength]=0;
}
}

double roundP(double p){ // if X>0 -> RoundUp. if X<0 -> RoundDown
int pInteger = p;
if (p>0){
    if ((p-pInteger)>=0.5){pInteger+=1; p=(double)pInteger;}
    else if ((p-pInteger)<0.5){p=(double)pInteger;}
}
else if (p<0){
         if (fabs(p-pInteger)>=0.5){pInteger-=1; p=(double)pInteger;}
         else if (fabs(p-pInteger)<0.5){p=(double)pInteger;}
}
return p;
}

void findExtensionMatrixInverse(){
int x,y;
extensionMatrixInverse = calloc(6,sizeof(double*));
for(x=0;x<6;x++){extensionMatrixInverse[x]=calloc(6,sizeof(double));}
extensionMatrixInverse = mInverse(6,6,"lf", extensionMatrix);
}

void findKeyInverse(){
int x;
keyInverse = calloc(6,sizeof(double*));
for(x=0;x<6;x++){keyInverse[x]=calloc(6,sizeof(double));}
keyInverse = mInverse(6,6,"lf", key);
}

void emptyDoubleArray(){
int i = cipherTextLength();
while(i>=0){
      xEncDecryption[i]=0;
      cipherLettersDecryption[i]=0;
      i--;
}
}


/*MAIN FLOW IN DECRYPTION*/
int decryption(){

int x=0;
while(1){
    x=0;
    system("cls");
    if (x=keyMatrixDec()){
            if (x==-1){return -1;}
            else if (x==1){
                     if (symbols()){
                        if(cipherTextFun()){
                          if(decrypt(cipher_Text)){return 1;}
                          else {return 0;}

                    }
                    }
            }
   }
}
return 0;
}






void howToUse(){
HANDLE hOutPut = GetStdHandle(STD_OUTPUT_HANDLE);
system("cls");
SetConsoleTextAttribute(hOutPut, 15);
char s1[] ="\n *Encryption:\n   ~The Key: is a unique key matrix that consists of 36 integers and used to encrypt the plain-text and -later- to decrypt the cipher-text.\n      Choose a proper Key by one of the following methods:\n\t1.Generate Key: Let the program generate the Key for you, the Key -by your choice- can contain Positive Integers Only, Negative Integers Only or Both.\n\t2.Type the Key on the Console Screen.\n\t3.Load a Key from a .txt file.\n \n   ~The Symbols: a set of 11 symbols that is used to form the ciphers.\n\t1.You can use the default symbols that already predefined in the program.\n\t2.You can Type or Load different set of Symbols from a .txt file, and here you need to consider the following:\n\t    First: You cannot choose Letters to be one of the Symbols because they are reserved for the program.\n\t     Second: Do not repeat any Symbol in the same set because that will cause a failure in encryption and decryption.\n\n   ~The Plain Text: is the text that you want to encrypt.\n\t1.You can type your plain-text on the Console Screen or you can load it from a file.\n\n   ~The Cipher Text: is the encrypted plain text by using the Key and the Symbols.\n\t1.You can save the cipher-text into a "".txt"" file to be used later in the Decryption.\n\n     /Finally Send the Key, the Symbols and the Cipher Text separately to the Receiver for Decryption./\n\n\n";

char s2[] = " **Decryption: To Decrypt the Cipher-Text:\n\t1.Insert the Key that sent to you by typing it on the Console Screen or by loading it from its file.\n\t2.Type/Load the Symbols sent to you. If the Default Symbols were used in Encryption then just choose the Use Default option.\n\t3.Type/Load the Cipher-Text that sent to you, then choose Decrypt to generate the Plain-Text.\n\n";

char s3[] = "  ***Important Notes & Hints:\n    -This program only accepts the English Language.\n    -The files that can be used to save the work in and to load it from are of type .txt.\n    -It is always advisable to save your work (i.e. Key, Symbols and Cipher-text) into files and to send them separately to the other party.\n    -You can use a unique Key and Symbols agreed between you and the receiver to be used every time you send or receive a cipher-text. Here, you do not need to generate a new Key or Symbols, you need to send the Key's and the Symbols' Files for one time only and then you just need to send/receive the cipher-text every time you want to correspond with the other party.\n";


printf(s1);
printf("\n   -------------------------------------------------------------------------------------\n\n");
printf(s2);
printf("\n   -------------------------------------------------------------------------------------\n\n");
printf(s3);
printf("\n   -------------------------------------------------------------------------------------\n\n");
SetConsoleTextAttribute(hOutPut, 2);
printf("\n   Press Enter To Continue...");
SetConsoleTextAttribute(hOutPut, 15);
while(getchar()!='\n');
}

void about(){
HANDLE hOutPut = GetStdHandle(STD_OUTPUT_HANDLE);
system("cls");
printf("\n\n");
centerText("HILLY",15,0);
printf("\n");
centerText("An Improved Version Of Hill Cipher",15,0);
printf("\n\n");
centerText("Author: Yahya Almardeny",11,0);
printf("\n\n");
centerText("E-mail: almardeny@gmail.com",15,0);
printf("\n");
centerText("All Rights Reserved 2016",15,0);
printf("\n");
SetConsoleTextAttribute(hOutPut, 2);
printf("\n\n\n  Press Enter To Continue...");
SetConsoleTextAttribute(hOutPut, 15);
while(getchar()!='\n');
}


int gcd(int a){
    int temp;
    int b = 26;
    while (b != 0)
    {
        temp = a % b;

        a = b;
        b = temp;
    }
    return a;

}




