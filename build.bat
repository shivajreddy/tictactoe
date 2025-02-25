
@echo off

@echo "Compiling..."
gcc ./src/main.c -o ./build/out.exe -Wall -Wextra -std=c99 -I .\include\ -L .\lib\ -lraylib -lopengl32 -lgdi32 -lwinmm

@if %errorlevel% equ 0 (
    @echo "Compilation successful."
    @echo "Running the program with argument 3..."
    .\build\out.exe 20
) else (
    @echo "Compilation failed."
)
