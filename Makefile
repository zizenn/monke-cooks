default:
	gcc -o game.exe src\main.c src\main_menu.c src\campaign_menu.c src\game.c -Iinclude -Llib -lraylib -lgdi32 -lwinmm