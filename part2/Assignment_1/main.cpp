/////////////////////////////////////////////////////////////////////////////////////////
// Comp 369
// Assignment 1
/////////////////////////////////////////////////////////////////////////////////////////


#include <fstream>
#include <string>
#include <stdio.h>
#include <iostream>
#include <vector>
#include <allegro.h>
using namespace std;


//create some colors
#define WHITE makecol(255,255,255)
#define BLACK makecol(0,0,0)
#define RED makecol(255,0,0)
#define GREEN makecol(0,128,0)
#define BLUE makecol(0,0,255)
#define SMOKE makecol(140,130,120)

//Boolean variables for state control
int game_start = FALSE;
int choosing_chapter = FALSE;
int chapter_chosen = FALSE;
int quiz_active = FALSE;
int quiz_progress = FALSE;

MIDI *music;
int pos, length;

BITMAP *buffer;
BITMAP *title;
int ret;
int mx, my, mb;
int chapter;
int x = 0;
int score;
string choice;
vector <string> quiz;


//Displays game name
void title_screen()
{
	title = load_bitmap("title_screen.bmp", NULL);
	blit(title, screen, 0, 0, 0, 0, title->w, title->h);
	destroy_bitmap(title);
	textprintf_ex(screen, font, 0, 0, 1, -1, "%dx%d", SCREEN_W, SCREEN_H);

	rest(1500);
}

void info_screen()
{
	clear_bitmap(screen);
	textout_centre_ex(screen, font, "About:", 400, 225, 10, -1);
	textout_centre_ex(screen, font, "This is a multiple choice game that will test your knowledge about the course: COMP 369", 400, 300, 10, -1);
	textout_centre_ex(screen, font, "You can choose to be quizzed on a single chapter of the text or the entire text", 400, 325, 10, -1);
	textout_centre_ex(screen, font, "(Hit ESC at any time to quit)", 400, 350, 10, -1);
	textout_centre_ex(screen, font, "Press any key to continue...", 400, 500, 10, -1);


}

int mouseinside(int x1, int y1, int x2, int y2)
{
	if (mouse_x > x1 && mouse_x < x2 && mouse_y > y1 && mouse_y < y2)
		return 1;
	else
		return 0;
}


//Displays main menu
void main_menu()
{
	mb = (mouse_b & 1);
	scare_mouse();
	clear_bitmap(screen);
	textout_centre_ex(screen, font, "Main Menu:", 400, 125, 10, -1);
	rectfill(screen, 250, 175, 350, 210, GREEN);
	rectfill(screen, 450, 175, 550, 210, GREEN);
	textout_ex(screen, font, "Entire Text", 257, 190, 10, -1);
	textout_ex(screen, font, "One Chapter", 457, 190, 10, -1);
	unscare_mouse();
	game_start = TRUE;

}

