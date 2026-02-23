#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include <ctime>
using namespace std;

class Card {
    public:
    string cn;
    int atk;
    int def;
};

Card draw(vector<Card> &deck){
    int x = rand() % deck.size(); 
    Card mons = deck[x];
    deck.erase(deck.begin() + x);
    return mons;
}

int main(){
    srand(time(0));
    vector<Card> card = {};
    for(int i = 0; i < 2; i++){
        Card mons = draw(card);
        cout << "[ " << mons.cn << "," << mons.atk << "," << mons.def <<  " ] " ;
    }
    cout << "\n";
    for (int i = 0; i < card.size(); i++) {
        cout << "[ " << card[i].cn << "," << card[i].atk << "," << card[i].def << "] ";
    }
    return 0;
}