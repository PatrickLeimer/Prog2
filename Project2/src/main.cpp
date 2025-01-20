#include <iostream>
#include <fstream>
#include <cstring>
#include <string>
#include <vector>
#include <stdexcept>
using namespace std;

#include "tgaReader.h"
#include "algorithms.h"

void printHelpMessage() {
    cout << "Project 2: Image Processing, Fall 2024\n\n";
    cout << "Usage:\n\t./project2.out [output] [firstImage] [method] [...]\n";
}

// Ends in tga
bool TGAExtensionchecker(const string& filename) {
    if (filename.length() < 4) {
        return false;
    }
    return filename.substr(filename.length() - 4, 4) == ".tga";
}

int main(int argc, char* argv[]) {
    try {
        // Handle no arguments or --help
        if (argc == 1 || (argc == 2 && strcmp(argv[1], "--help") == 0)) {
            printHelpMessage();
            return 0;
        }

        // Validate the number of arguments (at least output and source files)
        if (argc < 3) {
            printHelpMessage();
            return 0;
        }

        // Validate output file
        string outputFile = argv[1];
        if (!TGAExtensionchecker(outputFile)) {
            cerr << "Invalid file name.\n";
            return 1;
        }

        // Validate source file
        string sourceFile = argv[2];
        if (!TGAExtensionchecker(sourceFile)) {
            cerr << "Invalid file name.\n";
            return 1;
        }

        // Load the tracking image
        TGAfile trackingImage = readTGA(sourceFile);

        // Start parsing methods from argv[3] onwards
        int currentArg = 3;
        while (currentArg < argc) {
            string method = argv[currentArg];
            currentArg++;

            // Handle each method
            if (method == "multiply") {
                if (currentArg >= argc) {
                    cerr << "Missing argument.\n";
                    return 1;
                }
                string secondFile = argv[currentArg];
                currentArg++;

                if (!TGAExtensionchecker(secondFile)) {
                    cerr << "Invalid argument, invalid file name.\n";
                    return 1;
                }

                TGAfile secondImage = readTGA(secondFile);
                trackingImage = multiply(trackingImage, secondImage);
                cout << "Multiplying " << sourceFile << " and " << secondFile << " ...\n";
            }
            else if (method == "subtract") {
                if (currentArg >= argc) {
                    cerr << "Missing argument.\n";
                    return 1;
                }
                string secondFile = argv[currentArg];
                currentArg++;

                if (!TGAExtensionchecker(secondFile)) {
                    cerr << "Invalid argument, invalid file name.\n";
                    return 1;
                }

                TGAfile secondImage = readTGA(secondFile);
                trackingImage = subtract(trackingImage, secondImage);
                cout << "Subtracting " << secondFile << " from " << sourceFile << " ...\n";
            }
            else if (method == "overlay") {
                if (currentArg >= argc) {
                    cerr << "Missing argument.\n";
                    return 1;
                }
                string secondFile = argv[currentArg];
                currentArg++;

                if (!TGAExtensionchecker(secondFile)) {
                    cerr << "Invalid argument, invalid file name.\n";                    return 1;
                }

                TGAfile secondImage = readTGA(secondFile);
                trackingImage = overlay(trackingImage, secondImage);
                cout << "Overlaying " << secondFile << " onto " << sourceFile << " ...\n";
            }
            else if (method == "screen") {
                if (currentArg >= argc) {
                    cerr << "Missing argument.\n";
                    return 1;
                }
                string secondFile = argv[currentArg];
                currentArg++;

                if (!TGAExtensionchecker(secondFile)) {
                    cerr << "Invalid argument, invalid file name.\n";
                    return 1;
                }

                TGAfile secondImage = readTGA(secondFile);
                trackingImage = screen(trackingImage, secondImage);
                cout << "Screening " << secondFile << " with " << sourceFile << " ...\n";
            }
            else if (method == "combine") {
                if (currentArg + 1 >= argc) {
                    cerr << "Missing argument.\n";
                    return 1;
                }
                string greenFile = argv[currentArg];
                string blueFile = argv[currentArg + 1];
                currentArg += 2;

                if (!TGAExtensionchecker(greenFile) || !TGAExtensionchecker(blueFile)) {
                    cerr << "Invalid argument, invalid file name.\n";
                    return 1;
                }

                TGAfile greenImage = readTGA(greenFile);
                TGAfile blueImage = readTGA(blueFile);
                trackingImage = combineChannels(trackingImage, greenImage, blueImage);
                cout << "Combining channels of " << sourceFile << ", " << greenFile << ", and " << blueFile << " ...\n";
            }
            else if (method == "flip") {
                rotate(trackingImage); // Assuming rotate 180 degrees is equivalent to flipping
                cout << "Flipping " << sourceFile << " ...\n";
            }
            else if (method == "onlyred") {
                // Extract only red channel
                for (auto& pixel : trackingImage.pixels) {
                    pixel.green = 0;
                    pixel.blue = 0;
                }
                cout << "Extracting red channel from " << sourceFile << " ...\n";
            }
            else if (method == "onlygreen") {
                // Extract only green channel
                for (auto& pixel : trackingImage.pixels) {
                    pixel.red = 0;
                    pixel.blue = 0;
                }
                cout << "Extracting green channel from " << sourceFile << " ...\n";
            }
            else if (method == "onlyblue") {
                // Extract only blue channel
                for (auto& pixel : trackingImage.pixels) {
                    pixel.red = 0;
                    pixel.green = 0;
                }
                cout << "Extracting blue channel from " << sourceFile << " ...\n";
            }
            else if (method == "addred") {
                if (currentArg >= argc) {
                    cerr << "Missing argument.\n";
                    return 1;
                }
                string valueStr = argv[currentArg];
                currentArg++;
                try {
                    int value = stoi(valueStr);
                    // Add value to red channel with clamping
                    for (auto& pixel : trackingImage.pixels) {
                        int newRed = static_cast<int>(pixel.red) + value;
                        pixel.red = clamp(newRed);
                    }
                    cout << "Adding " << (value >= 0 ? "+" : "") << value << " to red channel of " << sourceFile << " ...\n";
                }
                catch (const invalid_argument&) {
                    cerr << "Invalid argument, expected number.\n";
                    return 1;
                }
            }
            else if (method == "addgreen") {
                if (currentArg >= argc) {
                    cerr << "Missing argument.\n";
                    return 1;
                }
                string valueStr = argv[currentArg];
                currentArg++;
                try {
                    int value = stoi(valueStr);
                    // Add value to green channel with clamping
                    for (auto& pixel : trackingImage.pixels) {
                        int newGreen = static_cast<int>(pixel.green) + value;
                        pixel.green = clamp(newGreen);
                    }
                    cout << "Adding " << (value >= 0 ? "+" : "") << value << " to green channel of " << sourceFile << " ...\n";
                }
                catch (const invalid_argument&) {
                    cerr << "Invalid argument, expected number.\n";
                    return 1;
                }
            }
            else if (method == "addblue") {
                if (currentArg >= argc) {
                    cerr << "Missing argument.\n";
                    return 1;
                }
                string valueStr = argv[currentArg];
                currentArg++;
                try {
                    int value = stoi(valueStr);
                    // Add value to blue channel with clamping
                    for (auto& pixel : trackingImage.pixels) {
                        int newBlue = static_cast<int>(pixel.blue) + value;
                        pixel.blue = clamp(newBlue);
                    }
                    cout << "Adding " << (value >= 0 ? "+" : "") << value << " to blue channel of " << sourceFile << " ...\n";
                }
                catch (const invalid_argument&) {
                    cerr << "Invalid argument, expected number.\n";
                    return 1;
                }
            }
            else if (method == "scalered") {
                if (currentArg >= argc) {
                    cerr << "Missing argument.\n";
                    return 1;
                }
                string valueStr = argv[currentArg];
                currentArg++;
                try {
                    int scale = stoi(valueStr);
                    if (scale < 0) {
                        cerr << "Invalid argument, expected number.\n";
                        return 1;
                    }
                    for (auto& pixel : trackingImage.pixels) {
                        int newRed = static_cast<int>(pixel.red) * scale;
                        pixel.red = clamp(newRed);
                    }
                    cout << "Scaling the red channel of " << sourceFile << " by " << scale << " ...\n";
                }
                catch (const invalid_argument&) {
                    cerr << "Invalid argument, expected number.\n";
                    return 1;
                }
            }
            else if (method == "scalegreen") {
                if (currentArg >= argc) {
                    cerr << "Missing argument.\n";
                    return 1;
                }
                string valueStr = argv[currentArg];
                currentArg++;
                try {
                    int scale = stoi(valueStr);
                    if (scale < 0) {
                        cerr << "Invalid argument, expected number.\n";
                        return 1;
                    }
                    for (auto& pixel : trackingImage.pixels) {
                        int newGreen = static_cast<int>(pixel.green) * scale;
                        pixel.green = clamp(newGreen);
                    }
                    cout << "Scaling the green channel of " << sourceFile << " by " << scale << " ...\n";
                }
                catch (const invalid_argument&) {
                    cerr << "Invalid argument, expected number.\n";
                    return 1;
                }
            }
            else if (method == "scaleblue") {
                if (currentArg >= argc) {
                    cerr << "Missing argument.\n";
                    return 1;
                }
                string valueStr = argv[currentArg];
                currentArg++;
                try {
                    int scale = stoi(valueStr);
                    if (scale < 0) {
                        cerr << "Invalid argument, expected number.\n";
                        return 1;
                    }
                    for (auto& pixel : trackingImage.pixels) {
                        int newBlue = static_cast<int>(pixel.blue) * scale;
                        pixel.blue = clamp(newBlue);
                    }
                    cout << "Scaling the blue channel of " << sourceFile << " by " << scale << " ...\n";
                }
                catch (const invalid_argument&) {
                    cerr << "Invalid argument, expected number.\n";
                    return 1;
                }
            }
            else {
                cerr << "Invalid method name.\n";
                return 1;
            }
        }

        // After all manipulations, save the tracking image
        writeTGA(outputFile, trackingImage);
        cout << "... and saving output to " << outputFile << "!\n";

    } catch (const runtime_error& e) {
        cerr << "Error: " << e.what() << endl;
        return 1;
    }
}

    // try {
    // // #1 Working
    // TGAfile layer1 = readTGA("input/layer1.tga");
    // TGAfile pattern1 = readTGA("input/pattern1.tga");
    // TGAfile resultMultiply = multiply(layer1, pattern1);
    // writeTGA("output/part1.tga", resultMultiply);
    // // #2 Working
    // TGAfile car = readTGA("input/car.tga");
    // TGAfile layer2 = readTGA("input/layer2.tga");
    // TGAfile r = subtract(car, layer2);
    // writeTGA("output/part2.tga", r);
    // // #3 Fixed
    // TGAfile txt = readTGA("input/text.tga");
    // TGAfile pattern2 = readTGA("input/pattern2.tga");
    // TGAfile rMultiply1 = multiply(layer1, pattern2);
    // TGAfile r1 = screen(rMultiply1, txt);
    // writeTGA("output/part3.tga", r1);
    // // #4 Working
    // TGAfile crcl = readTGA("input/circles.tga");
    // TGAfile r2 = multiply(layer2, crcl);
    // TGAfile sub1 = subtract(r2, pattern2);
    // writeTGA("output/part4.tga", sub1);
    // // #5 Fixed
    // TGAfile rOver = overlay(layer1, pattern1);
    // writeTGA("output/part5.tga", rOver);
    // // #6 Working
    // TGAfile carG = readTGA("input/car.tga");
    // addGChannel(carG, 200);
    // writeTGA("output/part6.tga", carG);
    // // #7 Working
    // TGAfile carSca = readTGA("input/car.tga");
    // scaleRBChannels(carSca, 4.0f, 0.0f);
    // writeTGA("output/part7.tga", carSca);
    // // #8 Working
    // TGAfile multipleCarChannels = readTGA("input/car.tga");
    // separateChannels(multipleCarChannels, "output/part8_b.tga", "output/part8_g.tga", "output/part8_r.tga");
    // // #9 Working
    // TGAfile blayer = readTGA("input/layer_blue.tga");
    // TGAfile glayer = readTGA("input/layer_green.tga");
    // TGAfile rlayer = readTGA("input/layer_red.tga");
    // TGAfile comb = combineChannels(rlayer, glayer, blayer);
    // writeTGA("output/part9.tga", comb);
    // // #10 Working
    // TGAfile txt2 = readTGA("input/text2.tga");
    // rotate(txt2);
    // writeTGA("output/part10.tga", txt2);
    //
    // } catch (const runtime_error& e) {
    //     cerr << "Error: " << e.what() << endl;
    // }


// example/EXAMPLE_part10.tga
// fc.exe /b .\output\part10.tga .\examples\EXAMPLE_part10.tga
