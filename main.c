#include <acknex.h>
#include <default.c>

#include "commons.c"
#include "animation.c"
#include "player.c"
//#include "enemy.c"
#include "npc.c"




SOUND* snd_mtlgr_big = "mtlgrbig.ogg"; 
SOUND* snd_mtlgr_sml = "mtlgrsml.ogg"; 
SOUND* snd_death="c_-g3-sounds-voi.wav";
SOUND* snd_death_enemy="BIGFILES.DAT_00792.wav";
SOUND* snd_player_hit_enemy="BIGFILES.DAT_00800.wav";

SOUND* snd_pickup = "BIGFILES.DAT_00788.wav";


#define MOUSE_FACTOR		-20		// Change sign to turn in reverse direction
#define MIN_DIF_2_MOVE		10		// Minimum angle difference to start moving

ENTITY* cameraEntity;
ENTITY* iconEntity; 

var snd_handle; 

var CAMERA_OFFSET =		380;
var farc=0;
var FCarc=80;



var PlayerLife = 400;
var PlayerHealth = 4;
var PlayerHitTimer = 0;
var PickUpCount = 0;
var FMusic;

var LevelPickups[2];
var LevelBonusPickups[2];

function ShowLevelPickups()
{
	
if (LevelPickups[0]==1){ent_create ("red.bmp", vector(832,-93,355), NULL); }else{ent_create ("redblank.bmp", vector(832,-93,355), NULL); }
if (LevelPickups[1]==1){ent_create ("red.bmp", vector(832,-186,355), NULL); }else{ent_create ("redblank.bmp", vector(832,-186,355), NULL); }
	
if (LevelBonusPickups[0]==1){ent_create ("wite.bmp", vector(832,-93,451), NULL); }else{ent_create ("whiteblank.bmp", vector(832,-93,451), NULL); }
if (LevelBonusPickups[1]==1){ent_create ("wite.bmp", vector(832,-186,451), NULL); }else{ent_create ("whiteblank.bmp", vector(832,-186,451), NULL); }
}


PANEL* panel_black =
{
	pos_x = 0; pos_y = 0;
	size_x = 1920; size_y = 1080;
	red = 0;
	green = 0;
	blue = 0;
	flags = LIGHT | SHOW | TRANSLUCENT ;
	
}


action BonusCoin()
{
	set(my,PASSABLE); 
	my.frame = my.skill1; // set to start frame  
	while (player == NULL) 
	{
		wait(1);
	} 
	while (vec_dist (player.x, my.x) > 100) 
	{ 
		my.frame += 0.3*time_step; 
		if (my.frame > my.skill1) 
		{ 
			my.frame = 1; // loop 
		}  
		wait (1);
	} 
	snd_handle=snd_play(snd_pickup,50,0);
	PickUpCount +=1;
	Explo.x=my.x;
	Explo.y=my.y;
	Explo.z=my.z;
	Explo.frame=1;
	
	wait(1);
	ent_remove(me); 	
}

function fade_out()
{
	media_tune(FMusic,1,0,0);
	panel_black.alpha = 100;
		var i=1;
	
	{
	while (panel_black.alpha >1)
	{
		panel_black.alpha -= 8*time_step; 
		if (i<=50)
		media_tune(FMusic,i,0,0);
	   i=i+0.2;
		wait(2);
	}
	panel_black.alpha = 0;
		media_tune(FMusic,50,0,0);	
	}
}

function fade_in()
{
	panel_black.alpha = 0;
	var i=50;
	while (i>1)
	{
	while (panel_black.alpha <100)
	{
		panel_black.alpha += 8*time_step; 
		if (i>1)
		media_tune(FMusic,i,0,0);
		i=i-0.2;
		wait(2);
	}
	panel_black.alpha = 100;
	i=i-0.2;	
	}
}

ENTITY* ent_sky;
function player_death()
{
	printf("Game over");
	sys_exit("666");
}



