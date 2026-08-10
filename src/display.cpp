// display.cpp
#include "display.hpp"
#include "MiniFB_enums.h"
#include <stdexcept>
#include "cpu.hpp"

Display::Display(uint32_t w, uint32_t h) : width(w), height(h), buffer(w * h, 0) {
    window = mfb_open("Gloey", width, height);
    if (!window) {
        throw std::runtime_error("Impossible d'ouvrir la fenêtre minifb");
    }
    mfb_set_target_fps(60);
    mfb_set_char_input_callback(window, char_input_callback);
}

Display::~Display() {
    mfb_close(window);
}

void Display::setPixel(uint32_t x, uint32_t y, uint32_t color) {
    if (x >= width || y >= height) return;
    buffer[y * width + x] = color;
}

bool Display::update() {
    mfb_update_state state = mfb_update_ex(window, buffer.data(), width, height);
    return state == MFB_STATE_OK;
}