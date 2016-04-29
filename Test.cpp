#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")						//no console window behind game window

#include<allegro5\allegro.h>
#include<allegro5\allegro_native_dialog.h>
#include<allegro5\allegro_primitives.h>
#include<allegro5\allegro_image.h>
#include <allegro5\allegro_font.h>
#include <allegro5\allegro_ttf.h>
#include <allegro5\allegro_audio.h>
#include <allegro5\allegro_acodec.h>

#include "Enemy.h"
#include "Spaceship.h"
#include "Bullet.h"
#include "Global.h"
#include "BackGround.h"
#include "Barrier.h"

using namespace std;

//GLOBALS
enum KEYS {LEFT, RIGHT, SPACE};
bool keys[5] = {false,false,false};
int gameState = 1;																		//initial gamestate is menu
extern int width;
extern int height;
int numAlive = NUM_COLUMNS*NUM_ROWS;
int Current_EnemyCount = NUM_COLUMNS*NUM_ROWS;
int EnemyWaveCount = 0;
int b = 0;

//METHODS
void setEnemy();
void collideEnemy(int&);
void collidePlayer();
void moveDown(int&);
void enemyShoot();
void updateBullet();
bool reachEnd();
void InitBackground(BackGround &back, float x, float y, float velx, float vely, int width, int height, int dirX, int dirY, ALLEGRO_BITMAP *image);
void UpdateBackground(BackGround &back);
void DrawBackground(BackGround &back);
void CollideBarrier();
void Reactivate_Enemies();
void EnemyReachEnd();
void BulletBarrierCollide();
void InitialiseBarriers();
void Reactivate_Barriers();
void updateEnemyCount();
//INITIALISE
Spaceship	player(width/2, height*4/5);
Bullet		playerBullet(player.x_pos, player.y_pos,10,true);
Bullet		enemyBullet(0,0,10,false);
Enemy		arrEnem[NUM_COLUMNS][NUM_ROWS];												//array of objects
Barrier Asteroid[3];

BackGround BG;
BackGround MG;
BackGround FG;

// \/ MOVE TO CLASS \/
struct MovingBackground
{
	float x;
	float y;
	float velX;
	float velY;
	int dirX;
	int dirY;
	int width;
	int height;
	ALLEGRO_BITMAP *BG;
};