function main(){
	camera.clip_near = 0;
	video_window(vector(0,0,0),vector(0,0,0),0,"Return Of The Gecko v0.0.3");	
	//	video_switch(12,0,2);
	video_screen = 0;
	video_mode = 12;
	
	shadow_stencil = 1;
	mouse_mode = 4;
	if (game_load("test",0)<=0){game_save("test",0,SV_VARS);}
	level_load("md.wmb");
	ent_sky = ent_createlayer("spacecube1+6.bmp", SKY | CUBE, 1);  
	camera.arc= 80;
	wait(2);
	FMusic =	media_loop("Echoes_of_Time.mp3",NULL,50);
	ShowLevelPickups();
	
	fade_out();
	
	
	while(1)
	{
		wait(1);

		if (player.z<=-400)
		{
			fade_in();
			
			snd_handle=snd_play(snd_death,50,0);
			wait(-2);
			fade_out();		
			player.z = 0; 
			player.x = 0; 
			player.y = 0;
			camera.z = player.z+170; 
			PlayerLife -= 1; 	
			
			if (PlayerLife>-1)
			{
				PlayerHealth = 4;
			} 
			else 
			{
				player_death();
			}
		}
	}
}




// skill1: speed
// skill2: distance
action platform_pan()
{
	wait(1);
	var angle;
	var dist;
	var tmpx;
	var tmpy;
	var tmpx2;
	var tmpy2;
	var tmpxd;
	var tmpyd;

	while(1)
	{
		my.pan += my.skill1*time_step;			
		if (vec_dist (player.x, my.x) <  my.skill2)
		if (abs(my.z-player.z)<100)
		{		
			angle = atan((player.x-my.x)/(player.y-my.y));

			if ((my.y<player.y)&&(my.x>player.x)){angle =  270+(90+angle);}
			if ((my.y>player.y)&&(my.x>player.x)){angle = 180+ angle;}
			if ((my.y>player.y)&&(my.x<player.x)){angle = 90+ (90+angle);}
			
			tmpx = (player.x-my.x);
			tmpy = (player.y-my.y);	
			dist = vec_dist (player.x, my.x);
			tmpx = sinv(integer(angle))*dist;
			tmpy = cosv(integer(angle))*dist;
			tmpx2 = sinv(integer(angle+my.skill1))*dist;
			tmpy2 = cosv(integer(angle+my.skill1))*dist;
			tmpxd = tmpx-tmpx2;
			tmpyd = tmpy-tmpy2;
			player.y +=tmpyd*time_step;
			player.x +=tmpxd*time_step;
			player.pan +=my.skill1*time_step;
		}
		wait(1);
	}
}

