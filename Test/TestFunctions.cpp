#include <cmath>
#include "TestFunctions.h"
#include "../Core/Constants.h"

using namespace cagd;
using namespace std;

GLdouble spiral_on_cone::u_min = -TWO_PI;
GLdouble spiral_on_cone::u_max = +TWO_PI;

DCoordinate3 spiral_on_cone::d0(GLdouble u)
{
	return DCoordinate3(u * cos(u), u * sin(u), u);
}

DCoordinate3 spiral_on_cone::d1(GLdouble u)
{
	GLdouble c = cos(u), s = sin(u);
	return DCoordinate3(c - u * s, s + u * c, 1.0);
}

DCoordinate3 spiral_on_cone::d2(GLdouble u)
{
	GLdouble c = cos(u), s = sin(u);
	return DCoordinate3(-2.0 * s - u * c, 2.0 * c - u * s, 0);
}

GLdouble torus_knot::u_min = 0;
GLdouble torus_knot::u_max = +3*TWO_PI;

DCoordinate3 torus_knot::d0(GLdouble u)
{
	return DCoordinate3((2+cos(2*u/3))*cos(u),
						(2+cos(2*u/3))*sin(u),
						sin(2*u/3));
}

DCoordinate3 torus_knot::d1(GLdouble u)
{
	return DCoordinate3(-(2/3)*cos(u)*sin(2*u/3)-(2+cos(2*u/3))*sin(u),
						-sin(2*u/3)*(2/3)*sin(u)+(2+cos(2*u/3))*cos(u),//(1/6)*(cos(u/3)+12*cos(u)+5*cos(5*u/3)),
						(2/3)*cos(2*u/3));
}

DCoordinate3 torus_knot::d2(GLdouble u)
{
	return DCoordinate3((-2/3)*cos(2*u/3)*cos(u)*2/3-sin(2*u/3)*(2/3)*(-sin(u))-sin(2*u/3)*(2.3)*(-sin(u))+(2+cos(2*u/3))*(-cos(u)),//1/18)*(-cos(u/3)-36*cos(u)-25*cos(5*u/3)),
						(-2/3)*cos(2*u/3)*sin(u)*2/3-sin(2*u/3)*(2/3)*(cos(u))-sin(2*u/3)*(2.3)*(cos(u))-(2+cos(2*u/3))*sin(u),//(1/18)*(-sin(u/3)-36*sin(u)-25*sin(5*u/3)),
						-(4/9)*sin(2*u/3));
}


GLdouble epicycloid::u_min = 0;
GLdouble epicycloid::u_max = TWO_PI;
GLint epicycloid::constant2=6+1;

DCoordinate3 epicycloid::d0(GLdouble u)
{
	return DCoordinate3(constant2*cos(u)-cos(constant2*u),
						constant2*sin(u)-sin(constant2*u),
						0);
}

DCoordinate3 epicycloid::d1(GLdouble u)
{
	return DCoordinate3(-constant2*sin(u)+sin(constant2*u)*constant2,
						constant2*cos(u)-cos(constant2*u)*constant2,
						0);
}

DCoordinate3 epicycloid::d2(GLdouble u)
{
	return DCoordinate3(-constant2*cos(u)+cos(constant2*u)*constant2*constant2,
						-constant2*sin(u)+sin(constant2*u)*constant2*constant2,
						0);
}

GLdouble vivianis::u_min = -TWO_PI;
GLdouble vivianis::u_max = TWO_PI;
GLint vivianis::constant2=4+1;

DCoordinate3 vivianis::d0(GLdouble u)
{
	return DCoordinate3(constant2*(1+cos(u)),
						constant2*sin(u),
						2*constant2*sin(u/2));
}

DCoordinate3 vivianis::d1(GLdouble u)
{
	return DCoordinate3(-constant2*sin(u),
						constant2*cos(u),
						constant2*cos(u/2));
}

DCoordinate3 vivianis::d2(GLdouble u)
{
	return DCoordinate3(-constant2*cos(u),
						-constant2*sin(u),
						-constant2*sin(u/2)/2);
}

GLint mobius::constant2=2;
GLdouble mobius::u_min = -TWO_PI;//*constant2;
GLdouble mobius::u_max = TWO_PI;//*constant2;

DCoordinate3 mobius::d0(GLdouble u)
{
	return DCoordinate3(cos(u)+constant2*cos(u/2)*cos(u),
						sin(u)+constant2*cos(u/2)*sin(u),
						constant2*sin(u/2));
}

DCoordinate3 mobius::d1(GLdouble u)
{
	return DCoordinate3(-constant2*cos(u)*sin(u/2)/2-sin(u)-constant2*cos(u/2)*sin(u),
						constant2*cos(u)*cos(u/2)+cos(u)-constant2*sin(u/2)*sin(u)/2,
						constant2*cos(u/2)/2);
}

DCoordinate3 mobius::d2(GLdouble u)
{
	return DCoordinate3(-5*constant2*cos(u)*cos(u/2)/4-cos(u)+constant2*sin(u/2)*sin(u),
						-constant2*cos(u)*sin(u/2)-sin(u)-constant2*cos(u/2)*sin(u)*5/4,
						-constant2*sin(u/2)/4);
}


