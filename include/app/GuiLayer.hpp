#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <math.h>



#include "external/raylib/raylib.h"
#include "external/raylib/raygui.h"

#include "app/Application.hpp"
#include "app/Context.hpp"
#include "core/Layer.hpp"

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
class GUILayer : public Core::Layer
{
public:
    GUILayer();
    virtual ~GUILayer();

    virtual void update(double dt) override;
    virtual void render() override;
    virtual void onEvent(Core::Event &event) override;
    virtual void stop() override;

    /**
     * @brief Called once initialized to link the app context (shared data) between all different layers
     * @param context The context of the app
     */
    virtual void setContext(std::shared_ptr<Context> context) override { m_terrain = context->terrain; }

    /**
     * @brief apply all the changes of the sliders then dispach to the scene layer
     */
    void applySliderChanges(std::string name, float value);
    void renderGui();

    /**
     * @brief this alow to toggle on and off the GUI
     */
    void toggleGUI() { m_showGUI = !m_showGUI; }

    /**
     * @brief getter for the bool showGUI
     * @return bool if we show the GUI or not
     */
    bool getShowGUI() const { return m_showGUI; }
    char seed[24] = "SEED";
    std::vector<slider> sliders;

private:
    std::shared_ptr<Core::Terrain> m_terrain; // Shared terrain from the app context
    bool m_textBoxEditMode;

    bool m_showGUI;
};