// FLAG1: PlayerActive
// FLAG2: AutomaticHeight
// FLAG3: Rotate
// skill1: X
// skill2: Y
// skill3: Z
// skill4: Speed
// skill5: Timer
// skill6: RotateSpeed
// skill7: Distance (range)
action platform_Z()
{
	wait(1);

	var startx = my.x;
	var starty = my.y;
	var startz;
	var endz;
	var vector = 3;
	var endx = startx + my.skill1;
	var endy = starty + my.skill2;
	var angle;
	var dist;
	var tmpx;
	var tmpy;
	var tmpx2;
	var tmpy2;
	var tmpxd;
	var tmpyd;

	if (my.skill3>0)
	{
		startz = my.z;
		endz = startz + my.skill3;
	}
	else
	{
		endz = my.z;
		startz = endz + my.skill3;	
	}
	
	if is(my,FLAG1){}
	else{vector=0;}
	
	while(1)
	{
		if is(my,FLAG3)
		{my.pan += time_step * my.skill6;}
		
		if (vec_dist (player.x, my.x) <  my.skill7)
		{
			if (my.z<=endz){vector=0;}
			if (my.z>=startz){vector=1;}
		}
		
		if is(my,FLAG2)
		{
			if (vector > 2)
			{
				if (vector ==4 && player.z<=endz){vector=1;}	
				if (vector ==5 && player.z>=endz){vector=0;}	
			}
		}
		
		while (vector ==0) 
		{ 	if is(my,FLAG3)
			{
				my.pan += my.skill6*time_step;			
				if (vec_dist (player.x, my.x) <  my.skill7)
				if (abs(my.z-player.z)<100)
				{		
					angle = atan((player.x-my.x)/(player.y-my.y));
					
					if ((my.y<player.y)&&(my.x>player.x)){angle =  270+(90+angle);}
					if ((my.y>player.y)&&(my.x>player.x)){angle = 180+ angle;}
					if ((my.y>player.y)&&(my.x<player.x)){angle = 90+ (90+angle);}
					
					tmpx = (player.x-my.x);
					tmpy = (player.y-my.y);	
					dist = vec_dist (player.x, my.x);
					tmpx = sinv(integer(angle))*dist;
					tmpy = cosv(integer(angle))*dist;
					tmpx2 = sinv(integer(angle+my.skill6))*dist;
					tmpy2 = cosv(integer(angle+my.skill6))*dist;
					tmpxd = tmpx-tmpx2;
					tmpyd = tmpy-tmpy2;
					player.y +=tmpyd*time_step;
					player.x +=tmpxd*time_step;
					player.pan +=my.skill6*time_step;
				}
			}
			if (my.z<=endz)
			{
				my.z +=my.skill4 * time_step;
				if (vec_dist (player.x, my.x) <  my.skill7)
				{
					player.z +=my.skill4 * time_step;
					camera.z +=my.skill4 * time_step;
					if (my.z>=player.z-40){player.z +=40;
						camera.z +=5;
					}
				} 
				wait(1); 
			} else 
			{
			if is(my,FLAG1){vector=4;} else {vector = 1;}
				wait(-my.skill5);		
			}
		}	
		while (vector ==1)
		{	if is(my,FLAG3)
			{
				my.pan += my.skill6*time_step;			
				if (vec_dist (player.x, my.x) <  my.skill7)
				if (abs(my.z-player.z)<100)
				{		
					angle = atan((player.x-my.x)/(player.y-my.y));
					
					if ((my.y<player.y)&&(my.x>player.x)){angle =  270+(90+angle);}
					if ((my.y>player.y)&&(my.x>player.x)){angle = 180+ angle;}
					if ((my.y>player.y)&&(my.x<player.x)){angle = 90+ (90+angle);}
					
					tmpx = (player.x-my.x);
					tmpy = (player.y-my.y);	
					dist = vec_dist (player.x, my.x);
					tmpx = sinv(integer(angle))*dist;
					tmpy = cosv(integer(angle))*dist;
					tmpx2 = sinv(integer(angle+my.skill6))*dist;
					tmpy2 = cosv(integer(angle+my.skill6))*dist;
					tmpxd = tmpx-tmpx2;
					tmpyd = tmpy-tmpy2;
					player.y +=tmpyd*time_step;
					player.x +=tmpxd*time_step;
					player.pan +=my.skill6*time_step;
				}
			}
			
			if (my.z>=startz)
			{
				my.z -=my.skill4 * time_step;
				if (vec_dist (player.x, my.x) <  my.skill7)
				{
					player.z -=my.skill4 * time_step;
					camera.z -=my.skill4 * time_step;
				} 
				wait(1);
			} else 
			{
			if is(my,FLAG1){vector=5;} else {vector =0;}
				wait(-my.skill5);
			}
		}
		wait(1);
	}
}


