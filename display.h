#ifndef NEW_DISPLAY_H
#define NEW_DISPLAY_H

#include <Windows.h>  
#include <iostream>  
#include <sstream>
#include <iomanip> 
#include <vector>
#include "Draw.h"

using namespace std;
 
// ANSI codes
const string GRAY = "\033[30m";  
const string RED = "\033[31m";   
const string GREEN = "\033[32m"; 
const string YELLOW = "\033[33m";
const string BLUE = "\033[34m";  
const string PINK = "\033[35m";  
const string AQUA = "\033[36m";  
const string ORANGE = "\033[38;5;94m";
const string RESET = "\033[0m";   

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

Card_info E[10]; // Bot: 0-4 (Spell/Trap), 5-9 (Monster)
Card_info P[10]; // Player: 0-4 (Monster), 5-9 (Spell/Trap)

const int Screen_Width = 110;
const int Screen_height = 32;
char Screen[Screen_height][Screen_Width];
int Screen_Color[Screen_height][Screen_Width];

int enermyhp = 4000;
int playerhp = 4000;

// ----------------------FUNCTION PROTOTYPE-------------------------
void createScreen();
void create_border();
void Details();
void render_card();
void updateScreen();
void create_linex(const int x, const int y, int length, int color = 0);
void create_liney(const int x, const int y, int length, int color = 0);
int hp_color(char A);
void Put_Card(int x ,int y ,int Type, int Color,int id, char owner);

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

inline void CopyCardInfo(Card_info & A, const Card& B){
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

// ฟังก์ชันหลักสำหรับอัปเดตหน้าจอโดยรับค่าตรงจาก GameManager
inline void RenderBoard(int pLP, int eLP, 
                 const vector<Card>& pMon, const vector<Card>& pST, 
                 const vector<Card>& eMon, const vector<Card>& eST) {
    enermyhp = eLP;
    playerhp = pLP;

    for(int i=0; i<10; i++) { E[i].cardstatus = 0; P[i].cardstatus = 0; }

    for(int i=0; i < eST.size(); i++) {
        CopyCardInfo(E[i], eST[i]);
        E[i].cardstatus = (eST[i].status == 0) ? 2 : eST[i].status; 
    }
    for(int i=0; i < eMon.size(); i++) {
        CopyCardInfo(E[5+i], eMon[i]);
        E[5+i].cardstatus = eMon[i].status;
    }

    for(int i=0; i < pMon.size(); i++) {
        CopyCardInfo(P[i], pMon[i]);
        P[i].cardstatus = pMon[i].status;
    }
    for(int i=0; i < pST.size(); i++) {
        CopyCardInfo(P[5+i], pST[i]);
        P[5+i].cardstatus = (pST[i].status == 0) ? 2 : pST[i].status;
    }

    createScreen();
    create_border();
    Details();
    render_card();
    updateScreen();
}

// ----------------------FUNCTION DEFINITION-------------------------
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
            else if(Screen_Color[i][j] == 1) cout << GRAY;
            else if(Screen_Color[i][j] == 2) cout << RED;
            else if(Screen_Color[i][j] == 3) cout << GREEN;
            else if(Screen_Color[i][j] == 4) cout << YELLOW;
            else if(Screen_Color[i][j] == 5) cout << BLUE;
            else if(Screen_Color[i][j] == 6) cout << PINK;
            else if(Screen_Color[i][j] == 7) cout << AQUA;
            else if(Screen_Color[i][j] == 8) cout << ORANGE;
            cout << Screen[i][j] << RESET;
        }
        cout << "\n";
    }
}

void render_card(){
    // วาดการ์ดของ Bot (E)
    for(int i = 0 ; i < 2 ; i++){
        for(int j = 0 ; j < 5 ; j++){
            int id = i*5 + j;
            if(E[id].cardstatus != 0) {
                int Color = (E[id].type == "Monster") ? 8 : ((E[id].type == "Spell") ? 6 : 2);
                Put_Card(32 + 10*j, 5 + 5*i, E[id].cardstatus, Color, id, 'E');
            }
        }
    }
    // วาดการ์ดของ Player (P)
    for(int i = 0 ; i < 2 ; i++){
        for(int j = 0 ; j < 5 ; j++){
            int id = i*5 + j;
            if(P[id].cardstatus != 0) {
                int Color = (P[id].type == "Monster") ? 8 : ((P[id].type == "Spell") ? 6 : 2);
                Put_Card(32 + 10*j, 17 + 5*i, P[id].cardstatus, Color, id, 'P');
            }
        }
    }
}

void Put_Card(int x ,int y ,int Type, int Color,int id, char owner){
    Card_info &card = (owner == 'E') ? E[id] : P[id];
    
    if(Type == 1){ // หงายหน้าโจมตี หรือ หงายการ์ดเวทย์
        create_linex(x+2,y,4,Color);
        create_linex(x+2,y+3,4,Color);
        create_liney(x+1,y+1,3,Color);
        create_liney(x+6,y+1,3,Color);
        if(card.type == "Monster") drawText(x+2, y+4, card.atk, 0); // โชว์ ATK
    }
    else if(Type == 2){ // คว่ำหน้าป้องกัน หรือ เซ็ตการ์ด
        create_linex(x,y+1,7,Color);
        create_linex(x,y+3,7,Color);
        create_liney(x-1,y+2,2,Color);
        create_liney(x+7,y+2,2,Color);
        if (owner == 'P' && card.type == "Monster") {
            drawText(x+2, y+4, card.def, 0); // โชว์ DEF ให้ฝั่งตัวเองเห็น
        } else {
            drawText(x+2, y+4, "DEF", 0); // ซ่อนสเตตัสถ้าเป็นของบอทหรือการ์ดเวทย์หมอบ
        }
    }
}

void Details(){
    drawText(88, 2, "HP :     ",0);
    drawText(93, 2, enermyhp, hp_color('E'));
    drawText(13, 29, "HP :     ",0);
    drawText(18, 29, playerhp, hp_color('P'));
}

int hp_color(char A){
    int hp = (A == 'E') ? enermyhp : playerhp;
    if(hp > 1500) return 3; // เขียว
    else if(hp > 1000) return 0; // ขาว
    else if(hp > 500) return 4; // เหลือง
    else return 2; // แดง
}

void create_linex(const int x, const int y, int length, int color){
    for(int l = 0; l < length ; l++){
        if(x+l >= Screen_Width) break;
        Screen_Color[y][l+x] = color;
        Screen[y][l+x] = '_';
    }
}

void create_liney(const int x, const int y, int length, int color){
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

#endif