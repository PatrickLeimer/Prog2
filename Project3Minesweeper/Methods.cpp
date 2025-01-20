//
// Created by patri on 11/21/2024.
//

#include "Methods.h"
using namespace sf;


//Helper methods
Sprite loadSprite(const string& textureName) {
    string path = "files/images/" + textureName + ".png";
    auto *texture = new Texture();
    if( not texture->loadFromFile(path) ) {
        cout << "Error loading texture" << endl;
    }
    Sprite sprite;
    sprite.setTexture(*texture);
    return sprite;
}

map<string, Sprite> getSprite(int& row, int& col) {
    map<string, Sprite> sprites;
    vector<string> spriteNames = {"mine", "tile_hidden", "tile_revealed",
        "number_1", "number_2", "number_3", "number_4", "number_5", "number_6", "number_7", "number_8",
        "flag", "face_happy", "face_win", "face_lose", "digits", "debug", "pause", "play", "leaderboard" };

    for(auto & spriteName : spriteNames) {
        sprites.emplace(spriteName, loadSprite(spriteName));
    }

    float formulaX = ((col / 2.0f) * 32 ) - 32;
    float formulaY = (row + 0.5f) * 32;
    //Faces positions
    sprites["face_happy"].setPosition(formulaX, formulaY);
    sprites["face_win"].setPosition(formulaX, formulaY);
    sprites["face_lose"].setPosition(formulaX, formulaY);
    //Button positions
    sprites["debug"].setPosition((col * 32) - 304, formulaY);
    sprites["pause"].setPosition((col * 32) - 240, formulaY);
    sprites["play"].setPosition((col * 32) - 240, formulaY);
    sprites["leaderboard"].setPosition((col * 32) - 176, formulaY);

    return sprites;
}