action platform_Y()
{
	wait(1);

	var startx = my.x;
	var starty = my.y;
	var startz;
	var endz;
	var vector = 3;
	var endx = startx + my.skill1;
	var endy = starty + my.skill2;
	var angle;
	var dist;
	var tmpx;
	var tmpy;
	var tmpx2;
	var tmpy2;
	var tmpxd;
	var tmpyd;

	if (my.skill2>0)
	{
		starty = my.y;
		endy = starty + my.skill2;
	}
	else
	{
		endy = my.y;
		starty = endy + my.skill2;	
	}
	
	if is(my,FLAG1){}
	else{vector=0;}
	
	while(1)
	{
		if is(my,FLAG3)
		{my.pan += time_step * my.skill6;}
		
		if (vec_dist (player.x, my.x) <  my.skill7)
		{
			if (my.y<=endy){vector=0;}
			if (my.y>=starty){vector=1;}
		}
		
		if is(my,FLAG2)
		{
			if (vector > 2)
			{
				if (vector ==4 && player.y<=endy){vector=1;}	
				if (vector ==5 && player.y>=endy){vector=0;}	
			}
		}
		
		while (vector ==0) 
		{ 	if is(my,FLAG3)
			{
				my.pan += my.skill6*time_step;			
				if (vec_dist (player.x, my.x) <  my.skill7)
				if (abs(my.z-player.z)<100)
				{		
					angle = atan((player.x-my.x)/(player.y-my.y));
					
					if ((my.y<player.y)&&(my.x>player.x)){angle =  270+(90+angle);}
					if ((my.y>player.y)&&(my.x>player.x)){angle = 180+ angle;}
					if ((my.y>player.y)&&(my.x<player.x)){angle = 90+ (90+angle);}
					
					tmpx = (player.x-my.x);
					tmpy = (player.y-my.y);	
					dist = vec_dist (player.x, my.x);
					tmpx = sinv(integer(angle))*dist;
					tmpy = cosv(integer(angle))*dist;
					tmpx2 = sinv(integer(angle+my.skill6))*dist;
					tmpy2 = cosv(integer(angle+my.skill6))*dist;
					tmpxd = tmpx-tmpx2;
					tmpyd = tmpy-tmpy2;
					player.y +=tmpyd*time_step;
					player.x +=tmpxd*time_step;
					player.pan +=my.skill6*time_step;
				}
			}
			if (my.y<=endy)
			{
				my.y +=my.skill4 * time_step;
				if (vec_dist (player.x, my.x) <  my.skill7)
				{
					player.y +=my.skill4 * time_step;
					camera.y +=my.skill4 * time_step;
					if (my.y>=player.y-40){player.y +=40;
						camera.y +=5;
					}
				} 
				wait(1); 
			} else 
			{
			if is(my,FLAG1){vector=4;} else {vector = 1;}
				wait(-my.skill5);		
			}
		}	
		while (vector ==1)
		{	if is(my,FLAG3)
			{
				my.pan += my.skill6*time_step;			
				if (vec_dist (player.x, my.x) <  my.skill7)
				if (abs(my.z-player.z)<100)
				{		
					angle = atan((player.x-my.x)/(player.y-my.y));
					
					if ((my.y<player.y)&&(my.x>player.x)){angle =  270+(90+angle);}
					if ((my.y>player.y)&&(my.x>player.x)){angle = 180+ angle;}
					if ((my.y>player.y)&&(my.x<player.x)){angle = 90+ (90+angle);}
					
					tmpx = (player.x-my.x);
					tmpy = (player.y-my.y);	
					dist = vec_dist (player.x, my.x);
					tmpx = sinv(integer(angle))*dist;
					tmpy = cosv(integer(angle))*dist;
					tmpx2 = sinv(integer(angle+my.skill6))*dist;
					tmpy2 = cosv(integer(angle+my.skill6))*dist;
					tmpxd = tmpx-tmpx2;
					tmpyd = tmpy-tmpy2;
					player.y +=tmpyd*time_step;
					player.x +=tmpxd*time_step;
					player.pan +=my.skill6*time_step;
				}
			}
			
			if (my.y>=starty)
			{
				my.y -=my.skill4 * time_step;
				if (vec_dist (player.x, my.x) <  my.skill7)
				{
					player.y -=my.skill4 * time_step;
					camera.y -=my.skill4 * time_step;
				} 
				wait(1);
			} else 
			{
			if is(my,FLAG1){vector=5;} else {vector =0;}
				wait(-my.skill5);
			}
		}
		wait(1);
	}
}

function player_hit()
{
	if (PlayerHitTimer==0)
	{
		PlayerHitTimer= 50;
		PlayerHealth -= 1;		
		if (PlayerHealth<1)
		{
			
			PlayerLife -=1;
			if (PlayerLife>0)
			{	
			snd_handle=snd_play(snd_death,50,0);
			
				fade_in();wait(-5);
				player.x = 0;
				player.y = 0;
				player.z = 0;
				PlayerHealth=4;
				
				fade_out();
			} 
			else 
			{player_death();}
		}
		else 
		{	
			set(player,TRANSLUCENT);
			player.alpha = 30;		
			while (PlayerHitTimer>=0)
			{
				PlayerHitTimer -= 1 * time_step;
				wait(1);		
			}
		reset(player,TRANSLUCENT);}
		PlayerHitTimer= 0;
	}
}