void chapter_choice()
{
	
	scare_mouse();
	clear_bitmap(screen);
	rectfill(screen, 50, 25, 150, 60, GREEN);
	textout_ex(screen, font, "Chapter 1", 60, 40, 10, -1);
	rectfill(screen, 250, 25, 350, 60, GREEN);
	textout_ex(screen, font, "Chapter 2", 260, 40, 10, -1);
	rectfill(screen, 450, 25, 550, 60, GREEN);
	textout_ex(screen, font, "Chapter 3", 460, 40, 10, -1);
	rectfill(screen, 650, 25, 750, 60, GREEN);
	textout_ex(screen, font, "Chapter 4", 660, 40, 10, -1);
	rectfill(screen, 50, 125, 150, 160, GREEN);
	textout_ex(screen, font, "Chapter 5", 60, 140, 10, -1);
	rectfill(screen, 250, 125, 350, 160, GREEN);
	textout_ex(screen, font, "Chapter 6", 260, 140, 10, -1);
	rectfill(screen, 450, 125, 550, 160, GREEN);
	textout_ex(screen, font, "Chapter 7", 460, 140, 10, -1);
	rectfill(screen, 650, 125, 750, 160, GREEN);
	textout_ex(screen, font, "Chapter 8", 660, 140, 10, -1);
	rectfill(screen, 50, 225, 150, 260, GREEN);
	textout_ex(screen, font, "Chapter 9", 60, 240, 10, -1);
	rectfill(screen, 250, 225, 350, 260, GREEN);
	textout_ex(screen, font, "Chapter 10", 260, 240, 10, -1);
	rectfill(screen, 450, 225, 550, 260, GREEN);
	textout_ex(screen, font, "Chapter 11", 460, 240, 10, -1);
	rectfill(screen, 650, 225, 750, 260, GREEN);
	textout_ex(screen, font, "Chapter 12", 660, 240, 10, -1);
	rectfill(screen, 50, 325, 150, 360, GREEN);
	textout_ex(screen, font, "Chapter 13", 60, 340, 10, -1);
	rectfill(screen, 250, 325, 350, 360, GREEN);
	textout_ex(screen, font, "Chapter 14", 260, 340, 10, -1);
	rectfill(screen, 450, 325, 550, 360, GREEN);
	textout_ex(screen, font, "Chapter 15", 460, 340, 10, -1);
	rectfill(screen, 650, 325, 750, 360, GREEN);
	textout_ex(screen, font, "Chapter 16", 660, 340, 10, -1);
	rectfill(screen, 50, 425, 150, 460, GREEN);
	textout_ex(screen, font, "Chapter 17", 60, 440, 10, -1);
	rectfill(screen, 250, 425, 350, 460, GREEN);
	textout_ex(screen, font, "Chapter 18", 260, 440, 10, -1);
	rectfill(screen, 450, 425, 550, 460, GREEN);
	textout_ex(screen, font, "Chapter 19", 460, 440, 10, -1);
	rectfill(screen, 50, 525, 150, 560, GREEN);
	textout_ex(screen, font, "Chapter 20", 60, 540, 10, -1);
	rectfill(screen, 250, 525, 350, 560, GREEN);
	textout_ex(screen, font, "Chapter 21", 260, 540, 10, -1);
	rectfill(screen, 450, 525, 550, 560, GREEN);
	textout_ex(screen, font, "Chapter 22", 460, 540, 10, -1);
	unscare_mouse;

	game_start = FALSE;
	choosing_chapter = TRUE;
}

vector<string> text_proccessor(int chapter)
{
	string text;
	vector <string> qVec;
	ifstream in;
	in.open("chapter" + to_string(chapter) + ".txt");

		while (getline(in, text)) 
		{
			qVec.push_back(text);
		}	
	in.close();
	
	return qVec;
}

void display_stats() {
	if (chapter != 0) {
		textprintf_ex(screen, font, 50, 0, 1, -1, "Chapter: %d", chapter);
		textprintf_ex(screen, font, 700, 0, 1, -1, "%d/10", score);
	}
	else {
		textout_ex(screen, font, "Chapter: Entire Text", 50, 0, 1, -1);
		textprintf_ex(screen, font, 600, 0, 1, -1, "%d/220", score);
	}
	textprintf_ex(screen, font, 350, 0, 1, -1, "Question: %d", (x / 6) + 1);
}

void quiz_screen(vector<string> quiz, int index) 
{
	
	scare_mouse();
	clear_bitmap(screen);
	rectfill(screen, 100, 325, 125, 350, GREEN);
	textout_ex(screen, font, "A", 110, 335, 10, -1);
	rectfill(screen, 100, 450, 125, 475, GREEN);
	textout_ex(screen, font, "B", 110, 460, 10, -1);
	rectfill(screen, 450, 325, 475, 350, GREEN);
	textout_ex(screen, font, "C", 460, 335, 10, -1);
	rectfill(screen, 450, 450, 475, 475, GREEN);
	textout_ex(screen, font, "D", 460, 460, 10, -1);
	textout_centre_ex(screen, font, quiz[index].c_str(), 400, 150, 10, -1);
	textout_ex(screen, font, quiz[index + 1].c_str(), 135, 335, 10, -1);
	textout_ex(screen, font, quiz[index + 2].c_str(), 135, 460, 10, -1);
	textout_ex(screen, font, quiz[index + 3].c_str(), 485, 335, 10, -1);
	textout_ex(screen, font, quiz[index + 4].c_str(), 485, 460, 10, -1);
	display_stats();
	unscare_mouse;
	

}

