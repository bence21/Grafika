#pragma once

#include "../Core/DCoordinates3.h"

namespace cagd
{
	namespace spiral_on_cone
	{
		extern GLdouble u_min, u_max;

		DCoordinate3 d0(GLdouble);
		DCoordinate3 d1(GLdouble);
		DCoordinate3 d2(GLdouble);
	}
	namespace torus_knot
	{
		extern GLdouble u_min, u_max;

		DCoordinate3 d0(GLdouble);
		DCoordinate3 d1(GLdouble);
		DCoordinate3 d2(GLdouble);
	}
	namespace epicycloid
	{
		extern GLdouble u_min, u_max;
		extern GLint constant2;

		DCoordinate3 d0(GLdouble);
		DCoordinate3 d1(GLdouble);
		DCoordinate3 d2(GLdouble);
	}
	namespace vivianis
	{
		extern GLdouble u_min, u_max;
		extern GLint constant2;

		DCoordinate3 d0(GLdouble);
		DCoordinate3 d1(GLdouble);
		DCoordinate3 d2(GLdouble);
	}
	namespace mobius
	{
		extern GLdouble u_min, u_max;
		extern GLint constant2;

		DCoordinate3 d0(GLdouble);
		DCoordinate3 d1(GLdouble);
		DCoordinate3 d2(GLdouble);
	}
	namespace torus
	{
		extern GLdouble parameter_1, parameter_2;
		extern GLdouble u_min, u_max,
						v_min, v_max;
		DCoordinate3 d00(GLdouble u, GLdouble v); // zeroth order partial derivative, i.e. surface point
		DCoordinate3 d10(GLdouble u, GLdouble v); // first order partial derivative in direction u
		DCoordinate3 d01(GLdouble u, GLdouble v); // first order partial derivative in direction v
	}

	namespace mobiusSurface
	{
		extern GLdouble parameter_1, parameter_2;
		extern GLdouble u_min, u_max,
						v_min, v_max;
		DCoordinate3 d00(GLdouble u, GLdouble v); // zeroth order partial derivative, i.e. surface point
		DCoordinate3 d10(GLdouble u, GLdouble v); // first order partial derivative in direction u
		DCoordinate3 d01(GLdouble u, GLdouble v); // first order partial derivative in direction v
	}

	namespace steiner
	{
		extern GLdouble parameter_1, parameter_2;
		extern GLdouble u_min, u_max,
						v_min, v_max;
		DCoordinate3 d00(GLdouble u, GLdouble v); // zeroth order partial derivative, i.e. surface point
		DCoordinate3 d10(GLdouble u, GLdouble v); // first order partial derivative in direction u
		DCoordinate3 d01(GLdouble u, GLdouble v); // first order partial derivative in direction v
	}

	namespace enneper
	{
		extern GLdouble parameter_1, parameter_2;
		extern GLdouble u_min, u_max,
						v_min, v_max;
		DCoordinate3 d00(GLdouble u, GLdouble v); // zeroth order partial derivative, i.e. surface point
		DCoordinate3 d10(GLdouble u, GLdouble v); // first order partial derivative in direction u
		DCoordinate3 d01(GLdouble u, GLdouble v); // first order partial derivative in direction v
	}

	namespace dini
	{
		extern GLdouble parameter_1, parameter_2;
		extern GLdouble u_min, u_max,
						v_min, v_max;
		DCoordinate3 d00(GLdouble u, GLdouble v); // zeroth order partial derivative, i.e. surface point
		DCoordinate3 d10(GLdouble u, GLdouble v); // first order partial derivative in direction u
		DCoordinate3 d01(GLdouble u, GLdouble v); // first order partial derivative in direction v
	}
}