bool welcomeScreen(string& userName) {

        //Access values from config file
    int cols, rows;
    ifstream config("files/config.cfg");
    if (config.is_open()) {
        config >> cols;
        config >> rows;
    }
    config.close();

        // ADD FROM CONFIG FILE //
    const int sWidth = cols * 32;
    const int sHeight = (rows * 32) + 100;
    cout << "cols" << sWidth << "rows" << sHeight << endl;

        //Render welcome window
    RenderWindow welcome_window(VideoMode(sWidth, sHeight), "MineSweeper", Style::Close); //Windows

        //Declare variables for welcome text --------------------------------------------
    Text welcomeText;
    Font welcomeFont;

        //Load font
    welcomeFont.loadFromFile("files/font.ttf");
    // Welcome message
    welcomeText.setFont(welcomeFont);
    welcomeText.setCharacterSize(24);
    welcomeText.setStyle(Text::Bold | Text::Underlined);
    welcomeText.setString("WELCOME TO MINESWEEPER");

        // Text placement: Center
    FloatRect welcomeRect = welcomeText.getLocalBounds();
    //FloatRect is just a typedef for a common type which is using float data type
    welcomeText.setOrigin(welcomeRect.left + welcomeRect.width / 2.0f, welcomeRect.top + welcomeRect.height / 2.0f );
    welcomeText.setPosition( Vector2f( sWidth/2.0f, sHeight/2.0f -150) );

        //Prompt message --------------------------------------------
    Text promptText;
    promptText.setFont(welcomeFont);
    promptText.setCharacterSize(20);
    promptText.setStyle(Text::Bold);
    promptText.setString("Enter your name: ");

        //Placement
    FloatRect promptRect = promptText.getLocalBounds();
    promptText.setOrigin(promptRect.left + promptRect.width / 2.0f, promptRect.top + promptRect.height / 2.0f);
    promptText.setPosition( Vector2f( sWidth/2.0f, sHeight/2.0f -75) );

        //Username input --------------------------------------------
    Text userNameText;
    string cursor = "|";
        //Style and starting value
    userNameText.setFont(welcomeFont);
    userNameText.setCharacterSize(18);
    userNameText.setFillColor(Color::Yellow);
    userNameText.setStyle(Text::Bold);
    userNameText.setString(cursor);

        //Placement
    FloatRect userNameRect = userNameText.getLocalBounds();
    userNameText.setOrigin(userNameRect.left + userNameRect.width / 2.0f, userNameRect.top + userNameRect.height / 2.0f);
    userNameText.setPosition( Vector2f( sWidth/2.0f, sHeight/2.0f -45) );


    while(welcome_window.isOpen()) {
        Event event;
        while(welcome_window.pollEvent(event)) {
            switch(event.type) {
                    //Event that you close
                case Event::Closed:
                welcome_window.close();
                break;
                    //Event that you enter text
                case Event::TextEntered:
                    cout << static_cast<char>(event.text.unicode) << endl;
                    if(isalpha(static_cast<char>(event.text.unicode))) {
                        if(cursor.length() > 0 && cursor.length() <= 10) {
                            cursor.pop_back();
                            cursor += static_cast<char>(event.text.unicode);
                            cursor += "|";

                                // Placement
                            FloatRect userNameRect = userNameText.getLocalBounds();
                            userNameText.setString(cursor);
                            userNameText.setOrigin(userNameRect.left + userNameRect.width / 2.0f, userNameRect.top + userNameRect.height / 2.0f);
                            userNameText.setPosition( Vector2f( sWidth/2.0f, sHeight/2.0f -45) );

                        } //End of 2nd if
                    } //End of 1st if
                break; //End of Text entered

                    //Event where key is pressed, either backspace or return
                case Event::KeyPressed:
                        //Backspace delete from string
                    if(event.key.code == Keyboard::BackSpace && cursor.length() > 1) {
                            //Delete from string and update cursor
                        cursor.pop_back();
                        cursor.pop_back();
                        cursor += "|";
                            // Placement
                        FloatRect userNameRect = userNameText.getLocalBounds();
                        userNameText.setString(cursor);
                        userNameText.setOrigin(userNameRect.left + userNameRect.width / 2.0f, userNameRect.top + userNameRect.height / 2.0f);
                        userNameText.setPosition( Vector2f( sWidth/2.0f, sHeight/2.0f -45) );

                    }

                        // Return key pressed
                    if(event.key.code == Keyboard::Return) {
                        if(cursor.length() > 1) {
                            cursor.pop_back();
                            string finalUserName;
                            finalUserName = toupper(cursor[0]);
                            for (int i = 1; i < cursor.length(); i++) {
                                finalUserName += tolower(cursor[i]);
                            }
                            cout << finalUserName << endl;
                            userName = finalUserName;
                            return true;
                        }
                    } //End of Return pressed key

                break;//End of KeyPressed event

            } //End of switch


        } //End of event while loop

            //Rendering Display constantly when open
        welcome_window.clear(Color::Blue);
        welcome_window.draw(welcomeText);
        welcome_window.draw(promptText);
        welcome_window.draw(userNameText);
        welcome_window.display();
    } // End of while loop for open window
    return false;
} //End of welcome screen method


