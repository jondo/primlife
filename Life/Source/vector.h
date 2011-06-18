//////////////////////////////////////////////////////////////////////
//
// vector.h
//
// Math functions associated with motion and acceleration
//

#pragma once
#include "Etools.h"

////////////////////////////////////////////////////////////////////////////////////
// Class Vector
//
//
//
const double RADIANS = PI / 180.0;
const double limit   = 2.0F;  // 3
const double rlimit  = 3.0F;  // 12 diameter of 100 rotating 8 deg

class Vector
{
protected:
    double dx;	// Rate of change of position X
    double dy;  // Rate of change of position Y
    double x;	// Our position X
    double y;   // Our position Y
    double dr;  // Rotation rate in degrees
    double r;   // Rotation position in degrees

	// These variables are calculated based on the difference
	// of the origin from the center of mass
	double drx; // Change position X based on rotation rate
	double dry; // Change position Y based on rotation rate

public:
    double mass;

public:

    Vector(void);

	virtual void Serialize(CArchive& ar);

    // Proposes movement in rotation, and translation
	// Always call tryRotate first!!
    int tryRotate(const POINT& origin, const POINT& center);
    int tryStepX();
    int tryStepY();

	// Actually make movement
    void makeStep(void);

    int GetX() { return (int) x; }
    int GetY() { return (int) y; }
    void backStepX(void) { x -= dx; }
    void backStepY(void) { y -= dy; }
    void backRotate(void){ r -= dr; }

    void invertDeltaY(void) { dy = -dy; }
    void invertDeltaX(void) { dx = -dx; }

	double CheckLimit(const double value) const
	{
		if (value > limit)
			return limit;
		else
			if (value < -limit)
				return -limit;

		return value;
	}
	
	double CheckRLimit(const double value) const
	{
		if (value > rlimit)
			return rlimit;
		else
			if (value < -rlimit)
				return -rlimit;

		return value;
	}
	
	void setDeltaX(const double DX)
	{
		dx = CheckLimit(DX);
	}
    void setDeltaX(const int DX)
	{
		dx = CheckLimit((double) DX);
	}
    void setDeltaY(const double DY)
	{
		dy = CheckLimit(DY);
	}
    void setDeltaY(const int DY)
	{
		dy = CheckLimit((double) DY);
	}
    void setDeltaRotate(const double DR)
	{ 
		dr = CheckRLimit(DR);
	}
    void setDeltaRotate(const int DR)
	{ 
		dr = CheckRLimit((double) DR);
	}
	void setMass(double MASS) { mass = MASS; }
	void addMass(double MASS) { mass += MASS; }

    void adjustDeltaX(double DX){dx += DX;}
    void adjustDeltaY(double DY)      { dy += DY; }
    void adjustDeltaRotate(double DR) { dr += DR; }

    void accelerateX(double ddx)
	{
		dx = CheckLimit(dx + ddx / mass);
		/*(((double)ddx) / mass);*/
	}

    void accelerateY(double ddy)
	{
		/*(((double)ddy) / mass);*/ 
		dy = CheckLimit(dy + ddy / mass);
	}
    void accelerateRotation(double ddr)
	{ 
		dr = CheckRLimit(dr + ddr / mass);
	}

    void friction(double fric) { dx -= (fric * dx); dy -= (fric * dy); dr -= (fric * dr);}

    double getDeltaX(void) const { return dx; }
    double getDeltaY(void) const { return dy; }
    double getDeltaRotate(void) { return dr;  }
    int getRotate(void)      { return (int) r; }

    void setX(int X)      { x = (double) X; }
    void setY(int Y)      { y = (double) Y; }
    void setRotate(int R) { r = (double) R; }

    double collisionX(Vector &enemy)
    {
      return ((mass - enemy.mass) * dx + 2 * enemy.mass * enemy.dx) / (mass + enemy.mass);
    }

	// Mass Impact equation from page 290 of J.P. Den Hartog "Mechanics"
	// Returns the resultant velocity vector
	double collisionResult(const double emass, const double DX, const double eDX) const
	{
		return ((mass - emass) * DX + 2 * emass * eDX) / (mass + emass);
	}

    double collisionY(Vector &enemy)
    {
      return ((mass - enemy.mass) * dy + 2 * enemy.mass * enemy.dy) / (mass + enemy.mass);
    }

    double distance(const int x1, const int y1) const { return sqrt((double)((x1*x1) + (y1*y1))); }
    double distance(const double x1, const double y1) const { return sqrt((x1*x1) + (y1*y1)); }

    double rotationComponent(const int x1, const int y1, const int x2, const int y2) const
    { 
      double dist = distance(x1, y1);
      if (dist)
        return ((double)((y1 * x2)-(x1 * y2))) / dist;
      else
        return 0;
    }

    double rotationComponent(double x1, double y1, double x2, double y2)
    { 
      double dist = distance(x1, y1);

      if (dist)
        return (((x1 * y2)-(y1 * x2))) / dist;
      else
        return 0;
    }

    double motionComponent(double vector, double rotation)
	{
		return (fabs(fabs(vector) - fabs(rotation)) < .0001)?0.0:sqrt((vector * vector) - (rotation * rotation));
	}

    double fraction(double motion, int x1, double center) { return (motion * (double)(x1)) / center; }

	// Circumference per degree times the number of degrees per turn provides a vector.
    double VectorR(const double radius) const { return (RADIANS * radius * dr); }

	// Cos (alpha) = deltaX / radius  &  Cos(alpha) = Yr / Vr, solved for Yr
    double deltaYr(const double Vr, const int deltaX, const double radius) const
	{
		return (radius != 0)?Vr * deltaX / radius: 0;
	}

    double deltaXr(const double Vr, const int deltaY, const double radius) const
	{
		return (radius != 0)?-Vr * deltaY / radius: 0;
	}

	// a positive dr causes the biot to move clockwise on the screen
	// a delta y below the origin (greater y value) is a negative deltaY
	void RotatedDelta(double& Vx, double& Vy, const int deltaX, const int deltaY, const double radius) const
	{
		// Current velocity vector at this radius
		double Vr = VectorR(radius);

		// Our cumulative velocity in the DX and DY directions
		// to impart to the other object.
		Vx = getDeltaX() + deltaXr(Vr, deltaY, radius);
		Vy = getDeltaY() + deltaYr(Vr, deltaX, radius);
	}                    
};


// Propose next rotation movement, but don't do it yet
inline int Vector::tryRotate(const POINT& origin, const POINT& center)
{
	CLine line(center, origin);

	double deltaC = distance(origin.x - center.x, origin.y - center.y);
	double deltaR = RADIANS * dr + line.Angle();

	drx = (center.x + (deltaC * cos(deltaR))) - origin.x;
	dry = (center.y + (deltaC * sin(deltaR))) - origin.y;

	return (int(r + dr) - int(r));
}


// Propose next X movement, but don't do it yet
inline int Vector::tryStepX(void)
{
	return int(x + dx + drx) - int(x);
}


// Propose next Y movement, but don't do it yet
inline int Vector::tryStepY(void)
{
	return int(y + dy + dry) - int(y);
}


// Actually make the step
inline void Vector::makeStep(void)
{
	int rx = int(drx);
//	drx -= double(vx);
	int ry = int(dry);
//	dry -= double(vy);
	x += (dx + double(drx));
	y += (dy + double(dry));
	r += dr;

	if (r >= 360.0)
	{
		r -= 360.0;
	}
	else
	{
		if (r <= -360.0)
			r += 360.0;
	}
}
