#include "core/testPerlinNoise.hpp"
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

// --- Fonction principale ---
void testPerlinNoiseRaylib()
{
    PerlinNoise_ perlinObj;

    const int screenWidth = 1200;
    const int screenHeight = 800;
    InitWindow(screenWidth, screenHeight, "Perlin Noise 3D Terrain + Layers View");
    SetTargetFPS(60);

    // --- Paramètres Perlin ---
    siv::PerlinNoise perlin(12345);
    const int terrainSize = 100;
    const float spacing = 1.0f;
    float noiseScale = 0.1f;
    int octaves = 6;
    float persistence = 0.5f;
    float lacunarity = 2.0f;

    // --- Terrain data ---
    float heights[terrainSize][terrainSize];
    std::vector<std::vector<std::vector<float>>> layerHeights(
        octaves, std::vector<std::vector<float>>(terrainSize, std::vector<float>(terrainSize)));

    float minHeight = 1000.0f, maxHeight = -1000.0f;

    // --- Génération du terrain et stockage des couches ---
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

    // --- Génération des heatmaps 2D ---
    std::vector<Texture2D> layerTextures;
    for (int i = 0; i < octaves; i++) {
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

        if (autoRotate) {
            rotationAngle += deltaTime * 30.0f;
            camera.position.x = cosf(rotationAngle * DEG2RAD) * 80.0f;
            camera.position.z = sinf(rotationAngle * DEG2RAD) * 80.0f;
        }

        if (IsKeyPressed(KEY_SPACE)) wireframeMode = !wireframeMode;
        if (IsKeyPressed(KEY_A)) autoRotate = !autoRotate;
        if (IsKeyPressed(KEY_L)) showLayers = !showLayers;
        if (showLayers) {
            if (IsKeyPressed(KEY_RIGHT)) currentLayer++;
            if (IsKeyPressed(KEY_LEFT)) currentLayer--;
            if (currentLayer < 0) currentLayer = 0;
            if (currentLayer >= octaves) currentLayer = octaves - 1;
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

        // --- UI / Overlay ---
        DrawRectangle(5, 5, 400, 240, (Color){0, 0, 0, 128});
        DrawText("PERLIN NOISE 3D TERRAIN TEST", 10, 10, 20, WHITE);
        DrawText("SPACE: Toggle wireframe", 10, 60, 15, WHITE);
        DrawText("R: Generate new terrain", 10, 80, 15, WHITE);
        DrawText("A: Toggle auto-rotation", 10, 100, 15, WHITE);
        DrawText("L: Toggle layer heatmaps", 10, 120, 15, WHITE);
        DrawText("Arrow keys: Move camera / Change layer", 10, 140, 15, WHITE);
        DrawText(TextFormat("Wireframe: %s", wireframeMode ? "ON" : "OFF"), 10, 200, 15, wireframeMode ? YELLOW : WHITE);
        DrawText(TextFormat("Auto-rotate: %s", autoRotate ? "ON" : "OFF"), 10, 220, 15, autoRotate ? YELLOW : WHITE);

        // --- Affichage des heatmaps ---
        if (showLayers) {
            int thumbSize = 100;
            int padding = 10;
            for (int i = 0; i < octaves; i++) {
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
