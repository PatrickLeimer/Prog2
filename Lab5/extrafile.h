// Created by Patrick on 10/19/2024.
//
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <climits>
#include <iomanip>
#include <algorithm>
using namespace std;

struct Weapon {
    string name;
    int lengthPlusNullTerm;
    int powerRat;
    float powerConsumption;

    Weapon(string name, int lengthPlusNullTerm, int powerRat, float powerConsumption) {
        this->name = name;
        this->lengthPlusNullTerm = lengthPlusNullTerm;
        this->powerRat = powerRat;
        this->powerConsumption = powerConsumption;
    }

};

struct Starship {
    string name;
    int nameLen;

    string shipClass;
    int shipClassLen;

    short shipLenMetric;
    int shieldCapTJ;
    float warpSpeed;

    int numWeaponsEquipped;
    vector<Weapon> weapons;

    Starship() {
       nameLen = 0;
        shipClassLen = 0;
        shipLenMetric = 0;
        shieldCapTJ = 0;
        warpSpeed = 0.0f;
        numWeaponsEquipped = 0;
        weapons = vector<Weapon>();
    }
    Starship(string name, string shipClass, short shipLenMetric, int shieldCapTJ, float warpSpeed, const vector<Weapon>& weapons)
    {
        this->name = name;
        this->nameLen = name.length() + 1;
        this->shipClass = shipClass;
        this->shipClassLen = shipClass.length() + 1;

        this->shipLenMetric = shipLenMetric;
        this->shieldCapTJ = shieldCapTJ;
        this->warpSpeed = warpSpeed;

        this->numWeaponsEquipped = weapons.size();
        this->weapons = weapons;
    }

};

Starship readStarship(ifstream &file) {
    Starship starship;
    file.read(reinterpret_cast<char*>(&(starship.nameLen)), sizeof(int));  //4 bytes
    string nameBuffer(starship.nameLen, '\0');
    file.read(&nameBuffer[0], starship.nameLen);
    starship.name = nameBuffer;

    file.read(reinterpret_cast<char*>(&(starship.shipClassLen)), sizeof(int));
    string classBuffer(starship.shipClassLen, '\0');
    file.read(&classBuffer[0], starship.shipClassLen);
    starship.shipClass = classBuffer;

    file.read(reinterpret_cast<char*>(&(starship.shipLenMetric)), sizeof(short));

    file.read(reinterpret_cast<char*>(&(starship.shieldCapTJ)), sizeof(int));

    file.read(reinterpret_cast<char*>(&(starship.warpSpeed)), sizeof(float));

    file.read(reinterpret_cast<char*>(&(starship.numWeaponsEquipped)), sizeof(int));

    // Read weapons
    vector<Weapon> weapons;
    for (int i = 0; i < starship.numWeaponsEquipped; ++i) {
        int weaponNameLength;
        file.read(reinterpret_cast<char*>(&weaponNameLength), sizeof(int));

        string weaponName(weaponNameLength, '\0');
        file.read(&weaponName[0], weaponNameLength);

        int powerRating;
        file.read(reinterpret_cast<char*>(&powerRating), sizeof(int));

        float powerConsumption;
        file.read(reinterpret_cast<char*>(&powerConsumption), sizeof(float));

        weapons.emplace_back(weaponName, weaponNameLength, powerRating, powerConsumption);
    }

    starship.weapons = weapons;
    return starship;
}

string trim(string s) {
    string trimmed = s;
    trimmed.erase(remove(trimmed.begin(), trimmed.end(), '\r'), trimmed.end());
    trimmed.erase(remove(trimmed.begin(), trimmed.end(), '\0'), trimmed.end());
    return trimmed;
}


void printAllShips(const vector<Starship>& ships) {
    for (int i = 0; i < ships.size(); ++i) {
        cout << "Name: " << trim(ships[i].name) << endl;
        cout << "Class: " << trim(ships[i].shipClass) << endl;
        cout << "Length: " << ships[i].shipLenMetric << endl;
        cout << "Shield capacity: " << ships[i].shieldCapTJ << endl;
        cout << "Maximum Warp: " << ships[i].warpSpeed << endl;
        cout << "Armaments: " << endl;

        if (ships[i].numWeaponsEquipped > 0) {
            for (const auto& weapon : ships[i].weapons) {
                cout << trim(weapon.name) << ", " << weapon.powerRat << ", " <<  weapon.powerConsumption << endl;
            }
            int totalFirepower = 0;
            for (const auto& weapon : ships[i].weapons) {
                totalFirepower += weapon.powerRat;
            }
            cout << "Total firepower: " << totalFirepower;
            if (i + 1 != ships.size()) {
              cout << "\n\n";
            }
        } else {
          cout << "Unarmed\n" << endl;
        }
    }
}


