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

// screen size 110x31 chars

#include <iostream>  
#include <Windows.h>  
#include <sstream>
#include <iomanip> 

#include "Draw.h"

using namespace std;
 
// ANSI codes
const string GRAY = "\033[30m";   // Cyan text  1
const string RED = "\033[31m";    // Cyan text  2
const string GREEN = "\033[32m";  // Cyan text  3
const string YELLOW = "\033[33m"; // Cyan text  4
const string BLUE = "\033[34m";   // Cyan text  5
const string PINK = "\033[35m";   // Cyan text  6
const string AQUA = "\033[36m";   // Cyan text  7
const string ORANGE = "\033[38;5;94m";

const string BK_BG = "\033[40m";   // Cyan text  
const string R_BG = "\033[41m";   // Cyan text  
const string G_BG = "\033[42m"; // White text  
const string Y_BG = "\033[43m"; // Blue background  
const string B_BG = "\033[44m";   // Cyan text  
const string P_BG = "\033[45m";   // Cyan text
const string AQ_BG = "\033[46m";   // Cyan text

const string RESET = "\033[0m";   // 0





class Card_info {
    public:
        int id;
        string name;
        string type;
        int stars;
        int atk;
        int def;
        string effect;
        int status;
        int power;
        int cardstatus = 0;
};

Card_info E[10];
Card_info P1,P2,P3,P4,P5,P6,P7,P8,P9,P10;

int Card1[2][5] = {{E[1].cardstatus,E[2].cardstatus,E[3].cardstatus,E[4].cardstatus,E[5].cardstatus},
                  {E[6].cardstatus,E[7].cardstatus,E[8].cardstatus,E[9].cardstatus,E[10].cardstatus}};
int Card2[2][5] = {{P1.cardstatus,P2.cardstatus,P3.cardstatus,P4.cardstatus,P5.cardstatus},
                  {P6.cardstatus,P7.cardstatus,P8.cardstatus,P9.cardstatus,P10.cardstatus}};

                                    
// UI Setting

const int Screen_Width = 110;
const int Screen_height = 32;
char Screen[Screen_height][Screen_Width];
int Screen_Color[Screen_height][Screen_Width];



// ----------------------FUNCTION PROTOTYPE-------------------------
template <typename T>
void printCentered(const T& input, int width);

template<typename T>
void drawText(int x, int y, T value,int color );

void updateScreen();
void render_card();
void create_border();
void create_linex(const int,const int ,int , int);
void create_liney(const int,const int ,int , int);
void createScreen();
int hp_color(char A);
void Details();

void Put_Card(int , int ,int , int,int);

void Start();

void Place_Card(int x , char T, Card B, int naew);

void CopyCardInfo(Card_info & A,Card B);


// ------------------------------MAIN--------------------------------


int enermyhp = 1000;
int playerhp = 2000;

Card A(1,"Hello World","Monster",5,1200,700,"heheh");
Card A2(2,"Hello World","Spell",5,1200,700,"heheh");

int main(){
    Start();


    A.show();
}



// ----------------------FUNCTION DEFINITION-------------------------
void Start(){
    createScreen();
    create_border();
    Details();

    
    Place_Card(2,'E',A,1);
    Place_Card(7,'E',A2,2);


    render_card();

    updateScreen();
}
void createScreen(){
    for(int i = 0; i < Screen_height ; i++){
        for(int j = 0; j < Screen_Width ; j++){
            Screen[i][j] = ' ';
            Screen_Color[i][j] = 0;
        }
    }
}
void updateScreen(){
    for(int i = 0; i < Screen_height ; i++){
        for(int j = 0; j < Screen_Width ; j++){
            if(Screen_Color[i][j] == 0) cout << RESET;
            if(Screen_Color[i][j] == 1) cout << GRAY;
            if(Screen_Color[i][j] == 2) cout << RED;
            if(Screen_Color[i][j] == 3) cout << GREEN;
            if(Screen_Color[i][j] == 4) cout << YELLOW;
            if(Screen_Color[i][j] == 5) cout << BLUE;
            if(Screen_Color[i][j] == 6) cout << PINK;
            if(Screen_Color[i][j] == 7) cout << AQUA;
            if(Screen_Color[i][j] == 8) cout << ORANGE;
            cout << Screen[i][j] << RESET;
        }
        cout << i << "\n" ;
    }
}
void render_card(){
    for(int i = 0 ; i < 2 ; i++){
        for(int j = 0 ; j < 5 ; j++){
            int Color;
                if(E[i*5+j].type == "Monster") Color = 8;
                if(E[i*5+j].type == "Spell") Color = 6;
                if(E[i*5+j].type == "Trap") Color = 2;

            Put_Card(32 + 10*j,5 + 5*i , Card1[i][j] ,Color,5*i+j);
            
        }
        for(int j = 0 ; j < 5 ; j++){
            
            Put_Card(32 + 10*j,17 + 5*i , Card2[i][j] ,2,5*i+j);
            
        }
    }
}

