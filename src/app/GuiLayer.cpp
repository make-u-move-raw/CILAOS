#include <math.h>

#include "app/GuiLayer.hpp"

GUILayer::~GUILayer() {}
GUILayer::GUILayer()
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

void GUILayer::applySliderChanges(std::string name, float Value)
{

    auto &app = Application::getInstance();

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
void GUILayer::renderGui()
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

void GUILayer::render()
{
    if (m_showGUI)
    {
        renderGui();
    }
}

void GUILayer::onEvent(Core::Event &event)
{
    std::cout << "Event on the GUI Layer" << std::endl;

    std::cout << "EVENT Type : ";
    switch (event.type)
    {
    case Core::EventType::CHANGE_NOISESCALE:
        std::cout << "noisecale" << std::endl;
        m_terrain->setNoiseAmplitude(event.value);
        break;
    case Core::EventType::CHANGE_PERSISTENCE:
        std::cout << "Persitence" << std::endl;
        m_terrain->setNoisePersistence(event.value);
        break;
    case Core::EventType::CHANGE_LACUNARITY:
        std::cout << "Lacunarity" << std::endl;
        m_terrain->setNoiseLacunarity(event.value);
        break;
    case Core::EventType::CHANGE_FREQUENCY:
        std::cout << "Frequency" << std::endl;
        m_terrain->setNoiseFrequency(event.value);
        break;
    case Core::EventType::CHANGE_OCTAVES:
        std::cout << "Octaves" << std::endl;
        m_terrain->setNoiseOctaves(event.value);
        break;
    case Core::EventType::CHANGE_SEED:
        std::cout << "Seed" << std::endl;
        m_terrain->regenerateTerrain((unsigned int)event.value);
        break;
    case Core::EventType::REGENERATE:
        std::cout << "Regenerate" << std::endl;

        break;
    case Core::EventType::SETTINGS:
        std::cout << "Settings" << std::endl;
        break;
    }
    m_terrain->regenerateTerrain(123124);
}

void GUILayer::update(double dt) {}
void GUILayer::stop() {}