//
// Created by patri on 10/26/2024.
//

#pragma once

#include "tgaReader.h"
using namespace std;

unsigned char clamp(int val) {
  // Check if within range
  if (val < 0) {
    val = 0;
  } else if (val > 255) {
    val = 255;
    }
  return (unsigned char)val;
 }

unsigned char floatToChar(float value) {
  return clamp((int)(value * 255.0f + 0.5f));
}

float normalize(unsigned char val) {
  return static_cast<float>(val / 255.0f);
}

//Multiply
TGAfile multiply(const TGAfile& file, const TGAfile& other) {
  if (file.header.width != other.header.width || file.header.height != other.header.height) {
    throw runtime_error("Image size don't match");
  }

  TGAfile r = other;
  for(int i = 0; i < file.pixels.size(); ++i) {
      r.pixels[i].blue = floatToChar(normalize(file.pixels[i].blue) * normalize(other.pixels[i].blue));
      r.pixels[i].green = floatToChar(normalize(file.pixels[i].green) * normalize(other.pixels[i].green));
      r.pixels[i].red = floatToChar(normalize(file.pixels[i].red) * normalize(other.pixels[i].red));
  }
  return r;
}

//
TGAfile screen(const TGAfile& file, const TGAfile& other) {
  if (file.header.width != other.header.width || file.header.height != other.header.height) {
    throw runtime_error("Image size don't match");
  }

  TGAfile r = other;
  for(int i = 0; i < file.pixels.size(); ++i) {
    r.pixels[i].blue = floatToChar( 1.0f - (1.0f - normalize(file.pixels[i].blue)) * (1.0f - normalize(other.pixels[i].blue) ));
    r.pixels[i].green = floatToChar( 1.0f - (1.0f - normalize(file.pixels[i].green)) * (1.0f - normalize(other.pixels[i].green) ));
    r.pixels[i].red = floatToChar( 1.0f - (1.0f - normalize(file.pixels[i].red)) * (1.0f - normalize(other.pixels[i].red) ));

  }
  return r;
}

// Substract
TGAfile subtract(const TGAfile& file, const TGAfile& other) {
  if (file.header.width != other.header.width || file.header.height != other.header.height) {
    throw runtime_error("Image size don't match");
  }

  TGAfile r = other;

  for(int i = 0; i < file.pixels.size(); ++i) {
    r.pixels[i].blue = clamp(file.pixels[i].blue - other.pixels[i].blue);
    r.pixels[i].green = clamp(file.pixels[i].green - other.pixels[i].green);
    r.pixels[i].red = clamp(file.pixels[i].red - other.pixels[i].red);

  }
  return r;

}
//Add
TGAfile addition(const TGAfile& file, const TGAfile& other) {
  if (file.header.width != other.header.width || file.header.height != other.header.height) {
    throw runtime_error("Image size don't match");
  }

  TGAfile r = other;

  for(int i = 0; i < file.pixels.size(); ++i) {
    r.pixels[i].blue = clamp(file.pixels[i].blue + other.pixels[i].blue);
    r.pixels[i].green = clamp(file.pixels[i].green + other.pixels[i].green);
    r.pixels[i].red = clamp(file.pixels[i].red + other.pixels[i].red);

  }
  return r;

}

TGAfile overlay(const TGAfile& file, const TGAfile& other) {
  if (file.header.width != other.header.width || file.header.height != other.header.height) {
    throw runtime_error("Image size don't match");
  }

  TGAfile r = other;
  for(int i = 0; i < file.pixels.size(); ++i) {
    float n1 = normalize(file.pixels[i].blue);
    float n2 = normalize(other.pixels[i].blue);
    r.pixels[i].blue = floatToChar(n2 <= 0.5f ? 2.0f * n1 * n2 : 1.0f - 2.0f * (1.0f - n1) * (1.0f - n2));

    float n3 = normalize(file.pixels[i].green);
    float n4 = normalize(other.pixels[i].green);
    r.pixels[i].green = floatToChar(n4 <= 0.5f ? 2.0f * n3 * n4 : 1.0f - 2.0f * (1.0f - n3) * (1.0f - n4));

    float n5 = normalize(file.pixels[i].red);
    float n6 = normalize(other.pixels[i].red);
    r.pixels[i].red = floatToChar(n6 <= 0.5f ? 2.0f * n5 * n6 : 1.0f - 2.0f * (1.0f - n5) * (1.0f - n6));
  }
  return r;
}

void addGChannel(TGAfile& file, int value) {
  for (Pixel& pixel : file.pixels) {
    pixel.green = clamp(pixel.green + value);
  }
}

void scaleRBChannels(TGAfile& file, float red, float blue) {
  for (Pixel& pixel : file.pixels) {
    pixel.blue = clamp(static_cast<int>(pixel.blue * blue));
    pixel.red = clamp(static_cast<int>(pixel.red * red));
  }
}

void separateChannels(const TGAfile& file, const string& bluePath, const string& greenPath, const string& redPath) {
  TGAfile b = file;
  TGAfile g = file;
  TGAfile r = file;

  for (int i = 0; i < file.pixels.size(); ++i) {
    b.pixels[i].red = b.pixels[i].green = file.pixels[i].blue;
    g.pixels[i].red = g.pixels[i].blue = file.pixels[i].green;
    r.pixels[i].green = r.pixels[i].blue = file.pixels[i].red;

  }
  writeTGA(bluePath, b);
  writeTGA(greenPath, g);
  writeTGA(redPath, r);

}

TGAfile combineChannels(const TGAfile& file1, const TGAfile& file2, const TGAfile& file3) { //RGB

  if (file1.header.width != file2.header.width || file1.header.width != file3.header.width ||
      file1.header.height != file2.header.height || file1.header.height != file3.header.height) {
    throw runtime_error("Image sizes don't match");
      }
  TGAfile r = file1;

  for (int i = 0; i < file1.pixels.size(); ++i) {

    r.pixels[i].blue = file3.pixels[i].blue;
    r.pixels[i].green = file2.pixels[i].green;
    r.pixels[i].red = file1.pixels[i].red;

  }
  return r;
}

void rotate(TGAfile& file) {
  reverse(file.pixels.begin(), file.pixels.end());
}
