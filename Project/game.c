#include "game.h"

#define SHIFT_ONE_LEFT_DEC(arr, size) for(k=0; k < size-1; k++){arr[k] = arr[k+1];}size--;

uint8_t last_event;

static uint8_t gs_state = STATE_BEGIN; 
static uint8_t gs_sequence = 0;

static int8_t gs_current_height = 1;
static int8_t gs_next_height = 1;
static int16_t gs_transition = 0;

static uint8_t p_action = 0;
static uint8_t p_sequence = 0;

static uint8_t gs_obs_grace = 0;

static int16_t gs_tobs[MAX_OBS];
static uint8_t	 gs_tobs_num = 0;
static int16_t gs_bobs[MAX_OBS];
static uint8_t	 gs_bobs_num = 0;

static int16_t gs_deco_up[MAX_DECO];
static uint8_t	 gs_deco_up_num = 0;
static int16_t gs_deco_down[MAX_DECO];
static uint8_t	 gs_deco_down_num = 0;

void next_point(uint16_t x, uint16_t y)
{
	uint32_t value;
	x = (x < LEVEL_WIDTH) ? x : LEVEL_WIDTH-1;
	y = (y < LEVEL_HEIGHT) ? y : LEVEL_HEIGHT-1;
	value = ((y*ADC_MAX/LEVEL_HEIGHT)<<16) | (x*ADC_MAX/LEVEL_HEIGHT);

	xQueueSendToBack(draw_queue, &value, portMAX_DELAY);
}

uint8_t get_event()
{
	return last_event;
}

void game_reset()
{
	gs_state = STATE_BEGIN;
	gs_sequence = LEVEL_WIDTH/INTRO_SPEED;
	gs_tobs_num = 0;
	gs_bobs_num = 0;

	p_action = 0;
	p_sequence = 0;
	
	gs_obs_grace = 0;

	gs_deco_up_num = 0;
	gs_deco_down_num = 0;

	gs_current_height = 2;
	gs_next_height = 2;
	gs_transition = 0;

}

void game_end()
{
	gs_state = STATE_DEAD;
	gs_sequence = 50;
}

