#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H//cp

#include <Windows.h> 
#include "new_display.h" 
#include <vector>
#include <iostream>
#include <algorithm> 
#include <random>    
#include <ctime>
using namespace std;
#include "Show.h"

class GameManager {
public:
    int LP[2]; 
    vector<Card> deck[2];
    vector<Card> hand[2];
    
    vector<Card> monsterZone[2];   
    vector<Card> spellTrapZone[2]; 
    vector<Card> graveyard[2];     
    int turnCount;                 
    int currentPlayer;             

public:
    GameManager() {
        LP[0] = 4000;
        LP[1] = 4000;
        turnCount = 1;
        currentPlayer = 0; 
    }

    void setDecks(vector<Card>& playerDeck, vector<Card>& botDeck) {
        deck[0] = playerDeck;
        deck[1] = botDeck;

        auto rng = default_random_engine(time(0));
        shuffle(deck[0].begin(), deck[0].end(), rng);
        shuffle(deck[1].begin(), deck[1].end(), rng);
    }

    void startDuel() {
        cout << "\n==============================\n";
        cout << "         DUEL START!!!        \n";
        cout << "==============================\n";

        initialDraw();
        while (LP[0] > 0 && LP[1] > 0) {
            playTurn(currentPlayer);
            currentPlayer = 1 - currentPlayer; 
            turnCount++;
        }
        announceWinner();
    }

    void displayBoard() {
        RenderBoard(LP[0], LP[1], monsterZone[0], spellTrapZone[0], monsterZone[1], spellTrapZone[1]);
    }

public:
    void drawCard(int playerIndex, int amount = 1) {
        for (int i = 0; i < amount; i++) {
            if (deck[playerIndex].empty()) {
                cout << "\n[!] Deck Out! ";
                if (playerIndex == 0) cout << "You have no more cards to draw. You LOSE!\n";
                else cout << "Bot has no more cards to draw. You WIN!\n";
                LP[playerIndex] = 0; 
                return;
            }
            Card drawnCard = deck[playerIndex].back();
            deck[playerIndex].pop_back(); 
            drawnCard.status = 5;
            hand[playerIndex].push_back(drawnCard);

            if (playerIndex == 0) cout << "You drew: \033[33m" << drawnCard.name << "\033[0m\n";
            else cout << "Bot drew a card.\n";
        }
    }

private:
    int executeBattle(Card attacker, Card target, int &attackerLP, int &defenderLP, string attackerOwner, string defenderOwner) {
        if (target.status == 1) { 
            if (attacker.atk > target.atk) {
                int dmg = attacker.atk - target.atk;
                cout << defenderOwner << "'s monster is DESTROYED! \033[31m" << defenderOwner << " takes " << dmg << " damage!\033[0m\n";
                defenderLP -= dmg;
                return 1; 
            } 
            else if (attacker.atk < target.atk) {
                int dmg = target.atk - attacker.atk;
                cout << attackerOwner << "'s attack failed! Monster is DESTROYED! \033[31m" << attackerOwner << " takes " << dmg << " damage!\033[0m\n";
                attackerLP -= dmg;
                return 2; 
            } 
            else {
                cout << "Both monsters have equal ATK! Both are DESTROYED!\n";
                return 3; 
            }
        } 
        else { 
            if (attacker.atk > target.def) {
                cout << defenderOwner << "'s defending monster is DESTROYED! (No damage to LP)\n";
                return 1; 
            } 
            else if (attacker.atk < target.def) {
                int dmg = target.def - attacker.atk;
                cout << attackerOwner << " couldn't break the defense! \033[31m" << attackerOwner << " takes " << dmg << " recoil damage!\033[0m\n";
                attackerLP -= dmg;
                return 0; 
            } 
            else {
                cout << "Attack stopped. No monsters destroyed.\n";
                return 0; 
            }
        }
    }