action EnemyLine()
{
	var startx = my.x;
	var endx= my.x+my.skill1;
	var temp[3];
	set(my,SHADOW|CAST);
	wait(-1);
	while(1)
	{
		if (player.x>startx && player.x<endx)
		{
			my.x=player.x;
		}
		vec_set(temp,player.x); 
		vec_sub(temp,my.x);
		vec_to_angle(my.pan,temp); 
		my.tilt=0;
		my.roll=0;
		
		my.skill1 += 3*time_step;
		if (my.skill1 > 100) my.skill1 -= 100; 
		ent_animate(me,"walk",my.skill1,ANM_CYCLE);
		
		if (vec_dist(my.x,player.x)< 90)
		{
			player_hit();
		}
		
		wait(1);
		if (my.skill1>50 || my.skill1<10){ 
	if (vec_dist(my.x,player.x)<100){if (player.IsAttack==1){break;}}	}
	}
	snd_handle=snd_play(snd_player_hit_enemy,50,0);
	Explo.x=my.x;
	Explo.y=my.y;
	Explo.z=my.z+30;
	Explo.frame=1;
	snd_handle=snd_play(snd_death_enemy,50,0);
	wait(1);
	my.z = -1000;
	while (PlayerHitTimer>0) {wait(-1);}
	ent_remove(me); 
}


action EnemyLineY()
{
	var starty = my.y;
	var endy= my.y+my.skill1;
	var temp[3];
	set(my,SHADOW|CAST);
	wait(-1);
	while(1)
	{
		if (player.y>starty && player.y<endy)
		{
			my.y=player.y;
		}
		vec_set(temp,player.x); 
		vec_sub(temp,my.x);
		vec_to_angle(my.pan,temp); 
		my.tilt=0;
		my.roll=0;
		
		my.skill1 += 3*time_step;
		if (my.skill1 > 100) my.skill1 -= 100; 
		ent_animate(me,"walk",my.skill1,ANM_CYCLE);
		
		if (vec_dist(my.x,player.x)< 90)
		{
			player_hit();
		}
		
		wait(1);
		if (my.skill1>50 || my.skill1<10){ 
	if (vec_dist(my.x,player.x)<100){if (player.IsAttack==1){break;}}	}
	}
	snd_handle=snd_play(snd_player_hit_enemy,50,0);
	Explo.x=my.x;
	Explo.y=my.y;
	Explo.z=my.z+30;
	Explo.frame=1;
	snd_handle=snd_play(snd_death_enemy,50,0);
	wait(1);
	my.z = -1000;
	while (PlayerHitTimer>0) {wait(-1);}
	ent_remove(me); 
}

PANEL* panel_hud =
{
	pos_x = 10;
	pos_y = 10;
	digits(20,30, "Life: %.0f", "Arial#20bi", 1, PlayerLife); 
	digits(20,50, "Health: %.0f", "Arial#20bi", 1, PlayerHealth); 
	digits(20,70, "Pickups: %.0f", "Arial#20bi", 1, PickUpCount); 
	flags = SHOW | OUTLINE;
}

action TVlevel1()
{
	while (1)
	{
		wait(-1);
		if (vec_dist(my.x,player.x)<=150){break;	}
	}
	fade_in();
	
	wait(-2);
	ent_remove(ent_sky);
	level_load("l1.wmb");
	fade_out();
	PickUpCount = 0;
	
	
	
	
	media_stop(FMusic);	
	FMusic =	media_loop("Day_of_Chaos.mp3",NULL,50);
	fog_color = 1; 
	camera.fog_start = 1500; 
	camera.fog_end = 2000;
	sky_color.red = 0;
	sky_color.green = 0;
	sky_color.blue = 0; 
}

action TVlevel2()
{
	while (1)
	{
		wait(-1);
		if (vec_dist(my.x,player.x)<=150){break;	}
	}
	fade_in();
	
	wait(-2);
	ent_remove(ent_sky);
	level_load("l2.wmb");
	fade_out();
	PickUpCount = 0;
	
	
	
	ent_sky = ent_createlayer("8a+6.tga", SKY | CUBE, 1);  
	
	media_stop(FMusic);	
	FMusic =	media_loop("Sao Meo.mp3",NULL,50);
	fog_color = 1; 
	camera.fog_start = 1500; 
	camera.fog_end = 2000;
	sky_color.red = 0;
	sky_color.green = 0;
	sky_color.blue = 0; 
}


