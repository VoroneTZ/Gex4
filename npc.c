
function npc_movement(var* pathDist){
	VECTOR vDir;
	VECTOR newPosition;
	var npcDisplacement = walkSpeed * time_step;
	//Calculate new position

	path_spline(me,newPosition,*pathDist);
	newPosition.z = my.z;	// Adjust the height of the direction vector
	*pathDist += npcDisplacement;
	//Get direction (remove tilt)
	vec_diff(vDir,newPosition,my.x);
	vec_to_angle(me.pan,vDir);
	my.tilt = 0;
	me.CurrentAction = ActionWalk;
	
	//Move NPC
	handle_movement(vector(npcDisplacement,0,0), NULL, 0);
}
 
function npc_talk(){
	var talking = false;
	var dist = 0;
	if(my.TalkingState == WaitingTalk || my.TalkingState == Talking){
		dist = c_scan(me.x, me.pan, vector(180,0,100),IGNORE_ME | IGNORE_PASSABLE | SCAN_ENTS | SCAN_LIMIT);
		if(dist<=0){
			my.TalkingState = TalkingOutOfRange;
		}
		else talking = true;
	}
	if(my.TalkingState==WaitingTalk)
		my.CurrentAction = ActionIdle;
	if(my.TalkingState==Talking)
		my.CurrentAction = ActionTalk;
	return talking;
}

function npc_event(){
  	switch (event_type){
    	case EVENT_TRIGGER:
	    	my.TalkingState = WaitingTalk;
	      	return;
	}
}

action npc_action(){
	//	Adjust collision parameters
	wait(1);	
	my.eflags |= FAT;
	my.SizeOffset = 56;
	my.RadiusSize = 28;
	my.GravityAccel = 2;
	my.trigger_range = 200;

	my.emask |= ENABLE_TRIGGER;
	my.event = npc_event;
	my.trigger_range = 200;
	
	vec_set(my.min_x,vector(-my.RadiusSize,-my.RadiusSize,-my.SizeOffset));
	vec_set(my.max_x,vector(my.RadiusSize,my.RadiusSize,my.SizeOffset));
	var pathDist = 0;		//Stores walked distance over the path

	set(my,SHADOW|CAST);
	
	while(1) {
		if(!npc_talk())
			//my.CurrentAction = ActionIdle;
			npc_movement(pathDist);
		handle_animation();
		wait(1);
	}
}