    void showPlayerHand() {
        cout << "\n======================================================\n";
        cout << "                  [ YOUR HAND ]                       \n";
        cout << "======================================================\n";
        if (hand[0].empty()) {
            cout << "  (Your hand is empty)\n";
        } else {
            for (int i = 0; i < hand[0].size(); i++) {
                Card c = hand[0][i];
                string colorCode = "\033[0m"; 
                if (c.type == "Monster") colorCode = "\033[38;5;94m"; 
                else if (c.type == "Spell") colorCode = "\033[0;32m"; 
                else if (c.type == "Trap") colorCode = "\033[0;35m";  

                cout << " [" << i + 1 << "] " << colorCode << c.name << "\033[0m ";
                if (c.type == "Monster") {
                    cout << "(Monster) - ATK: " << c.atk << " / DEF: " << c.def << " / LV: " << c.stars;
                } else {
                    cout << "(" << c.type << ")";
                }
                cout << "\n";
            }
        }
        cout << "======================================================\n";
    }

private:
    void playTurn(int playerIndex) {
        if (playerIndex == 0) cout << "\n--- Player 1's Turn --- (Turn " << turnCount << ")\n";
        else cout << "\n--- Bot's Turn --- (Turn " << turnCount << ")\n";

        if(turnCount >= 2) drawPhase(playerIndex);
        
        if (playerIndex == 0) {
            playerMainPhase();
            if (turnCount > 1) playerBattlePhase(); 
        } else {
            botMainPhase();
            if (turnCount > 1) botBattlePhase();
        }
        endPhase(playerIndex);
    }

    void initialDraw() {
        drawCard(0, 5); 
        drawCard(1, 5); 
    }

    void drawPhase(int playerIndex) {
        if (playerIndex == 0) cout << "\n[ Phase: DRAW PHASE ]\n";
        drawCard(playerIndex, 1);
    }

