#define CAMERA_SCAN	0
#define CAMERA_ENTITY	1

var ZNormalTolerance = 0.71;	// Maximum angle considered as climbable
var cameraType = CAMERA_SCAN;	// Camera type
function changeCamera(var clickedButton);
function closeConfig();


////////////////////////////////////////////////////////////////////////////////////////////////////
// Configuration Panel
////////////////////////////////////////////////////////////////////////////////////////////////////

PANEL* configPanel = {
	bmap = "confPan.tga";
    pos_x = 10;
    pos_y = 10;
    digits (15, 85, 1.4, *, 1, ZNormalTolerance);
    hslider(20, 95, 110, "slider.tga", 0, 1, ZNormalTolerance);
    
    button_radio(45, 165, "radioOn.tga", "radioOff.tga", "radioOn.tga", changeCamera, NULL, NULL);
    button_radio(115, 165, "radioOn.tga", "radioOff.tga", "radioOn.tga", changeCamera, NULL, NULL);
    
    button(50, 220, "buttonOn.bmp", "buttonOff.bmp", "buttonOff.bmp", closeConfig, NULL, NULL);
    
    flags = OVERLAY;
}
function changeCamera(var clickedButton){
	if(clickedButton==1)
		cameraType = CAMERA_SCAN;
	else
		cameraType = CAMERA_ENTITY;
}

function closeConfig(){
	configPanel.flags &= ~VISIBLE;
}

function showConfig(){
	configPanel.flags |= VISIBLE;
}

function WrapAngle(var currentAngle){
	currentAngle %= 360;
	if (currentAngle < 0) currentAngle += 360;
	return 	currentAngle;
}

function SmoothAngle(var targetAng, var currentAng) {
	targetAng = WrapAngle(targetAng);
	currentAng = WrapAngle(currentAng);
	var resultAng = currentAng - targetAng;
	//	lowest angle
	if (resultAng > 180) resultAng -= 360;
	if (resultAng < -180) resultAng -= -360;
	resultAng /= 4;			//	smooth factor
	resultAng *= time_step;
	resultAng = WrapAngle(currentAng - resultAng);
	return resultAng;
}