int main(void)
{
	setEnemy();
	enemyBullet.isFriendly = false;
	srand((unsigned)time(NULL));

	//primitive variables
	const int FPS = 60;
	bool done = false;
	bool redraw = true;

	InitialiseBarriers();

	MovingBackground MBG;

	if(!al_init())
	{
		al_show_native_message_box(NULL,NULL,NULL,"Could not initialize allegro",NULL,NULL);
		return -1;
	}
	
	ALLEGRO_BITMAP *bgImage = NULL;
	ALLEGRO_BITMAP *mgImage = NULL;
	ALLEGRO_BITMAP *fgImage = NULL;

	//Allegro variables
	ALLEGRO_DISPLAY *DISPLAY = NULL;
	ALLEGRO_BITMAP *picHealth[7];
	ALLEGRO_BITMAP *picShip = NULL;
	ALLEGRO_BITMAP *picBullet = NULL;
	ALLEGRO_BITMAP *picEnemy = NULL;
	ALLEGRO_BITMAP *Game = NULL;
	ALLEGRO_BITMAP *MENU = NULL;
	
	
	ALLEGRO_EVENT_QUEUE *TestQueue = NULL;
	ALLEGRO_TIMER *timer = NULL;
	ALLEGRO_FONT *font25 = NULL;
	ALLEGRO_FONT *font50 = NULL;
	ALLEGRO_SAMPLE *blaster = NULL;
	ALLEGRO_SAMPLE *explosion = NULL;
	ALLEGRO_SAMPLE *music = NULL;
	ALLEGRO_SAMPLE *startGame = NULL;
	ALLEGRO_BITMAP *Barrier[5];
	ALLEGRO_BITMAP *AsImage[3];

	//Allegro Module Init
	al_init_image_addon();
	al_set_new_window_position(400, 200);												//set pos of game window
	DISPLAY = al_create_display(width, height);
	al_hide_mouse_cursor(DISPLAY);
	al_init_primitives_addon();
	al_install_keyboard();
	al_init_font_addon();
	al_init_ttf_addon();
	al_install_audio();
	al_init_acodec_addon();

	al_reserve_samples(4);
	blaster = al_load_sample("XWing-Laser.ogg");
	explosion = al_load_sample("Blast.ogg");
	startGame = al_load_sample("xwing.ogg");
	music = al_load_sample("Star_Wars.ogg");

	if (!DISPLAY)
	{
		al_show_native_message_box(NULL, NULL, NULL, "Could not initialize display", NULL, NULL);
		return -1;
	}

	//Load Pictures
	picBullet=al_load_bitmap("Lazer.png");
	picShip =al_load_bitmap("player1.png");
	picEnemy = al_load_bitmap("enemy.png");
    Game = al_load_bitmap("Goodpic.png");
	MENU = al_load_bitmap("star_sky.png");

	Barrier[0] = al_load_bitmap("B4.png");
	Barrier[1] = al_load_bitmap("B3.png");
	Barrier[2] = al_load_bitmap("B2.png");
	Barrier[3] = al_load_bitmap("B1.png");
	Barrier[4] = al_load_bitmap("B0.png");

	for (int i = 0; i < 5; i++)
		al_convert_mask_to_alpha(Barrier[i], al_map_rgb(255, 255, 255));

	AsImage[0] = Barrier[4];
	AsImage[1] = Barrier[4];
	AsImage[2] = Barrier[4];

	picHealth[0] = al_load_bitmap("1.png");
	picHealth[1] = al_load_bitmap("2.png");
	picHealth[2] = al_load_bitmap("3.png");
	picHealth[3] = al_load_bitmap("4.png");
	picHealth[4] = al_load_bitmap("5.png");
	picHealth[5] = al_load_bitmap("6.png");
	picHealth[6] = al_load_bitmap("blank.png");

	bgImage = al_load_bitmap("starBG.png");
	mgImage = al_load_bitmap("starMG.jpg");
	fgImage = al_load_bitmap("starFG.png");

	for (int i = 0; i < 7; i++)
		al_convert_mask_to_alpha(picHealth[i], al_map_rgb(0, 0, 0));

	al_convert_mask_to_alpha(picShip,al_map_rgb(0,0,0));
	al_convert_mask_to_alpha(picEnemy, al_map_rgb(0, 0, 0));
	al_convert_mask_to_alpha(picBullet, al_map_rgb(0, 0, 0));
	al_convert_mask_to_alpha(mgImage, al_map_rgb(0, 0, 0));

	InitBackground(BG, 0, 0, 1, 0, 800, 600, -1, 1, bgImage);
	InitBackground(MG, 0, 0, 3, 0, 2000, 768, -1, 1, mgImage);
	InitBackground(FG, 0, 0, 5, 0, 800, 600, -1, 1, fgImage);

	al_set_display_icon(DISPLAY, picShip);

	timer=al_create_timer(1.0/FPS);

	TestQueue= al_create_event_queue();
	al_register_event_source(TestQueue,al_get_keyboard_event_source());
	al_register_event_source(TestQueue,al_get_display_event_source(DISPLAY));
	al_register_event_source(TestQueue,al_get_timer_event_source(timer));
	
	al_start_timer(timer);
	int score = 0;
	int frameCount = 0;
	font25 = al_load_font("Legacy.ttf", 38, 0);
	font50 = al_load_font("STARWARS.TTF", 55, 0);
	al_play_sample(music, 1, 0, 1, ALLEGRO_PLAYMODE_LOOP, NULL);
	
	while (!done)
	{
		ALLEGRO_EVENT GETKEY;
		al_wait_for_event(TestQueue, &GETKEY);
		

		if (GETKEY.type == ALLEGRO_EVENT_DISPLAY_CLOSE)								//will allow clicking X button to close program
		{
			done = true;
		}
		else if (GETKEY.type == ALLEGRO_EVENT_TIMER)
		{
			redraw = true;
			frameCount++;
			if (keys[LEFT])
				player.MoveSpaceshipLeft();
			if (keys[RIGHT])
				player.MoveSpaceshipRight();
			if (keys[SPACE])														//Spacebar will fire
			{
				if (gameState == 1)
					gameState = 2;
				else if(gameState ==2)
				{
					if (playerBullet.status ==0)
					{
						al_play_sample(blaster, 1, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);
						playerBullet.status = 1;
					}
				}
				
			}

			UpdateBackground(BG);
			UpdateBackground(MG);
			UpdateBackground(FG);
			collideEnemy(score);
			updateEnemyCount();
			CollideBarrier();
			collidePlayer();
			BulletBarrierCollide();
			EnemyReachEnd();
			enemyShoot();
			moveDown(frameCount);
			updateBullet();
		}

		
		else if (GETKEY.type==ALLEGRO_EVENT_KEY_DOWN)
		{
			switch(GETKEY.keyboard.keycode)
			{
			case ALLEGRO_KEY_ESCAPE:													//esc to end the game
				if (gameState == 2)
					gameState = 3;
				else
					done = true;
				break;
			case ALLEGRO_KEY_RIGHT:
				keys[RIGHT] = true;
				break;
			case ALLEGRO_KEY_LEFT:
				keys[LEFT] = true;
				break;
			case ALLEGRO_KEY_SPACE:
				keys[SPACE]=true;
				break;
			}
		}

		else if (GETKEY.type == ALLEGRO_EVENT_KEY_UP)
		{
			switch (GETKEY.keyboard.keycode)
			{
			case ALLEGRO_KEY_RIGHT:
				keys[RIGHT] = false;
				break;
			case ALLEGRO_KEY_LEFT:
				keys[LEFT] = false;
				break;
			case ALLEGRO_KEY_SPACE:
				keys[SPACE] = false;
				break;
			}
		}

		if (redraw && al_is_event_queue_empty(TestQueue)) //rendering
		{
			redraw = false;
			if (gameState == 1)
			{
				al_draw_bitmap(MENU, 0, 0, 0);
				
				al_draw_text(font25, al_map_rgb(255, 40, 78), width / 2, height- 750, ALLEGRO_ALIGN_CENTRE, "AMMST  PRESENTS");
				al_draw_text(font50, al_map_rgb(255, 40, 78), (width / 2), (height) - 690, ALLEGRO_ALIGN_CENTRE, "DARTH   INVADERS");
				al_draw_text(font25, al_map_rgb(255, 40, 78), (width / 2), (height) - 350, ALLEGRO_ALIGN_CENTRE, "PRESS SPACE TO START");
				al_draw_text(font25, al_map_rgb(255, 40, 78), (width / 2), (height -300) , ALLEGRO_ALIGN_CENTRE, "PRESS ESC TO EXIT");
				}

			else if (gameState ==2)
			{
				DrawBackground(BG);
				DrawBackground(MG);
				DrawBackground(FG);

				// Make into method \/
				for (b = 0; b < 3; b++)
				{
					if (Asteroid[b].life_points != -1)
					{
						AsImage[b] = Barrier[Asteroid[b].life_points];
					}

					if (Asteroid[b].life_points == 5)
						AsImage[b] = Barrier[4];
					if (Asteroid[b].life_points == 4)
						AsImage[b] = Barrier[3];
					if (Asteroid[b].life_points == 3)
						AsImage[b] = Barrier[2];
					if (Asteroid[b].life_points == 2)
						AsImage[b] = Barrier[1];
					if (Asteroid[b].life_points == 1)
						AsImage[b] = Barrier[0];
				}


				if (Asteroid[0].active == true)
					al_draw_bitmap(AsImage[0], 50, 500, 0);
				if (Asteroid[1].active == true)
					al_draw_bitmap(AsImage[1], 435, 500, 0);
				if (Asteroid[2].active == true)
					al_draw_bitmap(AsImage[2], 820, 500, 0);



				if (playerBullet.status == 1 && player.active)											//if bullet still active
				{
						playerBullet.Increment();															//bullet will move pos
						al_draw_bitmap(picBullet, playerBullet.x_pos, playerBullet.y_pos, 0);				//redraw at new pos	
						if (playerBullet.y_pos < 20)
						{
							playerBullet.status = 0;
							updateBullet();
						}
				}

				if (enemyBullet.status == 1)
				{
					enemyBullet.Increment();															//bullet will move pos
					al_draw_bitmap(picBullet, enemyBullet.x_pos, enemyBullet.y_pos, 0);
				}

				switch (player.health)
				{
				case 60:
					al_draw_bitmap(picHealth[0], 10, 40, 0);
					break;
				case 50:
					al_draw_bitmap(picHealth[1], 10, 40, 0);
					break;
				case 40:
					al_draw_bitmap(picHealth[2], 10, 40, 0);
					break;
				case 30:
					al_draw_bitmap(picHealth[3], 10, 40, 0);
					break;
				case 20:
					al_draw_bitmap(picHealth[4], 10, 40, 0);
					break;
				case 10:
					al_draw_bitmap(picHealth[5], 10, 40, 0);
					break;
				case 0:
					al_draw_bitmap(picHealth[6], 10, 40, 0);
					break;
				}

				if (player.health > 0)
				{
					al_draw_bitmap(picShip, player.x_pos - 45, player.y_pos, 0);
				}

				bool test = reachEnd();
				for (int i = 0; i < NUM_COLUMNS; i++)
				{
					

					for (int j = 0; j < NUM_ROWS; j++)
					{

						if (arrEnem[i][j].active)
						{
							arrEnem[i][j].Move(test);
							al_draw_bitmap(picEnemy, arrEnem[i][j].x_pos, arrEnem[i][j].y_pos, 0);
						}
					}
									
				}

				al_draw_textf(font25, al_map_rgb(255, 0, 0), 10, 0, 0, "SCORE: %i", score);								
			}

			else if(gameState == 3)
			{
				if (player.health == 0)
				{
					al_play_sample(explosion, 1, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);
					player.health = 1;															// <--- bush method to make it only sound once :p
				}
				al_draw_textf(font50, al_map_rgb(255, 0, 0), width/2 -250, height/2 -200, 0, "FINAL SCORE: %i", score);
			}
			al_flip_display();																	//flip display to show all drawn objects
			al_clear_to_color(al_map_rgb(0, 0, 0));												//background colour
		 }
	}

	//Destroy allegro variables
	al_destroy_sample(blaster);
	al_destroy_sample(explosion);
	al_destroy_sample(startGame);
	al_destroy_sample(music);

	al_destroy_event_queue(TestQueue);
	al_destroy_timer(timer);
	al_destroy_display(DISPLAY);

	al_destroy_font(font25);
	al_destroy_font(font50);

	al_destroy_bitmap(Game);
	al_destroy_bitmap(MENU);
	al_destroy_bitmap(picEnemy);
	al_destroy_bitmap(picShip);
	al_destroy_bitmap(picBullet);
	al_destroy_bitmap(bgImage);
	al_destroy_bitmap(mgImage);
	al_destroy_bitmap(fgImage);
	for (int i = 0; i < 7; i++)
		al_destroy_bitmap(picHealth[i]);
	
	// ADD YOUR BITMAPS ETC TO al_destroy !!

	return 0;
}