    void playerMainPhase() {
        bool endPhase = false;
        bool hasSummoned = false; // เช็คว่าเทิร์นนี้ลงมอนสเตอร์ไปหรือยัง
        while (!endPhase) {
            
            // เรียกกราฟิกให้แสดงกระดานล่าสุด
            displayBoard();
            
            cout << "\n[ Phase: MAIN PHASE 1 ]\n";
            showPlayerHand();
            
            cout << "\nOptions:\n";
            if (!hand[0].empty()) cout << "[1-" << hand[0].size() << "] Select a card to Check Info & Play\n";
            cout << "[0] End Main Phase\n";
            cout << "Choose action: ";
            
            int choice;
            cin >> choice;
            
            if (choice == 0) {
                endPhase = true;
                cout << "\nEnding Main Phase...\n";
            } 
            else if (choice > 0 && choice <= hand[0].size()) {
                int cardIndex = choice - 1;
                Card selectedCard = hand[0][cardIndex];
                selectedCard.show(); 

                // --- กรณีเลือกลงมอนสเตอร์ ---
                if (selectedCard.type == "Monster" && !hasSummoned) {
                    cout << "\nWhat would you like to do with this monster?\n";
                    if (monsterZone[0].size() >= 5) {
                        cout << "\033[31m[!] Your Monster Zone is full! You can only Cancel.\033[0m\n";
                        cout << "[0] Cancel\n";
                        int cancelChoice; cin >> cancelChoice;
                        continue;
                    } else {
                        cout << "[1] Normal Summon (Face-up Attack)\n";
                        cout << "[2] Set (Face-down Defense)\n";
                        cout << "[0] Cancel (Keep in hand)\n";
                    }
                    cout << "Choice: ";
                    
                    int posChoice;
                    cin >> posChoice;

                    if (posChoice == 1 || posChoice == 2) {
                        // ---- ระบบสังเวยมอนสเตอร์ (Tribute Summon) ----
                        int reqTribute = 0;
                        if (selectedCard.stars == 5 || selectedCard.stars == 6) reqTribute = 1;
                        else if (selectedCard.stars >= 7) reqTribute = 2;

                        if (reqTribute > 0) {
                            if (monsterZone[0].size() < reqTribute) {
                                cout << "\033[31m[!] You don't have enough monsters to tribute! (Requires " << reqTribute << ")\033[0m\n";
                                continue; // มอนสเตอร์บนสนามไม่พอ ให้ข้ามไปเลือกไพ่ใบอื่น
                            }
                            
                            cout << "\n\033[33m>> Level " << selectedCard.stars << " monster requires " << reqTribute << " tribute(s).\033[0m\n";
                            for (int t = 0; t < reqTribute; t++) {
                                int tributeChoice;
                                while (true) {
                                    cout << "Select your monster on the field to tribute (1-" << monsterZone[0].size() << "): ";
                                    cin >> tributeChoice;
                                    if (tributeChoice >= 1 && tributeChoice <= monsterZone[0].size()) {
                                        break;
                                    }
                                    cout << "\033[31m[!] Invalid choice. Try again.\033[0m\n";
                                }
                                int tIndex = tributeChoice - 1;
                                
                                // ย้ายมอนสเตอร์ที่สังเวยลงสุสาน
                                graveyard[0].push_back(monsterZone[0][tIndex]);
                                monsterZone[0].erase(monsterZone[0].begin() + tIndex);
                                
                                cout << ">> \033[31mMonster tributed!\033[0m\n";
                            }
                        }
                        // ---------------------------------------------

                        selectedCard.status = posChoice; 
                        monsterZone[0].push_back(selectedCard); 
                        hand[0].erase(hand[0].begin() + cardIndex); 
                        
                        if (posChoice == 1) cout << "\n>> \033[38;5;94mYou Summoned '" << selectedCard.name << "' in Attack Position!\033[0m\n";
                        else cout << "\n>> \033[38;5;94mYou Set a monster in face-down Defense Position.\033[0m\n";
                        hasSummoned = true;
                    }

                } 
                else if (selectedCard.type == "Monster" && hasSummoned) {
                    cout << "\033[31m[!] You have already Normal Summoned/Set a monster this turn! You can only Cancel.\033[0m\n";
                    int cancelChoice; cout << "[0] Cancel\nChoice: "; cin >> cancelChoice;
                }

                // --- กรณีเลือกใช้เวทมนตร์ หรือ กับดัก ---
                else if (selectedCard.type == "Spell" || selectedCard.type == "Trap") {
                    cout << "\nWhat would you like to do with this card?\n";
                    if (spellTrapZone[0].size() >= 5) {
                        cout << "\033[31m[!] Your Spell/Trap Zone is full! You can only Cancel.\033[0m\n";
                        int cancelChoice; cout << "[0] Cancel\nChoice: "; cin >> cancelChoice;
                        continue;
                    } else {
                        if (selectedCard.type == "Spell") {
                            cout << "[1] Activate Spell (Face-up)\n";
                            cout << "[2] Set Spell (Face-down)\n";
                        } else { 
                            cout << "[2] Set Trap (Face-down)\n";
                        }
                        cout << "[0] Cancel (Keep in hand)\n";
                    }
                    cout << "Choice: ";
                    
                    int posChoice;
                    cin >> posChoice;

                    if (posChoice == 1 && selectedCard.type == "Spell") {
                        cout << "\n>> \033[0;32mYou Activated Spell Card: '" << selectedCard.name << "'!\033[0m\n";
                        selectedCard.status = 1;
                        selectedCard.spellEffect(this, 0);
                        graveyard[0].push_back(selectedCard); // <--- แก้ไขให้เวทมนตร์ใช้แล้วลงสุสาน
                        hand[0].erase(hand[0].begin() + cardIndex);
                    } 
                    else if (posChoice == 2) {
                        selectedCard.status = 2; 
                        spellTrapZone[0].push_back(selectedCard);
                        hand[0].erase(hand[0].begin() + cardIndex);
                        cout << "\n>> You Set a card in your Spell/Trap Zone.\n";
                    }
                }
            } else {
                cout << "\033[31m[!] Invalid choice. Please try again.\033[0m\n";
            }
        }
    }

