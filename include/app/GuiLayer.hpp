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
 * @struct Slidertype
 * @brief Struct to track the name of the sliders
 */
enum class SliderType {
    NoiseScale,
    Persistence,
    Lacunarity,
    Frequency,
    Octaves,
    Seed
};
/**
 * @struct MenuState
 * @brief Struct to track the state of the Menu display
 */
enum class MenuState
{
    None,        // no menu displayed
    MainMenu,    // Menu displayed
    Settings,    // settings Window
    KeyBindings  // Keybinds Window
};

/**
 * @struct Slider
 * @brief Struct to track easely all the sliders
 */
struct Slider
{
    std::string name;
    SliderType type;
    bool Editmode = false;
    float value;
    float minValue;
    float maxValue;
    bool isInteger;
};

/**
 * @class GUI layer
 * @brief The layer responsible for all the GUI informatations
 */
class GUILayer : public Core::Layer
{
public:
    GUILayer();
    virtual ~GUILayer();

    virtual void update(double dt) override;
    virtual void render() override;

    /**
     * @brief Handles GUI-related events and updates the terrain parameters accordingly.
     * @param event Reference to the event to be processed.
     */
    virtual void onEvent(Core::Event &event) override;

    /**
     * @brief Destruction of OpenGL elements (VAO & VBO) and window
     */
    virtual void stop() override;

    /**
     * @brief Called each time specific physics need to be applied
     * @param dt The timestep between two rendered frames
     */
    virtual void fixedUpdate(double dt) override;

    /**
     * @brief Called once initialized to link the app context (shared data) between all different layers
     * @param context The context of the app
     */
    virtual void setContext(std::shared_ptr<Context> context) override { m_terrain = context->terrain; }

    /**
     * @brief apply all the changes of the sliders then dispach to the scene layer
     * @param name The name of the change to apply
     * @param value The value of the current change
     */
    void applySliderChanges(SliderType type, float value);

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
    std::vector<Slider> sliders;

    /**
     * @brief quick on/off to display the Menu
     */
    bool setMenuState(){
        if(m_menuState!=MenuState::MainMenu){
             m_menuState = MenuState::MainMenu;
             return true;
        }else{
            m_menuState = MenuState::None;
            return false;
        }
    }

private:
    std::shared_ptr<Core::Terrain> m_terrain;                      // Shared terrain from the app context
    std::unordered_map<SliderType, float> m_pendingSliderChanges; // Unordered map for storing changed values between two updates of GUI
    bool m_hasPendingChanges = false;                              // Flag indicating if slider values changed
    bool m_textBoxEditMode;
    bool m_showGUI;

    /**
     * @brief This function renders all the sliders and the GUI elements it includes the backpanel, sliders, buttons, text input
     * Note : The updates occur separately from the rendering
     */
    void m_renderGUI();


    /**
     * @brief Renders the main menu.
     */
    void m_renderMainMenu();
    /**
     * @brief Renders the settings window.
     */
    void m_renderSettingsWindow();
    /**
     * @brief Renders the key bindings window.
     */
    void m_renderKeyBindingsWindow();

    MenuState m_menuState = MenuState::None;
};