
#include <acknex.h>
#include <default.c>
#include <litec.h>

function main()
{
  vec_set(screen_size,vector(800,400,0));
  vec_set(screen_color,vector(50,1,1)); // dark blue
  vec_set(sky_color,vector(50,1,1)); // dark blue
  video_window(NULL,NULL,0,"My New Game");
  d3d_antialias = 1;
  shadow_stencil = 3;
  
  //level_load("colAngel.wmb");
  vec_set(camera.x,vector(-250,0,50));
  vec_set(camera.pan,vector(0,-15,0));

}
