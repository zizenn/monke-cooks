default:
	gcc -o game.exe main.c main_menu.c campaign_menu.c -I include -L lib -lraylib -lgdi32 -lwinmm