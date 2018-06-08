#include <acknex.h>
#include <default.c>

#include "commons.c"
#include "animation.c"
#include "player.c"
//#include "enemy.c"
#include "npc.c"

var PlayerLife = 4;
var PlayerHealth = 4;
var PlayerHitTimer = 0;
var FMusic;


ENTITY* ent_sky;
function player_death()
{
	printf("Game over");
	sys_exit("666");
}
function main(){
	camera.clip_near = 0;
	video_window(vector(0,0,0),vector(0,0,0),0,"Return Of The Gecko v0.0.1");	
//	video_switch(12,0,2);
	video_screen = 0;
	video_mode = 12;
	
	shadow_stencil = 1;
	mouse_mode = 4;
	level_load("md.wmb");
	ent_sky = ent_createlayer("spacecube1+6.bmp", SKY | CUBE, 1);  
	on_f1 = showConfig;
// button_state(configPanel,1,ON); 
	wait(2);
	FMusic =	media_loop("Echoes_of_Time.mp3",NULL,50);
	while(1)
	{
		wait(1);

		if (player.z<=-400){player.z = 0; player.x = 0; player.y = 0;camera.z = player.z+170; PlayerLife -= 1; 	if (PlayerLife>-1)
			{
	PlayerHealth = 4;
				} 
			else 
			{player_death();}}
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

function player_hit()
{
	if (PlayerHitTimer==0)
	{
		PlayerHitTimer= 50;
		PlayerHealth -= 1;		
		if (PlayerHealth<1)
		{
			PlayerHealth=4;
			PlayerLife -=1;
			if (PlayerLife>0)
			{
				player.x = 0;
				player.y = 0;
				player.z = 0;
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
      
   }
}

PANEL* panel_hud =
{
	pos_x = 10;
	pos_y = 10;
	digits(20,20, "Life: %.0f", "Arial#20bi", 1, PlayerLife); 
   digits(20,50, "Health: %.0f", "Arial#20bi", 1, PlayerHealth); 
	flags = SHOW | OUTLINE;
}

action TVlevel1()
{
	while (1)
	{
		wait(-1);
		if (vec_dist(my.x,player.x)<=150){break;	}
	}
	ent_remove(ent_sky);
	level_load("l1.wmb");
	media_stop(FMusic);	
	FMusic =	media_loop("Day_of_Chaos.mp3",NULL,50);
	fog_color = 1; 
	camera.fog_start = 1000; 
	camera.fog_end = 1500;
	sky_color.red = 0;
   sky_color.green = 0;
   sky_color.blue = 0; 
}