void collidePlayer()
{
	if (enemyBullet.status)
	{
		if (enemyBullet.y_pos  < player.y_pos + player.boxheight								//checks if within box
			&& enemyBullet.y_pos > player.y_pos - player.boxheight
			&& enemyBullet.x_pos < player.x_pos + player.boxright
			&& enemyBullet.x_pos > player.x_pos - player.boxleft)
		{
			enemyBullet.status = 0;																//bullet set to not active
			player.health -= 10;
		}
	}

	if (player.health == 0)
	{
		player.active = false;
		gameState = 3;
	}
}

void setEnemy()
{
	//row 1
	arrEnem[0][0].set(width / 2 - sp, 65);			//left
	arrEnem[1][0].set(width / 2, 65);			//middle
	arrEnem[2][0].set(width / 2 + sp, 65);			//right 
	arrEnem[3][0].set(width / 2 - 2 * sp, 65);
	arrEnem[4][0].set(width / 2 + 2 * sp, 65);
	arrEnem[5][0].set(width / 2 - 3 * sp, 65);
	arrEnem[6][0].set(width / 2 + 3 * sp, 65);
	arrEnem[7][0].set(width / 2 - 4 * sp, 65);

	//row 2
	arrEnem[0][1].set(width / 2 - sp, 130);		//left
	arrEnem[1][1].set(width / 2, 130);			//middle
	arrEnem[2][1].set(width / 2 + sp, 130);		//right 
	arrEnem[3][1].set(width / 2 - 2 * sp, 130);
	arrEnem[4][1].set(width / 2 + 2 * sp, 130);
	arrEnem[5][1].set(width / 2 - 3 * sp, 130);
	arrEnem[6][1].set(width / 2 + 3 * sp, 130);
	arrEnem[7][1].set(width / 2 - 4 * sp, 130);

	//row 3
	arrEnem[0][2].set(width / 2 - sp, 195);		//left
	arrEnem[1][2].set(width / 2, 195);			//middle
	arrEnem[2][2].set(width / 2 + sp, 195);		//right 
	arrEnem[3][2].set(width / 2 - 2 * sp, 195);
	arrEnem[4][2].set(width / 2 + 2 * sp, 195);
	arrEnem[5][2].set(width / 2 - 3 * sp, 195);
	arrEnem[6][2].set(width / 2 + 3 * sp, 195);
	arrEnem[7][2].set(width / 2 - 4 * sp, 195);

	//row 4
	arrEnem[0][3].set(width / 2 - sp, 260);
	arrEnem[1][3].set(width / 2, 260);
	arrEnem[2][3].set(width / 2 + sp, 260);
	arrEnem[3][3].set(width / 2 - 2 * sp, 260);
	arrEnem[4][3].set(width / 2 + 2 * sp, 260);
	arrEnem[5][3].set(width / 2 - 3 * sp, 260);
	arrEnem[6][3].set(width / 2 + 3 * sp, 260);
	arrEnem[7][3].set(width / 2 - 4 * sp, 260);

}

