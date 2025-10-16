#include "core/testPerlinNoise.hpp"
#include "app/GUI_layer.hpp"
#include "raylib.h"
#include <vector>
#include <cmath>
#include <iostream>

// --- Fonction utilitaire pour générer une heatmap 2D ---
Image GeneratePerlinHeatmap(const std::vector<std::vector<float>>& values, int width, int height)
{
    Image img = GenImageColor(width, height, BLACK);

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            float val = (values[y][x] + 1.0f) / 2.0f; // Normaliser entre 0 et 1
            val = fminf(fmaxf(val, 0.0f), 1.0f);      // Clamp
            unsigned char r = (unsigned char)(val * 255);
            unsigned char g = (unsigned char)(val * 255);
            unsigned char b = (unsigned char)((1.0f - val) * 255);
            ImageDrawPixel(&img, x, y, (Color){r, g, b, 255});
        }
    }

    return img;
}

// --- Fonction pour régénérer le terrain avec les paramètres du GUI ---
void RegenerateTerrain(PerlinNoise_& perlinObj, 
                       siv::PerlinNoise& perlin,
                       float heights[][100], 
                       std::vector<std::vector<std::vector<float>>>& layerHeights,
                       float& minHeight, float& maxHeight,
                       GUIlayer& gui,
                       int terrainSize)
{
    minHeight = 1000.0f;
    maxHeight = -1000.0f;

    // Récupérer les valeurs des sliders
    float noiseScale = gui.sliders[0].value / 10.0f; // Ajuster l'échelle
    float persistence = gui.sliders[1].value;
    float lacunarity = gui.sliders[2].value;
    int octaves = (int)gui.sliders[3].value;

    // Redimensionner layerHeights si le nombre d'octaves a changé
    if (layerHeights.size() != octaves) {
        layerHeights.resize(octaves, std::vector<std::vector<float>>(terrainSize, std::vector<float>(terrainSize)));
    }

    // Générer le nouveau terrain
    for (int z = 0; z < terrainSize; z++) {
        for (int x = 0; x < terrainSize; x++) {
            float height = perlinObj.perlinFractal(perlin, x * noiseScale, z * noiseScale, 0.5f,
                                                   octaves, persistence, lacunarity);
            heights[z][x] = (height + 1.0f) * 10.0f;

            const auto& layers = perlinObj.getPerlinLayers();
            for (int i = 0; i < octaves; i++) {
                layerHeights[i][z][x] = layers[i];
            }

            if (heights[z][x] < minHeight) minHeight = heights[z][x];
            if (heights[z][x] > maxHeight) maxHeight = heights[z][x];
        }
    }
}

