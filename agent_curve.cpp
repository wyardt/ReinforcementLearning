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
}Agent_TypeDef;

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

float findNextBiggestReward(Agent_TypeDef *agt, uint16_t step);
int main()
{
	/* init value function */
	for (uint16_t i = 0; i < ROW; i++)
	{
		for (uint16_t j = 0; j < COL; j++)
		{
			V[i][j] = rand()/ RAND_MAX;
		}
		V[agent.target_position.x][agent.target_position.y] = 1;
	}
	/* generate map X */
	agent.current_position = { 8, 9 };
	agent.target_position = { 2, 7 };

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

	for (uint16_t ii = 0; ii < ROW; ii++)
	{
		for (uint16_t jj = 0; jj < ROW; jj++)
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
						//while ((valueT = findNextBiggestReward(&agent, step)) > 0.000001)
						while (step < 200)
						{
							valueT = findNextBiggestReward(&agent, step);
							step++;
							V[i][j] += valueT;
						}
						;
					}
				}
			}
			if (abs(V[ii][jj] - Vtemp[ii][jj]) > 0.000001)
			{
				;
			}
			else
			{
				break;
			}
		}
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

Action_TypeDef findMax(float *v)
{
	uint16_t i = 0;

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

float findNextBiggestReward(Agent_TypeDef *agt, uint16_t step)
{
	Action_TypeDef act;
	Position_TypeDef *p;
	float v[4] = { 0 };
	p = &agt->current_position;


	/* exit when reach the target position? */
	if ((p->x == agt->target_position.x) && (p->y == agt->target_position.y))
		return 0.0;

	if ((p->y != 0) && (X[p->x][p->y - 1] != false))
	{
		v[action_up] = V[p->x][p->y - 1];
	}
	if ((p->y < ROW - 1) && (X[p->x][p->y + 1] != false))
	{
		v[action_down] = V[p->x][p->y + 1];
	}
	if ((p->x != 0) && (X[p->x + 1][p->y] != false))
	{
		v[action_right] = V[p->x + 1][p->y];
	}
	if ((p->x != COL - 1) && (X[p->x - 1][p->y] != false))
	{
		v[action_left] = V[p->x - 1][p->y];
	}

	switch (act = findMax(v))
	{
	case action_up:
		p->y--;
		break;
	case action_down:
		p->y++;
		break;
	case action_left:
		p->x--;
		break;
	case action_right:
		p->x++;
		break;
	default:
		break;
	}

	float reward_this_step = v[act];
	if (step == 1)
	{
		return reward_this_step;
	}
	step--;
	while (step != 0)
	{
		reward_this_step = agt->gamma * reward_this_step;
		step--;
	}
	return reward_this_step;
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