void game_loop()
{
	static uint16_t i, j, k;
	static uint16_t x, y;

	//Game state changes are made first

	//State at the beginning of the game
	if(gs_state == STATE_BEGIN){
		if(gs_sequence){
			gs_sequence--;
		}else{
			gs_state = STATE_NORMAL;
		}
	//Normal state, this is the state active most of the time	
	}else if(gs_state == STATE_NORMAL){
		//Manage events
		if(p_sequence){
			p_sequence--;
		}else{
			if(p_action){
				p_action = P_MOVE_NONE;
			}else{
				switch(get_event()){
				case EV_UP:
					p_action = P_MOVE_UP;
					p_sequence = P_DURATION;
					break;
				case EV_DOWN:
					p_action = P_MOVE_DOWN;
					p_sequence = P_DURATION;
					break;
				default:
					break;
				}
			}
		}

		//Manage elevation
		if(gs_transition > 0){
			gs_transition -= SPEED;
		}else{
			gs_transition = 0;
			gs_current_height = gs_next_height;
			if(rand() < PROB_HEIGHT){ //Change transition
				uint8_t decision = rand();
				if(decision&0x01){//Two options
					gs_next_height++;
					if(gs_next_height > HEIGHT_LEVELS - 1){
						gs_next_height = HEIGHT_LEVELS - 1;
					}else{
						gs_transition = TRANSITION_WIDTH + LEVEL_WIDTH;
					}
				}else{
					gs_next_height--;
					if(gs_next_height < 0){
						gs_next_height = 0;
					}else{
						gs_transition = TRANSITION_WIDTH + LEVEL_WIDTH;
					}
				}
			}
		}

		//Manage obstacles
		if(gs_obs_grace){
			gs_obs_grace--;
		}else{
			if(rand() < PROB_OBS){ //Add new potential tobs
				uint8_t decision = rand();
				if(decision&0x01 && gs_tobs_num < MAX_OBS){//Two options
					gs_tobs[gs_tobs_num++] = LEVEL_WIDTH;
				}else if(gs_bobs_num < MAX_OBS){
					gs_bobs[gs_bobs_num++] = LEVEL_WIDTH;
				}
				gs_obs_grace = OBS_GRACE_PERIOD;
			}
		}

		for(i = 0; i < gs_tobs_num; i++){ //Move tobs'
			gs_tobs[i] -= SPEED;
			if(gs_tobs[i] < 0){
				SHIFT_ONE_LEFT_DEC(gs_tobs, gs_tobs_num);
			}
		}

		for(i = 0; i < gs_bobs_num; i++){ //Move bobs'
			gs_bobs[i] -= SPEED;
			if(gs_bobs[i] < 0){
				SHIFT_ONE_LEFT_DEC(gs_bobs, gs_bobs_num);
			}
		}

		//Manage decos
		if(gs_deco_up_num < MAX_DECO){ //Add new potential up deco
			if(rand() < PROB_DECO){
				gs_deco_up[gs_deco_up_num++] = LEVEL_WIDTH;
			}
		}

		for(i = 0; i < gs_deco_up_num; i++){ //Move up decos
			gs_deco_up[i] -= SPEED;
			if(gs_deco_up[i] < 0){
				SHIFT_ONE_LEFT_DEC(gs_deco_up, gs_deco_up_num);
			}
		}

		if(gs_deco_down_num < MAX_DECO){ //Add new potential down deco
			if(rand() < PROB_DECO){
				gs_deco_down[gs_deco_down_num++] = LEVEL_WIDTH;
			}
		}

		for(i = 0; i < gs_deco_down_num; i++){ //Move down decos
			gs_deco_down[i] -= SPEED;
			if(gs_deco_down[i] < 0){
				SHIFT_ONE_LEFT_DEC(gs_deco_down, gs_deco_down_num);
			}
		}

		x = P_X(p_sequence);
		y = P_Y(p_sequence, x);
		//Check for collisions
		for(i = 0; i < gs_tobs_num; i++){ //Collision with tobs
			int16_t dx = x - gs_tobs[i];
			if( (dx < TOBS_X_COLLIDE) && (dx > -TOBS_X_COLLIDE) && (CENTER_FLOOR_Y(x) - y < TOBS_Y_COLLIDE) ){
				game_end();
			}
		}

		for(i = 0; i < gs_bobs_num; i++){ //Collision with bobs
			int16_t dx = x - gs_bobs[i];
			if( (dx < BOBS_X_COLLIDE) && (dx > -BOBS_X_COLLIDE) && (y - CENTER_FLOOR_Y(x) < BOBS_Y_COLLIDE) ){
				game_end();
			}
		}

	//This state occurs once the game ends
	}else if(gs_state == STATE_DEAD){
		if(gs_sequence){
			gs_sequence--;
		}else{
			game_reset();
		}
		
	}
	//Here we draw everything

	//First we draw the floor
	//This could be done before the event handling, so that we can draw while we process 
	j = (gs_state == STATE_BEGIN) ? gs_sequence*INTRO_SPEED : 0; //Used to draw entrance at beginning
	for(i = j; i < LEVEL_WIDTH; i += 2){ //Top floor
		x = i;
		y = TOP_FLOOR_Y(i);
		next_point(x, y);
	}
	for(i = j; i < LEVEL_WIDTH; i += 2){ //Bottom floor
		x = i;
		y = BOTTOM_FLOOR_Y(i);
		next_point(x, y);
	}

	//Then we draw the player
	x = P_X(p_sequence);
	y = P_Y(p_sequence, x);

	if(gs_state == STATE_DEAD){ //Code for exploding animation
		for(i = 0; i < ship_l; i++){
			if(rand() > (49-gs_sequence)*0x7ff){
				next_point(x + (ship_x[i]*(25+49-gs_sequence)/25), y + (ship_y[i]*(25+49-gs_sequence)/25));
			}
		}
	}else{
		for(i = 0; i < ship_l; i++){
			next_point(x + ship_x[i], y + ship_y[i]);
		}
	}

	//Finally, we draw the obstacles

	for(i = 0; i < gs_tobs_num; i++){
		x = gs_tobs[i];
		y = TOP_FLOOR_Y(x) + TOBS_OFFSET;
		for(j = 0; j < tobs_l; j++){
			next_point(x + tobs_x[j], y + tobs_y[j]);
		}
	}
	for(i = 0; i < gs_bobs_num; i++){
		x = gs_bobs[i];
		y = BOTTOM_FLOOR_Y(x) + BOBS_OFFSET;
		for(j = 0; j < bobs_l; j++){
			next_point(x + bobs_x[j], y + bobs_y[j]);
		}
	}

	//And add a bit of decoration
	for(i = 0; i < gs_deco_up_num; i++){
		x = gs_deco_up[i];
		y = TOP_FLOOR_Y(x) + DECO_OFFSET;
		next_point(x,y);
		next_point(x,y);
		next_point(x,y);
		next_point(x,y);
	}
	for(i = 0; i < gs_deco_down_num; i++){
		x = gs_deco_down[i];
		y = BOTTOM_FLOOR_Y(x) - DECO_OFFSET;
		next_point(x,y);
		next_point(x,y);
		next_point(x,y);
		next_point(x,y);
	}
}