void printStrongestWeaponShip(const vector<Starship>& ships) {
    const Starship* strongestShip = nullptr;
    int maxPowerRating = -1;

    for (const auto& ship : ships) {
        for (const auto& weapon : ship.weapons) {
            if (weapon.powerRat > maxPowerRating) {
                maxPowerRating = weapon.powerRat;
                strongestShip = &ship;
            }
        }
    }

    if (strongestShip) {
        cout << "Name: " << trim(strongestShip->name) << endl;
        cout << "Class: " << trim(strongestShip->shipClass) << endl;
        cout << "Length: " << strongestShip->shipLenMetric << endl;
        cout << "Shield capacity: " << strongestShip->shieldCapTJ << endl;
        cout << "Maximum Warp: " << strongestShip->warpSpeed << endl;
        cout << "Armaments: " << endl;

        if (strongestShip->numWeaponsEquipped > 0) {
            int totalFirepower = 0;

            for (const auto& weapon : strongestShip->weapons) {
                cout << trim(weapon.name) << ", " << weapon.powerRat << ", " << weapon.powerConsumption << endl;
                totalFirepower += weapon.powerRat;
            }

            cout << "Total firepower: " << totalFirepower << endl;
        } else {
            cout << "Unarmed\n" << endl;
        }

    } else {
        cout << "No ships found with weapons." << endl;
    }
}

void printStrongestShipOverall(const vector<Starship>& ships) {
    const Starship* strongestShip = nullptr;
    int maxCombinedPower = -1;

    for (const auto& ship : ships) {
        int totalPower = 0;
        for (const auto& weapon : ship.weapons) {
            totalPower += weapon.powerRat;
        }

        if (totalPower > maxCombinedPower) {
            maxCombinedPower = totalPower;
            strongestShip = &ship;
        }
    }

    if (strongestShip) {
        cout << "Name: " << trim(strongestShip->name) << endl;
        cout << "Class: " << trim(strongestShip->shipClass) << endl;
        cout << "Length: " << strongestShip->shipLenMetric << endl;
        cout << "Shield capacity: " << strongestShip->shieldCapTJ << endl;
        cout << "Maximum Warp: " << strongestShip->warpSpeed << endl;
        cout << "Armaments: " << endl;

        if (strongestShip->numWeaponsEquipped > 0) {
            int totalFirepower = 0;

            for (const auto& weapon : strongestShip->weapons) {
                cout << trim(weapon.name) << ", " << weapon.powerRat << ", " << weapon.powerConsumption << endl;
                totalFirepower += weapon.powerRat;
            }

            cout << "Total firepower: " << totalFirepower << endl;
        } else {
            cout << "Unarmed\n" << endl;
        }

    } else {
        cout << "No ships found with weapons." << endl;
    }
}


void printWeakestShip(const vector<Starship>& ships) {
    const Starship* weakestShip = nullptr;
    int minPowerRating = INT_MAX;

    for (const auto& ship : ships) {
        if (!ship.weapons.empty()) {
            int minShipPower = INT_MAX;
            for (const auto& weapon : ship.weapons) {
                if (weapon.powerRat < minShipPower) {
                    minShipPower = weapon.powerRat;
                }
            }

            if (minShipPower < minPowerRating) {
                minPowerRating = minShipPower;
                weakestShip = &ship;
            }
        }
    }

    if (weakestShip) {
        cout << "Name: " << trim(weakestShip->name) << endl;
        cout << "Class: " << trim(weakestShip->shipClass) << endl;
        cout << "Length: " << weakestShip->shipLenMetric << endl;
        cout << "Shield capacity: " << weakestShip->shieldCapTJ << endl;
        cout << "Maximum Warp: " << weakestShip->warpSpeed << endl;
        cout << "Armaments: " << endl;

        if (weakestShip->numWeaponsEquipped > 0) {
            int totalFirepower = 0;

            for (const auto& weapon : weakestShip->weapons) {
                cout << trim(weapon.name) << ", " << weapon.powerRat << ", " << weapon.powerConsumption << endl;
                totalFirepower += weapon.powerRat;
            }

            cout << "Total firepower: " << totalFirepower << endl;
        } else {
            cout << "Unarmed\n" << endl;
        }

    } else {
        cout << "No ships found with weapons." << endl;
    }
}

void printUnarmedShips(const vector<Starship>& ships) {
    bool foundUnarmedShip = false;

    for (int i = 0; i < ships.size(); ++i) {
        if (ships[i].weapons.empty()) {
            if (!(i+1 == ships.size())) {
              cout << "\n" << endl;
            }

            cout << "Name: " << trim(ships[i].name) << endl;
            cout << "Class: " << trim(ships[i].shipClass) << endl;
            cout << "Length: " << ships[i].shipLenMetric << endl;
            cout << "Shield capacity: " << ships[i].shieldCapTJ << endl;
            cout << "Maximum Warp: " << ships[i].warpSpeed << endl;
            cout << "Armaments: \n" << "Unarmed";
            foundUnarmedShip = true;
        }
    }

    if (!foundUnarmedShip) {
        cout << "No unarmed ships found." << endl;
    }
}