// --- Fonction principale ---
void testPerlinNoiseRaylib()
{
    PerlinNoise_ perlinObj;
    GUIlayer gui;

    const int screenWidth = 1200;
    const int screenHeight = 800;
    InitWindow(screenWidth, screenHeight, "Perlin Noise 3D Terrain + Layers View");
    SetTargetFPS(60);

    // --- Paramètres Perlin ---
    siv::PerlinNoise perlin(12345);
    const int terrainSize = 100;
    const float spacing = 1.0f;

    // --- Terrain data ---
    float heights[terrainSize][terrainSize];
    std::vector<std::vector<std::vector<float>>> layerHeights;
    float minHeight = 1000.0f, maxHeight = -1000.0f;

    // Génération initiale du terrain
    RegenerateTerrain(perlinObj, perlin, heights, layerHeights, minHeight, maxHeight, gui, terrainSize);

    // --- Génération des heatmaps 2D initiales ---
    std::vector<Texture2D> layerTextures;
    int currentOctaves = (int)gui.sliders[3].value;
    for (int i = 0; i < currentOctaves; i++) {
        Image img = GeneratePerlinHeatmap(layerHeights[i], terrainSize, terrainSize);
        layerTextures.push_back(LoadTextureFromImage(img));
        UnloadImage(img);
    }

    // --- Caméra 3D ---
    Camera3D camera = { 0 };
    camera.position = (Vector3){ 50.0f, 80.0f, 50.0f };
    camera.target = (Vector3){ 50.0f, 0.0f, 50.0f };
    camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };
    camera.fovy = 45.0f;
    camera.projection = CAMERA_PERSPECTIVE;

    bool wireframeMode = false;
    bool autoRotate = true;
    bool showLayers = false;
    float rotationAngle = 0.0f;
    int currentLayer = 0;

    // Variables pour détecter les changements
    float prevNoiseScale = gui.sliders[0].value;
    float prevPersistence = gui.sliders[1].value;
    float prevLacunarity = gui.sliders[2].value;
    float prevOctaves = gui.sliders[3].value;
    bool needsRegeneration = false;

    // --- Couleurs du terrain ---
    Color waterColor = { 30, 144, 255, 255 };
    Color sandColor = { 238, 214, 175, 255 };
    Color grassColor = { 34, 139, 34, 255 };
    Color forestColor = { 0, 100, 0, 255 };
    Color mountainColor = { 139, 137, 137, 255 };
    Color snowColor = { 255, 250, 250, 255 };

    // --- Boucle principale ---
    while (!WindowShouldClose()) {
        float deltaTime = GetFrameTime();

        // Vérifier les changements dans les sliders
        if (prevNoiseScale != gui.sliders[0].value ||
            prevPersistence != gui.sliders[1].value ||
            prevLacunarity != gui.sliders[2].value ||
            prevOctaves != gui.sliders[3].value) {
            
            needsRegeneration = true;
            
            // Mettre à jour les valeurs précédentes
            prevNoiseScale = gui.sliders[0].value;
            prevPersistence = gui.sliders[1].value;
            prevLacunarity = gui.sliders[2].value;
            prevOctaves = gui.sliders[3].value;
        }

        // Régénérer le terrain si nécessaire
        if (needsRegeneration) {
            // Libérer les anciennes textures
            for (auto& tex : layerTextures) {
                UnloadTexture(tex);
            }
            layerTextures.clear();

            // Régénérer le terrain
            RegenerateTerrain(perlinObj, perlin, heights, layerHeights, minHeight, maxHeight, gui, terrainSize);

            // Régénérer les heatmaps
            currentOctaves = (int)gui.sliders[3].value;
            for (int i = 0; i < currentOctaves; i++) {
                Image img = GeneratePerlinHeatmap(layerHeights[i], terrainSize, terrainSize);
                layerTextures.push_back(LoadTextureFromImage(img));
                UnloadImage(img);
            }

            needsRegeneration = false;
        }

        if (autoRotate) {
            rotationAngle += deltaTime * 30.0f;
            camera.position.x = cosf(rotationAngle * DEG2RAD) * 80.0f;
            camera.position.z = sinf(rotationAngle * DEG2RAD) * 80.0f;
        }

        if (IsKeyPressed(KEY_SPACE)) wireframeMode = !wireframeMode;
        if (IsKeyPressed(KEY_A)) autoRotate = !autoRotate;
        if (IsKeyPressed(KEY_L)) showLayers = !showLayers;
        if (IsKeyPressed(KEY_G)) gui.toggleGUI(); // Toggle GUI avec G
        
        if (showLayers) {
            if (IsKeyPressed(KEY_RIGHT)) currentLayer++;
            if (IsKeyPressed(KEY_LEFT)) currentLayer--;
            if (currentLayer < 0) currentLayer = 0;
            if (currentLayer >= currentOctaves) currentLayer = currentOctaves - 1;
        }

        // --- Rendu ---
        BeginDrawing();
        ClearBackground((Color){135, 206, 235, 255});

        BeginMode3D(camera);
        DrawPlane((Vector3){50, -2, 50}, (Vector2){100, 100}, (Color){144, 238, 144, 255});

        for (int z = 0; z < terrainSize - 1; z++) {
            for (int x = 0; x < terrainSize - 1; x++) {
                Vector3 v1 = { x * spacing, heights[z][x], z * spacing };
                Vector3 v2 = { (x + 1) * spacing, heights[z][x + 1], z * spacing };
                Vector3 v3 = { x * spacing, heights[z + 1][x], (z + 1) * spacing };
                Vector3 v4 = { (x + 1) * spacing, heights[z + 1][x + 1], (z + 1) * spacing };

                float normalizedHeight = (heights[z][x] - minHeight) / (maxHeight - minHeight);
                Color color;
                if (normalizedHeight < 0.2f) color = waterColor;
                else if (normalizedHeight < 0.3f) color = sandColor;
                else if (normalizedHeight < 0.6f) color = grassColor;
                else if (normalizedHeight < 0.8f) color = forestColor;
                else if (normalizedHeight < 0.9f) color = mountainColor;
                else color = snowColor;

                if (wireframeMode) {
                    DrawLine3D(v1, v2, color);
                    DrawLine3D(v1, v3, color);
                    DrawLine3D(v2, v4, color);
                    DrawLine3D(v3, v4, color);
                } else {
                    DrawTriangle3D(v1, v3, v2, color);
                    DrawTriangle3D(v2, v3, v4, color);
                }
            }
        }

        EndMode3D();

        // --- Afficher le GUI ---
        if (gui.getShowGUI()) {
            gui.renderGui();
        }

        // --- UI / Overlay ---
        DrawRectangle(5, 5, 400, 240, (Color){0, 0, 0, 128});
        DrawText("PERLIN NOISE 3D TERRAIN TEST", 10, 10, 20, WHITE);
        DrawText("SPACE: Toggle wireframe", 10, 60, 15, WHITE);
        DrawText("R: Generate new terrain", 10, 80, 15, WHITE);
        DrawText("A: Toggle auto-rotation", 10, 100, 15, WHITE);
        DrawText("L: Toggle layer heatmaps", 10, 120, 15, WHITE);
        DrawText("G: Toggle GUI", 10, 140, 15, WHITE);
        DrawText("Arrow keys: Move camera / Change layer", 10, 160, 15, WHITE);
        DrawText(TextFormat("Wireframe: %s", wireframeMode ? "ON" : "OFF"), 10, 200, 15, wireframeMode ? YELLOW : WHITE);
        DrawText(TextFormat("Auto-rotate: %s", autoRotate ? "ON" : "OFF"), 10, 220, 15, autoRotate ? YELLOW : WHITE);

        // --- Affichage des heatmaps ---
        if (showLayers) {
            int thumbSize = 100;
            int padding = 10;
            for (int i = 0; i < currentOctaves; i++) {
                int x = screenWidth - thumbSize - padding;
                int y = padding + i * (thumbSize + padding);
                DrawTexturePro(layerTextures[i],
                               (Rectangle){0, 0, (float)terrainSize, (float)terrainSize},
                               (Rectangle){(float)x, (float)y, (float)thumbSize, (float)thumbSize},
                               (Vector2){0, 0}, 0.0f, WHITE);
                DrawText(TextFormat("Layer %d", i + 1), x - 70, y + thumbSize / 2 - 8, 15, WHITE);
                if (i == currentLayer)
                    DrawRectangleLines(x - 2, y - 2, thumbSize + 4, thumbSize + 4, YELLOW);
            }
        }

        EndDrawing();
    }

    // --- Nettoyage ---
    for (auto& tex : layerTextures)
        UnloadTexture(tex);

    CloseWindow();
}