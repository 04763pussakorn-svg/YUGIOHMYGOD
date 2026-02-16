#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include <ctime>
using namespace std;

struct Card {
    // string 
    int cn;
    // int atk;
    // int def;
};

Card draw(vector<Card> &deck){
    int x = rand() % deck.size(); 
    Card mons = deck[x];
    deck.erase(deck.begin() + x);
    return mons;
}

int main(){
    srand(time(0));
    vector<Card> whatdahell = {{1},{2},{3},{5},{10},{9},{6},{7},{8}};
    for(int i = 0; i < 5; i++){
        Card mons = draw(whatdahell);
        cout << mons.cn << " ";
    }
    cout << "\n";
    for (int i = 0; i < whatdahell.size(); i++) {
        cout << whatdahell[i].cn << " ";
    }
    return 0;
}