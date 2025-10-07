#include <iostream>
#include "my_lib/ex_func.h"
#include "raylib/raylib.h"
#include "my_lib/rectangle.h"
/**
 * @mainpage A 3D project using raylib
 *
 * # Welcome to Cilaos documentation
 *
 * Please refer to README of the repo for more informations.
 *
 *
 * ## Example
 * @code
 * Rectangle r(3.0, 4.0);
 * std::cout << r.surface() << std::endl;
 * @endcode
 */

int main(void)
{
    int a;
    MyLib::Rectangle rect = MyLib::Rectangle(5., 2.);

    double perim = rect.perim();
    a = sum_until(5);

    std::cout << "Hello world !" << std::endl;
    std::cout << "The from 0 to a" << " is " << a << std::endl;
    std::cout << "The perimeter of the rectangle is " << perim << std::endl;

    InitWindow(800, 450, "raylib [core] example - basic window");

    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawText("Congrats! You created your first window!", 190, 200, 20, BLACK);
        EndDrawing();
    }

    CloseWindow();

    return 0;
}