action LevelBonusPickup1()
{
	if (LevelBonusPickups[1] == 1){set(my,TRANSLUCENT); my.alpha=50;}
	var LDirection=0;
	var LCount=0;
	set(my,PASSABLE); 
	while (player == NULL) 
	{
		wait(1);
	} 
	while (vec_dist (player.x, my.x) > 100) 
	{ 
		if (LDirection==0)
		{
			LCount = LCount +0.1;
			my.z=my.z+0.1;
			if (LCount>25){LDirection=1;LCount=0;}
			
		}
		if (LDirection==1)
		{
			LCount = LCount +0.1;
			my.z=my.z-0.1;
			if (LCount>25){LDirection=0;LCount=0;}	
			
		}

		wait (2);
	} 
	LevelBonusPickups[1] = 1;
	snd_handle=snd_play(snd_pickup,50,0);
	Explo.x=my.x;
	Explo.y=my.y;
	Explo.z=my.z;
	Explo.frame=1;
	
	wait(1);
	ent_remove(me); 
}

action LevelPickup1()
{
	if (LevelPickups[0] == 1){set(my,TRANSLUCENT); my.alpha=50;}
	
	
	var LDirection=0;
	var LCount=0;
	set(my,PASSABLE); 
	while (player == NULL) 
	{
		wait(1);
	} 
	while (vec_dist (player.x, my.x) > 100) 
	{ 
		if (LDirection==0)
		{
			LCount = LCount +0.1;
			my.z=my.z+0.1;
			if (LCount>25){LDirection=1;LCount=0;}
			
		}
		if (LDirection==1)
		{
			LCount = LCount +0.1;
			my.z=my.z-0.1;
			if (LCount>25){LDirection=0;LCount=0;}	
			
		}

		wait (2);
	} 
	LevelPickups[0] = 1;
	if (PickUpCount==56){LevelBonusPickups[0]=1;}
	
	snd_handle=snd_play(snd_pickup,50,0);

	Explo.x=my.x;
	Explo.y=my.y;
	Explo.z=my.z;
	Explo.frame=1;
	
	
	fade_in();
	
	wait(-2);
	//ent_remove(ent_sky);
	level_load("md.wmb");
	game_save("test",0,SV_VARS);
	PickUpCount = 0;
	media_stop(FMusic);	
	
	ent_sky = ent_createlayer("spacecube1+6.bmp", SKY | CUBE, 1);  
	wait(2);
	FMusic =	media_loop("Echoes_of_Time.mp3",NULL,50);
	ShowLevelPickups();
	fade_out();
	fog_color = 0; 	
	camera.fog_start = 1500; 
	camera.fog_end = 2000;
	sky_color.red = 0;
	sky_color.green = 0;
	sky_color.blue = 0; 
}

action LevelPickup2()
{
	
	if (LevelPickups[1] == 1){set(my,TRANSLUCENT); my.alpha=50;}
	var LDirection=0;
	var LCount=0;
	set(my,PASSABLE); 
	while (player == NULL) 
	{
		wait(1);
	} 
	while (vec_dist (player.x, my.x) > 100) 
	{ 
		if (LDirection==0)
		{
			LCount = LCount +0.1;
			my.z=my.z+0.1;
			if (LCount>25){LDirection=1;LCount=0;}
			
		}
		if (LDirection==1)
		{
			LCount = LCount +0.1;
			my.z=my.z-0.1;
			if (LCount>25){LDirection=0;LCount=0;}	
			
		}

		wait (2);
	} 
	LevelPickups[1] = 1;
	
	if (PickUpCount==56){LevelBonusPickups[0]=1;}
	
	snd_handle=snd_play(snd_pickup,50,0);

	Explo.x=my.x;
	Explo.y=my.y;
	Explo.z=my.z;
	Explo.frame=1;
	
	fade_in();
	
	wait(-2);
	//ent_remove(ent_sky);
	level_load("md.wmb");
	
	PickUpCount = 0;
	media_stop(FMusic);	
	game_save("test",0,SV_VARS);
	ent_sky = ent_createlayer("spacecube1+6.bmp", SKY | CUBE, 1);  
	wait(2);
	FMusic =	media_loop("Echoes_of_Time.mp3",NULL,50);
	ShowLevelPickups();
	fade_out();
	fog_color = 0; 	
	camera.fog_start = 1500; 
	camera.fog_end = 2000;
	sky_color.red = 0;
	sky_color.green = 0;
	sky_color.blue = 0; 
}







