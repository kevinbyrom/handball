// GALACTIC HANDBALL : ASSIGNMENT #5    
// KEVIN BYROM : CIS 119

// ** NOTE ** THIS PROJECT MUST BE COMPILED IN VC++ 1.x WITH THE 
//			  LIBRARY "GAMELIB.LIB" 


// INCLUDES

#include <io.h>
#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <dos.h>
#include <bios.h>
#include <fcntl.h>
#include <memory.h>
#include <malloc.h>
#include <math.h>
#include <string.h>
#include <time.h>

#include "graph3.h" // Graphics Libraries
#include "graph4.h"
#include "graph6.h"
#include "graph11.h" // Keyboard Library

// DEFINES

#define PLAY_MODE 0
#define MAX_BOUNCES 3

// PROTOTYPES

void Draw_Screen(void);

// TYPES

typedef struct ball_struct
{

	int x, y; // coordinates
	int xdir, ydir; // direction
	int speed; // ball speed
	
} ball_typ;

typedef struct star_struct
{

	int x,y; // coordinates
	int speed; // speed

} star_typ;

// OBJECTS

ball_typ ball;
sprite ball_spr, paddle_spr, tile_spr;
pcx_picture imagery, title;
star_typ dark_star[50], mid_star[30], light_star[10];

// GLOBAL VARIABLES

int paddle_x;
int done;
int tiles[5][15];
int score;                  
int score_change;

// MAIN 

void Draw_Screen(void)
{

// DRAW SCREEN
		
	int index;
	
	Fill_Double_Buffer(0);
	
	sprite_width=20;
	sprite_height=20;         
		
	if (PLAY_MODE==0)
	{

		for (index=0; index<50; index++)
  		{                               
	
	    	Plot_Pixel_Fast_DB(dark_star[index].x, dark_star[index].y, 8);
	    	dark_star[index].y+=dark_star[index].speed;
		    if (dark_star[index].y>=200) 
		    {
		        dark_star[index].y=dark_star[index].y-200;
		        dark_star[index].x=rand()%316+2;
		    }
		}

		for (index=0; index<30; index++)
  		{
	
	    	Plot_Pixel_Fast_DB(mid_star[index].x, mid_star[index].y, 7);
	    	mid_star[index].y+=mid_star[index].speed;
		    if (mid_star[index].y>=200) 
		    {
		        mid_star[index].y=mid_star[index].y-200;
		        mid_star[index].x=rand()%316+2;
		    }
		}

		for (index=0; index<10; index++)
  		{
	
	    	Plot_Pixel_Fast_DB(light_star[index].x, light_star[index].y, 15);
	    	light_star[index].y+=light_star[index].speed;
		    if (light_star[index].y>=200) 
		    {
		        light_star[index].y=light_star[index].y-200;
		        light_star[index].x=rand()%316+2;
		    }
		}
	
	}
							 
	Bline_DB(0,0,319,0,1); //Top line
	Bline_DB(0,0,0,199,1); //Left line
	Bline_DB(319,0,319,199,1); // Right line
	
	ball_spr.curr_frame=0;
	ball_spr.x=ball.x;
	ball_spr.y=ball.y;
	sprite_width=7;
	sprite_height=7;
	Draw_Sprite_DB((sprite_ptr)&ball_spr);
		
	paddle_spr.curr_frame=0;
	paddle_spr.x=paddle_x;
	paddle_spr.y=197;
	sprite_width=40;
	sprite_height=3;
	Draw_Sprite_DB((sprite_ptr)&paddle_spr);
			
	Show_Double_Buffer(double_buffer);

}

