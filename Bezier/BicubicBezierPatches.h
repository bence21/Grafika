#pragma once

#include "../Core/TensorProductSurfaces3.h"


namespace cagd
{
    class BicubicBezierPatch: public TensorProductSurface3
	{
    public:
		GLuint north=-1;
		GLuint east=-1;
		GLuint south=-1;
		GLuint west=-1;

        BicubicBezierPatch();

        //we have to implement pure virtual methods introduced in class TensorProductSurface3

        GLboolean UBlendingFunctionValues(GLdouble u_knot, RowMatrix<GLdouble>& blending_values) const;
        GLboolean VBlendingFunctionValues(GLdouble v_knot, RowMatrix<GLdouble>& blending_values) const;
        GLboolean CalculatePartialDerivatives(GLuint maximum_order_of_partial_derivatives, GLdouble u, GLdouble v, PartialDerivatives& pd) const;

        //..
    };
}
