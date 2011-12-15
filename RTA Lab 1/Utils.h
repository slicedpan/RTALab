#pragma once

#include "glut.h"
#include <vector>
#include <string>

GLuint LoadTextureRAW( const char * filename, int wrap, int height, int width );
void loadFile(std::vector<unsigned char>& buffer, const std::string& filename);
int decodePNG(std::vector<unsigned char>& out_image, unsigned long& image_width, unsigned long& image_height, const unsigned char* in_png, size_t in_size, bool convert_to_rgba32);