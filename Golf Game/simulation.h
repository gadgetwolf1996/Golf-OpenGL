/*-----------------------------------------------------------
  Simulation Header File
  -----------------------------------------------------------*/
#include"vecmath.h"
#include <time.h>

/*-----------------------------------------------------------
  Macros
  -----------------------------------------------------------*/
#define TABLE_X			(0.6f) 
#define TABLE_Z			(1.2f)
#define TABLE_Y			(0.1f)
#define BALL_RADIUS		(0.05f)
#define BALL_MASS		(0.1f)
#define HOLE_RADIUS		(0.05f)
#define TWO_PI			(6.2832f)
#define	SIM_UPDATE_MS	(10)
#define NUM_BALLS		(2)		
#define NUM_CUSHIONS	(4)		
#define NUM_HOLES	(1)
#define MAX_PARTICLES	(200)

/*-----------------------------------------------------------
  plane normals
  -----------------------------------------------------------*/
/*
extern vec2	gPlaneNormal_Left;
extern vec2	gPlaneNormal_Top;
extern vec2	gPlaneNormal_Right;
extern vec2	gPlaneNormal_Bottom;
*/


/*-----------------------------------------------------------
  cushion class
  -----------------------------------------------------------*/
class cushion
{
public:
	vec2	vertices[2]; //2d
	vec2	centre;
	vec2	normal;

	void MakeNormal(void);
	void MakeCentre(void);
};


/*-----------------------------------------------------------
  holes class
  -----------------------------------------------------------*/
class hole
{
public:
	vec2	position; //2d
	float	radius;
	hole()
	{radius=HOLE_RADIUS; }
};



/*-----------------------------------------------------------
  ball class
  -----------------------------------------------------------*/

class ball
{
	static int ballIndexCnt;
public:
	vec2	position;
	vec2	velocity;
	float	radius;
	float	mass;
	int		index;
	bool in_hole;

	ball(): in_hole(0),position(0.0), velocity(0.0), radius(BALL_RADIUS),
		mass(BALL_MASS) {index = ballIndexCnt++; Reset();}
	
	void Reset(void);
	void ApplyImpulse(vec2 imp);
	void ApplyFrictionForce(int ms);
	void DoPlaneCollision(const cushion &c);
	int DoBallCollision(ball &b, int l);
	void Update(int ms);
	
	bool HasHitPlane(const cushion &c) const;
	bool HasHitBall(const ball &b) const;

	void HitPlane(const cushion &c);
	void HitBall(ball &b);
};

/*-----------------------------------------------------------
  table class
  -----------------------------------------------------------*/
class table
{
public:
	int level;
	float tx, tz;
	ball balls[NUM_BALLS];	
	cushion cushions[NUM_CUSHIONS];	
	hole holes[NUM_HOLES];
	void SetupCushions();
	void SetupHoles(void);
	void Update(int ms);	
	bool AnyBallsMoving(void) const;
};

/*-----------------------------------------------------------
  global table
  -----------------------------------------------------------*/
extern table gTable;
