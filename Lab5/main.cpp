#include <iostream>
#include <fstream>
#include "extrafile.h"
using namespace std;

int main()
{
    cout << "Which file(s) to open?\n";
    cout << "1. friendlyships.shp" << endl;
    cout << "2. enemyships.shp" << endl;
    cout << "3. Both files" << endl;
    int option;
    cin >> option;

    vector<Starship> ships;
    // Load files here
    if (option == 1 || option == 3) {
        ifstream friendFile("friendlyships.shp", ios::binary);
        if (friendFile.is_open()) {
            int count;
            friendFile.read(reinterpret_cast<char*>(&count), sizeof(int));
            for (int i = 0; i < count; ++i) {
                ships.push_back(readStarship(friendFile));
            }
            friendFile.close();
        } else {
            cout << "Error opening friendlyships.shp" << endl;
        }
    }

    if (option == 2 || option == 3) {
        ifstream enemyFile("enemyships.shp", ios::binary);
        if (enemyFile.is_open()) {
            int count;
            enemyFile.read(reinterpret_cast<char*>(&count), sizeof(int));
            for (int i = 0; i < count; ++i) {
                ships.push_back(readStarship(enemyFile));
            }
            enemyFile.close();
        } else {
            cout << "Error opening enemyships.shp" << endl;
        }
    }


    cout << "1. Print all ships" << endl;
    cout << "2. Starship with the strongest weapon" << endl;
    cout << "3. Strongest starship overall" << endl;
    cout << "4. Weakest ship (ignoring unarmed)" << endl;
    cout << "5. Unarmed ships" << endl;

    cin >> option;

    // Work your magic here
    switch(option) {
        case 1:
            printAllShips(ships);
            break;
        case 2:
            printStrongestWeaponShip(ships);
            break;
        case 3:
            printStrongestShipOverall(ships);
            break;
        case 4:
            printWeakestShip(ships);
            break;
        case 5:
            printUnarmedShips(ships);
            break;
        default:
            cout << "Invalid option!" << endl;
            break;
    }


    return 0;
}