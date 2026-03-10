#include <Windows.h>
#include "Show.h"
#include "GameManager.h"

int main() {
    srand(time(0));

    Card potOfGreed("Pot of Greed", "Spell", 0, 0, 0, "Draw 2 cards.");
    potOfGreed.spellEffect = [](GameManager* gm, int casterIndex) {
        cout << "\n\033[0;32m[EFFECT] " << (casterIndex == 0 ? "You draw" : "Bot draws") << " 2 cards!\033[0m\n";
        gm->drawCard(casterIndex, 2); // สั่ง GameManager ให้ผู้เล่นจั่ว 2 ใบ
    };

    Card darkHole("Dark Hole", "Spell", 0, 0, 0, "Destroy all monsters on the field.");
    darkHole.spellEffect = [](GameManager* gm, int casterIndex) {
        cout << "\n\033[0;32m[EFFECT] DARK HOLE! All monsters are sucked into the void and DESTROYED!\033[0m\n";
        
        // กวาดมอนสเตอร์ฝั่งผู้เล่นลงสุสาน
        for(Card c : gm->monsterZone[0]) gm->graveyard[0].push_back(c);
        gm->monsterZone[0].clear();
        
        // กวาดมอนสเตอร์ฝั่งบอทลงสุสาน
        for(Card c : gm->monsterZone[1]) gm->graveyard[1].push_back(c);
        gm->monsterZone[1].clear();
    };

    Card heavyStorm("Heavy Storm", "Spell", 0, 0, 0, "Destroy all Spell and Trap cards on the field.");
    heavyStorm.spellEffect = [](GameManager* gm, int casterIndex) {
        cout << "\n\033[0;32m[EFFECT] HEAVY STORM! All Spells and Traps are blown away!\033[0m\n";
        
        // กวาดเวท/กับดักฝั่งผู้เล่นลงสุสาน
        for(Card c : gm->spellTrapZone[0]) gm->graveyard[0].push_back(c);
        gm->spellTrapZone[0].clear();
        
        // กวาดเวท/กับดักฝั่งบอทลงสุสาน
        for(Card c : gm->spellTrapZone[1]) gm->graveyard[1].push_back(c);
        gm->spellTrapZone[1].clear();
    };

    Card cyclone("Cyclone", "Spell", 0, 0, 0, "Destroy 1 Spell/Trap Card opponent controls.");
    cyclone.spellEffect = [](GameManager* gm, int casterIndex) {
        int oppIndex = 1 - casterIndex;
        if (gm->spellTrapZone[oppIndex].empty()) {
            cout << ">> Opponent has no Spells/Traps to destroy.\n"; return;
        }

        int targetIdx = 0;
        if (casterIndex == 0) { // ถ้าเราเป็นคนร่าย
            cout << "Select an opponent's Spell/Trap to destroy:\n";
            for (int i = 0; i < gm->spellTrapZone[oppIndex].size(); i++) {
                // ถ้าหมอบอยู่จะไม่แสดงชื่อ
                string cName = (gm->spellTrapZone[oppIndex][i].status == 2) ? "Face-down Card" : gm->spellTrapZone[oppIndex][i].name;
                cout << "[" << i + 1 << "] " << cName << "\n";
            }
            int choice; cout << "Choice: "; cin >> choice;
            if (choice > 0 && choice <= gm->spellTrapZone[oppIndex].size()) targetIdx = choice - 1;
        } else { // ถ้าบอทเป็นคนร่าย ให้สุ่มทำลาย
            targetIdx = rand() % gm->spellTrapZone[oppIndex].size();
        }

        cout << "\n\033[0;32m[EFFECT] Cyclone destroys a card!\033[0m\n";
        gm->graveyard[oppIndex].push_back(gm->spellTrapZone[oppIndex][targetIdx]);
        gm->spellTrapZone[oppIndex].erase(gm->spellTrapZone[oppIndex].begin() + targetIdx);
    };

    Card shieldCrush("Shield Crush", "Spell", 0, 0, 0, "Destroy 1 Defense Position monster opponent controls.");
    shieldCrush.spellEffect = [](GameManager* gm, int casterIndex) {
        int oppIndex = 1 - casterIndex;
        vector<int> defIndices; // เก็บตำแหน่งมอนสเตอร์ที่ป้องกันอยู่
        
        for (int i = 0; i < gm->monsterZone[oppIndex].size(); i++) {
            if (gm->monsterZone[oppIndex][i].status == 2) defIndices.push_back(i);
        }
        
        if (defIndices.empty()) {
            cout << ">> Opponent has no Defense Position monsters.\n"; return;
        }

        int chosenIdx = defIndices[0];
        if (casterIndex == 0) { // เราเป็นคนเลือก
            cout << "Select Defense monster to destroy:\n";
            for (int i = 0; i < defIndices.size(); i++) cout << "[" << i + 1 << "] Defense Monster\n";
            int choice; cout << "Choice: "; cin >> choice;
            if (choice > 0 && choice <= defIndices.size()) chosenIdx = defIndices[choice - 1];
        } else { // บอทสุ่มเลือก
            chosenIdx = defIndices[rand() % defIndices.size()];
        }

        cout << "\n\033[0;32m[EFFECT] Shield Crush shatters the defense! " << gm->monsterZone[oppIndex][chosenIdx].name << " is DESTROYED!\033[0m\n";
        gm->graveyard[oppIndex].push_back(gm->monsterZone[oppIndex][chosenIdx]);
        gm->monsterZone[oppIndex].erase(gm->monsterZone[oppIndex].begin() + chosenIdx);
    };

    Card monsterReborn("Monster Reborn", "Spell", 0, 0, 0, "Special Summon 1 monster from your Graveyard.");
    monsterReborn.spellEffect = [](GameManager* gm, int casterIndex) {
        if (gm->monsterZone[casterIndex].size() >= 5) {
            cout << ">> Your Monster Zone is full!\n"; return;
        }

        vector<int> gyIndices;
        for (int i = 0; i < gm->graveyard[casterIndex].size(); i++) {
            if (gm->graveyard[casterIndex][i].type == "Monster") gyIndices.push_back(i);
        }
        if (gyIndices.empty()) {
            cout << ">> No monsters in your Graveyard to revive.\n"; return;
        }

        int targetGyIdx = gyIndices[0];
        if (casterIndex == 0) { // เราเป็นคนเลือกตัวชุบ
            cout << "Select a monster to revive:\n";
            for (int i = 0; i < gyIndices.size(); i++) {
                Card c = gm->graveyard[casterIndex][gyIndices[i]];
                cout << "[" << i + 1 << "] " << c.name << " (ATK: " << c.atk << ")\n";
            }
            int choice; cout << "Choice: "; cin >> choice;
            if (choice > 0 && choice <= gyIndices.size()) targetGyIdx = gyIndices[choice - 1];
        } else { 
            // บอทฉลาด จะเลือกชุบตัวที่ ATK สูงสุดเสมอ
            int maxAtk = -1;
            for (int idx : gyIndices) {
                if (gm->graveyard[casterIndex][idx].atk > maxAtk) {
                    maxAtk = gm->graveyard[casterIndex][idx].atk;
                    targetGyIdx = idx;
                }
            }
        }

        Card revivedCard = gm->graveyard[casterIndex][targetGyIdx];
        revivedCard.status = 1; // ชุบมาในโหมดโจมตีเสมอ
        gm->monsterZone[casterIndex].push_back(revivedCard);
        gm->graveyard[casterIndex].erase(gm->graveyard[casterIndex].begin() + targetGyIdx);
        
        cout << "\n\033[0;32m[EFFECT] MONSTER REBORN! Welcome back, " << revivedCard.name << "!\033[0m\n";
    };

    Card rushRecklessly("Rush Recklessly", "Spell", 0, 0, 0, "Target 1 monster you control; it gains 700 ATK.");
    rushRecklessly.spellEffect = [](GameManager* gm, int casterIndex) {
        if (gm->monsterZone[casterIndex].empty()) {
            cout << ">> You have no monsters to buff!\n"; return;
        }

        int targetIdx = 0;
        if (casterIndex == 0) { // เราเลือกตัวที่จะบัฟพลัง
            cout << "Select a monster to buff (+700 ATK):\n";
            for (int i = 0; i < gm->monsterZone[0].size(); i++) {
                cout << "[" << i + 1 << "] " << gm->monsterZone[0][i].name << " (ATK: " << gm->monsterZone[0][i].atk << ")\n";
            }
            int choice; cout << "Choice: "; cin >> choice;
            if (choice > 0 && choice <= gm->monsterZone[0].size()) targetIdx = choice - 1;
        } else { 
            // บอทจะเลือกบัฟพลังให้ตัวที่เก่งที่สุดบนสนาม
            int maxAtk = -1;
            for (int i = 0; i < gm->monsterZone[1].size(); i++) {
                if (gm->monsterZone[1][i].atk > maxAtk) { maxAtk = gm->monsterZone[1][i].atk; targetIdx = i; }
            }
        }

        gm->monsterZone[casterIndex][targetIdx].atk += 700;
        cout << "\n\033[0;32m[EFFECT] " << gm->monsterZone[casterIndex][targetIdx].name << " goes wild! ATK becomes " << gm->monsterZone[casterIndex][targetIdx].atk << "!\033[0m\n";
    };

    Card darkMagicAttack("Dark Magic Attack", "Spell", 0, 0, 0, "If you control 'Dark Magician', destroy all opponent's Spells and Traps.");
    darkMagicAttack.spellEffect = [](GameManager* gm, int casterIndex) {
        bool hasDM = false;
        // เช็คว่าบนสนามเรามี Dark Magician ไหม
        for (Card c : gm->monsterZone[casterIndex]) {
            if (c.name == "Dark Magician") { hasDM = true; break; }
        }

        if (hasDM) {
            int oppIndex = 1 - casterIndex;
            cout << "\n\033[0;32m[EFFECT] DARK MAGIC ATTACK!!! All opponent's Spells and Traps are obliterated!\033[0m\n";
            for(Card c : gm->spellTrapZone[oppIndex]) gm->graveyard[oppIndex].push_back(c);
            gm->spellTrapZone[oppIndex].clear();
        } else {
            cout << "\n\033[31m[!] Effect Failed! You do not control 'Dark Magician'.\033[0m\n";
        }
    };

    Card burstStream("Burst Stream of Destruction", "Spell", 0, 0, 0, "If you control 'Blue-Eyes White Dragon', destroy all opponent's monsters.");
    burstStream.spellEffect = [](GameManager* gm, int casterIndex) {
        bool hasBlueEyes = false;
        
        // เช็คว่าบนสนามเรามี Blue-Eyes White Dragon ไหม
        for (Card c : gm->monsterZone[casterIndex]) {
            if (c.name == "Blue-Eyes White Dragon") { 
                hasBlueEyes = true; 
                break; 
            }
        }

        if (hasBlueEyes) {
            int oppIndex = 1 - casterIndex;
            cout << "\n\033[0;36m[EFFECT] BURST STREAM OF DESTRUCTION!!!\033[0m\n";
            cout << "\033[0;32mAll opponent's monsters are obliterated by the white lightning!\033[0m\n";
            
            // กวาดมอนสเตอร์ศัตรูลงสุสานทั้งหมด
            for(Card c : gm->monsterZone[oppIndex]) {
                gm->graveyard[oppIndex].push_back(c);
            }
            gm->monsterZone[oppIndex].clear(); // ล้างมอนสเตอร์บนสนามศัตรู
            
        } else {
            cout << "\n\033[31m[!] Effect Failed! You do not control 'Blue-Eyes White Dragon'.\033[0m\n";
        }
    };


    //Trap
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

    Card magicCylinder("Magic Cylinder", "Trap", 0, 0, 0, "Negate attack and inflict damage equal to attacker's ATK.");
    magicCylinder.spellEffect = [](GameManager* gm, int casterIndex) {
        int oppIndex = 1 - casterIndex;
        if (gm->monsterZone[oppIndex].empty()) return;

        int targetIdx = 0;
        if (casterIndex == 0) { // เราเลือกว่าจะสะท้อนการโจมตีของตัวไหน
            cout << "Select the attacking monster to reflect its ATK:\n";
            for (int i = 0; i < gm->monsterZone[oppIndex].size(); i++) {
                cout << "[" << i + 1 << "] " << gm->monsterZone[oppIndex][i].name << " (ATK: " << gm->monsterZone[oppIndex][i].atk << ")\n";
            }
            int choice; cout << "Choice: "; cin >> choice;
            if (choice > 0 && choice <= gm->monsterZone[oppIndex].size()) targetIdx = choice - 1;
        } else { // บอทจะเลือกสะท้อนตัวที่ ATK เยอะสุดของเรา
            int maxAtk = -1;
            for (int i = 0; i < gm->monsterZone[oppIndex].size(); i++) {
                if (gm->monsterZone[oppIndex][i].atk > maxAtk) { maxAtk = gm->monsterZone[oppIndex][i].atk; targetIdx = i; }
            }
        }
        
        int reflectedDamage = gm->monsterZone[oppIndex][targetIdx].atk;
        cout << "\n\033[0;35m[TRAP EFFECT] MAGIC CYLINDER! The attack is reflected!\033[0m\n";
        cout << "\033[31mOpponent takes " << reflectedDamage << " damage!\033[0m\n";
        gm->LP[oppIndex] -= reflectedDamage; // หักเลือดศัตรู
    };

    Card sakuretsuArmor("Sakuretsu Armor", "Trap", 0, 0, 0, "Destroy the attacking monster.");
    sakuretsuArmor.spellEffect = [](GameManager* gm, int casterIndex) {
        int oppIndex = 1 - casterIndex;
        if (gm->monsterZone[oppIndex].empty()) return;

        int targetIdx = 0;
        if (casterIndex == 0) {
            cout << "Select the attacking monster to DESTROY:\n";
            for (int i = 0; i < gm->monsterZone[oppIndex].size(); i++) {
                cout << "[" << i + 1 << "] " << gm->monsterZone[oppIndex][i].name << "\n";
            }
            int choice; cout << "Choice: "; cin >> choice;
            if (choice > 0 && choice <= gm->monsterZone[oppIndex].size()) targetIdx = choice - 1;
        } else {
            int maxAtk = -1;
            for (int i = 0; i < gm->monsterZone[oppIndex].size(); i++) {
                if (gm->monsterZone[oppIndex][i].atk > maxAtk) { maxAtk = gm->monsterZone[oppIndex][i].atk; targetIdx = i; }
            }
        }
        cout << "\n\033[0;35m[TRAP EFFECT] SAKURETSU ARMOR! " << gm->monsterZone[oppIndex][targetIdx].name << " is sliced into pieces!\033[0m\n";
        
        // ย้ายมอนสเตอร์เป้าหมายลงสุสาน
        gm->graveyard[oppIndex].push_back(gm->monsterZone[oppIndex][targetIdx]);
        gm->monsterZone[oppIndex].erase(gm->monsterZone[oppIndex].begin() + targetIdx);
    };

    Card drainShield("Draining Shield", "Trap", 0, 0, 0, "Negate attack and gain LP equal to attacker's ATK.");
    drainShield.spellEffect = [](GameManager* gm, int casterIndex) {
        int oppIndex = 1 - casterIndex;
        if (gm->monsterZone[oppIndex].empty()) return;

        int targetIdx = 0;
        if (casterIndex == 0) {
            cout << "Select the attacking monster to absorb its ATK:\n";
            for (int i = 0; i < gm->monsterZone[oppIndex].size(); i++) {
                cout << "[" << i + 1 << "] " << gm->monsterZone[oppIndex][i].name << " (ATK: " << gm->monsterZone[oppIndex][i].atk << ")\n";
            }
            int choice; cout << "Choice: "; cin >> choice;
            if (choice > 0 && choice <= gm->monsterZone[oppIndex].size()) targetIdx = choice - 1;
        } else {
            int maxAtk = -1;
            for (int i = 0; i < gm->monsterZone[oppIndex].size(); i++) {
                if (gm->monsterZone[oppIndex][i].atk > maxAtk) { maxAtk = gm->monsterZone[oppIndex][i].atk; targetIdx = i; }
            }
        }
        int healAmount = gm->monsterZone[oppIndex][targetIdx].atk;
        cout << "\n\033[0;35m[TRAP EFFECT] DRAINING SHIELD! The attack energy is absorbed!\033[0m\n";
        cout << "\033[32mYou recover " << healAmount << " LP!\033[0m\n";
        
        gm->LP[casterIndex] += healAmount; // เพิ่มเลือดให้คนใช้กับดัก
    };

    Card negateAttack("Negate Attack", "Trap", 0, 0, 0, "Negate the opponent's attack.");
    negateAttack.spellEffect = [](GameManager* gm, int casterIndex) {
        cout << "\n\033[0;35m[TRAP EFFECT] NEGATE ATTACK! A swirling vortex absorbs the attack completely!\033[0m\n";
    };

    Card spellbindingCircle("Spellbinding Circle", "Trap", 0, 0, 0, "Decrease an attacking monster's ATK by 700.");
    spellbindingCircle.spellEffect = [](GameManager* gm, int casterIndex) {
        int oppIndex = 1 - casterIndex;
        if (gm->monsterZone[oppIndex].empty()) return;

        int targetIdx = 0;
        if (casterIndex == 0) {
            cout << "Select the attacking monster to bind (-700 ATK):\n";
            for (int i = 0; i < gm->monsterZone[oppIndex].size(); i++) {
                cout << "[" << i + 1 << "] " << gm->monsterZone[oppIndex][i].name << " (ATK: " << gm->monsterZone[oppIndex][i].atk << ")\n";
            }
            int choice; cout << "Choice: "; cin >> choice;
            if (choice > 0 && choice <= gm->monsterZone[oppIndex].size()) targetIdx = choice - 1;
        } else {
            int maxAtk = -1;
            for (int i = 0; i < gm->monsterZone[oppIndex].size(); i++) {
                if (gm->monsterZone[oppIndex][i].atk > maxAtk) { maxAtk = gm->monsterZone[oppIndex][i].atk; targetIdx = i; }
            }
        }
        
        // หักพลังโจมตีลง 700 (ไม่ให้ติดลบ)
        gm->monsterZone[oppIndex][targetIdx].atk -= 700;
        if (gm->monsterZone[oppIndex][targetIdx].atk < 0) gm->monsterZone[oppIndex][targetIdx].atk = 0;
        
        cout << "\n\033[0;35m[TRAP EFFECT] SPELLBINDING CIRCLE! " << gm->monsterZone[oppIndex][targetIdx].name << " is bound by magic!\033[0m\n";
        cout << "Its ATK drops to " << gm->monsterZone[oppIndex][targetIdx].atk << "!\n";
    };

    Card castleWalls("Castle Walls", "Trap", 0, 0, 0, "Increase 1 of your monster's DEF by 500.");
    castleWalls.spellEffect = [](GameManager* gm, int casterIndex) {
        if (gm->monsterZone[casterIndex].empty()) {
            cout << ">> You have no monsters to protect!\n"; return;
        }

        int targetIdx = 0;
        if (casterIndex == 0) {
            cout << "Select a monster to reinforce (+500 DEF):\n";
            for (int i = 0; i < gm->monsterZone[0].size(); i++) {
                cout << "[" << i + 1 << "] " << gm->monsterZone[0][i].name << " (DEF: " << gm->monsterZone[0][i].def << ")\n";
            }
            int choice; cout << "Choice: "; cin >> choice;
            if (choice > 0 && choice <= gm->monsterZone[0].size()) targetIdx = choice - 1;
        } else {
            // บอทจะเลือกป้องกันให้ตัวที่มี DEF สูงสุด
            int maxDef = -1;
            for (int i = 0; i < gm->monsterZone[1].size(); i++) {
                if (gm->monsterZone[1][i].def > maxDef) { maxDef = gm->monsterZone[1][i].def; targetIdx = i; }
            }
        }
        
        gm->monsterZone[casterIndex][targetIdx].def += 500;
        cout << "\n\033[0;35m[TRAP EFFECT] CASTLE WALLS! A sturdy wall surrounds " << gm->monsterZone[casterIndex][targetIdx].name << "!\033[0m\n";
        cout << "Its DEF rises to " << gm->monsterZone[casterIndex][targetIdx].def << "!\n";
    };

    //Deck 1
    vector<Card> deck1 = {
        Card("Beaver Warrior", "Monster", 4, 1200, 1500, "\"A beaver that is skilled in combat.\""),
        Card("Celtic Guardian", "Monster", 4, 1400, 1200, "\"An elf who learned to wield a sword, he baffles enemies with lightning-swift attacks.\""),
        Card("Curse of Dragon", "Monster", 5, 2000, 1500, "\"A wicked dragon that taps into dark forces.\""),
        Card("Dark Magician", "Monster", 7, 2500, 2100, "\"The ultimate wizard in terms of attack and defense.\""),
        Card("Feral Imp", "Monster", 4, 1300, 1400, "\"A playful little fiend that lurks in the dark.\""),
        Card("Gaia The Fierce Knight", "Monster", 7, 2300, 2100, "\"A knight whose horse travels faster than wind.\""),
        Card("Griffore", "Monster", 4, 1600, 1000, "\"This monster's tough hide deflects almost any attack.\""),
        Card("Beaver Warrior", "Monster", 4, 1200, 1500, "\"A beaver that is skilled in combat.\""),
        Card("Celtic Guardian", "Monster", 4, 1400, 1200, "\"An elf who learned to wield a sword, he baffles enemies with lightning-swift attacks.\""),
        Card("Curse of Dragon", "Monster", 5, 2000, 1500, "\"A wicked dragon that taps into dark forces.\""),
        Card("Feral Imp", "Monster", 4, 1300, 1400, "\"A playful little fiend that lurks in the dark.\""),
        Card("Giant Soldier of Stone", "Monster", 3, 1300, 2000, "\"A giant warrior made of stone. \""),
        Card("Griffore", "Monster", 4, 1600, 1000, "\"This monster's tough hide deflects almost any attack.\""),
        Card("Giant Soldier of Stone", "Monster", 3, 1300, 2000, "\"A giant warrior made of stone. \""),
    };//d
    deck1.push_back(potOfGreed);
    deck1.push_back(darkHole);
    deck1.push_back(heavyStorm);
    deck1.push_back(cyclone);
    deck1.push_back(shieldCrush);
    deck1.push_back(monsterReborn);
    deck1.push_back(rushRecklessly);
    deck1.push_back(darkMagicAttack);
    deck1.push_back(mirrorForce);
    deck1.push_back(magicCylinder);
    deck1.push_back(sakuretsuArmor);
    deck1.push_back(drainShield);
    deck1.push_back(negateAttack);
    deck1.push_back(spellbindingCircle);
    deck1.push_back(castleWalls);

    



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
        Card("Battle Ox", "Monster", 4, 1700, 1000, "\"A monster with tremendous power, it destroys enemies with a swing of its axe.\""),
        Card("Saggi the Dark Clown", "Monster", 3, 600, 1500, "\"This clown appears from nowhere and executes very strange moves to avoid enemy attacks.\""),
        Card("Vorse Raider", "Monster", 4, 1900, 1200, "\"This wicked Beast-Warrior does every horrid thing imaginable, and loves it! His axe bears the marks of his countless victims.\""),
        Card("X-Head Cannon", "Monster", 4, 1800, 1500, "\"A monster with a mighty cannon barrel, it is able to integrate its attacks. It attacks in many ways by combining and separating with other monsters.\""),
        Card("Lord of D.", "Monster", 4, 1300, 1200, "\"The lord of the dragon.\""),
        Card("Ryu-Kishin Powered", "Monster", 4, 1600, 1200, "\"A gargoyle enhanced by the powers of darkness. Very sharp talons make it a worthy opponent.\""),
        Card("Saggi the Dark Clown", "Monster", 3, 600, 1500, "\"This clown appears from nowhere and executes very strange moves to avoid enemy attacks.\""),
    };
    deck2.push_back(potOfGreed);
    deck2.push_back(darkHole);
    deck2.push_back(heavyStorm);
    deck2.push_back(cyclone);
    deck2.push_back(shieldCrush);
    deck2.push_back(monsterReborn);
    deck2.push_back(rushRecklessly);
    deck2.push_back(burstStream);
    deck2.push_back(mirrorForce);
    deck2.push_back(magicCylinder);
    deck2.push_back(sakuretsuArmor);
    deck2.push_back(drainShield);
    deck2.push_back(negateAttack);
    deck2.push_back(spellbindingCircle);
    deck2.push_back(castleWalls);
    
    

    GameManager game;
    int urDeck;
    cout << "Select Your Deck!   <Bot will select the another Deck>\n[1] Dark Magician\n[2] Blue Eyes White Dragon" << endl;
    cin >> urDeck;
    if(urDeck == 1) {
        cout << "You Selected Dark Magician Deck!\n";
        game.setDecks(deck1, deck2);}
    else if(urDeck == 2) {
        cout << "You Selected Blue Eyes White Dragon Deck!\n";
        game.setDecks(deck2, deck1);}
    
    game.startDuel();
//Muhahaha
    return 0;
}
