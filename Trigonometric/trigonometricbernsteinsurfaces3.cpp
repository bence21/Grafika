#include "TrigonometricBernsteinSurfaces3.h"

#include <cmath>

using namespace std;

namespace cagd
{
	//    GLvoid TrigonometricBernsteinSurface3
	GLvoid TrigonometricBernsteinSurface3::_CalculateBinomialCoefficients(
			GLuint order, TriangularMatrix<GLdouble> &bc
			)
	{
		bc.ResizeRows(order + 1);
		bc(0,0) = 1.0;
		for(GLuint r=1;r<=order;r++){
			bc(r,0) = 1.0;
			bc(r,r) = 1.0;

			for(GLuint i=1; i<=r/2; ++i){
				bc(r,i) = bc(r-1, i-1) + bc(r-1,i);
				bc(r,r-i) = bc(r,i);
			}
		}
	}

	GLboolean TrigonometricBernsteinSurface3::_CalculateNormalizingCoefficients(
			GLuint order, GLdouble alpha, RowMatrix<GLdouble> &c)
	{
		if(!order){
			c.ResizeColumns(0);
			return GL_FALSE;
		}
		GLuint size=2*order+1;
		c.ResizeColumns(size);
		GLuint sa = pow(sin(alpha/2.0), (GLint)(2*order));
		GLdouble ca = 2.0 * cos(alpha / 2.0);

		for(GLuint i=0; i<=order; ++i){
			c[i] = 0.0;
			for(GLuint r=0;r<=order;++r){
				c[i] +=_bc(order, i-r) * _bc(i-r,r) * pow(ca,(GLint)(i-2 * r));
			}
			c[i] /= sa;
			c[size-1-i] = c[i];
		}

		return GL_TRUE;
	}

	TrigonometricBernsteinSurface3::TrigonometricBernsteinSurface3(
			GLdouble alpha, GLuint n, GLdouble beta, GLuint m):
		TensorProductSurface3(0.0,alpha,0.0,beta,2*n+1,2*m+1),
		_alpha(alpha), _beta(beta), _n(n), _m(m)
	{
		GLuint max_order = (_n > _m ? _n : _m);
		_CalculateBinomialCoefficients(max_order, _bc);

		_CalculateNormalizingCoefficients(_n,_alpha,_u_c);
		_CalculateNormalizingCoefficients(_m,_beta,_v_c);

	}

	GLboolean TrigonometricBernsteinSurface3::UBlendingFunctionValues(
			GLdouble u, RowMatrix<GLdouble>& values
			) const
	{
		if(u<0.0 || u>_alpha){
			values.ResizeColumns(0);
			return GL_FALSE;
		}

		GLuint size = 2 * _n +1;

		values.ResizeColumns(size);

		for(GLuint i=0;i<size; ++i){
			values[i] = 0.0;
		}
		if(u==0.0)
		{
			values[0]=1.0;
		}
		else{
			if(u==_alpha){
				values[size-1]=1.0;
			}
			else{
				GLdouble sau=sin((_alpha-u)/2.0),su=sin(u/2.0);
				if(u<_alpha/2.0){
					GLdouble factor=su/sau;
					GLdouble sau_order=pow(sau,(GLint)(size-1));

					values[0]=sau_order;

					for(GLint i=1;i<size;++i){
						values[i]=values[i-1]*factor;
					}
				}
				else{
					GLdouble factor=sau/su;
					GLdouble su_order=pow(su,(GLint)(size-1));

					values[size-1]=su_order;

					for(GLint i=size-2;i>=0;--i){
						values[i]=values[i+1]*factor;
					}
				}
				for(GLuint i=0;i<size;++i){
					values[i]*=_u_c[i];
				}
			}
		}
		return GL_TRUE;
	}

