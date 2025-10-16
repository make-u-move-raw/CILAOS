#pragma once
#include <iostream>
#include "external/raylib/raylib.h"
#include "external/raylib/raygui.h"
#include "core/Layer.hpp"
#include <string>
#include <vector>

/**
 * @struct Slider
 * @brief Struct to track easely all the sliders
 */
struct slider
{
    std::string name;
    bool Editmode = false;
    float value;
    float minValue;
    float maxValue;
    bool isInteger;
};

/**
 * @class GUI layze
 * @brief The layer responsible for all the GUI informatations
 */
class GUIlayer : public Core::Layer
{
public:
    GUIlayer();
    void renderGui();
    void toggleGUI();
    bool getShowGUI();
    char seed[24] = "SEED";
    bool textBoxEditMode;
    std::vector<slider> sliders;

private:
    bool showGUI;
};