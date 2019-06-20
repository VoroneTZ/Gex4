
#define MOUSE_FACTOR		-20		// Change sign to turn in reverse direction
#define MIN_DIF_2_MOVE		10		// Minimum angle difference to start moving

ENTITY* cameraEntity;
ENTITY* iconEntity; 
var CAMERA_OFFSET =		380;
var farc=0;
var FCarc=80;

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