/////////////////////////////////////////////////////////////////////////////////////////
//	Handles the camera movement, we will position the camera CAMERA_OFFSET quants away
//	from the player in the direction from player to camera's current position
//	updateCamera uses a c_trace, updateCamera2 moves an entity
//  updateCamera doesn't stucks behind walls, updateCamera2 does
//  updateCamera let you see behind walls, updateCamera2 doesn't
/////////////////////////////////////////////////////////////////////////////////////////
function updateCamera(var currCameraHeight){
	VECTOR tempVect;
	
	//	Store new camera position in tempVect
	vec_diff(tempVect,camera.x, my.x);			// 	The direction should be from player to camera
	tempVect.z = 0;								//	remove height	
	vec_normalize(tempVect,CAMERA_OFFSET);		//	it should be CAMERA_OFFSET quants away
	//	adjust height
	currCameraHeight += mouse_force.y * MOUSE_FACTOR * time_step*3;
	if (key_a || key_s || key_d ||key_w)
	{
	  if (currCameraHeight<80){currCameraHeight +=10* time_step;}
	  if (currCameraHeight>200){currCameraHeight -=10* time_step;}	
	}
	tempVect.z = clamp(currCameraHeight, -CAMERA_OFFSET*2, CAMERA_OFFSET*2);
	//	Rotate with mouse movement
	vec_rotateaxis(tempVect, vector(0,0,1), mouse_force.x * MOUSE_FACTOR * time_step) ;
	vec_rotateaxis(tempVect, vector(0,0,1), -key_q  * 8 * time_step) ;
	vec_rotateaxis(tempVect, vector(0,0,1), key_e * 8  * time_step) ;
	vec_add(tempVect, my.x);
	if(c_trace(my.x, tempVect, IGNORE_ME | IGNORE_PASSABLE|IGNORE_MODELS)!=0){
		vec_set(camera.x, target);	
	}
	else{
		vec_set(camera.x, tempVect);
	}
	//	Look at player
	vec_diff(tempVect,my.x, camera.x);
	vec_to_angle(camera.pan,tempVect);
//	camera.z = player.z+170;
}

function updateCamera2(var currCameraHeight){
	VECTOR tempVect;

	//	Store new camera position in tempVect
	vec_diff(tempVect,camera.x, my.x);			// 	The direction should be from player to camera
	tempVect.z = 0;								//	remove height
	vec_normalize(tempVect,CAMERA_OFFSET);		//	it should be CAMERA_OFFSET quants away
	//	adjust height
	currCameraHeight += mouse_force.y * MOUSE_FACTOR * time_step;
		if (key_a || key_s || key_d ||key_w)
	{
	  if (currCameraHeight<80){currCameraHeight +=10* time_step;}
	  if (currCameraHeight>200){currCameraHeight -=10* time_step;}	
	}
	tempVect.z = clamp(currCameraHeight, -CAMERA_OFFSET*2, CAMERA_OFFSET*2);
	//	Rotate with mouse movement
	vec_rotateaxis(tempVect, vector(0,0,1), mouse_force.x * MOUSE_FACTOR * time_step) ;
	vec_rotateaxis(tempVect, vector(0,0,1), key_q * 8 * time_step) ;
	vec_rotateaxis(tempVect, vector(0,0,1), -key_e * 8 * time_step) ;
	vec_add(tempVect,me.x);						//	Global position
	vec_diff(tempVect,tempVect, cameraEntity.x);//	Get camera's movement vector
	
	//	Move the camera entity to update collisions
	c_move(cameraEntity,tempVect,NULL, GLIDE | IGNORE_PASSABLE);
	vec_set(camera.x, cameraEntity.x);			//	Update camera's position
	
	//	Look at player
	vec_diff(tempVect,my.x, camera.x);
	vec_to_angle(camera.pan,tempVect);
}

