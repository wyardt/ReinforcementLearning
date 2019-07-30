// agent_curve.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "pch.h"
#include <iostream>
#include <stdlib.h>

#define ROW 10
#define COL 10

typedef struct {
	uint16_t x;
	uint16_t y;
}Position_TypeDef;

typedef struct {
	Position_TypeDef current_position;
	Position_TypeDef target_position;
	float gamma = 0.9;
	uint32_t t;
	float cost;
}Agent_TypeDef;

typedef struct {
	float vTemp;
	bool target_reached;
}NextState_TypeDef;

typedef struct {
	Position_TypeDef top;
	Position_TypeDef bottom;
}Obstacle_TypeDef;

typedef enum {
	action_up = 0,
	action_down,
	action_left,
	action_right
}Action_TypeDef;

Agent_TypeDef agent;
Obstacle_TypeDef obs = {
{3, 3},
{COL - 1, 5}
};

bool X[ROW][COL] = { 0 };
float V[ROW][COL] = { 0 };
uint32_t T = 0;

NextState_TypeDef getToNext(float gamma, Agent_TypeDef agt);
Action_TypeDef findMax(float *v);

int main()
{
	agent.current_position = { 8, 9 };
	agent.target_position = { 2, 7 };
	agent.cost = -0.0;
	agent.gamma = 0.999;

	/* init value function */
	for (uint16_t i = 0; i < ROW; i++)
	{
		for (uint16_t j = 0; j < COL; j++)
		{
			V[i][j] = 0.0;// (rand() % 100 + 1) / 100.0;
		}
		V[agent.target_position.x][agent.target_position.y] = 1;
	}
	std::cout << "init value function is\n";
	for (uint16_t i = 0; i < ROW; i++)
	{
		for (uint16_t j = 0; j < COL; j++)
		{
			if ((i >= obs.top.y) && (i <= obs.bottom.y)
				&& (j >= obs.top.x) && (j <= obs.bottom.x))
			{
				//std::cout << 0;
				printf("%9.9f-", 0);
			}
			else
			{
				//std::cout << V[i][j];
				printf("%9.9f-", V[i][j]);
			}
		}
		std::cout << '\n';
	}
	
	/* generate map X */
	
	std::cout << "map is \n";
	for (uint16_t i = 0; i < ROW; i++)
	{
		for (uint16_t j = 0; j < COL; j++)
		{
			if ((i >= obs.top.y) && (i <= obs.bottom.y)
				&& (j >= obs.top.x) && (j <= obs.bottom.x))
			{
				X[i][j] = false;
			}
			else
			{
				X[i][j] = true;
			}
			std::cout << ((X[i][j] == true) ? ((i == agent.target_position.x && j == agent.target_position.y) ? "E" : ((i == agent.current_position.x && j == agent.current_position.y) ? "S" : "O")) : "X");
		}
		std::cout << '\n';
	}

	float Vtemp[ROW][COL] = { 0 };

	while (T++ < 100)
	{
		for (uint16_t i = 0; i < ROW; i++)
		{
			for (uint16_t j = 0; j < COL; j++)
			{
				if (X[i][j] == true)
				{
					uint16_t step = 1;
					float valueT = 0;
					agent.current_position.x = i;
					agent.current_position.y = j;
					agent.t = 0;
					Vtemp[i][j] = (getToNext(agent.gamma, agent)).vTemp;
				}
			}
		}
#if 1
		bool shouldBeNextTime = false;
		for (uint16_t ii = 0; ii < ROW; ii++)
		{
			for (uint16_t jj = 0; jj < ROW; jj++)
			{
				if (abs(V[ii][jj] - Vtemp[ii][jj]) > 0.000001)
				{
					V[ii][jj] = Vtemp[ii][jj]; /* need renew next time */
					shouldBeNextTime = true;
				}
			}
		}
		if (false == shouldBeNextTime)
		{
			break;
		}
#endif
	}
	std::cout << "value function is\n";
	for (uint16_t i = 0; i < ROW; i++)
	{
		for (uint16_t j = 0; j < COL; j++)
		{
			if ((i >= obs.top.y) && (i <= obs.bottom.y)
				&& (j >= obs.top.x) && (j <= obs.bottom.x))
			{
				//std::cout << 0;
				printf("%9.9f-", 0);
			}
			else
			{
				//std::cout << V[i][j];
				printf("%9.9f-", V[i][j]);
			}
		}
		std::cout << '\n';
	}
}


