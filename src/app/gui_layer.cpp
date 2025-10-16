#include "app/GUI_layer.hpp"

GUIlayer::GUIlayer(){
    // sliders for noise scale, persistence, lacunarity, octaves
    sliders = {
        {"Noise Scale", false, 1.0f,0.0f,10.0f},
        {"Persistence", false, 1.0f,0.0f,10.0f},
        {"Lacunarity", false, 1.0f,0.0f,10.0f},
        {"Octaves", false, 1.0f,0.0f,10.0f,true}
    };
    showGUI=true;
    textBoxEditMode=false;
}

/**
 * @brief this alow to toggle on and off the GUI 
 */
void GUIlayer::toggleGUI(){
    this->showGUI = !this->showGUI;
}

bool GUIlayer::getShowGUI(){
    return this->showGUI;
}


/**
 * @brief This function renders all the sliders and the GUI elements
 */
void GUIlayer::renderGui(){

    int x = 90;  
    int y = 50;
    int width = 200;
    int height = 20;

    // Backpanel
    DrawRectangle(10, 20, 315, 280, Fade(WHITE, 0.8f));
    Rectangle bounds= { (float)x, (float)y, (float)width, (float)height };;

    for (auto &slider : sliders) {

        GuiSlider(bounds, slider.name.c_str(), NULL, &slider.value, slider.minValue, slider.maxValue);
        
        // if the slider is a integer
        if (slider.isInteger) 
        {
            // roundup the value
            slider.value = roundf(slider.value);
            DrawText(TextFormat("%.0f", slider.value), x + width + 5, y+5, 12, BLACK);

        }else{
            // if it's a float
            DrawText(TextFormat("%.2f", slider.value), x + width + 5, y+5, 12, BLACK);
        }
        

        // padding between each slider
        y += 40; 
        bounds = { (float)x, (float)y, (float)width, (float)height };
    }
    


    if(GuiTextBox(bounds,this->seed,16,textBoxEditMode))
    {
        this->textBoxEditMode = !this->textBoxEditMode;
    }
    y += 40; 
    bounds = { (float)x, (float)y, (float)width, (float)height };
    
    // Draw Regenerate button
    if (GuiButton(bounds, "Regenerate")) {
            // when clicked 
            std::cout << " ahhhh " ;
        }
}