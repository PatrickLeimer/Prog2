//
// Created by patri on 11/21/2024.
//

#include <iostream>
#include <vector>
#include <sstream>
#include <string>
#include <fstream>
#include <map>
#include <SFML/Graphics.hpp>
#include <unordered_map>
#include <chrono>

using namespace std;
using namespace sf;

bool welcomeScreen(string& userName);
bool gameWindow(string& userName);
bool leaderBoard(int w, int h, int mins, int secs, string& userName, bool won);

// Classes ---------------------------------------------------------------------------------------------------
struct Tile {
    int val, row, col;
        //Different states
    bool mine = false, revealed = false, flagged = false;
        //Recursion checker
    bool recCheck = false;

    vector<Tile*> adjacent; // Neighbour pointers

    map<string, Sprite> mapSprite;
    Sprite hiddenTile, revealedTile, valueTile, flagTile, mineTile;

        //Constructor
    Tile(map<string, Sprite>& mapS) {
        row = 0;
        col = 0;
        val = 0;
        mapSprite = mapS;
    }

        // Using recursion to reveal clicked tile and neighbors that aren't mines
    void tileReveal(int& num) {

        if(mine || revealed || flagged || recCheck) {
            return;
        } // base case

        if(val != 0 && !mine && !revealed) {
            revealed = true;
            recCheck = true;
            num++;
            return;
        } else if(val == 0) { //recursion continuation
            revealed = true;
            recCheck = false;
            num++;

            for(auto & i : adjacent) {
                i->tileReveal(num);
            }
            return;
        }

    }

        // Checks if the adjacent tiles to the mine are mines, if not, increase the value of the tile
    void neighborIncrease() {
        for(const auto & i : adjacent)
        {
            if(!i->mine)
                i->valIncrease();
        }
    }

        // Increment in the value
    void valIncrease() {
        val++;
        switch(val) {
            case 0:
                valueTile = mapSprite["tile_revealed"];
                break;
            case 1:
                valueTile = mapSprite["number_1"];
                break;
            case 2:
                valueTile = mapSprite["number_2"];
                break;
            case 3:
                valueTile = mapSprite["number_3"];
                break;
            case 4:
                valueTile = mapSprite["number_4"];
                break;
            case 5:
                valueTile = mapSprite["number_5"];
                break;
            case 6:
                valueTile = mapSprite["number_6"];
                break;
            case 7:
                valueTile = mapSprite["number_7"];
                break;
            case 8:
                valueTile = mapSprite["number_8"];
                break;
            default:
                break;
        }
        valueTile.setPosition(revealedTile.getPosition());
    }

};


struct GameBoard : public Drawable {
    int w, h, row, col, numMines, tiFlag, tiRevealed, tiles;

    vector<vector<Tile*>> gameboard;
    map<string, Sprite> mapSprite;
    map<int, Sprite> digitMap;

    Sprite happyFace, winFace, loseFace, debug, pause, play, leaderboard;

    bool gLost = false, gWin = false;

    bool debugClick = false, leaderBClick = false, faceClick = false;
    bool displayHface = true, displayLFace = false, inPause = false, playDisplay = false;

        //Constructor and gameboard starter
    GameBoard(int width, int height, int rows, int columns, int mines, int tileNum, map<string, Sprite>& sprites, map<int, Sprite>& digiMap) {
        w = width;
        h = height;
        row = rows;
        col = columns;
        numMines = mines;
        tiFlag = numMines;
        mapSprite = sprites;
        digitMap = digiMap;
        tiles = tileNum;
        tiRevealed = 0;

        happyFace = mapSprite["face_happy"];
        winFace = mapSprite["face_win"];
        loseFace = mapSprite["face_lose"];
        debug = mapSprite["debug"];
        pause = mapSprite["pause"];
        play = mapSprite["play"];
        leaderboard = mapSprite["leaderboard"];
            //Resize board
        gameboard.resize(row*col);

            //start the board
        startGBoard();
        startTNeighbors();
        startMines();

    }

        //
    void startGBoard() {
        float posX = 0, posY = 0;
        for(int i = 0; i < row; i++) {
            for(int j = 0; j < col; j++) {
                Tile* newTile = new Tile(mapSprite);
                newTile->row = i;
                newTile->col = j;

                newTile->hiddenTile = mapSprite["tile_hidden"];
                newTile->hiddenTile.setPosition(posX, posY);

                newTile->revealedTile = mapSprite["tile_revealed"];
                newTile->revealedTile.setPosition(posX, posY);

                newTile->flagTile = mapSprite["flag"];
                newTile->flagTile.setPosition(posX, posY);

                newTile->mineTile = mapSprite["mine"];
                newTile->mineTile.setPosition(posX, posY);

                gameboard[i].push_back(newTile);

                posX += 32;
            } //End of 2nd for
            posX = 0;
            posY += 32;
        } // End of 1st for

    }

