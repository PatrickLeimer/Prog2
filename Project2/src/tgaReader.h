// Created by patri on 10/25/2024.
#pragma once

#include <fstream>
#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

struct Header { //Metadata
    char idLength;
    char colorMapType;
    char dataTypeCode;
    short colorMapOrigin;
    short colorMapLength;
    char colorMapDepth;
    short xOrigin;
    short yOrigin;
    short width;
    short height;
    char bitsPerPixel;
    char imageDescriptor;
    int pixelCount;
        //Constructors
    Header(char idLength, char colorMapType, char dataTypeCode, short colorMapOrigin, short colorMapLength, char colorMapDepth, short xOrigin, short yOrigin, short width, short height, char bitsPerPixel, char imageDescriptor) {
        this->idLength = idLength;
        this->colorMapType = colorMapType;
        this->dataTypeCode = dataTypeCode;
        this->colorMapOrigin = colorMapOrigin;
        this->colorMapLength = colorMapLength;
        this->colorMapDepth = colorMapDepth;
        this->xOrigin = xOrigin;
        this->yOrigin = yOrigin;
        this->width = width;
        this->height = height;
        this->bitsPerPixel = bitsPerPixel;
        this->imageDescriptor = imageDescriptor;
    }
    Header() : idLength(0), colorMapType(0), dataTypeCode(0), colorMapOrigin(0),
           colorMapLength(0), colorMapDepth(0), xOrigin(0), yOrigin(0),
           width(0), height(0), bitsPerPixel(0), imageDescriptor(0), pixelCount(0) {}
};

struct Pixel { //BGR
    unsigned char blue;
    unsigned char green;
    unsigned char red;
        //Constructor
    Pixel(unsigned char b = 0, unsigned char g = 0, unsigned char r = 0) {
        this->blue = b;
        this->green = g;
        this->red = r;
    }

};

struct TGAfile {
    Header header;
    int size;
    vector<Pixel> pixels;

    TGAfile(Header h, int s, vector<Pixel>& p) {
        this->header = h;
        this->size = s;
        this->pixels = p;
    }
};

TGAfile readTGA(const string& in) {
    Header header;
    ifstream infile(in,ios::binary);
    if (!infile.is_open()) {
        cout << infile.is_open() << endl;
        throw runtime_error("File does not exist.");
    }

    infile.read(&header.idLength, sizeof(header.idLength));
    infile.read(&header.colorMapType, sizeof(header.colorMapType));
    infile.read(&header.dataTypeCode, sizeof(header.dataTypeCode));
    infile.read((char*)(&header.colorMapOrigin), sizeof(header.colorMapOrigin));
    infile.read((char*)(&header.colorMapLength), sizeof(header.colorMapLength));
    infile.read(&header.colorMapDepth, sizeof(header.colorMapDepth));
    infile.read((char*)(&header.xOrigin), sizeof(header.xOrigin));
    infile.read((char*)(&header.yOrigin), sizeof(header.yOrigin));
    infile.read((char*)(&header.width), sizeof(header.width));
    infile.read((char*)(&header.height), sizeof(header.height));
    infile.read(&header.bitsPerPixel, sizeof(header.bitsPerPixel));
    infile.read(&header.imageDescriptor, sizeof(header.imageDescriptor));

    //Declaring
    header.pixelCount = header.width * header.height;
    vector<Pixel> pixels;

    for (int i = 0; i < header.pixelCount; i++) {
        Pixel temp;
        infile.read((char*)(&temp.blue), sizeof(temp.blue));
        infile.read((char*)(&temp.green), sizeof(temp.green));
        infile.read((char*)(&temp.red), sizeof(temp.red));
        pixels.push_back(temp);
    }
    TGAfile tgaFile(header, header.pixelCount, pixels);
    return tgaFile;

} //End readTGA

