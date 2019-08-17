#ifndef AGENT_H_
#define AGENT_H_

#include <iostream>
#include <stdlib.h>
#include <cmath>

#define Y_MAX 10
#define X_MAX 10

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
	double gamma = 0.9;
	uint32_t t;
	double cost;
}Agent_TypeDef;

typedef struct {
	double reward;
	bool target_reached;
	Position_TypeDef nextPos;
}NextState_TypeDef;

typedef struct {
	double v[action_space_scale];
}Action_ValueTypeDef;

/* rectangular obstacle */
typedef struct {
	Position_TypeDef top;  /* top-left corner coordinate of the obstacle */
	Position_TypeDef bottom;  /* right-bottom corner coordinate of the obstacle */
}Obstacle_TypeDef;


class QLearning {
public:
	QLearning();
	NextState_TypeDef moveOneStep(Position_TypeDef pos, Action_TypeDef action);
	NextState_TypeDef iterator(Position_TypeDef pos, double discountOverrall);
	
	bool stateIsInRange(uint16_t x, uint16_t y);
	uint16_t updateQ(uint16_t x, uint16_t y, double q);
	void printQ(void);
	uint16_t lambda;

	double discount;
    double alpha;
    
	bool reachTarget(Position_TypeDef pos);
private:

	Obstacle_TypeDef obs;
	Position_TypeDef current_position;
	Position_TypeDef target_position;
	bool X[Y_MAX][X_MAX];
	double Q[Y_MAX][X_MAX];

	Action_TypeDef findMax(double *v);
};

#endif