    void playerBattlePhase() {
        bool hasAttackableMonster = false;
        for (Card c : monsterZone[0]) {
            if (c.status == 1) hasAttackableMonster = true; 
        }
        
        if (!hasAttackableMonster) {
            cout << "You have no monsters in Attack Position. Skipping Battle Phase...\n";
            return;
        }

        vector<bool> hasAttacked(monsterZone[0].size(), false);
        bool endPhase = false;

        while (!endPhase) {
            
            displayBoard();
            
            cout << "\n[ Phase: BATTLE PHASE ]\n";
            cout << "\n--- BATTLE MENU ---\n";
            int attackableCount = 0;
            
            for (int i = 0; i < monsterZone[0].size(); i++) {
                if (monsterZone[0][i].status == 1 && !hasAttacked[i]) {
                    cout << "[" << i + 1 << "] Attack with \033[38;5;94m" << monsterZone[0][i].name 
                         << "\033[0m (ATK: " << monsterZone[0][i].atk << ")\n";
                    attackableCount++;
                }
            }

            if (attackableCount == 0) {
                cout << "All your monsters have attacked!\n";
                break; 
            }

            cout << "[0] End Battle Phase\n";
            cout << "Choose an attacker: ";
            
            int attackerChoice;
            cin >> attackerChoice;

            if (attackerChoice == 0) {
                endPhase = true;
                cout << "\nEnding Battle Phase...\n";
            } 
            else if (attackerChoice > 0 && attackerChoice <= monsterZone[0].size() && 
                     monsterZone[0][attackerChoice - 1].status == 1 && !hasAttacked[attackerChoice - 1]) {
                
                int aIndex = attackerChoice - 1;
                Card attacker = monsterZone[0][aIndex];
                
                if (monsterZone[1].empty()) {
                    bool trapActivated = false; 
                    for (int t = 0; t < spellTrapZone[1].size(); t++) {
                        if (spellTrapZone[1][t].type == "Trap") {
                            int botTrapDecision = rand() % 100;
                            if (botTrapDecision < 70) {
                                cout << "\n\033[0;31m[!] BOT ACTIVATED A TRAP CARD: " << spellTrapZone[1][t].name << "!\033[0m\n";
                                spellTrapZone[1][t].spellEffect(this, 1); 
                                graveyard[1].push_back(spellTrapZone[1][t]);
                                spellTrapZone[1].erase(spellTrapZone[1].begin() + t);
                                trapActivated = true; 
                                break;
                            }
                        }
                    }
                    if (trapActivated) continue; 

                    cout << "\n>> \033[38;5;94m" << attacker.name << "\033[0m attacks Bot directly!!!\n";
                    LP[1] -= attacker.atk;
                    cout << "\033[31mBot takes " << attacker.atk << " damage!\033[0m (Bot LP: " << LP[1] << ")\n";
                    hasAttacked[aIndex] = true;
                } 
                else {
                    cout << "\nBot's Monsters:\n";
                    for (int j = 0; j < monsterZone[1].size(); j++) {
                        if (monsterZone[1][j].status == 1) {
                            cout << "[" << j + 1 << "] \033[38;5;94m" << monsterZone[1][j].name 
                                 << "\033[0m (ATK: " << monsterZone[1][j].atk << ")\n";
                        } else {
                            cout << "[" << j + 1 << "] Face-down Defense Monster\n";
                        }
                    }
                    cout << "[0] Cancel Attack\n";
                    cout << "Choose target: ";
                    
                    int targetChoice;
                    cin >> targetChoice;

                    if (targetChoice > 0 && targetChoice <= monsterZone[1].size()) {
                        int tIndex = targetChoice - 1;
                        Card target = monsterZone[1][tIndex];
                        hasAttacked[aIndex] = true; 

                        bool trapActivated = false; 
                        for (int t = 0; t < spellTrapZone[1].size(); t++) {
                            if (spellTrapZone[1][t].type == "Trap") {
                                cout << "\n\033[0;31m[!] BOT ACTIVATED A TRAP CARD: " << spellTrapZone[1][t].name << "!\033[0m\n";
                                spellTrapZone[1][t].spellEffect(this, 1); 
                                graveyard[1].push_back(spellTrapZone[1][t]);
                                spellTrapZone[1].erase(spellTrapZone[1].begin() + t);
                                trapActivated = true; break;
                            }
                        }
                        if (trapActivated) continue; 

                        cout << "\n>> \033[38;5;94m" << attacker.name << "\033[0m attacks ";
                        if (target.status == 1) cout << "\033[38;5;94m" << target.name << "\033[0m!\n";
                        else cout << "the face-down monster! (It flips up: \033[38;5;94m" << target.name << "\033[0m - DEF: " << target.def << ")\n";

                        int battleResult = executeBattle(attacker, target, LP[0], LP[1], "You", "Bot");

                        if (battleResult == 1 || battleResult == 3) { 
                            graveyard[1].push_back(target);
                            monsterZone[1].erase(monsterZone[1].begin() + tIndex);
                        }
                        if (battleResult == 2 || battleResult == 3) { 
                            graveyard[0].push_back(attacker);
                            monsterZone[0].erase(monsterZone[0].begin() + aIndex);
                            hasAttacked.erase(hasAttacked.begin() + aIndex); 
                        }
                    } else if (targetChoice != 0) {
                        cout << "\033[31m[!] Invalid target.\033[0m\n";
                    }
                }
            } else {
                cout << "\033[31m[!] Invalid choice or monster has already attacked.\033[0m\n";
            }
            if (LP[0] <= 0 || LP[1] <= 0) break; 
        }
    }