void collideEnemy(int &score)
{
	if (playerBullet.status)
	{
		for (int i = 0; i < NUM_COLUMNS; i++)											
		{
			for (int j = 0; j < NUM_ROWS; j++)															//collision detection ahead. Proceed with caution
			{
				if (arrEnem[i][j].active)																//checks if enemy active
				{
					if (playerBullet.y_pos - 25 < arrEnem[i][j].y_pos + arrEnem[i][j].boxheight			//checks if within box of enemy
						&& playerBullet.y_pos > arrEnem[i][j].y_pos - arrEnem[i][j].boxheight
						&& playerBullet.x_pos < arrEnem[i][j].x_pos + arrEnem[i][j].boxright
						&& playerBullet.x_pos > arrEnem[i][j].x_pos - arrEnem[i][j].boxleft)
					{
						playerBullet.status = 0;														//bullet set to not active
						arrEnem[i][j].active = false;													//enemy set to not active
						Current_EnemyCount--;
						switch (j)
						{
						case 0:
							score += 40;
							break;
						case 1:
							score += 20;
							break;
						case 2:
							score += 20;
							break;						
						default:
							score += 10;
							break;
						}
						numAlive--;
					}
				}
			}
		}
	}
}

void moveDown(int &frameCount)
{
	int interval = 60;												//how long enemies take to move down depends on how many enemies left

	if (numAlive > 30)
	{
		interval = 360;
	}
	else if (30 > numAlive && numAlive > 15)
	{
		interval = 300;
	}
	else if (15 > numAlive && numAlive > 8)
	{
		interval = 240;
	}
	else if (8 > numAlive && numAlive > 2)
	{
		interval = 120;
	}
	else if (numAlive == 0)
	{
		gameState = 3;
	}

	if (frameCount % interval == 0)
	{
		for (int i = 0; i < NUM_COLUMNS; i++)
		{
			for (int j = 0; j < NUM_ROWS; j++)
			{
				arrEnem[i][j].y_pos += 20;
			}
		}
	}
}

