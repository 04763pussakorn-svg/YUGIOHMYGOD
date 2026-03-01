#include <iostream>
#include <string>
#include <algorithm>
#include <random>
using namespace std;

void Duelstart() {
    int LP1 = 4000;
    int LP2 = 4000;
    int turn = 0;
};

class Turn {
    bool Setable;
    bool Summonable;
    bool Attackable;
    bool yourturn;
public:
    void StandbyandDraw();
    void Mainphase();
    void Attackphase();
    void Endphase();
};

void Turn::StandbyandDraw() {
    yourturn = true;
    int turn ++;
    if (turn % 2 != 0) {
        cout << "Player 1's turn!" << endl;
    } else {
        cout << "Player 2's turn!" << endl;
    }
    draw();
};

void Turn::Mainphase() { 
Setable = true;
Summonable = true;
Attackable = false;
};

void Turn::Attackphase() {
Setable = false;
Summonable = false;
Attackable = true;
}; //คนเริ่มคนแรกตีไม่ได้ 

void Turn::Endphase() {
Attackable = false;
yourturn = false;
};
 
string Graveyard[];

void playerWin(){	
	cout << "*******************************************************\n";
	cout << "*                                                     *\n";
	cout << "*                   YOU WIN!!!                        *\n";
	cout << "*                                                     *\n";
	cout << "*******************************************************\n";
};


void playerLose(){
	cout << "*******************************************************\n";
	cout << "*                                                     *\n";
	cout << "*                   YOU LOSE!!!                       *\n";
	cout << "*                                                     *\n";
	cout << "*******************************************************\n";
};
