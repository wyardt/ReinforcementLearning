#ifndef AGENT_H_
#define AGENT_H_

#include <iostream>
#include <stdlib.h>
#include <cmath>

#define ROW 10
#define COL 10

typedef enum {
	action_up = 0,
	action_down,
	action_left,
	action_right,
	action_space_scale
}Action_TypeDef;

typedef struct {
	uint16_t x;
	uint16_t y;
}Position_TypeDef;

typedef struct {
	Position_TypeDef start_position;
	Position_TypeDef current_position;
	Position_TypeDef target_position;
	float gamma = 0.9;
	uint32_t t;
	float cost;
}Agent_TypeDef;

typedef struct {
	float vTemp[action_space_scale];
	bool target_reached;
	Position_TypeDef next[action_space_scale];
}NextState_TypeDef;

typedef struct {
	float v[action_space_scale];
}Action_ValueTypeDef;

/* rectangular obstacle */
typedef struct {
	Position_TypeDef top;  /* top-left corner coordinate of the obstacle */
	Position_TypeDef bottom;  /* right-bottom corner coordinate of the obstacle */
}Obstacle_TypeDef;


class Qfunc {
	public:
	Qfunc();
	NextState_TypeDef moveOneStep(Position_TypeDef pos, Action_TypeDef action);
	float Q[ROW][COL];
	
	Agent_TypeDef agent;
	bool X[ROW][COL];
	private:
	Position_TypeDef currentPosition;
	bool reachTarget(Position_TypeDef *pos);
};

#endif