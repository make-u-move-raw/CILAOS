#include <math.h>
#include "app/GuiLayer.hpp"

GUIlayer::~GUIlayer()
{
}

GUIlayer::GUIlayer()
{
    // sliders for noise scale, persistence, lacunarity, octaves
    sliders = {
        {"Noise Scale", false, 1.0f, 0.0f, 10.0f},
        {"Persistence", false, 1.0f, 0.0f, 10.0f},
        {"Lacunarity", false, 1.0f, 0.0f, 10.0f},
        {"Frequency", false, 1.0f, 0.0f, 100.0f},
        {"Octaves", false, 1.0f, 0.0f, 10.0f, true}};
    m_showGUI = true;
    m_textBoxEditMode = false;
}

/**
 * @brief this alow to toggle on and off the GUI
 */
void GUIlayer::toggleGUI()
{
    this->m_showGUI = !this->m_showGUI;
}

/**
 * @brief getter for the bool showGUI
 * @return bool if we show the GUI or not
 */
bool GUIlayer::getShowGUI()
{
    return this->m_showGUI;
}

/**
 * @brief apply all the changes of the sliders then dispach to the scene layer
 */

void GUIlayer::applySliderChanges(std::string name, float Value)
{

    auto &app = Core::Application::getInstance();

    if (name == "Noise Scale")
    {
        Core::EventType eventType = Core::EventType::CHANGE_NOISESCALE;
        std::string name = "Noise Scale";
        Core::Event event(eventType, name, Value);

        app.dispatchEvents(event);
    }
    else if (name == "Persistence")
    {
        Core::EventType eventType = Core::EventType::CHANGE_PERSISTENCE;
        std::string name = "Persistene";
        Core::Event event(eventType, name, Value);
        app.dispatchEvents(event);
    }
    else if (name == "Lacunarity")
    {
        Core::EventType eventType = Core::EventType::CHANGE_LACUNARITY;
        std::string name = "Lacunarity";
        Core::Event event(eventType, name, Value);

        app.dispatchEvents(event);
    }
    else if (name == "Frequency")
    {
        Core::EventType eventType = Core::EventType::CHANGE_FREQUENCY;
        std::string name = "Frequency";
        Core::Event event(eventType, name, Value);

        app.dispatchEvents(event);
    }
    else if (name == "Octaves")
    {
        Core::EventType eventType = Core::EventType::CHANGE_OCTAVES;
        std::string name = "Octaves";
        Core::Event event(eventType, name, Value);
        app.dispatchEvents(event);
    }
    else if (name == "Seed")
    {
        Core::EventType eventType = Core::EventType::CHANGE_SEED;
        std::string name = "Seed";
        Core::Event event(eventType, name, Value);
        app.dispatchEvents(event);
    }
}

/**
 * @brief This function renders all the sliders and the GUI elements it includes the backpanel, sliders, buttons, text input
 */
void GUIlayer::renderGui()
{

    int x = 90;
    int y = 50;
    int width = 200;
    int height = 20;

    // Backpanel
    DrawRectangle(10, 20, 315, 315, Fade(WHITE, 0.8f));
    Rectangle bounds = {(float)x, (float)y, (float)width, (float)height};
    ;

    for (auto &slider : sliders)
    {
        // bug all the event are called.
        if (GuiSlider(bounds, slider.name.c_str(), NULL, &slider.value, slider.minValue, slider.maxValue))
        {
            applySliderChanges(slider.name, slider.value);
            std::cout << "AHHHHHH" << std::endl;
        }
        if (slider.isInteger)
        {
            // roundup the value
            slider.value = roundf(slider.value);
            DrawText(TextFormat("%.0f", slider.value), x + width + 5, y + 5, 12, BLACK);
        }
        else
        {
            // if it's a float
            DrawText(TextFormat("%.2f", slider.value), x + width + 5, y + 5, 12, BLACK);
        }

        // padding between each slider
        y += 40;
        bounds = {(float)x, (float)y, (float)width, (float)height};
    }

    if (GuiTextBox(bounds, this->seed, 16, m_textBoxEditMode))
    {
        // bloc the text input if not selected
        this->m_textBoxEditMode = !this->m_textBoxEditMode;
    }
    y += 40;
    bounds = {(float)x, (float)y, (float)width, (float)height};

    // Draw Regenerate button
    if (GuiButton(bounds, "Regenerate"))
    {
        // when clicked send a
        std::cout << "ahhhh" << std::endl;
        applySliderChanges("Seed", 0.0);
    }
}

void GUIlayer::render()
{
    if (m_showGUI)
    {
        renderGui();
    }
}

void GUIlayer::update(double dt)
{
}

void GUIlayer::stop()
{
}