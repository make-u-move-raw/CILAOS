#pragma once
#include <iostream>
#include "external/raylib/raylib.h"
#include "external/raylib/raygui.h"
#include "core/Layer.hpp"
#include <string>
#include <vector>

#include "core/Application.hpp"

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
    virtual ~GUIlayer();

    virtual void update(double dt) override;
    virtual void render() override;
    virtual void onEvent(Core::Event &event) {
        std::cout << "Event recieved by the GUI"<< std::endl;    
        }
    virtual void stop();

    void applySliderChanges(std::string name, float value);
    void renderGui();
    void toggleGUI();
    bool getShowGUI();
    char seed[24] = "SEED";
    std::vector<slider> sliders;

private:
    bool m_textBoxEditMode;

    bool m_showGUI;
};