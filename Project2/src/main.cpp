#include <iostream>
#include <fstream>

using namespace std;
#include "tgaReader.h"
#include "algorithms.h"

int main() {
    try {
    // #1 Working
    TGAfile layer1 = readTGA("input/layer1.tga");
    TGAfile pattern1 = readTGA("input/pattern1.tga");
    TGAfile resultMultiply = multiply(layer1, pattern1);
    writeTGA("output/part1.tga", resultMultiply);
    // #2 Working
    TGAfile car = readTGA("input/car.tga");
    TGAfile layer2 = readTGA("input/layer2.tga");
    TGAfile r = subtract(car, layer2);
    writeTGA("output/part2.tga", r);
    // #3 Fixed
    TGAfile txt = readTGA("input/text.tga");
    TGAfile pattern2 = readTGA("input/pattern2.tga");
    TGAfile rMultiply1 = multiply(layer1, pattern2);
    TGAfile r1 = screen(rMultiply1, txt);
    writeTGA("output/part3.tga", r1);
    // #4 Working
    TGAfile crcl = readTGA("input/circles.tga");
    TGAfile r2 = multiply(layer2, crcl);
    TGAfile sub1 = subtract(r2, pattern2);
    writeTGA("output/part4.tga", sub1);
    // #5 Fixed
    TGAfile rOver = overlay(layer1, pattern1);
    writeTGA("output/part5.tga", rOver);
    // #6 Working
    TGAfile carG = readTGA("input/car.tga");
    addGChannel(carG, 200);
    writeTGA("output/part6.tga", carG);
    // #7 Working
    TGAfile carSca = readTGA("input/car.tga");
    scaleRBChannels(carSca, 4.0f, 0.0f);
    writeTGA("output/part7.tga", carSca);
    // #8 Working
    TGAfile multipleCarChannels = readTGA("input/car.tga");
    separateChannels(multipleCarChannels, "output/part8_b.tga", "output/part8_g.tga", "output/part8_r.tga");
    // #9 Working
    TGAfile blayer = readTGA("input/layer_blue.tga");
    TGAfile glayer = readTGA("input/layer_green.tga");
    TGAfile rlayer = readTGA("input/layer_red.tga");
    TGAfile comb = combineChannels(rlayer, glayer, blayer);
    writeTGA("output/part9.tga", comb);
    // #10 Working
    TGAfile txt2 = readTGA("input/text2.tga");
    rotate(txt2);
    writeTGA("output/part10.tga", txt2);

    } catch (const runtime_error& e) {
        cerr << "Error: " << e.what() << endl;
    }
    return 0;
}

// example/EXAMPLE_part10.tga
// fc.exe /b .\output\part10.tga .\examples\EXAMPLE_part10.tga