        //Gathers the pointers to the neighbouring tiles
    void startTNeighbors() {
        for(int i = 0; i < row; i++) {
            for(int j = 0; j < col; j++) {
                cout << "value of current: " << gameboard[i][j]->val << " | ";

                if(i == 0 && j == 0) { // top left corner
                    gameboard[i][j]->adjacent.push_back(gameboard[i + 1][j]); // Right
                    gameboard[i][j]->adjacent.push_back(gameboard[i][j + 1]); // Below
                    gameboard[i][j]->adjacent.push_back(gameboard[i + 1][j + 1]); // Below right
                }
                else if(i == row - 1 && j == 0) { // bottom left corner
                    gameboard[i][j]->adjacent.push_back(gameboard[i - 1][j]); // Up
                    gameboard[i][j]->adjacent.push_back(gameboard[i][j + 1]); // Right
                    gameboard[i][j]->adjacent.push_back(gameboard[i - 1][j + 1]); // Up right
                }
                else if(i == 0 && j == col - 1) { // top right corner
                    gameboard[i][j]->adjacent.push_back(gameboard[i + 1][j]); // Below
                    gameboard[i][j]->adjacent.push_back(gameboard[i][j - 1]); // Left
                    gameboard[i][j]->adjacent.push_back(gameboard[i + 1][j - 1]); // Below left
                } else if (i == row - 1 && j == col - 1) { // bottom right corner
                    gameboard[i][j]->adjacent.push_back(gameboard[i - 1][j]); // Up
                    gameboard[i][j]->adjacent.push_back(gameboard[i][j - 1]); // Left
                    gameboard[i][j]->adjacent.push_back(gameboard[i - 1][j - 1]); // Up left
                } //End of 4 corners, into the middle parts

                else if (i == 0 && j > 0) { // top row
                    gameboard[i][j]->adjacent.push_back(gameboard[i][j - 1]); // Left
                    gameboard[i][j]->adjacent.push_back(gameboard[i][j + 1]); // Right
                    gameboard[i][j]->adjacent.push_back(gameboard[i + 1][j]); // Below
                    gameboard[i][j]->adjacent.push_back(gameboard[i + 1][j - 1]); // Below left
                    gameboard[i][j]->adjacent.push_back(gameboard[i + 1][j + 1]); // Below right
                } else if(i == row - 1 && j > 0){  // bottom row
                    gameboard[i][j]->adjacent.push_back(gameboard[i][j - 1]); // Left
                    gameboard[i][j]->adjacent.push_back(gameboard[i][j + 1]); // Right
                    gameboard[i][j]->adjacent.push_back(gameboard[i - 1][j]); // Up
                    gameboard[i][j]->adjacent.push_back(gameboard[i - 1][j - 1]); // Up left
                    gameboard[i][j]->adjacent.push_back(gameboard[i - 1][j + 1]); // Up right
                } else if (i > 0 && j == 0) { // left side
                    gameboard[i][j]->adjacent.push_back(gameboard[i][j + 1]); // Right
                    gameboard[i][j]->adjacent.push_back(gameboard[i - 1][j]); // Up
                    gameboard[i][j]->adjacent.push_back(gameboard[i + 1][j]); // Below
                    gameboard[i][j]->adjacent.push_back(gameboard[i - 1][j + 1]); // Up right
                    gameboard[i][j]->adjacent.push_back(gameboard[i + 1][j + 1]); // Below right
                } else if (i > 0 && j == col - 1) { // right side
                    gameboard[i][j]->adjacent.push_back(gameboard[i][j - 1]); // Left
                    gameboard[i][j]->adjacent.push_back(gameboard[i - 1][j]); // Up
                    gameboard[i][j]->adjacent.push_back(gameboard[i + 1][j]); // Below
                    gameboard[i][j]->adjacent.push_back(gameboard[i + 1][j - 1]); // Below left
                    gameboard[i][j]->adjacent.push_back(gameboard[i - 1][j - 1]); // Up left
                }

                else { // middle, 8 neighbours
                    gameboard[i][j]->adjacent.push_back(gameboard[i][j + 1]); // Right
                    gameboard[i][j]->adjacent.push_back(gameboard[i][j - 1]); // Left
                    gameboard[i][j]->adjacent.push_back(gameboard[i - 1][j]); // Up
                    gameboard[i][j]->adjacent.push_back(gameboard[i + 1][j]); // Below

                    gameboard[i][j]->adjacent.push_back(gameboard[i - 1][j - 1]); // Up left
                    gameboard[i][j]->adjacent.push_back(gameboard[i - 1][j + 1]); // Up right
                    gameboard[i][j]->adjacent.push_back(gameboard[i + 1][j - 1]); // Below left
                    gameboard[i][j]->adjacent.push_back(gameboard[i + 1][j + 1]); // Below right
                }

            } //End of 2nd for
        } //End of 1st for
    }