    void botMainPhase() {
        displayBoard();
        cout << "\n[ Phase: BOT MAIN PHASE 1 ]\n";
        cout << "Bot is thinking...\n";
        
        bool hasSummoned = false; 

        // บอทเช็คการ์ดในมือจากหลังมาหน้า
        for (int i = hand[1].size() - 1; i >= 0; i--) {
            Card selectedCard = hand[1][i];
            
            // --- AI บอทลงมอนสเตอร์ ---
            if (selectedCard.type == "Monster" && !hasSummoned && monsterZone[1].size() < 5) {
                int hesitation = rand() % 100;
                if (hesitation < 25) continue; // บอทลังเล แกล้งไม่ลงการ์ดใบนี้

                // ---- ระบบสังเวยมอนสเตอร์ของบอท ----
                int reqTribute = 0;
                if (selectedCard.stars == 5 || selectedCard.stars == 6) reqTribute = 1;
                else if (selectedCard.stars >= 7) reqTribute = 2;

                if (reqTribute > 0) {
                    if (monsterZone[1].size() < reqTribute) {
                        continue; // บอทมีมอนสเตอร์ไม่พอสังเวย ข้ามไพ่ใบนี้ไปเลย
                    }
                    
                    cout << "\n>> \033[33mBot tributes " << reqTribute << " monster(s)!\033[0m\n";
                    for (int t = 0; t < reqTribute; t++) {
                        // บอทสังเวยมอนสเตอร์ตัวแรกสุดบนสนามตัวเองเสมอ
                        graveyard[1].push_back(monsterZone[1][0]);
                        monsterZone[1].erase(monsterZone[1].begin());
                    }
                }
                // ------------------------------------------------

                int playStyle = rand() % 100;
                if (selectedCard.atk >= 1500 && playStyle < 80) {
                    selectedCard.status = 1;
                    monsterZone[1].push_back(selectedCard);
                    cout << ">> \033[38;5;94mBot Summons a Monster in Face-up Attack Position!\033[0m\n";
                } else {
                    selectedCard.status = 2;
                    monsterZone[1].push_back(selectedCard);
                    cout << ">> Bot Sets a Monster in face-down Defense Position.\n";
                }
                
                hand[1].erase(hand[1].begin() + i);
                hasSummoned = true; 
            }
            // --- AI บอทใช้เวท/กับดัก ---
            else if ((selectedCard.type == "Spell" || selectedCard.type == "Trap") && spellTrapZone[1].size() < 5) {
                int r = rand() % 100;
                if (selectedCard.type == "Trap" || r < 40) {
                    selectedCard.status = 2; // หมอบการ์ด
                    spellTrapZone[1].push_back(selectedCard);
                    cout << ">> Bot Sets a card in the Spell/Trap Zone.\n";
                    hand[1].erase(hand[1].begin() + i);
                } 
                else if (selectedCard.type == "Spell") {
                    cout << ">> \033[0;32mBot Activates Spell Card: '" << selectedCard.name << "'!\033[0m\n";
                    selectedCard.status = 1;
                    selectedCard.spellEffect(this, 1);
                    graveyard[1].push_back(selectedCard); // <--- แก้ไขให้เวทมนตร์บอทใช้แล้วลงสุสาน
                    hand[1].erase(hand[1].begin() + i);
                }
            }
        }
        cout << "\nBot ends its Main Phase.\n";
    }

