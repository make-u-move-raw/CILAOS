
#include "app/GuiLayer.hpp"

GUILayer::~GUILayer() {}
GUILayer::GUILayer()
{
    // sliders for noise scale, persistence, lacunarity, octaves
    sliders = {
        {"Noise Scale", SliderType::NoiseScale, false, 0.0001f, 0.002f, 0.01f, false},
        {"Persistence", SliderType::Persistence, false, 0.5f, 0.2f, 0.8f, false},
        {"Lacunarity", SliderType::Lacunarity, false, 2.0f, 1.5f, 3.0f, false},
        {"Frequency", SliderType::Frequency, false, 1.0f, 0.1f, 10.0f, false},
        {"Octaves", SliderType::Octaves, false, 5.0f, 1.0f, 8.0f, true}};
    m_showGUI = true;
    m_textBoxEditMode = false;
}

void GUILayer::applySliderChanges(SliderType type, float value)
{
    auto &app = Application::getInstance();

    if (!m_terrain)
    {
        std::cerr << "Error: m_terrain is null in GUILayer::applySliderChanges" << std::endl;
        return;
    }
    switch (type)
    {
    case SliderType::NoiseScale:
        std::cout << "Updating Noise Scale to " << value << std::endl;
        m_terrain->setNoiseAmplitude(value);
        break;

    case SliderType::Persistence:
        std::cout << "Updating Persistence to " << value << std::endl;
        m_terrain->setNoisePersistence(value);
        break;

    case SliderType::Lacunarity:
        std::cout << "Updating Lacunarity to " << value << std::endl;
        m_terrain->setNoiseLacunarity(value);
        break;

    case SliderType::Frequency:
        std::cout << "Updating Frequency to " << value << std::endl;
        m_terrain->setNoiseFrequency(value);
        break;

    case SliderType::Octaves:
        std::cout << "Updating Octaves to " << value << std::endl;
        m_terrain->setNoiseOctaves(value);
        break;

    case SliderType::Seed:
        std::cout << "Regenerating terrain with seed " << static_cast<unsigned int>(value) << std::endl;
        m_terrain->setSeed(static_cast<unsigned int>(value));
        break;
    }

    // Regénère le terrain
    Core::Event event(Core::EventType::REGENERATE, "GuiLayer", "SceneLayer");
    app.dispatchEvents(event);
}

float getSeedFromString(const char *seedStr)
{
    std::string str(seedStr);

    // Try to convert the seed to the value
    float value = std::atof(seedStr);
    // if it's a word atof returns 0.0
    if (value != 0.0f)
    {
        return value;
    }

    float asciiSeed = 0.0f;
    int multiplier = 1;
    // we take the word then convert it to ascii to have a float
    for (char c : str)
    {
        asciiSeed += static_cast<int>(c) * multiplier;
        multiplier *= 256;
    }

    return asciiSeed;
}

void GUILayer::m_renderGUI()
{
    int x = 90;
    int y = 50;
    int width = 200;
    int height = 20;

    // Backpanel
    DrawRectangle(10, 20, 315, 315, Fade(WHITE, 0.8f));
    Rectangle bounds = {(float)x, (float)y, (float)width, (float)height};

    for (auto &slider : sliders)
    {
        if (GuiSlider(bounds, slider.name.c_str(), NULL, &slider.value, slider.minValue, slider.maxValue))
        {
            m_pendingSliderChanges[slider.type] = slider.value;
            m_hasPendingChanges = true;
        }
        if (slider.isInteger)
        {
            slider.value = roundf(slider.value);
            DrawText(TextFormat("%.0f", slider.value), x + width + 5, y + 5, 12, BLACK);
        }
        else
            DrawText(TextFormat("%.3f", slider.value), x + width + 5, y + 5, 12, BLACK);

        // padding between each slider
        y += 40;
        bounds = {(float)x, (float)y, (float)width, (float)height};
    }

    // Prevent text input if not selected
    if (GuiTextBox(bounds, this->seed, 16, m_textBoxEditMode))
        this->m_textBoxEditMode = !this->m_textBoxEditMode;

    y += 40;
    bounds = {(float)x, (float)y, (float)width, (float)height};

    // Draw Regenerate button
    if (GuiButton(bounds, "Regenerate"))
        applySliderChanges(SliderType::Seed, getSeedFromString(this->seed));
}

void GUILayer::render()
{
    if (m_showGUI)
        m_renderGUI();
}

void GUILayer::onEvent(Core::Event &event)
{
}

void GUILayer::update(double dt) {}
void GUILayer::fixedUpdate(double dt)
{
    for (const auto &[type, value] : m_pendingSliderChanges)
    {
        applySliderChanges(type, value);
    }

    m_pendingSliderChanges.clear();
    m_hasPendingChanges = false;
}
void GUILayer::stop() {}