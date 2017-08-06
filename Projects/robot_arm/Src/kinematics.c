#include "kinematics.h"
#include <math.h>

typedef struct {
	uint16_t x;		// mm
	uint16_t y;		// mm
	uint16_t z;		// mm
} coord_cart_t;

typedef struct {
	double angle;	// radian
	uint16_t r;		// mm
	uint16_t z;		// mm
} coord_polar_t;

// All values in mm and rad

// Convert from degrees to radians
double deg_to_rad(int16_t deg)
{
	return ((double) deg / 360.0) * 2.0 * M_PI;
}

// Convert from radians to degrees
int16_t deg_to_rad(double rad)
{
	return (rad / (2.0 * M_PI)) * 360.0;
}


// Convert from polar to carthesian coordinates
void polar_to_cart(coord_polar_t* pos_polar, coord_cart_t* pos_cart)
{
	// Calculate coord X
	pos_cart->x = (double) (cos(pos_polar->angle)) * pos_polar->r;

	// Calculate coord Y
	pos_cart->y = (double) (sin(pos_polar->angle)) * pos_polar->r;

	// Calculate coord Z
	pos_cart->z = pos_polar->z;

	return;
}

// Convert from carthesian to polar coordinates
void cart_to_polar(coord_cart_t* pos_cart, coord_polar_t* pos_polar)
{
	// Calculate coord R
	pos_polar->r = sqrt(pow((double) pos_cart->x, 2.0) + pow((double) pos_cart->y, 2.0));

	// Calculate angle
	pos_polar->angle = atan2((double) pos_cart->x, (double) pos_cart->y);

	// Calculate coord Z
	pos_polar->z = pos_cart->z;

	return;
}






// l1: vertical arm
// l2: horizontal arm

// Convert xyz -> r, z, theta0



// Convert theta0, r, z -> xyz


// Calculate theta0, r, z from theta0, theta1, theta2


// Calculate theta0, theta1, theta2
