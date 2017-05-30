#pragma once

#include "../Core/Matrices.h"
#include "../Core/RealSquareMatrices.h"
#include "../Core/TensorProductSurfaces3.h"

namespace cagd
{
	class TrigonometricBernsteinSurface3: public TensorProductSurface3
	{
	protected:

		// shape parameters in directions u and v, respectively
		GLdouble _alpha,_beta;
		GLuint _n,_m;
		RowMatrix<GLdouble> _u_c,_v_c;
		TriangularMatrix<GLdouble> _bc;

		GLvoid _CalculateBinomialCoefficients(GLuint order, TriangularMatrix<GLdouble> &bc);
		GLboolean _CalculateNormalizingCoefficients(GLuint order, GLdouble alpha, RowMatrix<GLdouble>&c);

	public:
		TrigonometricBernsteinSurface3(GLdouble alpha,GLuint n,GLdouble beta, GLuint v_order);
		GLboolean UBlendingFunctionValues(GLdouble u_knot, RowMatrix<GLdouble> &blending_values) const;
		GLboolean VBlendingFunctionValues(GLdouble v_knot, RowMatrix<GLdouble> &blending_values) const;
		GLboolean CalculatePartialDerivatives(GLuint maximum_order_of_partial_derivatives, GLdouble u, GLdouble v, PartialDerivatives &pd) const;
	};
}