    void botBattlePhase() {
        cout << "\n[ Phase: BOT BATTLE PHASE ]\n";
        displayBoard();
        bool canAttack = false;
        for (Card c : monsterZone[1]) {
            if (c.status == 1) canAttack = true; 
        }
        if (!canAttack) {
            cout << "Bot has no monsters able to attack.\n";
            return;
        }

        int botCourage = 100; 
        int playerSetCards = spellTrapZone[0].size();
        if (playerSetCards > 0) {
            cout << "Bot is looking closely at your face-down cards...\n";
            botCourage -= (playerSetCards * 20); 
        }

        for (int i = monsterZone[1].size() - 1; i >= 0; i--) {
            if (monsterZone[1][i].status == 1) { 
                int roll = rand() % 100;
                if (roll < botCourage) {
                    Card attacker = monsterZone[1][i]; 

                    cout << ">> Bot's \033[38;5;94m" << attacker.name << "\033[0m declares an ATTACK!\n";

                    bool trapActivated = false; 
                    for (int t = 0; t < spellTrapZone[0].size(); t++) {
                        if (spellTrapZone[0][t].type == "Trap") {
                            cout << "\n\033[0;35m[?] Bot is attacking! You have a face-down Trap Card: '" << spellTrapZone[0][t].name << "'\033[0m\n";
                            cout << "Do you want to activate it?\n";
                            cout << "[1] Yes, activate it!\n";
                            cout << "[2] No, keep it face-down.\n";
                            cout << "Choice: ";
                            
                            int trapChoice;
                            cin >> trapChoice;

                            if (trapChoice == 1) {
                                cout << "\n\033[0;35m[!] YOU ACTIVATED A TRAP CARD: " << spellTrapZone[0][t].name << "!\033[0m\n";
                                spellTrapZone[0][t].spellEffect(this, 0); 
                                graveyard[0].push_back(spellTrapZone[0][t]);
                                spellTrapZone[0].erase(spellTrapZone[0].begin() + t);
                                trapActivated = true; 
                                //break; 
                            } else {
                                cout << ">> You chose to keep the Trap Card hidden.\n";
                            }
                        }
                    }

                    if (trapActivated) continue; 
                    
                    if (monsterZone[0].empty()) {
                        cout << ">> Bot attacks directly!\n";
                        LP[0] -= attacker.atk;
                        cout << "\033[31mYou take " << attacker.atk << " damage!\033[0m (Your LP: " << LP[0] << ")\n";
                    } else {
                        Card target = monsterZone[0][0]; 
                        cout << ">> Bot attacks your " << target.name << "!\n";
                        
                        int battleResult = executeBattle(attacker, target, LP[1], LP[0], "Bot", "You");

                        if (battleResult == 1 || battleResult == 3) { 
                            graveyard[0].push_back(target);
                            monsterZone[0].erase(monsterZone[0].begin()); 
                        }
                        if (battleResult == 2 || battleResult == 3) { 
                            graveyard[1].push_back(attacker);
                            monsterZone[1].erase(monsterZone[1].begin() + i); 
                        }
                    }
                } else {
                    cout << ">> Bot hesitates... \033[38;5;94m" << monsterZone[1][i].name << "\033[0m cancels its attack out of fear!\n";
                }
            }
        }
    }

    void endPhase(int playerIndex) {
        cout << "\n[ Phase: END PHASE ]\n";

        while (hand[playerIndex].size() > 6) {
            if (playerIndex == 0) {
                displayBoard();
                cout << "\033[31m[!] Hand size limit exceeded! You have " << hand[0].size() << " cards.\033[0m\n";
                cout << "You must discard down to 6 cards.\n";
                showPlayerHand(); 
                cout << "Choose a card to DISCARD (1-" << hand[0].size() << "): ";
                int choice;
                cin >> choice;

                if (choice > 0 && choice <= hand[0].size()) {
                    int discardIndex = choice - 1;
                    cout << ">> You discarded \033[31m" << hand[0][discardIndex].name << "\033[0m to the Graveyard.\n";
                    graveyard[0].push_back(hand[0][discardIndex]);
                    hand[0].erase(hand[0].begin() + discardIndex);
                } else {
                    cout << "\033[31m[!] Invalid choice. You must pick a card to discard.\033[0m\n";
                }

            } else {
                cout << "Bot's hand exceeds 6 cards. Bot must discard...\n";
                int discardIndex = rand() % hand[1].size();
                cout << ">> Bot discarded a card to the Graveyard.\n";
                graveyard[1].push_back(hand[1][discardIndex]);
                hand[1].erase(hand[1].begin() + discardIndex);
            }
        }

        if (playerIndex == 0) cout << "\n=== END OF PLAYER 1'S TURN ===\n";
        else cout << "\n=== END OF BOT'S TURN ===\n";
        
        if (playerIndex == 1) {
            cout << "\nPress Enter to start your next turn...";
            cin.ignore(); 
            cin.get();    
        }
    }

    void announceWinner() {
        if (LP[0] <= 0) cout << "\nYOU LOSE!!!\n";
        else if (LP[1] <= 0) cout << "\nYOU WIN!!!\n";
    }
    //Muhahaha
};

#endif