void end_screen() 
{
	clear_bitmap(screen);
	textout_centre_ex(screen, font, "Game Over", 400, 225, 10, -1);
	textout_centre_ex(screen, font, "Your Score:", 400, 300, 10, -1);
	if (chapter != 0) {
		textprintf_ex(screen, font, 382, 325, 1, -1, "%d/10", score);
	}
	else {
		textprintf_ex(screen, font, 380, 325, 1, -1, "%d/220", score);
	}
	textout_centre_ex(screen, font, "Thanks for playing!", 400, 400, 10, -1);
}

void multiple_choice(vector <string> quiz, string choice)
{
	quiz_progress = FALSE;
	if (chapter != 0) {
		if (x < 55) {
			quiz_screen(quiz, x);
		}
		else {
			end_screen();
		}
	}
	else {
		if (x < 1315) {	
			quiz_screen(quiz, x);
		}
	}
	quiz_active = TRUE;
}

void quiz_answer() 
{
	if (choice == quiz[x + 5]) {
		score++;
	}
	quiz_active = FALSE;
	textout_centre_ex(screen, font, "Correct answer was: ", 400, 500, 1, -1);
	textout_centre_ex(screen, font, quiz[x + 5].c_str(), 400, 520, 10, -1);
	rectfill(screen, 350, 565, 450, 600, GREEN);
	textout_ex(screen, font, "Next", 385,580, 10, -1);
	x = x + 6;
	quiz_progress = TRUE;
}