        // Placing the mines randomly
    void startMines() {
        srand(static_cast<unsigned>(time(nullptr))); // c++ version
        int mineRow = rand() % row;
        int mineCol = rand() % col;

        for (int i = numMines; i > 0; i--) {
            while (gameboard[mineRow][mineCol]->mine) {
                mineRow = rand() % row;
                mineCol = rand() % col;
            }
            cout << "Mine row and col " << mineRow << " " << mineCol << endl;
            gameboard[mineRow][mineCol]->mine = true;
            gameboard[mineRow][mineCol]->val = 10;
            gameboard[mineRow][mineCol]->neighborIncrease();
        }

    }


    void getPrint() {
        for (int i = 0; i < row; i++) {
            for (int j = 0; j < col; j++) {
                cout << "RCV " << i << " " << j << " " << gameboard[i][j]->val << " | ";
            }
        }
    }

        //Click functionalities
    bool click(int x, int y, bool tempCheck = false) {
    int rowI = 0, colI = 0;

    // Click within board and bounds?
    if (x >= 0 && x < w && y >= 0 && y < (row * 32)) {
        rowI = y / 32;
        colI = x / 32;
        cout << "RC - I" << rowI << colI << endl;
        // Ensure indices are within bounds
        if (rowI >= 0 && rowI < row && colI >= 0 && colI < col) {
            // Mine counter
            if(tempCheck && !gWin && !gLost && !leaderBClick && !inPause) {
                if(!gameboard[rowI][colI]->flagged) {
                    gameboard[rowI][colI]->flagged = true;
                    tiFlag--;
                } else {
                    gameboard[rowI][colI]->flagged = false;
                    tiFlag++;
                }
                return false;
            }
            // Game lost
            else if (gameboard[rowI][colI]->mine && !gameboard[rowI][colI]->flagged && !gWin && !gLost && !leaderBClick && !inPause) {
                displayLFace = true;
                displayHface = false;
                gLost = true;
                return false;
            }
            // Reveal tile
            else if (!gameboard[rowI][colI]->flagged && !gWin && !gLost && !leaderBClick && !inPause) {
                gameboard[rowI][colI]->tileReveal(tiRevealed);
                if (won()) {
                    return true;
                }
            }
        } else {
            // Indices out of bounds
            return false;
        }

    }
    // Button functionality
    else if (!tempCheck) {
        if (debug.getGlobalBounds().contains(x, y) && !leaderBClick && !inPause) {
            debugClick = !debugClick;
            cout << "Debug button clicked." << endl;
        } else if (leaderboard.getGlobalBounds().contains(x, y) && !gWin && !gLost) {
            leaderBClick = !leaderBClick;
            cout << "Leaderboard button clicked." << endl;
        } else if (pause.getGlobalBounds().contains(x, y) && !leaderBClick && !gWin && !gLost) {
            inPause = !inPause;
            playDisplay = !playDisplay;
            cout << "Pause button clicked." << endl;
        } else if (happyFace.getGlobalBounds().contains(x, y)) {
            // Reset the game
            resetGame();
            cout << "Happy face clicked. Game reset." << endl;
        } else {
            cout << "No button clicked or conditions not met." << endl;
        }
    }
    return false;
}

    void resetGame() {
        // Reset game state variables
        gLost = false;
        gWin = false;
        debugClick = false;
        leaderBClick = false;
        inPause = false;
        displayHface = true;
        displayLFace = false;
        tiFlag = numMines;
        tiRevealed = 0;

        // Clear the gameboard and restart it
        for (int i = 0; i < row; i++) {
            for (int j = 0; j < col; j++) {
                delete gameboard[i][j]; // Clean up the old tiles
            }
        }

        gameboard.clear();
        gameboard.resize(row);
        startGBoard();
        startTNeighbors();
        startMines();
    }

        //Helper method to check if game won
    bool won() {
        if (tiRevealed == tiles - numMines) {
            // Flag all remaining mines
            for(int i = 0; i < row; i++) {
                for(int j = 0; j < col; j++) {
                    if(gameboard[i][j]->mine && !gameboard[i][j]->flagged) {
                        gameboard[i][j]->flagged = true;
                        tiFlag--;
                    }
                }
            }
            gWin = true;
            return true;
        }
        return false;

    }