/////////////////////////////////////////////////////////////////////////////////////////
//	Get pressed keys
/////////////////////////////////////////////////////////////////////////////////////////
function updateInput(){
	 if (PlayerHealth>0){

	me.MovementAction = 0;
	if(key_ctrl)
		me.Running = 0;
	else
		me.Running = 1;
	if(key_cuu || key_w) me.MovementAction += mForward; 
	if(key_cud || key_s) me.MovementAction += mBackward; 
	if(key_cul || key_a) me.MovementAction += mTurnRight; 
	if(key_cur || key_d) me.MovementAction += mTurnLeft;
	if(key_1)CAMERA_OFFSET = CAMERA_OFFSET+1;
	if(key_2)CAMERA_OFFSET = CAMERA_OFFSET-1;
	if(key_5)FCarc = FCarc+1;
	if(key_6)FCarc = FCarc-1;
	if(key_3)cameraType = 1;
	if(key_4)cameraType = 2;
	}
}
/////////////////////////////////////////////////////////////////////////////////////////
//	Get update actions
/////////////////////////////////////////////////////////////////////////////////////////
function updateActions(){
	camera.arc +=farc;
	float displacement=0;
	if (me.MovementAction == 1 || 
		me.MovementAction == 2 || 
		me.MovementAction == 4 || 
		me.MovementAction == 5 || 
		me.MovementAction == 6 || 
		me.MovementAction == 8 || 
		me.MovementAction == 9 || 
		me.MovementAction == 10) {
	if (farc<2){farc +=1*time_step;}
		//	Get vector from player to camera
		VECTOR tempVector;
		vec_diff(tempVector,camera.x, my.x);
		ANGLE currentAngle;
		vec_to_angle(currentAngle,tempVector);
		float rotFactor;
		//	Get rotation angle
		switch (me.MovementAction) {
			case 1:
				rotFactor = 180; break;
			case 2:
				rotFactor = 0; break;
			case 4:
				rotFactor = 270; break;
			case 8:
				rotFactor = 90; break;
			case 5:
				rotFactor = 225; break;
			case 6:
				rotFactor = 315; break;
			case 9:
				rotFactor = 135; break;
			case 10:
				rotFactor = 45; break;
		}
		//	Rotate player
		rotFactor += currentAngle.pan;
		// Don't move until player has enogh rotation
		var bolRotated = 0;
		var smoothedAngle = SmoothAngle(rotFactor, me.pan);
		var angleDif;
		if(me.pan<smoothedAngle)
			angleDif = smoothedAngle - me.pan;
		else
			angleDif = me.pan - smoothedAngle;
		if(abs(angleDif) <= MIN_DIF_2_MOVE)
			bolRotated = 1;
		me.pan = smoothedAngle;
		if(me.Running) {
			displacement = runSpeed * time_step * bolRotated;
			me.CurrentAction = ActionRun;	
		}
		else {
			displacement = walkSpeed * time_step * bolRotated;
			me.CurrentAction = ActionWalk;	
		}
	}
	else{
			if (farc>0){farc -=1*time_step;}
		me.CurrentAction = ActionIdle;	
	}
	//	Move the player
	handle_movement(vector(displacement,0,0), NULL, NULL);	
	handle_animation();
}

function player_event(){
  	switch (event_type){
		case EVENT_SCAN:
	    	
	      	return;

	}
}
action icon_action(){
	while (1){
        my.frame += 0.8 * time_step;
        if (my.frame >= 13) my.frame -= 12;
        wait (1);
    }
}
action player_action() {
	wait(1);	
	//	Adjust collision parameters
	my.eflags |= FAT;	

	my.GravityAccel = 2;
	my.SizeOffset = 40;
	my.RadiusSize = 28;

	vec_set(my.min_x,vector(-my.RadiusSize,-my.RadiusSize,-my.SizeOffset));		
	vec_set(my.max_x,vector(my.RadiusSize,my.RadiusSize,my.SizeOffset));		

	set(my,SHADOW|CAST);
	my.emask |= ENABLE_SCAN;
	my.trigger_range = 200;
	my.event = player_event;
	
	player = my;
	//	Default camera position
	vec_set(camera.x,vector(player.x - CAMERA_OFFSET,player.y,player.z + (CAMERA_OFFSET/4)));
	vec_set(camera.pan,player.pan); 
	me.CurrentAction = ActionIdle;
	me.AnimationPercent = 0;
	//	Use a sphere to update camera position/collision
	cameraEntity = ent_create("sphere.mdl",camera.x,NULL);
   cameraEntity.flags |= INVISIBLE;
	cameraEntity.flags |= PASSABLE;

	var currCameraHeight = CAMERA_OFFSET/4;
	while(1){
	
	
		updateInput();
		currCameraHeight = camera.z - my.z;
		camera.arc= FCarc + abs(camera.z-player.z)/100;
		updateActions();	
		
		
		
		if(cameraType == 1)
			updateCamera2(currCameraHeight);
		else
			updateCamera(currCameraHeight);
		wait(1);
		
	}
}
