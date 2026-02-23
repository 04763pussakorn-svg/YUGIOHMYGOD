#include "draw.h"
// int main() {
  
//         updateScreen();
//         Sleep(500);

    
//     cout << "Game End";
    
//     return 0;  
// }
int main(){
    srand(time(0));
    vector<Card> deck = {
        Card("Beaver Warrior", "Monster", 4, 1200, 1500, "A beaver that is skilled in combat."),
        Card("Celtic Guardian", "Monster", 4, 1400, 1200, "\"An elf who learned to wield a sword, he baffles enemies with lightning-swift attacks.\""),
        Card("Curse of Dragon", "Monster", 5, 2000, 1500, "A wicked dragon that taps into dark forces."),
        Card("Dark Magician", "Monster", 7, 2500, 2100, "The ultimate wizard in terms of attack and defense."),
        Card("Feral Imp", "Monster", 4, 1300, 1400, "A playful little fiend that lurks in the dark."),
        Card("Gaia The Fierce Knight", "Monster", 7, 2300, 2100, "A knight whose horse travels faster than wind."),
        Card("Giant Soldier of Stone", "Monster", 3, 1300, 2000, "A giant warrior made of stone."),
        Card("Griffore", "Monster", 4, 1600, 1000, "\"This monster's tough hide deflects almost any attack.\""),
        Card("Pot of Greed", "Spell", 0, 0, 0, "Draw 2 cards."),
        Card("Mirror Force", "Trap", 0, 0, 0, "Destroy all attack position monsters.")
    };
    cout << "\n--- Draw!! ---" << "\n";
    for(int i = 0; i < 2; i++) {
        Card hand = draw(deck);
        hand.show();
    }
    cout << "\n--- Remaining cards ---" << "\n";
    for (int i = 0; i < deck.size(); i++) {
        cout << "[ " << deck[i].name << " (" << deck[i].type << ") ] ";
    }
    cout << endl;
    return 0;
}  