NextState_TypeDef getToNext(float gamma, Agent_TypeDef agt)
{
	NextState_TypeDef result = {
		0.0,
		false
	};
	Position_TypeDef p_save;

	Position_TypeDef *p = &agt.current_position;
	float v[4];
	Action_TypeDef act;

	/* get to the target position, exit */
	if ((agt.current_position.x == agent.target_position.x) && (agt.current_position.y == agent.target_position.y))
	{
		result.target_reached = true;
		result.vTemp = 1.0;
		return result;
	}

	/* find next move's value */
	if ((p->y != 0) && (X[p->x][p->y - 1] != false))
	{
		v[action_up] = V[p->x][p->y - 1];
	}
	if ((p->y != ROW - 1) && (X[p->x][p->y + 1] != false))
	{
		v[action_down] = V[p->x][p->y + 1];
	}
	if ((p->x != 0) && (X[p->x - 1][p->y] != false))
	{
		v[action_left] = V[p->x - 1][p->y];
	}
	if ((p->x != COL - 1) && (X[p->x + 1][p->y] != false))
	{
		v[action_right] = V[p->x + 1][p->y];
	}

	act = findMax(v);

	p_save.x = p->x;
	p_save.y = p->y;

	/* update value function of current position */
	switch (act)
	{
	case action_up:
		//V[p->x][p->y] = gamma * V[p->x][p->y - 1];
		//p->y--;
		p->y--;
		break;
	case action_down:
		//V[p->x][p->y] = gamma * V[p->x][p->y + 1];
		//p->y++;
		p->y++;
		break;
	case action_left:
		//V[p->x][p->y] = gamma * V[p->x - 1][p->y];
		//p->x--;
		p->x--;
		break;
	case action_right:
		//V[p->x][p->y] = gamma * V[p->x + 1][p->y];
		//p->x++;
		p->x++;
		break;
	default:
		break;
	}
	//V[p_save.x][p_save.y] = (getToNext(agt->gamma * gamma, agt)).vTemp + agt->cost;
	
	/* update the old value function */
	//V[p_save.x][p_save.y] = agt->gamma * V[p->x][p->y];
	V[p_save.x][p_save.y] += gamma * (getToNext(agent.gamma * gamma, agt)).vTemp;

	/* calculate the new value function */
	result.vTemp = (getToNext(agent.gamma * gamma, agt)).vTemp + agent.cost;

	return result;
}
#if 0
{
	NextState_TypeDef result = {
		0.0,
		false
	};
	Position_TypeDef p_save;

	Position_TypeDef *p = &agt->current_position;
	float v[4];
	Action_TypeDef act;

	agt->t++;

	/* get to the target position, exit */
	if ((agt->current_position.x == agt->target_position.x) && (agt->current_position.y == agt->target_position.y))
	{
		result.target_reached = true;
		result.vTemp = 1.0;
		return result;
	}

	/* find next move's value */
	if ((p->y != 0) && (X[p->x][p->y - 1] != false))
	{
		v[action_up] = V[p->x][p->y - 1];
	}
	if ((p->y != ROW - 1) && (X[p->x][p->y + 1] != false))
	{
		v[action_down] = V[p->x][p->y + 1];
	}
	if ((p->x != 0) && (X[p->x - 1][p->y] != false))
	{
		v[action_left] = V[p->x - 1][p->y];
	}
	if ((p->x != COL - 1) && (X[p->x + 1][p->y] != false))
	{
		v[action_right] = V[p->x + 1][p->y];
	}

	act = findMax(v);

	p_save.x = p->x;
	p_save.y = p->y;

	/* update value function of current position */
	switch (act)
	{
	case action_up:
		//V[p->x][p->y] = gamma * V[p->x][p->y - 1];
		//p->y--;
		p->y--;
		break;
	case action_down:
		//V[p->x][p->y] = gamma * V[p->x][p->y + 1];
		//p->y++;
		p->y++;
		break;
	case action_left:
		//V[p->x][p->y] = gamma * V[p->x - 1][p->y];
		//p->x--;
		p->x--;
		break;
	case action_right:
		//V[p->x][p->y] = gamma * V[p->x + 1][p->y];
		//p->x++;
		p->x++;
		break;
	default:
		break;
	}
	//V[p_save.x][p_save.y] = (getToNext(agt->gamma * gamma, agt)).vTemp + agt->cost;

	/* update the old value function */
	//V[p_save.x][p_save.y] = agt->gamma * V[p->x][p->y];
	V[p_save.x][p_save.y] += gamma * (getToNext(agt->gamma * gamma, agt)).vTemp;

	/* calculate the new value function */
	result.vTemp = (getToNext(agt->gamma * gamma, agt)).vTemp + agt->cost;

	return result;
}
#endif

Action_TypeDef findMax(float *v)
{
	uint16_t i = 0;

	if ((v[0] == 0) && (0 == v[1]) && (0 == v[2]) && (0 == v[3]))
	{
		uint16_t r = rand() % 3;
		return (Action_TypeDef)r;
	}

	if (v[0] > v[1])
	{
		i = 0;
	}
	else
	{
		i = 1;
	}
	if (v[2] > v[3])
	{
		if (v[2] > v[i])
		{
			return (Action_TypeDef)2;
		}
	}
	else
	{
		if (v[3] > v[i])
		{
			return (Action_TypeDef)3;
		}
	}
	return (Action_TypeDef)i;
}
// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门提示: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