void writeTGA(const string& out, TGAfile& tgaFile) {
    
    fstream outfile(out, ios::out | ios::binary);
    Header& header = tgaFile.header;

    if (!outfile.is_open()) {
        cout << outfile.is_open() << endl;
        throw runtime_error("Could not open input file: " + out);
    }

    outfile.write(&header.idLength, sizeof(header.idLength));
    outfile.write(&header.colorMapType, sizeof(header.colorMapType));
    outfile.write(&header.dataTypeCode, sizeof(header.dataTypeCode));
    outfile.write((char*)(&header.colorMapOrigin), sizeof(header.colorMapOrigin));
    outfile.write((char*)(&header.colorMapLength), sizeof(header.colorMapLength));
    outfile.write(&header.colorMapDepth, sizeof(header.colorMapDepth));
    outfile.write((char*)(&header.xOrigin), sizeof(header.xOrigin));
    outfile.write((char*)(&header.yOrigin), sizeof(header.yOrigin));
    outfile.write((char*)(&header.width), sizeof(header.width));
    outfile.write((char*)(&header.height), sizeof(header.height));
    outfile.write(&header.bitsPerPixel, sizeof(header.bitsPerPixel));
    outfile.write(&header.imageDescriptor, sizeof(header.imageDescriptor));

    //
    for (Pixel& pixel : tgaFile.pixels) {
        outfile.write((char*)(&pixel.blue), sizeof(pixel.blue));
        outfile.write((char*)(&pixel.green), sizeof(pixel.green));
        outfile.write((char*)(&pixel.red), sizeof(pixel.red));
    }

} //End writeTGA


