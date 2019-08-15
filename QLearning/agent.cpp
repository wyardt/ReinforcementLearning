#include "agent.h"



int main()
{
	float tempQ[Y_MAX][X_MAX] = { 0 };
	QLearning ql;
	uint16_t t = 1;

	while (1)
	{
		for (uint16_t y = 0; y < Y_MAX; y++)
		{
			for (uint16_t x = 0; x < X_MAX; x++)
			{
				Position_TypeDef cp;
				cp.x = x;
				cp.y = y;
				ql.lambda = 1;
					
				if (ql.stateIsInRange(y, x) == true)
				{
					NextState_TypeDef ns = ql.iterator(cp, ql.discount);

					tempQ[y][x] = ns.reward;
				}
			}
		}

			

		uint16_t shouldBeNextTime = 0;

		for (uint16_t yy = 0; yy < Y_MAX; yy++)
		{
			for (uint16_t xx = 0; xx < Y_MAX; xx++)
			{
				shouldBeNextTime += ql.updateQ(yy, xx, tempQ[yy][xx]);
			}
		}

		ql.printQ();

		if (0 == shouldBeNextTime)
		{
			break;
		}
	}
	/* todo draw the ultimate footprint */
}

QLearning::QLearning()
{
	target_position.x = 8;
	target_position.y = 2;
	discount = 0.999;
	obs.top.x = 3;
	obs.top.y = 3;
	obs.bottom.x = X_MAX - 1;
	obs.bottom.y = 6;
	lambda = 1;     /* only one step forward, which is TD(0) */

	/* init Q function */
	for (uint16_t y = 0; y < Y_MAX; y++)
	{
		for (uint16_t x = 0; x < X_MAX; x++)
		{
			Q[y][x] = 0.0;
		}
	}

	std::cout << "map is \n";
	for (uint16_t y = 0; y < Y_MAX; y++)
	{
		for (uint16_t x = 0; x < X_MAX; x++)
		{
			if ((y >= obs.top.y) && (y <= obs.bottom.y)
				&& (x >= obs.top.x) && (x <= obs.bottom.x))
			{
				X[y][x] = false;
			}
			else
			{
				X[y][x] = true;
			}
			std::cout << ((X[y][x] == true) ?
                                ((y == target_position.y && x == target_position.x) ? "E" : "O") : "X");
		}
		std::cout << '\n';
	}
}

bool QLearning::reachTarget(Position_TypeDef pos)
{
	if ((pos.x == target_position.x) && (pos.y == target_position.y))
	{
		return true;
	}
	else
	{
		return false;
	}
}

uint16_t QLearning::updateQ(uint16_t y, uint16_t x, double q)
{
	uint16_t ret = 0;

	if ((Q[y][x] - q < -0.000001) || (Q[y][x] - q > 0.000001))
	{
		/* need renew next time */
		ret = 1;
	}
	Q[y][x] = q;
	return ret;
}

bool QLearning::stateIsInRange(uint16_t y, uint16_t x)
{
	return ((X[y][x] == true) && ((y != target_position.y) || (x != target_position.x)));
}

void QLearning::printQ(void)
{
	std::cout << "Q function is\n";
	for (uint16_t y = 0; y < Y_MAX; y++)
	{
		for (uint16_t x = 0; x < X_MAX; x++)
		{
			if ((y >= obs.top.y) && (y <= obs.bottom.y)
				&& (x >= obs.top.x) && (x <= obs.bottom.x))
			{
				printf("%9.9f-", 0);
			}
			else
			{
				printf("%9.9f-", Q[y][x]);
			}
		}
		std::cout << '\n';
	}
}

NextState_TypeDef QLearning::moveOneStep(Position_TypeDef currentPos, Action_TypeDef action)
{
	Position_TypeDef nextPosition = currentPos;
	float retQvalue = 0.0;

	switch (action)
	{
	case action_up:
		if ((currentPos.y != 0) && (X[currentPos.y - 1][currentPos.x] != false))
		{
			currentPos.y--;
		}
		break;
	case action_down:
		if ((currentPos.y != Y_MAX - 1) && (X[currentPos.y + 1][currentPos.x] != false))
		{
			currentPos.y++;
		}
		break;
	case action_left:
		if ((currentPos.x != 0) && (X[currentPos.y][currentPos.x - 1] != false))
		{
			currentPos.x--;
		}
		break;
	case action_right:
		if ((currentPos.x != X_MAX - 1) && (X[currentPos.y][currentPos.x + 1] != false))
		{
			currentPos.x++;
		}
		break;
	default:
		break;
	}

	NextState_TypeDef ret;

	if (reachTarget(currentPos) != false)
	{
		ret.reward = 1.0;
		ret.target_reached = true;
	}
	else
	{
		ret.target_reached = false;
		ret.reward = 0.0 + discount * Q[currentPos.y][currentPos.x];
	}
	ret.nextPos = currentPos;
	return ret;
}

NextState_TypeDef QLearning::iterator(Position_TypeDef cp, float discountOverrall)
{
#if 1
	if (reachTarget(cp) == true)
	{
		NextState_TypeDef rewardFound;
		rewardFound.reward = 1.0;
		rewardFound.target_reached = true;
		return rewardFound;
	}
#endif	
	if (lambda == 0)
	{
		NextState_TypeDef rewardNotFound;
		rewardNotFound.target_reached = false;
		rewardNotFound.reward = 0.0;
		return rewardNotFound;
	}

	lambda--;

	/* scan all action's reward */			
	NextState_TypeDef nextQ[action_space_scale];

	/* get next Q's position and reward */
	nextQ[action_down] = moveOneStep(cp, action_down);
	nextQ[action_up] = moveOneStep(cp, action_up);
	nextQ[action_left] = moveOneStep(cp, action_left);
	nextQ[action_right] = moveOneStep(cp, action_right);
	
	float QNext[action_space_scale];
#if 0
	QNext[action_down] = nextQ[action_down].reward;
	QNext[action_up] = nextQ[action_up].reward;
	QNext[action_left] = nextQ[action_left].reward;
	QNext[action_right] = nextQ[action_right].reward;
#endif
	/* get next Q's value */
	QNext[action_down] = nextQ[action_down].reward;// + discountOverrall * Q[nextQ[action_down].nextPos.x][nextQ[action_down].nextPos.y];
	QNext[action_up] = nextQ[action_up].reward;// + discountOverrall * Q[nextQ[action_up].nextPos.x][nextQ[action_up].nextPos.y];
	QNext[action_left] = nextQ[action_left].reward;// + discountOverrall * Q[nextQ[action_left].nextPos.x][nextQ[action_left].nextPos.y];
	QNext[action_right] = nextQ[action_right].reward;// + discountOverrall * Q[nextQ[action_right].nextPos.x][nextQ[action_right].nextPos.y];

	/* find the most valuable action */
	Action_TypeDef act = findMax(QNext);
#if 0
	if (nextQ[act].target_reached == true)
	{
		return nextQ[act];
	}
#endif
	

	NextState_TypeDef ns = iterator(nextQ[act].nextPos, discount * discountOverrall);

	ns.reward = ns.reward + discountOverrall * Q[nextQ[act].nextPos.y][nextQ[act].nextPos.x];

	return ns;
}

Action_TypeDef QLearning::findMax(float *v)
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
