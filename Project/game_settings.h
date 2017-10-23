//***************************
//*		DEFINES FOR GAME  	*
//***************************

//Events code
#define EV_NONE		0
#define EV_UP		1
#define EV_DOWN		2

//Graphics and game loop code
#define ADC_MAX			4096

#define LEVEL_WIDTH		1365
#define LEVEL_HEIGHT	1365

#define FLOOR_SPACE		400
#define TRANSITION_WIDTH 2000

#define SPEED_FACTOR	5


#define SPEED			(LEVEL_WIDTH/20/SPEED_FACTOR)
#define INTRO_SPEED		SPEED

// gs == game_state
#define STATE_NORMAL	0
#define STATE_DEAD		1
#define STATE_BEGIN		2

//Height variables
#define PROB_HEIGHT 0x003f
#define HEIGHT_LEVELS 5

//Height math
#define LH						(LEVEL_HEIGHT/14)
#define AFTER_TRAN_DX(i)		((i > gs_transition) ? TRANSITION_WIDTH : (i - (gs_transition - TRANSITION_WIDTH)))
#define TRAN_DX(i)				((i > (gs_transition - TRANSITION_WIDTH)) ? AFTER_TRAN_DX(i) : 0)
#define CENTER_FLOOR_Y(i)		(3*LH + gs_current_height*2*LH + TRAN_DX(i)*(gs_next_height - gs_current_height)*2*LH/TRANSITION_WIDTH)
#define TOP_FLOOR_Y(i)			(CENTER_FLOOR_Y(i) + FLOOR_SPACE/2)
#define BOTTOM_FLOOR_Y(i)		(CENTER_FLOOR_Y(i) - FLOOR_SPACE/2)

#define P_MOVE_NONE		EV_NONE
#define P_MOVE_UP		EV_UP
#define P_MOVE_DOWN		EV_DOWN


//Player math
#define P_DURATION		24
#define P_MOVE			110

#define P_ABS(i)		(i*4*P_MOVE/P_DURATION - i*i*4*P_MOVE/P_DURATION/P_DURATION)
#define P_X(i)			(LEVEL_WIDTH/2 + 0)
#define P_Y(i, x)		(CENTER_FLOOR_Y(x) + ((p_action == P_MOVE_UP) ? P_ABS(i) : ((p_action == P_MOVE_DOWN) ? -P_ABS(i) : 0)))

//Obstacles 
#define MAX_OBS		5
#define PROB_OBS	0x03ff

#define OBS_GRACE_PERIOD	40

#define TOBS_OFFSET		-90
#define BOBS_OFFSET		100

#define TOBS_X_COLLIDE	40
#define TOBS_Y_COLLIDE	40

#define BOBS_X_COLLIDE	40
#define BOBS_Y_COLLIDE	40

//Decor
#define DECO_OFFSET LEVEL_HEIGHT/200

#define MAX_DECO 10
#define PROB_DECO 0x01ff