	GLboolean TrigonometricBernsteinSurface3::VBlendingFunctionValues(
			GLdouble v,RowMatrix<GLdouble>& values) const
	{
		if(v<0.0 || v>_beta){
			values.ResizeColumns(0);
			return GL_FALSE;
		}

		GLuint size=2*_m+1;

		values.ResizeColumns(size);
		for(GLuint j=0;j<size;++j){
			values[j]=0.0;
		}
		if(v==0.0){
			values[0]=1.0;
		}
		else{
			if(v==_beta){
				values[size-1]=1.0;
			}
			else{
				GLdouble sbv=sin((_beta-v)/2.0),sv=sin(v/2.0);
				if(v<_beta/2.0){
					GLdouble factor=sv/sbv;
					GLdouble sbv_order=pow(sbv,(GLuint)(size-1));

					values[0]=sbv_order;

					for(GLint j=1;j<size;++j){
						values[j]=values[j-1]*factor;
					}
				}
				else{
					GLdouble factor=sbv/sv;
					GLdouble sv_order=pow(sv,(GLint)(size-1));
					values[size-1]=sv_order;

					for(GLint j=size-2;j>=0;--j){
						values[j]=values[j+1]*factor;
					}
				}
				for(GLuint j=0;j<size;++j){
					values[j]*=_v_c[j];
				}
			}
		}
		return GL_TRUE;
	}

	GLboolean TrigonometricBernsteinSurface3::CalculatePartialDerivatives(
			GLuint maximum_order_of_partial_derivatives,
			GLdouble u, GLdouble v, PartialDerivatives& pd) const{
		if(u<_u_min||u>_u_max||v<_v_min||v>_v_max){
			pd.ResizeRows(0);
			return GL_FALSE;
		}

		pd.ResizeRows(maximum_order_of_partial_derivatives+1);
		pd.LoadNullVectors();

		GLuint u_size=2*_n+1,v_size=2*_m+1;

		RowMatrix<GLdouble> Au,Av;

		if(!UBlendingFunctionValues(u,Au)||!VBlendingFunctionValues(v,Av)){
			pd.ResizeRows(0);
			return GL_FALSE;
		}

		GLdouble sua=2.0*sin(_alpha/2.0), tua=tan(_alpha/2.0);
		GLdouble sva=2.0*sin(_beta/2.0), tva=tan(_beta/2.0);

		Matrix<GLdouble> dAu(maximum_order_of_partial_derivatives+1,u_size);
		Matrix<GLdouble> dAv(maximum_order_of_partial_derivatives+1,v_size);

		dAu.SetRow(0,Au);
		dAv.SetRow(0,Av);


		for (GLuint d = 1; d <= maximum_order_of_partial_derivatives; ++d)
		{
			for (GLuint i = 0; i < u_size; ++i)
			{
				dAu(d, i) = ((i > 0) ? _u_c[i] / _u_c[i - 1] * i / sua * dAu(d - 1, i - 1) : 0.0) - (_n - i) / tua * dAu(d - 1, i) - ((i < 2 * _n) ? (_u_c[i] / _u_c[i + 1] * (2.0 * _n - i) / sua) * dAu(d - 1, i + 1) : 0.0);
			}

			for (GLuint j = 0; j < v_size; ++j)
			{
				dAv(d, j) = ((j > 0) ? (_v_c[j] / _v_c[j - 1] * j / sva * dAv(d - 1, j - 1)) : 0.0) - (_m - j) / tva * dAv(d - 1, j) - ((j < 2 * _m) ? (_v_c[j] / _v_c[j + 1] * (2.0 * _m - j) / sva) * dAv(d - 1, j + 1) : 0.0);
			}
		}
		for (GLuint i = 0;i<u_size;++i) {
			RowMatrix<DCoordinate3>diff_v(maximum_order_of_partial_derivatives+1);
			for(GLuint j=0;j<v_size;++j) {
				for(GLuint d=0;d<=maximum_order_of_partial_derivatives;d++) {
					diff_v[d]+=_data(i,j)*dAv(d,j);
				}
			}
			for(GLuint d=0;d<=maximum_order_of_partial_derivatives;d++){
				for(GLuint r=0;r<=d;++r) {
					pd(d,r)+=diff_v[r]*dAu(d-r,i);
				}
			}
		}
		return GL_TRUE;
	}
}