bool gameWindow(string& userName) {

    while (true) {
        bool cont = true;
        while (cont) {
            //Access values from config file
            int cols(0), rows(0), mines(0);
            ifstream config("files/config.cfg");
            if (config.is_open()) {
                config >> cols;
                config >> rows;
                config >> mines;
            }
            config.close();
                // ADD FROM CONFIG FILE AND DECLARE //
            const int sWidth = cols * 32;
            const int sHeight = (rows * 32) + 100;
            int tiles = cols * rows;
            int mins = 0, secs = 0;
            bool gPaused = false, gWon = false, gLost = false, gLeaderboard = false;

                //Get Sprites + positions
            map<string, Sprite> sprites = getSprite(rows, cols);
            map<int, Sprite> digitMap;
            cout << "cols: " << cols << "width" << sWidth << " rows: " << sHeight << "mine count" << mines << endl; //Get size

                // Digit Mapping
            for (int i = 0; i < 11; i++) {
                IntRect rectangle(i * 21, 0, 21, 32);
                sprites["digits"].setTextureRect(rectangle);
                Sprite sprite = sprites["digits"];
                digitMap.emplace(i, sprite);
            }

                // Clock
            auto start = chrono::high_resolution_clock::now();
            auto paused = chrono::high_resolution_clock::now();
            auto elapsed = chrono::duration_cast<chrono::seconds>(chrono::high_resolution_clock::now() - paused).count();

                // Start gameboard and screen
            GameBoard game_board(sWidth, sHeight, rows, cols, mines, tiles, sprites, digitMap);
            RenderWindow gameWindow(VideoMode(sWidth, sHeight), "Minesweeper"); //Windows
            game_board.getPrint();

            while (gameWindow.isOpen()) {
                Event event;
                while (gameWindow.pollEvent(event)) {
                    switch(event.type) {
                        case Event::Closed:
                            gameWindow.close();
                            return false;

                        case Event::MouseButtonPressed:

                            Mouse mouse;
                            Vector2i coordinates = Mouse::getPosition(gameWindow);

                            // Map the pixel position to world coordinates
                            Vector2f actualCoords = gameWindow.mapPixelToCoords(coordinates);

                            cout << "Mouse click at world coordinates: " << actualCoords.x << " and " << actualCoords.y << endl;

                            if (event.mouseButton.button == Mouse::Right) { //Right click
                                game_board.click(coordinates.x, coordinates.y, true);
                                cout << "passed" << endl;
                            }
                            else if(event.mouseButton.button == Mouse::Left) {
                                cout << game_board.tiles - game_board.numMines - 1 << " Debug " << game_board.tiRevealed << endl;
                                if(game_board.click(coordinates.x, coordinates.y)) {
                                    gWon = true;
                                    gLeaderboard = true;
                                }
                                gLost = game_board.gLost;

                                if(sprites["leaderboard"].getGlobalBounds().contains(coordinates.x, coordinates.y)) {
                                    gLeaderboard = not gLeaderboard;
                                    if(gLeaderboard) {
                                        paused = chrono::high_resolution_clock::now();
                                    }
                                } else if (sprites["face_happy"].getGlobalBounds().contains(coordinates.x, coordinates.y)) {
                                    gameWindow.close();
                                    continue;
                                } else if (sprites["pause"].getGlobalBounds().contains(coordinates.x, coordinates.y)) {
                                    gPaused = not gPaused;
                                    if (gPaused) {
                                        paused = chrono::high_resolution_clock::now();
                                    } else {
                                        auto unstoppedTime = chrono::high_resolution_clock::now();
                                        elapsed += chrono::duration_cast<chrono::seconds>(unstoppedTime - paused).count();
                                    }
                                } // End else if pause
                            } //End of if left click
                    } //End of switch

                    gameWindow.clear(Color::White);
                    gameWindow.draw(game_board);

                    auto clock = chrono::duration_cast<chrono::seconds>(chrono::high_resolution_clock::now() - start);
                    int totalClock = clock.count();
                    //
                    if(not gPaused && not gLeaderboard && not gLost && not gWon) {
                        totalClock -= elapsed;
                        mins = totalClock / 60;
                        secs = totalClock % 60;

                    }
                        // Time display
                    int zeroMin = mins / 10 % 10;
                    int firstMin = mins % 10;
                    int zeroSec = secs / 10 % 10;
                    int firstSec = secs % 10;

                    digitMap[zeroMin].setPosition((cols * 32 ) - 97, 32 * (rows + 0.5f) + 16);
                    gameWindow.draw(digitMap[zeroMin]);

                    digitMap[firstMin].setPosition((cols * 32 ) - 97 + 21, 32 * (rows + 0.5f) + 16);
                    gameWindow.draw(digitMap[firstMin]);

                    digitMap[zeroSec].setPosition((cols * 32 ) - 54, 32 * (rows + 0.5f) + 16);
                    gameWindow.draw(digitMap[zeroSec]);

                    digitMap[firstSec].setPosition((cols * 32 ) - 54 + 21, 32 * (rows + 0.5f) + 16);
                    gameWindow.draw(digitMap[firstSec]);

                    gameWindow.display();

                    if(gLeaderboard) {
                        cout << " HOLAAA " << endl;
                        if (gWon) {
                            if (leaderBoard(sWidth, sHeight, mins, secs, userName, true)) {}
                            gLeaderboard = false;
                        } else {
                            if(leaderBoard(sWidth, sHeight, mins, secs, userName, false)) {}
                            gLeaderboard = false;
                        }
                        auto clock = chrono::high_resolution_clock::now();
                        elapsed += chrono::duration_cast<chrono::seconds>(clock - paused).count();

                        if(not gLost) {
                            game_board.leaderboardState();
                        }
                    }



                } // End of poll event loop

            } //End of while it is open

        } //
    } //End of while true

}

