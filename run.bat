gcc .\main.c .\grid.c -o main.exe -Wall -std=c99 -Wno-missing-braces -I include/ -L lib/ -lraylib -lopengl32 -lgdi32 -lwinmm 
main.exe
