// display.h
#pragma once
#include <MiniFB.h>
#include <vector>
#include <cstdint>

class Display {
public:
    Display(uint32_t width, uint32_t height);
    ~Display();

    void setPixel(uint32_t x, uint32_t y, uint32_t color);
    bool update();  

private:
    uint32_t width, height;
    std::vector<uint32_t> buffer; // 0xAARRGGBB format
    struct mfb_window* window;
};