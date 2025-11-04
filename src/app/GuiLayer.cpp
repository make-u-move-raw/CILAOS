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

void GUILayer::m_renderMainMenu()
{
    int screenWidth = GetScreenWidth();
    int screenHeight = GetScreenHeight();
    
    DrawRectangle(0, 0, screenWidth, screenHeight, Fade(BLACK, 0.7f));
    
    // Menu panel
    int menuWidth = 400;
    int menuHeight = 500;
    int menuX = (screenWidth - menuWidth) / 2;
    int menuY = (screenHeight - menuHeight) / 2;
    
    DrawRectangle(menuX, menuY, menuWidth, menuHeight, Fade(DARKGRAY, 0.95f));
    DrawRectangleLines(menuX, menuY, menuWidth, menuHeight, LIGHTGRAY);
    
    const char *title = "MENU";
    int titleWidth = MeasureText(title, 30);
    DrawText(title, menuX + (menuWidth - titleWidth) / 2, menuY + 30, 30, WHITE);
    
    // Boutons
    int buttonWidth = 300;
    int buttonHeight = 50;
    int buttonX = menuX + (menuWidth - buttonWidth) / 2;
    int buttonY = menuY + 100;
    int buttonSpacing = 70;
    
    Rectangle bounds;
    
    // resume
    bounds = {(float)buttonX, (float)buttonY, (float)buttonWidth, (float)buttonHeight};
    if (GuiButton(bounds, "Resume"))
        m_menuState = MenuState::None;
    
    // settings
    buttonY += buttonSpacing;
    bounds = {(float)buttonX, (float)buttonY, (float)buttonWidth, (float)buttonHeight};
    if (GuiButton(bounds, "Settings"))
        m_menuState = MenuState::Settings;
    
    // keybinds
    buttonY += buttonSpacing;
    bounds = {(float)buttonX, (float)buttonY, (float)buttonWidth, (float)buttonHeight};
    if (GuiButton(bounds, "Keybinds"))
        m_menuState = MenuState::KeyBindings;
    
    //  exit
    buttonY += buttonSpacing;
    bounds = {(float)buttonX, (float)buttonY, (float)buttonWidth, (float)buttonHeight};
    GuiSetStyle(BUTTON, BASE_COLOR_NORMAL, ColorToInt(RED));
    if (GuiButton(bounds, "Exit"))
    {
        // closes the window
        auto &app = Application::getInstance();
        app.stop();
    }
    GuiSetStyle(BUTTON, BASE_COLOR_NORMAL, ColorToInt(LIGHTGRAY));
    
    // Instructions
    const char *instruction = "Press ESC to close";
    int instrWidth = MeasureText(instruction, 15);
    DrawText(instruction, menuX + (menuWidth - instrWidth) / 2, menuY + menuHeight - 40, 15, LIGHTGRAY);
}

void GUILayer::m_renderSettingsWindow()
{
    int screenWidth = GetScreenWidth();
    int screenHeight = GetScreenHeight();
    
    DrawRectangle(0, 0, screenWidth, screenHeight, Fade(BLACK, 0.7f));
    
    int windowWidth = 500;
    int windowHeight = 600;
    int windowX = (screenWidth - windowWidth) / 2;
    int windowY = (screenHeight - windowHeight) / 2;
    
    DrawRectangle(windowX, windowY, windowWidth, windowHeight, Fade(DARKGRAY, 0.95f));
    DrawRectangleLines(windowX, windowY, windowWidth, windowHeight, LIGHTGRAY);
    
    const char *title = "SETTINGS";
    int titleWidth = MeasureText(title, 30);
    DrawText(title, windowX + (windowWidth - titleWidth) / 2, windowY + 20, 30, WHITE);
    
    int contentX = windowX + 50;
    int contentY = windowY + 80;
    int itemHeight = 50;

    contentY += itemHeight;
    DrawText("Display gui", contentX, contentY + 10, 20, WHITE);
    Rectangle DisplayGUI = {(float)(contentX + 200), (float)contentY, 150, 40};
    GuiCheckBox(DisplayGUI, "ON/OFF", &m_showGUI);
    
    contentY = windowY + windowHeight - 80;
    Rectangle backBounds = {(float)(windowX + 150), (float)contentY, 200, 50};
    if (GuiButton(backBounds, "<- Back"))
        m_menuState = MenuState::MainMenu;
}

void GUILayer::m_renderKeyBindingsWindow()
{
    int screenWidth = GetScreenWidth();
    int screenHeight = GetScreenHeight();
    
    DrawRectangle(0, 0, screenWidth, screenHeight, Fade(BLACK, 0.7f));
    
    int windowWidth = 500;
    int windowHeight = 600;
    int windowX = (screenWidth - windowWidth) / 2;
    int windowY = (screenHeight - windowHeight) / 2;
    
    DrawRectangle(windowX, windowY, windowWidth, windowHeight, Fade(DARKGRAY, 0.95f));
    DrawRectangleLines(windowX, windowY, windowWidth, windowHeight, LIGHTGRAY);
    
    const char *title = "Keybinds";
    int titleWidth = MeasureText(title, 30);
    DrawText(title, windowX + (windowWidth - titleWidth) / 2, windowY + 20, 30, WHITE);
    
    int contentX = windowX + 50;
    int contentY = windowY + 80;
    int lineHeight = 35;
    
    // Keybind list 
    struct KeyBinding {
        const char *action;
        const char *key;
    };
    
    KeyBinding bindings[] = {
        {"Move around", "Z Q S D"},
        {"Zoom +/-", "Scroll down / up"},
        {"Size of the terrain", "1,2,3,4,5"},
        {"Auto rotate", "R"},
        {"Generate random Terrain", "T"},
        {"Activate", "Z"},
        {"Menu", "ESC"},
        {"Display/hidde GUI", "not working yet"},
        {"Screenshot", "not working yet"},
        {"Fullscreen", "not working yet"}
    };
    
    for (const auto &binding : bindings)
    {
        DrawText(binding.action, contentX, contentY, 18, WHITE);
        DrawText(binding.key, contentX + 250, contentY, 18, YELLOW);
        contentY += lineHeight;
    }
    

    contentY = windowY + windowHeight - 80;
    Rectangle backBounds = {(float)(windowX + 150), (float)contentY, 200, 50};
    if (GuiButton(backBounds, "<- Back"))
        m_menuState = MenuState::MainMenu;
}

void GUILayer::render()
{
    if (!IsWindowReady()) return;

    if (m_showGUI && m_menuState == MenuState::None)
        m_renderGUI();
    
    switch (m_menuState)
    {
    case MenuState::MainMenu:
        m_renderMainMenu();
        break;
    case MenuState::Settings:
        m_renderSettingsWindow();
        break;
    case MenuState::KeyBindings:
        m_renderKeyBindingsWindow();
        break;
    case MenuState::None:
    default:
        break;
    }
}

void GUILayer::onEvent(Core::Event &event)
{
}

void GUILayer::update(double dt) {

    if (IsKeyPressed(KEY_ESCAPE)) {
        setMenuState();
}
}
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