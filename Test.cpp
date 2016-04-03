#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")						//no console window behind game window

#include<allegro5\allegro.h>
#include<allegro5\allegro_native_dialog.h>
#include<allegro5\allegro_primitives.h>
#include<allegro5\allegro_image.h>

#include <string>

#include "Enemy.h"
#include "Spaceship.h"
#include "Bullet.h"
#include "Global.h"

using namespace std;

//GLOBALS
enum KEYS {UP, DOWN, LEFT, RIGHT, SPACE};
bool keys[5] = {false,false,false,false,false};

//Initialise
Spaceship	a(width/2, height*4/5);												
Bullet		b(a.x_pos, a.y_pos,20,true);
Bullet		c(0,0,10,false);
Enemy		arrEnem[NUM_ENEMY];															//array of objects


int main(void)
{
	//5 random enemies for testing
	arrEnem[0].set(width/2 - sp,	65, width-300 - sp	,250 - sp	,15);				//left
	arrEnem[1].set(width/2,			65, width - 300		,250		,15);				//middle
	arrEnem[2].set(width/2 + sp,	65, width-300 + sp	,250 + sp	,15);				//right 
	arrEnem[3].set(width/2 - 2*sp,	65, width-300 - 2*sp,250 - 2*sp	,15);					 
	arrEnem[4].set(width/2 + 2*sp,	65, width-300 + 2*sp,250 + 2*sp	,15);					
	
	c.isFriendly = false;
	srand((unsigned)time(NULL));

	//primitive variables
	const int FPS = 60;
	bool done = false;
	bool redraw = true;

	if(!al_init())
	{
		al_show_native_message_box(NULL,NULL,NULL,"Could not initialize allegro",NULL,NULL);
		return -1;
	}
	
	//Allegro variables
	ALLEGRO_DISPLAY *DISPLAY = NULL;
	ALLEGRO_BITMAP *picShip = NULL;
	ALLEGRO_BITMAP *picBullet = NULL;
	ALLEGRO_BITMAP *picEnemy = NULL;
	ALLEGRO_EVENT_QUEUE *TestQueue = NULL;
	ALLEGRO_TIMER *timer = NULL;

	//Allegro Module Init
	al_init_image_addon();
	al_set_new_window_position(400, 100);												//set pos of game window
	DISPLAY = al_create_display(width, height);
	al_init_primitives_addon();
	al_install_keyboard();

	if (!DISPLAY)
	{
		al_show_native_message_box(NULL, NULL, NULL, "Could not initialize display", NULL, NULL);
		return -1;
	}

	picBullet=al_load_bitmap("Lazer.png");
	picShip =al_load_bitmap("player1.png");
	picEnemy = al_load_bitmap("enemy.png");



	al_convert_mask_to_alpha(picShip,al_map_rgb(0,0,0));
	al_convert_mask_to_alpha(picEnemy, al_map_rgb(0, 0, 0));
	al_convert_mask_to_alpha(picBullet, al_map_rgb(0, 0, 0));

	timer=al_create_timer(1.0/FPS);

	TestQueue= al_create_event_queue();
	al_register_event_source(TestQueue,al_get_keyboard_event_source());
	al_register_event_source(TestQueue,al_get_display_event_source(DISPLAY));
	al_register_event_source(TestQueue,al_get_timer_event_source(timer));
	
	al_start_timer(timer);

	while (!done)
	{

		ALLEGRO_EVENT GETKEY;
		al_wait_for_event(TestQueue, &GETKEY);

		if (GETKEY.type == ALLEGRO_EVENT_DISPLAY_CLOSE)								//will allow red X to close program
		{
			done = true;
		}
		else if (GETKEY.type == ALLEGRO_EVENT_TIMER)
		{
			redraw = true;
			if (keys[UP])
				a.MoveSpaceshipUp();
			if (keys[DOWN])
				a.MoveSpaceshipDown();
			if (keys[LEFT])
				a.MoveSpaceshipLeft();
			if (keys[RIGHT])
				a.MoveSpaceshipRight();
			if (keys[SPACE])															//Spacebar will fire
				b.status = 1;

			for (int i = 0; i < NUM_ENEMY; i++)											//collission detection ahead. Proceed with caution
			{
				if (b.status && arrEnem[i].active && b.isFriendly)										//checks if enemy and bullet active
				{
					if (b.y_pos - 25 < arrEnem[i].y_pos + arrEnem[i].boxheight			//checks if within box of enemy
						&& b.y_pos > arrEnem[i].y_pos - arrEnem[i].boxheight
						&& b.x_pos < arrEnem[i].x_pos + arrEnem[i].boxright
						&& b.x_pos > arrEnem[i].x_pos - arrEnem[i].boxleft)
					{

						b.status = 0;												//bullet set to not active
						arrEnem[i].active = false;									//enemy set to not active
					}
				}
			}

			if (b.status == 0)															//while bullet not active sets pos of ship as initial bullet pos
				b.UpdateBulletPos(a);

			//Enemy shooting back
			int randomEnemy;

			for (int i = 0; i < NUM_ENEMY; i++)
			{
				if (arrEnem[i].active && !c.status)
				{
					randomEnemy = rand() % NUM_ENEMY + 1;
					if (randomEnemy == i)
					{
						c.Updatebulletpos(arrEnem[i]);
						c.status = 1;
						//break;
					}

				}
			}
			for (int i = 0; i < NUM_ENEMY; i++)											//collission detection ahead. Proceed with caution
			{
				if (c.status && !c.isFriendly)										//checks if enemy and bullet active
				{
					if (c.y_pos  < a.y_pos + a.boxheight			//checks if within box of enemy
						&& c.y_pos > a.y_pos - a.boxheight
						&& c.x_pos < a.x_pos + a.boxright
						&& c.x_pos > a.x_pos - a.boxleft)
					{

						c.status = 0;												//bullet set to not active
						a.health -= 10;
					}
				}
			}
		}

		
		else if (GETKEY.type==ALLEGRO_EVENT_KEY_DOWN)
		{
			switch(GETKEY.keyboard.keycode)
			{
			case ALLEGRO_KEY_ESCAPE:													//esc to end the game
				done = true;
				break;
			case ALLEGRO_KEY_UP:
				keys[UP] = true;
				break;
			case ALLEGRO_KEY_DOWN:
				keys[DOWN] = true;
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
			case ALLEGRO_KEY_UP:
				keys[UP] = false;
				break;
			case ALLEGRO_KEY_DOWN:
				keys[DOWN] = false;
				break;
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

			if (b.status == 1)															//if bullet still active
			{
				b.Increment();															//bullet will move pos
				al_draw_bitmap(picBullet, b.x_pos, b.y_pos, 0);							//redraw at new pos
			}

			if (c.status == 1)
			{
				c.Increment();															//bullet will move pos
				al_draw_bitmap(picBullet, c.x_pos, c.y_pos, 0);
			}
			if (a.health > 0)
			{
				al_draw_bitmap(picShip, a.x_pos - 45, a.y_pos, 0);
			}			
		
			for (int i = 0; i < NUM_ENEMY;i++)								
			{
				if (arrEnem[i].active)													//if enemy is active
				{
					arrEnem[i].Move();													//enemy pos will change
					al_draw_bitmap(picEnemy, arrEnem[i].x_pos, arrEnem[i].y_pos, 0);		//redraw at new pos
				}
			}
			

			al_flip_display();															//flip display to show all drawn objects
			al_clear_to_color(al_map_rgb(0, 0, 0));										//background colour
		 }
	}

	//Destroy allegro variables
	al_destroy_event_queue(TestQueue);
	al_destroy_timer(timer);
	al_destroy_display(DISPLAY);
	al_destroy_bitmap(picEnemy);
	al_destroy_bitmap(picShip);
	al_destroy_bitmap(picBullet);

	return 0;
}