int main(void)
{
	//Initialize settings
	allegro_init();
	ret = set_gfx_mode(GFX_AUTODETECT_WINDOWED, 800, 600, 0, 0);
	if (ret != 0) {
		allegro_message(allegro_error);
		return 1;
	}
	set_color_depth(8);
	install_timer();
	install_mouse();
	install_keyboard();
	install_timer();
	
	

	title_screen();

	show_mouse(screen);

	info_screen();

	//Music
	if (install_sound(DIGI_AUTODETECT, MIDI_AUTODETECT, NULL) != 0) {
		allegro_message("Error initialising sound system\n%s\n", allegro_error);
		return 1;
	}

	/* read in the MIDI file */
	music = load_midi("toto-africa.mid");
	if (!music) {
		allegro_message("Error loading Midi file");
		return 1;
	}

	if (play_midi(music, 0) != 0) {
		allegro_message("Error playing Midi\n%s", allegro_error);
		return 1;
	}

	while (!keypressed());
	main_menu();

	//main loop
	while (!key[KEY_ESC])
	{
		//grab the current mouse values
		mx = mouse_x;
		my = mouse_y;
		mb = (mouse_b & 1);

		if (game_start == TRUE)
		{
			if (mouseinside(250, 175, 350, 210) && mb) {
				game_start = FALSE;
				chapter = 0;
				choosing_chapter = FALSE;
				chapter_chosen = TRUE;
			}
			//chose specific chapter
			else if (mouseinside(450, 175, 550, 210) && mb) {
				chapter_choice();
			}
		}

		if (choosing_chapter == TRUE) {
			if (mouseinside(50, 25, 150, 60) && mb) 
			{
				chapter = 1;
				choosing_chapter = FALSE;
				chapter_chosen = TRUE;
			}
			if (mouseinside(250, 25, 350, 60) && mb)
			{
				chapter = 2;
				choosing_chapter = FALSE;
				chapter_chosen = TRUE;
			}
			if (mouseinside(450, 25, 550, 60) && mb)
			{
				chapter = 3;
				choosing_chapter = FALSE;
				chapter_chosen = TRUE;
			}
			if (mouseinside(650, 25, 750, 60) && mb)
			{
				chapter = 4;
				choosing_chapter = FALSE;
				chapter_chosen = TRUE;
			}
			if (mouseinside(50, 125, 150, 160) && mb)
			{
				chapter = 5;
				choosing_chapter = FALSE;
				chapter_chosen = TRUE;
			}
			if (mouseinside(250, 125, 350, 160) && mb)
			{
				chapter = 6;
				choosing_chapter = FALSE;
				chapter_chosen = TRUE;
			}
			if (mouseinside(450, 125, 550, 160) && mb)
			{
				chapter = 7;
				choosing_chapter = FALSE;
				chapter_chosen = TRUE;
			}
			if (mouseinside(650, 125, 750, 160) && mb)
			{
				chapter = 8;
				choosing_chapter = FALSE;
				chapter_chosen = TRUE;
			}
			if (mouseinside(50, 225, 150, 260) && mb)
			{
				chapter = 9;
				choosing_chapter = FALSE;
				chapter_chosen = TRUE;
			}
			if (mouseinside(250, 225, 350, 260) && mb)
			{
				chapter = 10;
				choosing_chapter = FALSE;
				chapter_chosen = TRUE;
			}
			if (mouseinside(450, 225, 550, 260) && mb)
			{
				chapter = 11;
				choosing_chapter = FALSE;
				chapter_chosen = TRUE;
			}
			if (mouseinside(650, 225, 750, 260) && mb)
			{
				chapter = 12;
				choosing_chapter = FALSE;
				chapter_chosen = TRUE;
			}
			if (mouseinside(50, 325, 150, 360) && mb)
			{
				chapter = 13;
				choosing_chapter = FALSE;
				chapter_chosen = TRUE;
			}
			if (mouseinside(250, 325, 350, 360) && mb)
			{
				chapter = 14;
				choosing_chapter = FALSE;
				chapter_chosen = TRUE;
			}
			if (mouseinside(450, 325, 550, 360) && mb)
			{
				chapter = 15;
				choosing_chapter = FALSE;
				chapter_chosen = TRUE;
			}
			if (mouseinside(650, 325, 750, 360) && mb)
			{
				chapter = 16;
				choosing_chapter = FALSE;
				chapter_chosen = TRUE;
			}
			if (mouseinside(50, 425, 150, 460) && mb)
			{
				chapter = 17;
				choosing_chapter = FALSE;
				chapter_chosen = TRUE;
			}
			if (mouseinside(250, 425, 350, 460) && mb)
			{
				chapter = 18;
				choosing_chapter = FALSE;
				chapter_chosen = TRUE;
			}
			if (mouseinside(450, 425, 550, 460) && mb)
			{
				chapter = 19;
				choosing_chapter = FALSE;
				chapter_chosen = TRUE;
			}
			if (mouseinside(50, 525, 150, 560) && mb)
			{
				chapter = 20;
				choosing_chapter = FALSE;
				chapter_chosen = TRUE;
			}
			if (mouseinside(150, 525, 350, 560) && mb)
			{
				chapter = 21;
				choosing_chapter = FALSE;
				chapter_chosen = TRUE;
			}
			if (mouseinside(450, 525, 550, 560) && mb)
			{
				chapter = 22;
				choosing_chapter = FALSE;
				chapter_chosen = TRUE;
			}
		}
		if (chapter_chosen == TRUE) 
		{
			quiz = text_proccessor(chapter);
			quiz_screen(quiz, x);
			chapter_chosen = FALSE;
			quiz_active = TRUE;
		}
		if (quiz_active == TRUE) {
			// choice A
			if (mouseinside(100, 325, 125, 350) && mb) {
				choice = "a";
				quiz_answer();
				//multiple_choice(quiz, "a");
			}
			// choice B
			if (mouseinside(100, 400, 125, 475) && mb) {
				choice = "b";
				quiz_answer();

			}
			// choice C
			if (mouseinside(450, 325, 475, 350) && mb) {
				choice = "c";
				quiz_answer();
			}
			// choice D
			if (mouseinside(450, 450, 475, 475) && mb) {
				choice = "d";
				quiz_answer();
			}	
		}
		if (quiz_progress == TRUE) {
			if (mouseinside(350, 565, 450, 600) && mb) {
				multiple_choice(quiz, choice);
			}
		}

		
	rest(10);
	}

	stop_midi();
	destroy_midi(music);
	remove_sound();
	allegro_exit();
	return 0;
}
END_OF_MAIN()