void Initiation(void)
{

	int iy,ix,index;

	// Set Background
	
	for (iy=0; iy<10; iy++)
 	{
	 	for (ix=0; ix<16; ix++)
 		{
	 		tiles[iy][ix]=1;
 		}
 	}
	 	
	// Set Stars

	for (index=0; index<10; index++)
	{
		light_star[index].x=rand()%316+2;
		light_star[index].y=rand()%200;
		light_star[index].speed=rand()%3+6;
	}
	for (index=0; index<30; index++)
	{
		mid_star[index].x=rand()%316+2;
		mid_star[index].y=rand()%200;
		mid_star[index].speed=rand()%3+3;
	}
	for (index=0; index<50; index++)
	{
		dark_star[index].x=rand()%316+2;
		dark_star[index].y=rand()%200;
		dark_star[index].speed=rand()%2+1;
	}
	
	//Grab the bitmaps
	PCX_Init((pcx_picture_ptr)&imagery);
	PCX_Load("gdata.pcx",(pcx_picture_ptr)&imagery,1);
	
	sprite_width=40;
	sprite_height=3;
	
	Sprite_Init((sprite_ptr)&paddle_spr,0,0,0,0,0,0);
	PCX_Grab_Bitmap((pcx_picture_ptr)&imagery, (sprite_ptr)&paddle_spr,0,0,0);
	
	sprite_width=7;
	sprite_height=7;
	
	Sprite_Init((sprite_ptr)&ball_spr,0,0,0,0,0,0);
	PCX_Grab_Bitmap((pcx_picture_ptr)&imagery, (sprite_ptr)&ball_spr, 0,0,1);
	
	sprite_width=20;
	sprite_height=20;
	
	Sprite_Init((sprite_ptr)&tile_spr,0,0,0,0,0,0);
	PCX_Grab_Bitmap((pcx_picture_ptr)&imagery, (sprite_ptr)&tile_spr, 0,0,1);
	PCX_Grab_Bitmap((pcx_picture_ptr)&imagery, (sprite_ptr)&tile_spr, 1,1,1);
	PCX_Grab_Bitmap((pcx_picture_ptr)&imagery, (sprite_ptr)&tile_spr, 2,2,1);
	
	PCX_Delete((pcx_picture_ptr)&imagery);
	
	// Initiate all variables
	
	paddle_x=140;
	ball.x=137;
	ball.y=190;
	ball.xdir=-3;
	ball.ydir=-3;
	ball.speed=3;
	done=0;
	score=0;
	score_change=0;

}

void Show_Title(void)
{

	PCX_Init((pcx_picture_ptr)&title);
	PCX_Load("tdata.pcx",(pcx_picture_ptr)&title,1);
	PCX_Show_Buffer((pcx_picture_ptr)&title);
	PCX_Delete((pcx_picture_ptr)&title);
	while(!kbhit()){}
	Fade_Lights();
	_fmemset(video_buffer,(char)0,SCREEN_WIDTH*SCREEN_HEIGHT+1);

}

void main(void)
{
            
	srand ((unsigned)time(NULL)); //RANDOMIZE THE INTERNAL TIMER 
	
	Set_Video_Mode(VGA256);
	
	Show_Title();
	
	Initiation();
		
	if (!Create_Double_Buffer(SCREEN_HEIGHT))
	{
		printf("\nNot enough memory to create double buffer");
	}
	
	Install_Keyboard();
	
	while(!done)
	{
	
		ball.x+=ball.xdir;
		ball.y+=ball.ydir;
		
		if (key_table[INDEX_ESC])
		   done=1;
		   
		if (key_table[INDEX_RIGHT] || key_table[INDEX_LEFT])
		{
			
			if (key_table[INDEX_LEFT])
			{
				if (key_table[INDEX_CTRL])
					paddle_x-=5;
				paddle_x-=7;
			}
			if (key_table[INDEX_RIGHT])
			{
				if (key_table[INDEX_CTRL])
					paddle_x+=5;
				paddle_x+=7;	
		    }
		    
		    if (paddle_x<1)
		    	paddle_x=1;
		    if (paddle_x>278)
		    	paddle_x=278;
		}
		
		if (ball.x<=1)
		{
			ball.x=1;
			ball.xdir=ball.speed;
		}
		
		if (ball.x>=312)
		{
			ball.x=312;
			ball.xdir=-ball.speed;
		}
		
		if (ball.y<=1)
		{
			ball.y=1;
			ball.ydir=ball.speed;
		}
		
		if (ball.y>=190 && ball.x>(paddle_x-7) && ball.x<(paddle_x+47))
		{
			ball.y=190;
			ball.ydir=-ball.speed;
		    score+=1;
		    score_change+=1;
		    if (score_change==MAX_BOUNCES)
		    {
		    	score_change=0;
		    	ball.speed+=1;
		    }
		}
		
		if (ball.y>=190 && ball.x<=(paddle_x-7) || ball.y>=190 && ball.x>=(paddle_x+47))
			done=1;
		
		Draw_Screen();
	
		Delay(1);
		
	}
	
Fade_Lights();

Delete_Keyboard();

Set_Video_Mode(TEXT_MODE);

printf("Galactic Handball 1.0 by Kevin Byrom\nYour score was %d\n", score);

}
		