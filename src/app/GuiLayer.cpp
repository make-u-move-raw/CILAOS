
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

void GUILayer::applySliderChanges(std::string name, float value)
{
    auto &app = Application::getInstance();

    if (!m_terrain)
    {
        std::cerr << "Error: m_terrain is null in GUILayer::applySliderChanges" << std::endl;
        return;
    }

    if (name == "Noise Scale")
    {
        std::cout << "Updating Noise Scale to " << value << std::endl;
        m_terrain->setNoiseAmplitude(value);
    }
    else if (name == "Persistence")
    {
        std::cout << "Updating Persistence to " << value << std::endl;
        m_terrain->setNoisePersistence(value);
    }
    else if (name == "Lacunarity")
    {
        std::cout << "Updating Lacunarity to " << value << std::endl;
        m_terrain->setNoiseLacunarity(value);
    }
    else if (name == "Frequency")
    {
        std::cout << "Updating Frequency to " << value << std::endl;
        m_terrain->setNoiseFrequency(value);
    }
    else if (name == "Octaves")
    {
        std::cout << "Updating Octaves to " << value << std::endl;
        m_terrain->setNoiseOctaves(value);
    }
    else if (name == "Seed")
    {
        std::cout << "seed value " <<value<< std::endl;
        std::cout << "Regenerating terrain with seed " << static_cast<unsigned int>(value) << std::endl;
        m_terrain->setSeed(static_cast<unsigned int>(value));
    }

    // call the event to regenerate the terrain
    Core::EventType eventType = Core::EventType::REGENERATE;
    Core::Event event(eventType,"SceneLayer","GuiLayer"); 
    app.dispatchEvents(event);
}

float getSeedFromString(const char* seedStr)
{
    std::string str(seedStr);

    // Try to convert the seed to the value
    float value = std::atof(seedStr);
    // if it's a word atof returns 0.0
    if (value != 0.0f) {
        return value;
    }
    

    float asciiSeed = 0.0f;
    int multiplier = 1;
    // we take the word then convert it to ascii to have a float
    for (char c : str) {
        asciiSeed += static_cast<int>(c) * multiplier;
        multiplier *= 256;
    }

    return asciiSeed;
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
        if (GuiSlider(bounds, slider.name.c_str(), NULL, &slider.value, slider.minValue, slider.maxValue))
        {
            applySliderChanges(slider.name, slider.value);
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
        // when clicked send seed
        applySliderChanges("Seed", getSeedFromString(this->seed));
    }
}

void GUILayer::render()
{
    if (m_showGUI)
    {
        renderGui();
    }
}

/**
 * @brief Handles GUI-related events and updates the terrain parameters accordingly.
 * @param event Reference to the event to be processed.
 */
void GUILayer::onEvent(Core::Event &event)
{
}

void GUILayer::update(double dt) {}
void GUILayer::stop() {}