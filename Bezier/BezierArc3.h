#include "./LinearCombination3.h"

namespace cagd
{
    class BezierArc3: public LinearCombination3
    {
    public:
        BezierArc3();
        GLboolean BlendingFunctionValues(GLdouble u, RowMatrix<GLdouble> &values) const;
        GLboolean CalculateDerivatives(GLuint max_order_of_derivatives, GLdouble u, Derivatives &d) const;

    };
}