void enemyShoot()
{
	if (!enemyBullet.status)
	{
		for (int i = 0; i < NUM_COLUMNS; i++)
		{
			for (int j = 0; j < NUM_ROWS; j++)
			{
				if (arrEnem[i][j].active)
				{
					int randCol = rand() % NUM_COLUMNS;
					int randRow = rand() % NUM_ROWS;
					if (randCol == i && randRow == j)
					{
						enemyBullet.UpdatebulletPos(arrEnem[i][j]);
						enemyBullet.status = 1;
						break;
					}
				}
			}
		}
	}
}

void updateBullet()
{
	if (playerBullet.status == 0)												//while bullet not active sets pos of ship as initial bullet pos
		playerBullet.UpdateBulletPos(player);
}

bool reachEnd() //returns true if any enemy hits either of the sides
{
	for (int i = 0; i < NUM_COLUMNS; i++)
	{
		for (int j = 0; j < NUM_ROWS; j++)										
		{
			if (arrEnem[i][j].active)																//checks if enemy active
			{
				if (width-80 < arrEnem[i][j].x_pos && arrEnem[i][j].speed > 0) //if moving to the right and close to right wall
				{
					return true;
					break;
				}
				else if (10 > arrEnem[i][j].x_pos && arrEnem[i][j].speed < 0)	//if moving to the left and close to left wall
				{
					return true;
					break;
				}
			}
		}
	}
	return false;
}