//TORUS
GLdouble torus::u_min = -TWO_PI;
GLdouble torus::u_max = +TWO_PI;
GLdouble torus::v_min = -TWO_PI;
GLdouble torus::v_max = +TWO_PI;

DCoordinate3 torus::d00(GLdouble u, GLdouble v)
{
	GLdouble cu = cos(u), su = sin(u), cv = cos(v), sv = sin(v);
	return DCoordinate3((1 + 0.3 * cv) * cu, (1 + 0.3 * cv) * su, 0.3 * sv);
}

DCoordinate3 torus::d10(GLdouble u, GLdouble v)
{
	GLdouble cu = cos(u), su = sin(u), cv = cos(v), sv = sin(v);
	return DCoordinate3(-(1 + 0.3 * cv) * su, (1 + 0.3 * cv) * cu, 0);
}
DCoordinate3 torus::d01(GLdouble u, GLdouble v)
{
	GLdouble cu = cos(u), su = sin(u), cv = cos(v), sv = sin(v);
	return DCoordinate3(-0.3 * cu * sv, -0.3 * su * sv, 0.3 * cv);
}

//MOBIUS
GLdouble mobiusSurface::u_min = 0.0;
GLdouble mobiusSurface::u_max = +TWO_PI;
GLdouble mobiusSurface::v_min = -1;
GLdouble mobiusSurface::v_max = +1;

DCoordinate3 mobiusSurface::d00(GLdouble u, GLdouble v)
{
	return DCoordinate3((1 + v/2.0 * cos(u/2.0)) * cos(u), (1 + v/2.0 * cos(u/2.0)) * sin(u), v/2.0 * sin(u/2.0));
}

DCoordinate3 mobiusSurface::d10(GLdouble u, GLdouble v)
{
	return DCoordinate3(-(sin(u) + v/2.0 * (1/2.0*sin(u/2.0)*cos(u) + cos(u/2.0)*sin(u))), cos(u) + v/2.0*(-1/2.0*sin(u/2.0)*sin(u) + cos(u/2.0)*cos(u)), v/2.0 * 1/2.0 * cos(u/2.0));
}
DCoordinate3 mobiusSurface::d01(GLdouble u, GLdouble v)
{
	return DCoordinate3(1/2.0 * cos(u/2.0) * cos(u), 1/2.0 * cos(u/2.0) * sin(u), 1/2.0 * sin(u/2.0));
}

//STEINER
GLdouble steiner::u_min = 0.0;
GLdouble steiner::u_max = PI;
GLdouble steiner::v_min = 0.0;
GLdouble steiner::v_max = PI;

DCoordinate3 steiner::d00(GLdouble u, GLdouble v)
{
	GLdouble cu = cos(u), su = sin(u), cv = cos(v), sv = sin(v);
	return DCoordinate3(cu*cv*sv, su*cv*sv, cu*su*cv*cv);
}

DCoordinate3 steiner::d10(GLdouble u, GLdouble v)
{
	GLdouble cu = cos(u), su = sin(u), cv = cos(v), sv = sin(v);
	return DCoordinate3(cv*sv*(-su), cv*sv*cu, cv*cv*(-su*su + cu*cu));
}
DCoordinate3 steiner::d01(GLdouble u, GLdouble v)
{
	GLdouble cu = cos(u), su = sin(u), cv = cos(v), sv = sin(v);
	return DCoordinate3(cu * (-sv*sv + cv*cv), su * (-sv*sv + cv*cv), -cu*su*2.0*cv*sv);
}

//ENNEPER
GLdouble enneper::u_min = -2;
GLdouble enneper::u_max = +2;
GLdouble enneper::v_min = -2;
GLdouble enneper::v_max = +2;

DCoordinate3 enneper::d00(GLdouble u, GLdouble v)
{
	return DCoordinate3(u*(1 - u*u/3.0 + v*v) / 3.0, -v*(1 - v*v/3.0 + u*u) / 3.0, (u*u - v*v)/3.0);
}

DCoordinate3 enneper::d10(GLdouble u, GLdouble v)
{
	return DCoordinate3(- u*u + 1/3.0*(1+ v*v), -2.0/3.0*v*u, 2.0/3.0*u);
}
DCoordinate3 enneper::d01(GLdouble u, GLdouble v)
{
	return DCoordinate3(2.0*3.0*u*v, -1/3.0*(1.0+v*v+u*u), -2.0/3.0*v);
}

//DINI
GLdouble dini::u_min = -TWO_PI;
GLdouble dini::u_max = +TWO_PI;
GLdouble dini::v_min = -TWO_PI;
GLdouble dini::v_max = +TWO_PI;

DCoordinate3 dini::d00(GLdouble u, GLdouble v)
{
	GLdouble cu = cos(u), su = sin(u), cv = cos(v), sv = sin(v);
	return DCoordinate3(cu*sv, su*sv, cv + log(tan(v/2.0)) + 0.15*u);
}

DCoordinate3 dini::d10(GLdouble u, GLdouble v)
{
	GLdouble cu = cos(u), su = sin(u), cv = cos(v), sv = sin(v);
	return DCoordinate3(sv*(-su), sv*cu, 0.15);
}
DCoordinate3 dini::d01(GLdouble u, GLdouble v)
{
	GLdouble cu = cos(u), su = sin(u), cv = cos(v), sv = sin(v);
	return DCoordinate3(cu*cv, su*cv, -sv + 1.0/sv);
}
