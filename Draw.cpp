#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include <ctime>
using namespace std;

struct Card {
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
    vector<Card> whatdahell = {{"JohnCena",890,460},{"Conan",350,20},{"Jesus",777,888},{"Dr.KArn",999,999},{"GodUsoff",578,0}};
    for(int i = 0; i < 2; i++){
        Card mons = draw(whatdahell);
        cout << "[ " << mons.cn << "," << mons.atk << "," << mons.def <<  " ] " ;
    }
    cout << "\n";
    for (int i = 0; i < whatdahell.size(); i++) {
        cout << "[ " << whatdahell[i].cn << "," << whatdahell[i].atk << "," << whatdahell[i].def << "] ";
    }
    return 0;
}