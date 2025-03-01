// PositionalCache_CMake.cpp : Defines the entry point for the application.
//

#include "raylib.h"
#include <iostream>
#include "PositionalCache_CMake.h"

using namespace std;

int main(int argc, char** argv)
{

    // Initialize the window
    const int screenWidth = 800;
    const int screenHeight = 450;
    InitWindow(screenWidth, screenHeight, "Raylib Test - PositionalCache_CMake");

    // Set FPS
    SetTargetFPS(60);

    // Main game loop
    while (!WindowShouldClose())  // Detect if the window should close
    {
        // Start drawing
        BeginDrawing();
        ClearBackground(RAYWHITE);  // Clear screen with white background

        // Draw text
        DrawText("Hello, Raylib!", screenWidth / 2 - 100, screenHeight / 2, 20, DARKGRAY);

        EndDrawing();  // Stop drawing
    }

    // Cleanup
    CloseWindow();

    return 0;
}