/*
  TIC TAC TOE
  Author: smpl
  Date: 02-25-2025
*/

#include <stdio.h>
#include <stdlib.h>

#include "../include/raylib.h"
// #include "raylib.h"

//-----------------------------------------------------------------------
// Game Constants
//-----------------------------------------------------------------------
#define FPS 60
#define SCREEN_WIDTH 1000
#define SCREEN_HEIGHT 1000

int main() {

  InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "TIC TAC TOE");
  SetTargetFPS(FPS);

  // Main game loop
  while (!WindowShouldClose()) // Detect window close button or ESC key
  {
    // Draw
    //----------------------------------------------------------------------------------
    BeginDrawing();
    ClearBackground(MAROON);

    DrawText("hello here", 200, 200, 22, RAYWHITE);
    EndDrawing();
    //----------------------------------------------------------------------------------
  }

  CloseWindow(); // Close window and OpenGL context
  //--------------------------------------------------------------------------------------

  return EXIT_SUCCESS;
}
