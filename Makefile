default:
	gcc -o game.exe src/main.c -I ./include -L lib -lraylib -lgdi32 -lwinmm