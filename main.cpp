#include <Windows.h>
#include "Draw.h"
#include "GameManager.h"

int main() {
    srand(time(0));

    Card potOfGreed("Pot of Greed", "Spell", 0, 0, 0, "Draw 2 cards.");
    potOfGreed.spellEffect = [](GameManager* gm, int casterIndex) {
        cout << "\n\033[0;32m[EFFECT] " << (casterIndex == 0 ? "You draw" : "Bot draws") << " 2 cards!\033[0m\n";
        gm->drawCard(casterIndex, 2); // สั่ง GameManager ให้ผู้เล่นจั่ว 2 ใบ
    };

    Card mirrorForce("Mirror Force", "Trap", 0, 0, 0, "Destroy all attack position monsters.");
    mirrorForce.spellEffect = [](GameManager* gm, int casterIndex) {
        int oppIndex = 1 - casterIndex; // สลับหา index ของศัตรู
        cout << "\n\033[0;35m[TRAP EFFECT] Mirror Force activated! All attacking monsters are DESTROYED!\033[0m\n";
        
        // วนลูปเช็คโซนมอนสเตอร์ศัตรูจากหลังมาหน้า
        for (int i = gm->monsterZone[oppIndex].size() - 1; i >= 0; i--) {
            if (gm->monsterZone[oppIndex][i].status == 1) { // ถ้าติดสถานะโจมตี (1)
                gm->graveyard[oppIndex].push_back(gm->monsterZone[oppIndex][i]); // ส่งลงสุสาน
                gm->monsterZone[oppIndex].erase(gm->monsterZone[oppIndex].begin() + i); // ลบออกจากกระดาน
            }
        }
    };

    //Deck 1
    vector<Card> deck1 = {
        Card("Beaver Warrior", "Monster", 4, 1200, 1500, "\"A beaver that is skilled in combat.\""),
        Card("Celtic Guardian", "Monster", 4, 1400, 1200, "\"An elf who learned to wield a sword, he baffles enemies with lightning-swift attacks.\""),
        Card("Curse of Dragon", "Monster", 5, 2000, 1500, "\"A wicked dragon that taps into dark forces.\""),
        Card("Dark Magician", "Monster", 7, 2500, 2100, "\"The ultimate wizard in terms of attack and defense.\""),
        Card("Feral Imp", "Monster", 4, 1300, 1400, "\"A playful little fiend that lurks in the dark.\""),
        Card("Gaia The Fierce Knight", "Monster", 7, 2300, 2100, "\"A knight whose horse travels faster than wind.\""),
        Card("Giant Soldier of Stone", "Monster", 3, 1300, 2000, "\"A giant warrior made of stone. \""),
        Card("Griffore", "Monster", 4, 1600, 1000, "\"This monster's tough hide deflects almost any attack.\""),
        Card("Beaver Warrior", "Monster", 4, 1200, 1500, "\"A beaver that is skilled in combat.\""),
        Card("Celtic Guardian", "Monster", 4, 1400, 1200, "\"An elf who learned to wield a sword, he baffles enemies with lightning-swift attacks.\""),
        Card("Curse of Dragon", "Monster", 5, 2000, 1500, "\"A wicked dragon that taps into dark forces.\""),
        Card("Dark Magician", "Monster", 7, 2500, 2100, "\"The ultimate wizard in terms of attack and defense.\""),
        Card("Feral Imp", "Monster", 4, 1300, 1400, "\"A playful little fiend that lurks in the dark.\""),
        Card("Gaia The Fierce Knight", "Monster", 7, 2300, 2100, "\"A knight whose horse travels faster than wind.\""),
        Card("Giant Soldier of Stone", "Monster", 3, 1300, 2000, "\"A giant warrior made of stone. \""),
        Card("Griffore", "Monster", 4, 1600, 1000, "\"This monster's tough hide deflects almost any attack.\""),
        Card("Beaver Warrior", "Monster", 4, 1200, 1500, "\"A beaver that is skilled in combat.\""),
        Card("Celtic Guardian", "Monster", 4, 1400, 1200, "\"An elf who learned to wield a sword, he baffles enemies with lightning-swift attacks.\""),
        Card("Curse of Dragon", "Monster", 5, 2000, 1500, "\"A wicked dragon that taps into dark forces.\""),
        Card("Dark Magician", "Monster", 7, 2500, 2100, "\"The ultimate wizard in terms of attack and defense.\""),
        Card("Feral Imp", "Monster", 4, 1300, 1400, "\"A playful little fiend that lurks in the dark.\""),
        Card("Gaia The Fierce Knight", "Monster", 7, 2300, 2100, "\"A knight whose horse travels faster than wind.\""),
        Card("Giant Soldier of Stone", "Monster", 3, 1300, 2000, "\"A giant warrior made of stone. \""),
        Card("Griffore", "Monster", 4, 1600, 1000, "\"This monster's tough hide deflects almost any attack.\"")
    };
    deck1.push_back(potOfGreed);
    deck1.push_back(mirrorForce);

    //Deck 2
    vector<Card> deck2 = {
        Card("Blue-Eyes White Dragon", "Monster", 8, 3000, 2500, "\"This legendary dragon is a powerful engine of destruction. Virtually invincible, very few have faced this awesome creature and lived to tell the tale.\""),
        Card("Battle Ox", "Monster", 4, 1700, 1000, "\"A monster with tremendous power, it destroys enemies with a swing of its axe.\""),
        Card("Saggi the Dark Clown", "Monster", 3, 600, 1500, "\"This clown appears from nowhere and executes very strange moves to avoid enemy attacks.\""),
        Card("Vorse Raider", "Monster", 4, 1900, 1200, "\"This wicked Beast-Warrior does every horrid thing imaginable, and loves it! His axe bears the marks of his countless victims.\""),
        Card("X-Head Cannon", "Monster", 4, 1800, 1500, "\"A monster with a mighty cannon barrel, it is able to integrate its attacks. It attacks in many ways by combining and separating with other monsters.\""),
        Card("Lord of D.", "Monster", 4, 1300, 1200, "\"The lord of the dragon.\""),
        Card("Hyozanryu", "Monster", 7, 2100, 2800, "\"A dragon created from a massive diamond that sparkles with blinding light.\""),
        Card("Ryu-Kishin Powered", "Monster", 4, 1600, 1200, "\"A gargoyle enhanced by the powers of darkness. Very sharp talons make it a worthy opponent.\""),
        Card("Blue-Eyes White Dragon", "Monster", 8, 3000, 2500, "\"This legendary dragon is a powerful engine of destruction. Virtually invincible, very few have faced this awesome creature and lived to tell the tale.\""),
        Card("Battle Ox", "Monster", 4, 1700, 1000, "\"A monster with tremendous power, it destroys enemies with a swing of its axe.\""),
        Card("Saggi the Dark Clown", "Monster", 3, 600, 1500, "\"This clown appears from nowhere and executes very strange moves to avoid enemy attacks.\""),
        Card("Vorse Raider", "Monster", 4, 1900, 1200, "\"This wicked Beast-Warrior does every horrid thing imaginable, and loves it! His axe bears the marks of his countless victims.\""),
        Card("X-Head Cannon", "Monster", 4, 1800, 1500, "\"A monster with a mighty cannon barrel, it is able to integrate its attacks. It attacks in many ways by combining and separating with other monsters.\""),
        Card("Lord of D.", "Monster", 4, 1300, 1200, "\"The lord of the dragon.\""),
        Card("Hyozanryu", "Monster", 7, 2100, 2800, "\"A dragon created from a massive diamond that sparkles with blinding light.\""),
        Card("Ryu-Kishin Powered", "Monster", 4, 1600, 1200, "\"A gargoyle enhanced by the powers of darkness. Very sharp talons make it a worthy opponent.\""),
        Card("Blue-Eyes White Dragon", "Monster", 8, 3000, 2500, "\"This legendary dragon is a powerful engine of destruction. Virtually invincible, very few have faced this awesome creature and lived to tell the tale.\""),
        Card("Battle Ox", "Monster", 4, 1700, 1000, "\"A monster with tremendous power, it destroys enemies with a swing of its axe.\""),
        Card("Saggi the Dark Clown", "Monster", 3, 600, 1500, "\"This clown appears from nowhere and executes very strange moves to avoid enemy attacks.\""),
        Card("Vorse Raider", "Monster", 4, 1900, 1200, "\"This wicked Beast-Warrior does every horrid thing imaginable, and loves it! His axe bears the marks of his countless victims.\""),
        Card("X-Head Cannon", "Monster", 4, 1800, 1500, "\"A monster with a mighty cannon barrel, it is able to integrate its attacks. It attacks in many ways by combining and separating with other monsters.\""),
        Card("Lord of D.", "Monster", 4, 1300, 1200, "\"The lord of the dragon.\""),
        Card("Hyozanryu", "Monster", 7, 2100, 2800, "\"A dragon created from a massive diamond that sparkles with blinding light.\""),
        Card("Ryu-Kishin Powered", "Monster", 4, 1600, 1200, "\"A gargoyle enhanced by the powers of darkness. Very sharp talons make it a worthy opponent.\"")
    };
    deck2.push_back(potOfGreed);
    deck2.push_back(mirrorForce);
    deck2.push_back(potOfGreed);
    deck2.push_back(mirrorForce);

    GameManager game;
    game.setDecks(deck1, deck2);
    game.startDuel();
    //hello test test
    return 0;
}