//-----------------------------------------------------------------------------------------------
//
// unsigned char clamp(int val) {
//   // Check if within range
//   if (val < 0) {
//     val = 0;
//   } else if (val > 255) {
//     val = 255;
//     }
//   return (unsigned char)val;
//  }
//
// unsigned char floatToChar(float value) {
//   return clamp((int)(value * 255.0f + 0.5f));
// }
//
// float normalize(unsigned char val) {
//   return static_cast<float>(val / 255.0f);
// }
//
// //Multiply
// TGAfile multiply(const TGAfile& file, const TGAfile& other) {
//   if (file.header.width != other.header.width || file.header.height != other.header.height) {
//     throw runtime_error("Image size don't match");
//   }
//
//   TGAfile r = other;
//   for(int i = 0; i < file.pixels.size(); ++i) {
//       r.pixels[i].blue = floatToChar(normalize(file.pixels[i].blue) * normalize(other.pixels[i].blue));
//       r.pixels[i].green = floatToChar(normalize(file.pixels[i].green) * normalize(other.pixels[i].green));
//       r.pixels[i].red = floatToChar(normalize(file.pixels[i].red) * normalize(other.pixels[i].red));
//   }
//   return r;
// }
//
// //
// TGAfile screen(const TGAfile& file, const TGAfile& other) {
//   if (file.header.width != other.header.width || file.header.height != other.header.height) {
//     throw runtime_error("Image size don't match");
//   }
//
//   TGAfile r = other;
//   for(int i = 0; i < file.pixels.size(); ++i) {
//     r.pixels[i].blue = floatToChar( 1.0f - (1.0f - normalize(file.pixels[i].blue)) * (1.0f - normalize(other.pixels[i].blue) ));
//     r.pixels[i].green = floatToChar( 1.0f - (1.0f - normalize(file.pixels[i].green)) * (1.0f - normalize(other.pixels[i].green) ));
//     r.pixels[i].red = floatToChar( 1.0f - (1.0f - normalize(file.pixels[i].red)) * (1.0f - normalize(other.pixels[i].red) ));
//
//   }
//   return r;
// }
//
// // Substract
// TGAfile subtract(const TGAfile& file, const TGAfile& other) {
//   if (file.header.width != other.header.width || file.header.height != other.header.height) {
//     throw runtime_error("Image size don't match");
//   }
//
//   TGAfile r = other;
//
//   for(int i = 0; i < file.pixels.size(); ++i) {
//     r.pixels[i].blue = clamp(file.pixels[i].blue - other.pixels[i].blue);
//     r.pixels[i].green = clamp(file.pixels[i].green - other.pixels[i].green);
//     r.pixels[i].red = clamp(file.pixels[i].red - other.pixels[i].red);
//
//   }
//   return r;
//
// }
// //Add
// TGAfile addition(const TGAfile& file, const TGAfile& other) {
//   if (file.header.width != other.header.width || file.header.height != other.header.height) {
//     throw runtime_error("Image size don't match");
//   }
//
//   TGAfile r = other;
//
//   for(int i = 0; i < file.pixels.size(); ++i) {
//     r.pixels[i].blue = clamp(file.pixels[i].blue + other.pixels[i].blue);
//     r.pixels[i].green = clamp(file.pixels[i].green + other.pixels[i].green);
//     r.pixels[i].red = clamp(file.pixels[i].red + other.pixels[i].red);
//
//   }
//   return r;
//
// }
//
// TGAfile overlay(const TGAfile& file, const TGAfile& other) {
//   if (file.header.width != other.header.width || file.header.height != other.header.height) {
//     throw runtime_error("Image size don't match");
//   }
//
//   TGAfile r = other;
//   for(int i = 0; i < file.pixels.size(); ++i) {
//     float n1 = normalize(file.pixels[i].blue);
//     float n2 = normalize(other.pixels[i].blue);
//     r.pixels[i].blue = floatToChar(n2 <= 0.5f ? 2.0f * n1 * n2 : 1.0f - 2.0f * (1.0f - n1) * (1.0f - n2));
//
//     float n3 = normalize(file.pixels[i].green);
//     float n4 = normalize(other.pixels[i].green);
//     r.pixels[i].green = floatToChar(n4 <= 0.5f ? 2.0f * n3 * n4 : 1.0f - 2.0f * (1.0f - n3) * (1.0f - n4));
//
//     float n5 = normalize(file.pixels[i].red);
//     float n6 = normalize(other.pixels[i].red);
//     r.pixels[i].red = floatToChar(n6 <= 0.5f ? 2.0f * n5 * n6 : 1.0f - 2.0f * (1.0f - n5) * (1.0f - n6));
//   }
//   return r;
// }
//
// void addGChannel(TGAfile& file, int value) {
//   for (Pixel& pixel : file.pixels) {
//     pixel.green = clamp(pixel.green + value);
//   }
// }
//
// void scaleRBChannels(TGAfile& file, float red, float blue) {
//   for (Pixel& pixel : file.pixels) {
//     pixel.blue = clamp(static_cast<int>(pixel.blue * blue));
//     pixel.red = clamp(static_cast<int>(pixel.red * red));
//   }
// }
//
// void separateChannels(const TGAfile& file, const string& bluePath, const string& greenPath, const string& redPath) {
//   TGAfile b = file;
//   TGAfile g = file;
//   TGAfile r = file;
//
//   for (int i = 0; i < file.pixels.size(); ++i) {
//     b.pixels[i].red = b.pixels[i].green = file.pixels[i].blue;
//     g.pixels[i].red = g.pixels[i].blue = file.pixels[i].green;
//     r.pixels[i].green = r.pixels[i].blue = file.pixels[i].red;
//
//   }
//   writeTGA(bluePath, b);
//   writeTGA(greenPath, g);
//   writeTGA(redPath, r);
//
// }
//
// TGAfile combineChannels(const TGAfile& file1, const TGAfile& file2, const TGAfile& file3) { //RGB
//
//   if (file1.header.width != file2.header.width || file1.header.width != file3.header.width ||
//       file1.header.height != file2.header.height || file1.header.height != file3.header.height) {
//     throw runtime_error("Image sizes don't match");
//       }
//   TGAfile r = file1;
//
//   for (int i = 0; i < file1.pixels.size(); ++i) {
//
//     r.pixels[i].blue = file3.pixels[i].blue;
//     r.pixels[i].green = file2.pixels[i].green;
//     r.pixels[i].red = file1.pixels[i].red;
//
//   }
//   return r;
// }
//
// void rotate(TGAfile& file) {
//   reverse(file.pixels.begin(), file.pixels.end());
// }
