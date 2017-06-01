#include "./beziaerarc3.h"

using namespace cagd;

BezierArc3::BezierArc3():LinearCombination3(0.0,1.0,4)
{

}

BezierArc3::BlendingFunctionValues(GLdouble u, RowMatrix<GLdouble> &values) const
{
    if( u < 0.0 || u > 1.0)
    {
        return GL_FALSE;
    }

    values.ResizeColumns(_data.GetRowCount());

    GLdouble u2 = u*u, u3 = u2*u, w = 1.0-u, w2 = w*w, w3 = w2*w;

    values(0) = w3;
    values(1) = 3.0 * w2 * u;
    values(2) = 3.0 * w * u2;
    values(3) = u3;

    return GL_TRUE;
}

BezierArc3::CalculateDerivatives(GLuint max_order_of_derivatives, GLdouble u, Derivatives &d) const
{
    Matrix<GLdouble> dF(max_order_of_derivatives+1, _data.GetRowCount());

    GLdouble u2 = u * u, u3 = u2 * u, wu = 1.0 - u, wu2 = wu * wu, wu3 = wu2 * wu;


    //F0, F1, F2, F3
    dF(0,0) = wu3;
    dF(0,1) = 3.0 * wu2 * u;
    dF(0,2) = 3.0 * wu * u2;
    dF(0,3) = u3;

    if (max_order_of_derivatives >= 1)
    {
        //dF0, dF1, dF2, dF3
        dF(1,0) = -3.0 * wu2;
        dF(1,1) = -6.0 * wu * u + 3.0 * wu2;
        dF(1,2) = -3.0 * u2 + 6.0 * wu * u;
        dF(1,3) = 3.0 * u2;
    }

    if (max_order_of_derivatives >= 2)
    {
        //d2F0, d2F1, d2F2, d2F3
        dF(2,0) = 6.0 * wu;
        dF(2,1) = 18.0 * u - 12.0;
        dF(2,2) = 12.0 - 18.0 * u;
        dF(2,3) = -6.0 * wu;
    }

    d.ResizeRows(max_order_of_derivatives+1);
    d.LoadNullVectors();

    for(GLuint r = 0; r <= max_order_of_derivatives; r++)
    {
        for(GLuint i = 0; i < _data.GetRowCount() ; i++)
        {
            d[r] += _data[i] * dF(r,i);
        }
    }

    return GL_TRUE;

}














