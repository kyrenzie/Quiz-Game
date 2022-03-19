/////////////////////////////////////////////////////////////////////////////////////////
// Comp 369
// Assignment 1
/////////////////////////////////////////////////////////////////////////////////////////

#include <allegro.h>

BITMAP *buffer;
BITMAP *title;

//Displays game name
void title_screen()
{
	title = load_bitmap("title_screen.bmp", NULL);
	masked_blit(title, buffer, 0, 0, 0, 0, title->w, title->h);
	destroy_bitmap(title);
	textprintf_ex(buffer, font, 0, 0, 1, -1, "%dx%d", SCREEN_W, SCREEN_H);
	blit(buffer, screen, 0, 0, 0, 0, 800, 600);
}

void info_screen()
{}

//Displays main menu
void main_menu()
{
	clear_bitmap(buffer);
	textout_centre_ex(buffer, font, "Main Menu", 400, 300, 10, -1);
}

int main(void)
{
	//Initialize settings
	allegro_init();
	set_gfx_mode(GFX_AUTODETECT_WINDOWED, 800, 600, 0, 0);
	set_color_depth(16);
	install_mouse();
	install_keyboard();
	install_timer();


	//Create screen buffer
	buffer = create_bitmap(800, 600);

	title_screen();

	//Pause for title
	rest(3500);

	main_menu();
	
	show_mouse(buffer);

	//main loop
	while (!key[KEY_ESC])
	{
		blit(buffer, screen, 0, 0, 0, 0, 800, 600);
	}
	//exit program
	allegro_exit();
	return 0;
}
END_OF_MAIN()