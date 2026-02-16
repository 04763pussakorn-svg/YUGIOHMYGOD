// 1 chong = 5x9
// |   ____      ____      ____      ____      ____    |
// |  |    |    |    |    |    |    |    |    |    |   |
// |  |    |    |    |    |    |    |    |    |    |   |
// |  |____|    |____|    |____|    |____|    |____|   |
// |   9999      9999      9999      9999      9999    |
// |  _______   _______   _______   _______   _______  |
// | |       | |       | |       | |       | |       | |
// | |_______| |_______| |_______| |_______| |_______| |
// |   8888      8888      8888      8888      8888    |
//  ___________________________________________________


#include <iostream>  
#include <Windows.h>  
#include <sstream>
#include <iomanip> 

using namespace std;
 
// ANSI codes
const string GRAY = "\033[30m";   // Cyan text  
const string RED = "\033[31m";   // Cyan text  
const string GREEN = "\033[32m";   // Cyan text  
const string YELLOW = "\033[33m";   // Cyan text  
const string BLUE = "\033[34m";   // Cyan text  
const string PINK = "\033[35m";   // Cyan text  
const string AQUA = "\033[36m";   // Cyan text  

const string BK_BG = "\033[40m";   // Cyan text  
const string R_BG = "\033[41m";   // Cyan text  
const string G_BG = "\033[42m"; // White text  
const string Y_BG = "\033[43m"; // Blue background  
const string B_BG = "\033[44m";   // Cyan text  
const string P_BG = "\033[45m";   // Cyan text
const string AQ_BG = "\033[46m";   // Cyan text

const string RESET = "\033[0m";

// UI Setting
const int table_padding_left = 20;

const int screen_pad = 10;

int Card1[2][5] = {{0,0,0,0,0},
                  {0,1,1,1,0}};
int Card2[2][5] = {{0,0,1,0,0},
                  {0,0,0,0,0}};


// ----------------------FUNCTION PROTOTYPE-------------------------
template <typename T>
void printCentered(const T& input, int width);
void printrow(int Card1[2][5], int row);
void printTable(int [2][5],int [2][5]);
void updateScreen();


void sl(){
    cout << setw(screen_pad) << right << "|";
}
void sr(){
    cout << setw(table_padding_left+1) << right << "|" << endl;
}
void enermy_side(int hp){
    sl();
    cout << setw(table_padding_left+52) << right << "" << "  \\                 |" << endl;
    sl();
    cout << setw(table_padding_left+52) << right << "" << "   \\    HP : ";


    hp > 1500 ?  cout << GREEN : hp > 1000 ? cout << YELLOW : hp > 500 ? 
    cout << RESET : cout << RED ;

    printCentered(hp,4);
    cout << RESET << "   |" << endl;
    sl();
    cout << setw(table_padding_left+52) << right << "" << "    \\_______________|" << endl;
}                   
void player_side(int hp){
    sl();
    cout << "______________   " << setw(56+table_padding_left) << right << "|" << endl;
    sl();
    cout << "               \\ " << setw(56+table_padding_left) << right << "|" << endl;
    sl();
    cout << "   HP : " << setw(4) << left << hp << "    \\" << setw(56+table_padding_left) << right << "|" << endl;
    sl();
    cout << "_________________\\__________________________________________________________________________|" << endl;
}       
 
// ------------------------------MAIN--------------------------------
 


// ----------------------FUNCTION DEFINITION-------------------------

void updateScreen(){
    cout << setw(screen_pad) << right << " " << "____________________________________________________________________________________________" << endl;
    enermy_side(100);
    printTable(Card1,Card2);
    player_side(2000);
}

template <typename T>
void printCentered(const T& input, int width) {
    ostringstream text_str;
    text_str << input;
    int padding = width - (text_str.str()).length();
    int leftPad = padding / 2;
    int rightPad = padding - leftPad;

    cout << string(leftPad, ' ')
         << input
         << string(rightPad, ' ');
}

void printTable(int enermy_card[2][5] , int player_card[2][5]) {
    sl();
    cout << setw(table_padding_left) << right << " " << YELLOW <<"___________________________________________________ " << RESET;
    sr();
    printrow(enermy_card,0);
    printrow(enermy_card,1);
    sl();
    cout << setw(table_padding_left-1) << right << "" << YELLOW << "|                                                   |" << RESET;
    sr();
    printrow(player_card,0);
    printrow(player_card,1);
    sl();
    cout << setw(table_padding_left-1) << right << " " << YELLOW << "|___________________________________________________|" << RESET;
    sr();

}  

void printrow(int Card[2][5], int row){
    for(int i = 0;i < 5 ; i++){
        sl();
        cout << setw(table_padding_left) << right << YELLOW <<"    |" << RESET;
        switch (i)
        {
        case 0:
            for(int j = 0; j < 5 ; j++){
                if(Card[row][j] == 0){
                    cout << setw(9) << left << "" ;
                }else if(Card[row][j] == 1){
                    cout << setw(9) << left << "  ____";
                }else if(Card[row][j] == 2){
                    cout << setw(9) << left << " _______";
                }
                cout << " ";
            }
            break;
        case 1:
            for(int j = 0; j < 5 ; j++){
                if(Card[row][j] == 0){
                    cout << setw(9) << left << "";
                }else if(Card[row][j] == 1){
                    cout << setw(9) << left << " |    |";
                }else if(Card[row][j] == 2){
                    cout << setw(9) << left << "|       |";
                }
                cout << " ";
            }
            break;
        case 2:
            for(int j = 0; j < 5 ; j++){
                if(Card[row][j] == 0){
                    cout << setw(9) << left << "";
                }else if(Card[row][j] == 1){
                    cout << setw(9) << left << " |    |";
                }else if(Card[row][j] == 2){
                    cout << setw(9) << left << "|_______|";
                }
                cout << " ";
            }
            break;
        case 3:
            for(int j = 0; j < 5 ; j++){
                if(Card[row][j] == 0){
                    cout << setw(9) << left << "";
                }else if(Card[row][j] == 1){
                    cout << setw(9) << left << " |____|";
                }else if(Card[row][j] == 2){
                    cout << setw(9) << left << "";
                }
                cout << " ";
            }
            break;
        case 4:
            for(int j = 0; j < 5 ; j++){
                if(Card[row][j] == 0){
                    cout << setw(9) << left << "";
                }else if(Card[row][j] == 1){
                    printCentered(999, 9);
                }else if(Card[row][j] == 2){
                    printCentered("%DEF%", 9);
                }
                cout << " ";
            }
            break;
        
        }
        cout << YELLOW <<" |" << RESET;
        sr();
        
    }
}