struct Player {
    float time;
    string name;
};
bool comparePlayers(const Player& a, const Player& b) {
    return a.time < b.time;
}


bool leaderBoard(int w, int h, int mins, int secs, string& userName, bool won) {
    const int width = w / 2;
    const int height = h / 2;

    RenderWindow leaderboardWindow(VideoMode(width, height), "Leaderboard");

    vector<Player> leaderboard;
    vector<string> forDisplay;
    String text;

    ifstream file("files/leaderboard.txt");
    string line;

    while(getline(file, line)) {
        string mins;
        string secs;
        string name;
        float time;

        istringstream ss(line);

        getline(ss, mins, ':');
        getline(ss, secs, ',');
        getline(ss, name, ',');

        if (name.find('*') != string::npos) {
            name.pop_back();
        }
        string val = mins + secs;
        time = stof(val);

        leaderboard.push_back({time, name});
    }

    if(won) {
        string min1 = to_string(mins);
        string sec1 = to_string(secs);
        string value;

        if (secs < 10) {
            value = min1 + '0' + sec1;
        } else {
            value = min1 + sec1;
        }
        float time = stof(value);
        string name = userName + "*";
        leaderboard.push_back({time, name});

    }
    sort(leaderboard.begin(), leaderboard.end(), comparePlayers);


    int place = 1;

    ofstream write("files/leaderboard.txt");

    for (const auto& player : leaderboard) {
        float time = player.time;
        string name = player.name;
        string value;

        if (time >= 1000) {
            value = to_string(time);
            value.erase(4, value.length());
            value.insert(2, ":");
        }
        else if (time >= 100 && time < 1000) {
            value = "0" + to_string(time);
            value.erase(4, value.length());
            value.insert(2, ":");
        } else if (time >= 10 && time < 100) {
            value = "00" + to_string(time);
            value.erase(4, value.length());
            value.insert(2, ":");
        } else if (time > 0 && time < 10) {
            value = "000" + to_string(time);
            value.erase(4, value.length());
            value.insert(2, ":");
        } else {
            value = "0" + to_string(time);
            value.erase(6, value.length());
            value.erase(2, 2);
            value.insert(2, ":");
        }
        string position = to_string(place);
        forDisplay.push_back(position + ".\t" + value + "\t" + name + "\n\n");
        write << value << "," << name << endl;
        place++;
    }
    for (int i = 0; i < 5; i++) {
        text += forDisplay[i];
    }

    Text lbText;
    Font lbFont;

    lbFont.loadFromFile("files/font.ttf");

    lbText.setFont(lbFont);
    lbText.setCharacterSize(20);
    lbText.setStyle(Text::Bold | Text::Underlined);
    lbText.setString("LEADERBOARD");

    FloatRect ldTextRect = lbText.getLocalBounds();
    lbText.setOrigin(ldTextRect.left + ldTextRect.width / 2.0f, ldTextRect.top + ldTextRect.height / 2.0f);
    lbText.setPosition(Vector2f(width/2.0f, height/2.0f - 120));

    Text displayText;

    displayText.setFont(lbFont);
    displayText.setCharacterSize(18);
    displayText.setString(text);

    FloatRect displayTextRect = displayText.getLocalBounds();
    displayText.setOrigin(displayTextRect.left + displayTextRect.width / 2.0f, displayTextRect.top + displayTextRect.height / 2.0f);
    displayText.setPosition(width/2.0f, height/2.0f + 20);

    while (leaderboardWindow.isOpen()) {
        Event event;

        while (leaderboardWindow.pollEvent(event)) {
            switch (event.type) {
                case Event::Closed:
                    leaderboardWindow.close();
                    break;
            }
        }
        leaderboardWindow.clear(Color::Blue);
        leaderboardWindow.draw(lbText);
        leaderboardWindow.draw(displayText);

        leaderboardWindow.display();

    }

    return false;
}



// string1.length == string2length
// hi ih
// 2 for O(n2)
//