void InitBackground(BackGround &back, float x, float y, float velx, float vely, int width, int height, int dirX, int dirY, ALLEGRO_BITMAP *image)
{
	back.x = x;
	back.y = y;
	back.velX = velx;
	back.velY = vely;
	back.WIDTH = width;
	back.HEIGHT = height;
	back.dirX = dirX;
	back.dirY = dirY;
	back.image = image;
}

void UpdateBackground(BackGround &back)
{
	back.x += back.velX * back.dirX;
	if (back.x + back.WIDTH <= 0)
			back.x = 0;
}

void DrawBackground(BackGround &back)
{
	al_draw_bitmap(back.image, back.x, back.y, 0);
	al_draw_bitmap(back.image, back.x + back.WIDTH, back.HEIGHT, 0);
	al_draw_bitmap(back.image, back.x, back.HEIGHT, 0);		
	al_draw_bitmap(back.image, back.x + back.WIDTH, back.y, 0);
}


void InitialiseBarriers()
{
	Asteroid[0].SetBarrierBound(111, 10, 15);
	Asteroid[0].SetBarrierpos(50, 500);
	Asteroid[0].setLife(5);
	Asteroid[0].active = true;

	Asteroid[1].SetBarrierBound(111, 10, 15);
	Asteroid[1].SetBarrierpos(435, 500);
	Asteroid[1].setLife(5);
	Asteroid[1].active = true;

	Asteroid[2].SetBarrierBound(111, 10, 15);
	Asteroid[2].SetBarrierpos(820, 500);
	Asteroid[2].setLife(5);
	Asteroid[2].active = true;
}

void Reactivate_Enemies()
{
	for (int i = 0; i < NUM_COLUMNS; i++)
	{
		for (int j = 0; j < NUM_ROWS; j++)
		{
			arrEnem[i][j].active = true;
		}
	}
}

void CollideBarrier()
{
	for (int i = 0; i < 3; i++)
	{
		if (Asteroid[i].active && enemyBullet.status == 1)
		{
			if (enemyBullet.x_pos >(Asteroid[i].x_pos - Asteroid[i].Bleft) && enemyBullet.x_pos<(Asteroid[i].x_pos + Asteroid[i].Bright)
			&& enemyBullet.y_pos>(Asteroid[i].y_pos - Asteroid[i].BHeight) && enemyBullet.y_pos < (Asteroid[i].y_pos + Asteroid[i].BHeight))
			{
				if (Asteroid[i].life_points != 0)
				{
					Asteroid[i].life_points--;
					if (Asteroid[i].life_points == 0)
					{
						Asteroid[i].active = false;
					}
				}
				enemyBullet.status = 0;
			}
		}
	}
}

void BulletBarrierCollide()
{
	for (int i=0; i < 3; i++)
	{
		if (Asteroid[i].active && playerBullet.status == 1)
		{
			if (playerBullet.x_pos >(Asteroid[i].x_pos - Asteroid[i].Bleft) && playerBullet.x_pos<(Asteroid[i].x_pos + Asteroid[i].Bright)
				&& playerBullet.y_pos>(Asteroid[i].y_pos - Asteroid[i].BHeight) && playerBullet.y_pos < (Asteroid[i].y_pos + Asteroid[i].BHeight + 40))
			{
				playerBullet.status = 0;
			}
		}
	}
}






void updateEnemyCount()
{

	if (Current_EnemyCount == 0)
	{
		EnemyWaveCount++;
		Current_EnemyCount = NUM_COLUMNS*NUM_ROWS;
		numAlive = NUM_COLUMNS*NUM_ROWS;
		Reactivate_Enemies();
		Reactivate_Barriers();
		setEnemy();

		switch (player.health)
		{
		case 50:
			player.health += 10;
			break;
		case 40:
			player.health += 20;
			break;
		case 30:
			player.health += 20;
			break;
		case 20:
			player.health += 20;
			break;
		case 10:
			player.health += 20;
			break;
		}
	}
}



void EnemyReachEnd()
{
	for (int i = 0; i < NUM_COLUMNS; i++)
	{
		for (int j = 0; j < NUM_ROWS; j++)
		{
			if (arrEnem[i][j].active == true)
			{
				if (arrEnem[i][j].y_pos > Asteroid[0].y_pos - Asteroid[0].BHeight)
				{
					gameState = 3;
				}
			}
		}
	}		
}

void Reactivate_Barriers()
{
	for (b = 0; b < 3; b++)
	{
		Asteroid[b].life_points = 5;
		Asteroid[b].active = true;
	}

}

