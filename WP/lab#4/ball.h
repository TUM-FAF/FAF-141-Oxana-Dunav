#include <cmath>
#include <windows.h>

class Ball
{
public:
	POINT center;
	COLORREF color;
	int xVel, yVel;
    int randomColor[3];
    int shape; // 0 - circle, 1- rectangle

	Ball(POINT, const int);
	bool accelerateBall(const int, const int);
	bool setColor(const COLORREF);
	bool onBorderImpact(const RECT);
	bool moveBall(const HDC, const RECT, HBRUSH);
	COLORREF randColor();
};

bool checkBallCollision(Ball, Ball);
