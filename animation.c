#define mForward	1
#define mBackward	2
#define mTurnRight	4
#define mTurnLeft	8

#define runSpeed	34
#define walkSpeed	8

#define ActionWalk		1
#define ActionRun		2
#define ActionIdle		3
#define ActionFall		4
#define ActionTalk		5
#define ActionJump		6


#define WaitingTalk			1
#define Talking				2
#define TalkingOutOfRange	3

#define FallDistance skill28
#define FallVelocity skill29
#define GravityAccel skill30

#define CurrentAction skill31
#define PreviousAction skill32
#define AnimationPercent skill33

#define Running skill34
#define MovementAction skill35

#define Blending skill36
#define BlendingPercent skill37

#define SizeOffset	skill38
#define RadiusSize	skill39

#define TalkingState skill40
#define Isjump		skill41

////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	Handles the gravity	
////////////////////////////////////////////////////////////////////////////////////////////////////////////
function handle_gravity(var typeMovement){
VECTOR fromVec;
VECTOR toVec;
var downMovement;
vec_set(fromVec, me.x);
vec_set(toVec, me.x);
fromVec.z = fromVec.z -my.SizeOffset;
toVec.z = toVec.z - 5000;
if(c_trace(fromVec,toVec,IGNORE_ME|USE_AABB)>=0){
		if(trace_hit==1 && (normal.z >= ZNormalTolerance && normal.z <1)){
			typeMovement &= ~GLIDE;					
		}																							
	my.FallVelocity += (my.GravityAccel * time_step)/4;
	downMovement = c_move(me,vector(0,0,-my.FallVelocity),NULL,typeMovement);
	if(downMovement<=0){
		my.FallVelocity  = 0;
		my.FallDistance = 0;
		if (!key_space){
		my.Isjump = 0;}
	}
	else
		{
		my.FallDistance += downMovement; 
		my.Isjump = 2;}
	//Aqu� debemos de actualizar el c�digo de ca�da
	if(my.FallDistance>=80 && normal.z <=ZNormalTolerance) //If we are falling or sliding through a non climbable angle
		my.CurrentAction = ActionFall;
}
	
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	Handles entities movement
////////////////////////////////////////////////////////////////////////////////////////////////////////////

function jump_now()
{
if (me==player){
my.Isjump = 1;
var vertical_force = 35; 
var z1, z2;
while(vertical_force > 1) //while we have some jump force left
   {   
   z1 = my.z;         
   c_move(my, vector(0, 0, vertical_force * time_step), nullvector, 0);    
   z2 = my.z;
   camera.z += (z2-z1);       
   vertical_force -= 4.5 * time_step; // play with 2.5
   wait(1);
   }
my.Isjump = 2;}
}

function handle_movement(VECTOR* displace, VECTOR* absMovement, var otherFlags){
var typeGravity = IGNORE_MODELS | IGNORE_PASSABLE | GLIDE | otherFlags;

if(vec_length(displace)!=0){
	c_move(me,displace,absMovement,GLIDE | otherFlags);
	// Check if the angle is too steep
	if(trace_hit==1 && (normal.z >= ZNormalTolerance && normal.z <1)){
		typeGravity &= ~GLIDE;					
	}													
}
if (my.Isjump != 1){
handle_gravity(typeGravity);
if (key_space && my.Isjump ==0){
	jump_now();
} 
}
}




////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	Handles the animations
////////////////////////////////////////////////////////////////////////////////////////////////////////////
function handle_animation() {
STRING* animationName;
var animFactor;
var blendFactor;
var animStart = 0;
var animEnd = 100;
var animType = ANM_CYCLE;
switch(me.CurrentAction){
	case ActionIdle: 
		animationName = "default";
		animFactor = 1;
	//	break;
	case ActionRun: 
		animationName = "run";
		animFactor = 10;
	//	break;
	case ActionWalk: 
		animationName = "run";
		animFactor = 5;
	//	break;
	case ActionFall: 
		animationName = "falling";
		animFactor = 1;
		animType = 0;
	//	break;
}

if (my.Isjump == 1) {
	animationName = "jump";
		animFactor = 5;
		animType = 0;}

my.AnimationPercent += animFactor * time_step;
if (my.AnimationPercent > 100) my.AnimationPercent -= 100; 
  ent_animate(me,animationName,my.AnimationPercent,animType);

//	//	Action changed
//	if(my.CurrentAction!=my.PreviousAction){
//		my.Blending = 1;
//		my.BlendingPercent = 0;
//	}
//	//	Blend
//	if(my.Blending){
//		if(my.BlendingPercent>=100){					//	Blending ended
//			my.Blending = 0;							
//			my.BlendingPercent = 0;						
//			my.AnimationPercent = animStart;			//	Animation starts in animStart
//		} 
//		else{
//			ent_blend(animationName, animStart, my.BlendingPercent);	//	Blend animation
//			my.BlendingPercent += 45 * time_step;			
//		}
//	}
//	//	Animate when not blending
//	if(!my.Blending){
//		if (my.AnimationPercent > animEnd ){ 							//	Animation ended
//			if(animType == ANM_CYCLE)
//				my.AnimationPercent = animStart;						//	Restart if ciclic
//			else
//				my.AnimationPercent = animEnd;
//		}
//		ent_animate(player,animationName,my.AnimationPercent,animType);
//		my.AnimationPercent += animFactor * time_step;
//	}
my.PreviousAction = my.CurrentAction;								//	Update previous action
}