void Place_Card(int x , char T, Card B, int naew){
    if(T == 'E'){
        E[x].cardstatus = naew; CopyCardInfo(E[x],B);
    }
    
    for(int i=0;i<2;i++)
      for(int j=0;j<5;j++)
        Card1[i][j] = E[i*5 + j].cardstatus;
}

void CopyCardInfo(Card_info & A,Card B){
    A.id = B.id;
    A.name = B.name;
    A.type = B.type;
    A.stars = B.stars;
    A.atk = B.atk;
    A.def = B.def;
    A.effect = B.effect;
    A.status = B.status;
    A.power = B.power;
}

void Details(){
    drawText(88, 2, "HP :     ",0);
    drawText(93,2,enermyhp,hp_color('E'));
    drawText(13, 29, "HP :     ",0);
    drawText(18,29,playerhp,hp_color('P'));

}

void Put_Card(int x ,int y ,int Type, int Color,int id){
    if(Type == 1){
        create_linex(x+2,y,4,Color);
        create_linex(x+2,y+3,4,Color);
        create_liney(x+1,y+1,3,Color);
        create_liney(x+6,y+1,3,Color);
        drawText(x+2, y+4, E[id].atk,0);
    }
    if(Type == 2){
        create_linex(x,y+1,7,Color);
        create_linex(x,y+3,7,Color);
        create_liney(x-1,y+2,2,Color);
        create_liney(x+7,y+2,2,Color);
        drawText(x+2, y+4, E[id].def,0);
    }
}

int hp_color(char A){
    if(A == 'E'){
        return (enermyhp > 1500) ? 3 :
                (enermyhp > 1000) ? 0 :
                (enermyhp > 500)  ? 4 :
                                    2;

    }else if(A == 'P'){
        return (playerhp > 1500) ? 3 :
                (playerhp > 1000) ? 0 :
                (playerhp > 500)  ? 4 :
                                    2;
    }else{
        return 0;
    }
}

void create_linex(const int x, const int y, int length, int color = 0){
    for(int l = 0; l < length ; l++){
        if(x+l >= Screen_Width) break;
        Screen_Color[y][l+x] = color;
        Screen[y][l+x] = '_';
    }
}

void create_liney(const int x, const int y, int length, int color = 0){
    for(int l = 0; l < length ; l++){
        if(y+l >= Screen_height) break;
        Screen_Color[l+y][x] = color;
        Screen[l+y][x] = '|';
    }
}

void create_border(){
    create_linex(10,0,90,2);
    create_linex(10,30,90,7);
    create_linex(86,3,14,2);
    create_linex(10,27,14,7);
    create_liney(9,1,30);
    create_liney(100,1,30);
    drawText(85, 3, "\\",2);
    drawText(84, 2, "\\",2);
    drawText(83, 1, "\\",2);
    drawText(25, 28, "\\",7);
    drawText(26, 29, "\\",7);
    drawText(27, 30, "\\",7);
    create_liney(29,5,22,4);
    create_liney(81,5,22,4);
    create_linex(30,4,51,4);
    create_linex(30,26,51,4);
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
template<typename T>
void drawText(int x, int y, T value, int color) {
    if (y < 0 || y >= Screen_height) return;

    stringstream ss;
    ss << value;
    string text = ss.str();

    for (int i = 0; i < text.length(); i++) {
        if (x + i >= 0 && x + i < Screen_Width) {
            Screen_Color[y][x+i] = color;
            Screen[y][x + i] = text[i];
        }
    }
}
































/*
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
*/