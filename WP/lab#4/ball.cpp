#include "ball.h"

Ball::Ball(POINT pt, const int velocity)
{
	this->center = pt;
	this->xVel	 = velocity;
	this->yVel	 = velocity;
	this->color	 = randColor();
}

bool Ball::accelerateBall(const int dXVel, const int dYVel)
{
	xVel = dXVel;
	yVel = dYVel;
	return true;
}

bool Ball::setColor(const COLORREF clr)
{
	this->color = clr;
	return true;
}

bool Ball::onBorderImpact(const RECT rct)
{

	if(center.x + 25 > rct.right - 1){
	    xVel = -abs(xVel);
	    shape = 1;
    }

	if(center.x - 25 < rct.left + 1){
        xVel = abs(xVel);
        shape = 1;
    }

	if(center.y + 25 > rct.bottom - 1){
	    yVel = -abs(yVel);
    }

	if(center.y - 25 < rct.top + 1){
	    yVel = abs(yVel);
    }
	return true;
}

bool Ball::moveBall(const HDC hDC, const RECT rct, HBRUSH hBrush)
{
	hBrush = CreateSolidBrush(color);
	SelectObject(hDC, hBrush);

	onBorderImpact(rct);
	center.x += xVel;
	center.y += yVel;

	if(shape == 1)
    {
        Rectangle(hDC, center.x - 25, center.y - 25, center.x + 25, center.y + 25 );
        SelectObject(hDC, GetStockObject(WHITE_BRUSH));
        DeleteObject(hBrush);
        return true;
    }

	Ellipse(hDC, center.x - 25, center.y - 25, center.x + 25, center.y + 25);
	SelectObject(hDC, GetStockObject(WHITE_BRUSH));
	DeleteObject(hBrush);
	return true;
}

COLORREF Ball::randColor()
{
    randomColor[0] = rand()%255;
    randomColor[1] = rand()%255;
    randomColor[2] = rand()%255;
    return RGB(randomColor[0], randomColor[1], randomColor[2]);
}

bool checkBallCollision(Ball *b1, Ball *b2)
{
	float dist;
	POINT accumVel;

	dist = sqrt(pow((b1->center.x - b2->center.x), 2) + pow((b1->center.y - b2->center.y), 2));


	if(dist < 53)
	{
		b1->xVel = -abs(b1->xVel);
		b1->yVel = -abs(b1->yVel);

		b2->xVel = -abs(b2->xVel);
		b2->yVel = -abs(b2->yVel);

		accumVel.x = (abs(b1->xVel) + abs(b2->xVel)) / 2;
		accumVel.y = (abs(b1->yVel) + abs(b2->yVel)) / 2;

		b1->accelerateBall(-b1->xVel * accumVel.x / abs(b1->xVel), -b1->yVel * accumVel.y / abs(b1->yVel));
		b2->accelerateBall(-b2->xVel * accumVel.x / abs(b2->xVel), -b2->yVel * accumVel.y / abs(b2->yVel));
        b1->setColor(RGB(0,0,255));
	}


	return true;
}