        //Helper method to change the state of leaderboard display
    void leaderboardState() {
        leaderBClick = not leaderBClick;
    }

        //Rendering the board, overriding from Drawable class for easy drawing
    virtual void draw(RenderTarget& target, RenderStates states) const {
        for (int i = 0; i < row; i++) {
            for (int j = 0; j < col; j++) {

                    // Button functions drawings

                if(displayHface) {
                    target.draw(happyFace);
                } //End of drawing happy face
                if(displayLFace) {
                    target.draw(loseFace);
                } //End of drawing losing face
                if(gWin) {
                    target.draw(winFace);
                }
                if(not inPause) {
                    target.draw(pause);
                } else {
                    target.draw(play);
                    target.draw(gameboard[i][j]->revealedTile);
                } //Drawing the game if not paused
                if (leaderBClick) {
                    target.draw(gameboard[i][j]->revealedTile);
                }

                    //Debug
                if(gameboard[i][j]->mine && not inPause && not gLost && not leaderBClick && not gWin && debugClick) {
                    target.draw(gameboard[i][j]->hiddenTile);
                    target.draw(gameboard[i][j]->mineTile); // Draws the mines for the debug feature
                }
                else if (gameboard[i][j]->revealed && not inPause && not gLost && not leaderBClick && not gWin) {
                    target.draw(gameboard[i][j]->revealedTile);
                    target.draw(gameboard[i][j]->valueTile); // Draws the tiles revealed
                }
                else if (gameboard[i][j]->flagged && not inPause && not gLost && not leaderBClick) {
                    target.draw(gameboard[i][j]->hiddenTile);
                    target.draw(gameboard[i][j]->flagTile); // Draws the flagged tiles
                }
                else if (not inPause && not gLost && not leaderBClick) {
                    target.draw(gameboard[i][j]->hiddenTile); //Draws the pause button
                }
                else if (gLost && not inPause && not leaderBClick) { //Draws everything required once lost
                    target.draw(gameboard[i][j]->hiddenTile);
                    if(gameboard[i][j]->revealed) {
                        target.draw(gameboard[i][j]->revealedTile);
                        target.draw(gameboard[i][j]->valueTile); //Draws ?
                    } else if (gameboard[i][j]->mine) {
                        target.draw(gameboard[i][j]->revealedTile);
                        target.draw(gameboard[i][j]->mineTile); //Draws mine after the revealed tile
                    }
                } // End of lost
                else if (gWin) {
                    target.draw(winFace);
                    target.draw(gameboard[i][j]->hiddenTile);
                    if(gameboard[i][j]->revealed) {
                        target.draw(gameboard[i][j]->revealedTile);
                        target.draw(gameboard[i][j]->valueTile);
                    } else if (gameboard[i][j]->flagged) {
                        target.draw(gameboard[i][j]->flagTile);
                    }
                } //End of in case game is won

                target.draw(debug);
                target.draw(leaderboard);

                //Count mines
                int zeroI = tiFlag % 10; // Least significant digit
                int oneI = tiFlag / 10; //Tens place digit
                int twoI = tiFlag / 100; //Hundreds place digit

                if (tiFlag < 0) {
                    twoI = 10;
                    zeroI *= -1;
                    oneI *= -1;

                    Sprite temp = digitMap.at(twoI);
                    temp.setPosition(12, 32 * (row + 0.5f) + 16); //Formula given to use for the counter
                    target.draw(temp);

                    temp = digitMap.at(0);
                    temp.setPosition(33, 32 * (row + 0.5f) + 16);
                    target.draw(temp);

                    temp = digitMap.at(oneI);
                    temp.setPosition(33 + 21, 32 * (row + 0.5f) + 16);
                    target.draw(temp);

                    temp = digitMap.at(zeroI);
                    temp.setPosition(33 + 21 * 2, 32 * (row + 0.5f) + 16);
                    target.draw(temp);
                } else {
                    Sprite temp = digitMap.at(twoI);
                    temp.setPosition(33, 32 * (row + 0.5f) + 16);
                    target.draw(temp);

                    temp = digitMap.at(oneI);
                    temp.setPosition(33 + 21, 32 * (row + 0.5f) + 16);
                    target.draw(temp);

                    temp = digitMap.at(zeroI);
                    temp.setPosition(33 + 21 * 2, 32 * (row + 0.5f) + 16);
                    target.draw(temp);
                }

            } //End of 2nd for
        } //End of 1st for
    }

};
//End of classes ---------------------------------------------------------------------
