#ifndef IMAGE_HPP
#define IMAGE_HPP

#include "graphics_engine.hpp"

//Loads individual image as texture
SDL_Texture* loadTexture(SDL_Renderer* renderer, std::string path);

#endif