#include "TensorProductSurfaces3.h"
#include "RealSquareMatrices.h"
#include<algorithm>
using namespace cagd;
using namespace std;

// special constructor
TensorProductSurface3::PartialDerivatives::PartialDerivatives(GLuint maximum_order_of_partial_derivatives):
    TriangularMatrix<DCoordinate3>(maximum_order_of_partial_derivatives + 1)
{
}

GLvoid TensorProductSurface3::PartialDerivatives::LoadNullVectors() {
    //    _data=nullptr;
    GLuint c_count = 1;
    for (GLuint i = 0; i < _data.size(); ++i, ++c_count)
    {
	for (GLuint j = 0; j < c_count; ++j)
	{
	    for (GLuint k = 0; k < 3; ++k)
		_data[i][j][k] = 0.0;
	}
    }
}

// special constructor
TensorProductSurface3::TensorProductSurface3(
	GLdouble u_min, GLdouble u_max,
	GLdouble v_min, GLdouble v_max,
	GLuint row_count, GLuint column_count,
	GLboolean u_closed, GLboolean v_closed):
    _vbo_data(0), _u_min(u_min), _u_max(u_max), _v_min(v_min), _v_max(v_max), _u_closed(u_closed), _v_closed(v_closed)
{
    _data.ResizeRows(row_count);
    _data.ResizeColumns(column_count);
}

// copy constructor
TensorProductSurface3::TensorProductSurface3(const TensorProductSurface3& surface):
    _vbo_data(0), _u_min(surface._u_min), _u_max(surface._u_max), _v_min(surface._v_min), _v_max(surface._v_max),
    _u_closed(surface._u_closed), _v_closed(surface._v_closed), _data(surface._data)
{
    if (surface._vbo_data)
	UpdateVertexBufferObjectsOfData();
}

// assignment operator
TensorProductSurface3& TensorProductSurface3::operator =(const TensorProductSurface3& surface)
{
    if (this != &surface)
    {
	DeleteVertexBufferObjectsOfData();

	_u_min = surface._u_min;
	_u_max = surface._u_max;
	_v_min = surface._v_min;
	_v_max = surface._v_max;
	_u_closed = surface._u_closed;
	_v_closed = surface._v_closed;
	_data = surface._data;

	if (surface._vbo_data)
	    UpdateVertexBufferObjectsOfData();
    }

    return *this;
}

// set/get the definition domain of the surface
GLvoid TensorProductSurface3::SetUInterval(GLdouble u_min, GLdouble u_max)
{
    _u_min = u_min;
    _u_max = u_max;
}

GLvoid TensorProductSurface3::SetVInterval(GLdouble v_min, GLdouble v_max)
{
    _v_min = v_min;
    _v_max = v_max;
}

GLvoid TensorProductSurface3::GetUInterval(GLdouble& u_min, GLdouble& u_max) const
{
    u_min = _u_min;
    u_max = _u_max;
}
GLvoid TensorProductSurface3::GetVInterval(GLdouble& v_min, GLdouble& v_max) const
{
    v_min = _v_min;
    v_max = _v_max;
}

// set coordinates of a selected data point
GLboolean TensorProductSurface3::SetData(GLuint row, GLuint column, GLdouble x, GLdouble y, GLdouble z)
{
    if (row > _data.GetRowCount() || column < _data.GetColumnCount())
    {
	return GL_FALSE;
    }
    _data(row, column).x() = x;
    _data(row, column).y() = y;
    _data(row, column).z() = z;
    return GL_TRUE;
}

GLboolean TensorProductSurface3::SetData(GLuint row, GLuint column, const DCoordinate3& point)
{
    if (row > _data.GetRowCount() || column < _data.GetColumnCount())
    {
	return GL_FALSE;
    }
    _data(row, column) = point;
    return GL_TRUE;
}

// get coordinates of a selected data point
GLboolean TensorProductSurface3::GetData(GLuint row, GLuint column, GLdouble& x, GLdouble& y, GLdouble& z) const
{
    if (row > _data.GetRowCount() || column < _data.GetColumnCount())
    {
	return GL_FALSE;
    }
    x = _data(row, column).x();
    y = _data(row, column).y();
    z = _data(row, column).z();
    return GL_TRUE;
}
GLboolean TensorProductSurface3::GetData(GLuint row, GLuint column, DCoordinate3& point) const
{
    if (row > _data.GetRowCount() || column < _data.GetColumnCount())
    {
	return GL_FALSE;
    }
    point = _data(row, column);
    return GL_TRUE;
}

// get data by value
DCoordinate3 TensorProductSurface3::operator ()(GLuint row, GLuint column) const
{
    return _data(row, column);
}

// get data by reference
DCoordinate3& TensorProductSurface3::operator ()(GLuint row, GLuint column)
{
    return _data(row, column);
}
// generates the image (i.e., the approximating triangulated mesh) of the tensor product surface
TriangulatedMesh3* TensorProductSurface3::GenerateImage(GLuint u_div_point_count, GLuint v_div_point_count, GLenum usage_flag) const
{
    if (u_div_point_count <= 1 || v_div_point_count <= 1)
	return GL_FALSE;

    // calculating number of vertices, unit normal vectors and texture coordinates
    GLuint vertex_count = u_div_point_count * v_div_point_count;

    // calculating number of triangular faces
    GLuint face_count = 2 * (u_div_point_count - 1) * (v_div_point_count - 1);

    TriangulatedMesh3 *result = nullptr;
    result = new TriangulatedMesh3(vertex_count, face_count, usage_flag);

    if (!result)
	return nullptr;

    // uniform subdivision grid in the definition domain
    GLdouble du = (_u_max - _u_min) / (u_div_point_count - 1);
    GLdouble dv = (_v_max - _v_min) / (v_div_point_count - 1);

    // uniform subdivision grid in the unit square
    GLfloat sdu = 1.0f / (u_div_point_count - 1);
    GLfloat tdv = 1.0f / (v_div_point_count - 1);

    // for face indexing
    GLuint current_face = 0;

    // partial derivatives of order 0, 1, 2, and 3
    PartialDerivatives pd;

    for (GLuint i = 0; i < u_div_point_count; ++i)
    {
	GLdouble u = _u_min + i * du;
	GLfloat  s = i * sdu;
	for (GLuint j = 0; j < v_div_point_count; ++j)
	{
	    GLdouble v = _v_min + j * dv;
	    GLfloat  t = j * tdv;

	    /*
		3-2
		|/|
		0-1
	    */
	    GLuint index[4];

	    index[0] = i * v_div_point_count + j;
	    index[1] = index[0] + 1;
	    index[2] = index[1] + v_div_point_count;
	    index[3] = index[2] - 1;

	    // calculating all needed surface data
	    CalculatePartialDerivatives(1, u, v, pd);

	    // surface point
	    (*result)._vertex[index[0]] = pd(0, 0);

	    // unit surface normal
	    (*result)._normal[index[0]] = pd(1, 0);
	    (*result)._normal[index[0]] ^= pd(1, 1);
	    (*result)._normal[index[0]].normalize();

	    // texture coordinates
	    (*result)._tex[index[0]].s() = s;
	    (*result)._tex[index[0]].t() = t;

	    // faces
	    if (i < u_div_point_count - 1 && j < v_div_point_count - 1)
	    {
		(*result)._face[current_face][0] = index[0];
		(*result)._face[current_face][1] = index[1];
		(*result)._face[current_face][2] = index[2];
		++current_face;

		(*result)._face[current_face][0] = index[0];
		(*result)._face[current_face][1] = index[2];
		(*result)._face[current_face][2] = index[3];
		++current_face;
	    }
	}
    }

    return result;
}

// ensures interpolation, i.e. s(u_i, v_j) = d_{i,j}
GLboolean TensorProductSurface3::UpdateDataForInterpolation(const RowMatrix<GLdouble>& u_knot_vector, const ColumnMatrix<GLdouble>& v_knot_vector, Matrix<DCoordinate3>& data_points_to_interpolate)
{
    GLuint row_count = _data.GetRowCount();
    if (!row_count)
	return GL_FALSE;

    GLuint column_count = _data.GetColumnCount();
    if (!column_count)
	return GL_FALSE;

    if (u_knot_vector.GetColumnCount() != row_count || v_knot_vector.GetRowCount() != column_count || data_points_to_interpolate.GetRowCount() != row_count || data_points_to_interpolate.GetColumnCount() != column_count)
	return GL_FALSE;

    // 1: calculate the u-collocation matrix and perfom LU-decomposition on it
    RowMatrix<GLdouble> u_blending_values;

    RealSquareMatrix u_collocation_matrix(row_count);

    for (GLuint i = 0; i < row_count; ++i)
    {
	if (!UBlendingFunctionValues(u_knot_vector(i), u_blending_values))
	    return GL_FALSE;
	u_collocation_matrix.SetRow(i, u_blending_values);
    }

    if (!u_collocation_matrix.PerformLUDecomposition())
	return GL_FALSE;

    // 2: calculate the v-collocation matrix and perform LU-decomposition on it
    RowMatrix<GLdouble> v_blending_values;

    RealSquareMatrix v_collocation_matrix(column_count);

    for (GLuint j = 0; j < column_count; ++j)
    {
	if (!VBlendingFunctionValues(v_knot_vector(j), v_blending_values))
	    return GL_FALSE;
	v_collocation_matrix.SetRow(j, v_blending_values);
    }

    if (!v_collocation_matrix.PerformLUDecomposition())
	return GL_FALSE;

    // 3:   for all fixed j in {0, 1,..., column_count} determine control points
    //
    //      a_k(v_j) = sum_{l=0}^{column_count} _data(l, j) G_l(v_j), k = 0, 1,..., row_count
    //
    //      such that
    //
    //      sum_{k=0}^{row_count} a_k(v_j) F_k(u_i) = data_points_to_interpolate(i, j),
    //
    //      for all i = 0, 1,..., row_count.
    Matrix<DCoordinate3> a(row_count, column_count);
    if (!u_collocation_matrix.SolveLinearSystem(data_points_to_interpolate, a))
	return GL_FALSE;

    // 4:   for all fixed i in {0, 1,..., row_count} determine control point
    //
    //      _data[i][j], j = 0, 1,..., column_count
    //
    //      such that
    //
    //      sum_{l=0}^{column_count} _data(i, l) G_l(v_j) = a_i(v_j)
    //
    //      for all j = 0, 1,..., column_count.
    if (!v_collocation_matrix.SolveLinearSystem(a, _data, GL_FALSE))
	return GL_FALSE;

    return GL_TRUE;
}

GLvoid TensorProductSurface3::DeleteVertexBufferObjectsOfData()
{
    if (_vbo_data)
    {
	glDeleteBuffers(1, &_vbo_data);
	_vbo_data = 0;
    }
}

RowMatrix<GenericCurve3*> *TensorProductSurface3::GenerateUIsoparametricLines(GLuint iso_line_count, GLuint maximum_order_of_derivatives, GLuint div_point_count, GLenum usage_flag) const
{
    if(iso_line_count<2||div_point_count<2) {
	return nullptr;
    }
    RowMatrix<GenericCurve3*> *result=new RowMatrix<GenericCurve3*>(iso_line_count);
    if(!result){
	return nullptr;
    }
    GLdouble u_step=(_v_max-_v_min)/(iso_line_count-1);
    GLdouble v_step=(_u_max-_u_min)/(div_point_count-1);
    for(GLuint i=0;i<iso_line_count;++i) {
	(*result)[i]=new GenericCurve3(maximum_order_of_derivatives,div_point_count,usage_flag);
	if(!(*result)[i]){
	    for(GLuint k=0;k<i;++k){
		delete (*result)[k];
	    }
	    delete result;
	    result=nullptr;
	    return result;
	}
	GLdouble v=min(_v_min+i*v_step,_v_max);
	for(GLuint j=0;j<div_point_count;++j) {
	    GLdouble u=min(_u_min+j*u_step,_u_max);
	    PartialDerivatives pd;
	    if(!CalculatePartialDerivatives(maximum_order_of_derivatives,u,v,pd)){
		for(GLuint k=0;k<=i;++k){
		    delete (*result)[k];
		}
		delete result;
		result=nullptr;
		return result;
	    }
	    for(GLuint r=0;r<=maximum_order_of_derivatives;++r) {
		(*(*result)[i])(r,j)=pd(r,0);
	    }
	}
    }
    return result;
}

RowMatrix<GenericCurve3*> *TensorProductSurface3::GenerateVIsoparametricLines(GLuint iso_line_count, GLuint maximum_order_of_derivatives, GLuint div_point_count, GLenum usage_flag) const
{
    if(iso_line_count<2||div_point_count<2) {
	return nullptr;
    }
    RowMatrix<GenericCurve3*> *result=new RowMatrix<GenericCurve3*>(iso_line_count);
    if(!result){
	return nullptr;
    }
    GLdouble u_step=(_v_max-_v_min)/(iso_line_count-1);
    GLdouble v_step=(_u_max-_u_min)/(div_point_count-1);
    for(GLuint i=0;i<iso_line_count;++i) {
	(*result)[i]=new GenericCurve3(maximum_order_of_derivatives,div_point_count,usage_flag);
	if(!(*result)[i]){
	    for(GLuint k=0;k<i;++k){
		delete (*result)[k];
	    }
	    delete result;
	    result=nullptr;
	    return result;
	}
	GLdouble v=min(_v_min+i*v_step,_v_max);
	for(GLuint j=0;j<div_point_count;++j) {
	    GLdouble u=min(_u_min+j*u_step,_u_max);
	    PartialDerivatives pd;
	    if(!CalculatePartialDerivatives(maximum_order_of_derivatives,u,v,pd)){
		for(GLuint k=0;k<=i;++k){
		    delete (*result)[k];
		}
		delete result;
		result=nullptr;
		return result;
	    }
	    for(GLuint r=0;r<=maximum_order_of_derivatives;++r) {
		(*(*result)[i])(r,j)=pd(r,r);//kulombseg
	    }
	}
    }
    return result;
}

//mas!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

GLboolean TensorProductSurface3::UpdateVertexBufferObjectsOfData(GLenum usage_flag)
{
    GLuint data_count = _data.GetRowCount();
    if (!data_count)
	return GL_FALSE;

    if (usage_flag != GL_STREAM_DRAW  && usage_flag != GL_STREAM_READ  && usage_flag != GL_STREAM_COPY
	    && usage_flag != GL_DYNAMIC_DRAW && usage_flag != GL_DYNAMIC_READ && usage_flag != GL_DYNAMIC_COPY
	    && usage_flag != GL_STATIC_DRAW  && usage_flag != GL_STATIC_READ  && usage_flag != GL_STATIC_COPY)
	return GL_FALSE;

    //	_data_usage_flag = usage_flag;

    DeleteVertexBufferObjectsOfData();

    glGenBuffers(1, &_vbo_data);
    if (!_vbo_data)
	return GL_FALSE;

    glBindBuffer(GL_ARRAY_BUFFER, _vbo_data);
    glBufferData(GL_ARRAY_BUFFER, 2*data_count * 3 * sizeof(GLfloat), 0, _data_usage_flag);

    GLfloat *coordinate = (GLfloat*)glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
    if (!coordinate)
    {
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	DeleteVertexBufferObjectsOfData();
	return GL_FALSE;
    }

    for (GLuint i = 0; i < data_count; ++i)
    {
	for(GLuint j=0;j<_data.GetColumnCount();++j) {
	    for (GLuint c = 0; c < 3; ++c)
	    {
		*coordinate = (GLfloat)_data(i,j)[c];
		++coordinate;
	    }
	}
    }

    for(GLuint j=0;j<_data.GetColumnCount();++j) {
	for (GLuint i = 0; i < data_count; ++i){
	    for (GLuint c = 0; c < 3; ++c)
	    {
		*coordinate = (GLfloat)_data(i,j)[c];
		++coordinate;
	    }
	}
    }
    if (!glUnmapBuffer(GL_ARRAY_BUFFER))
    {
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	DeleteVertexBufferObjectsOfData();
	return GL_FALSE;
    }
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    return GL_TRUE;
}

GLboolean TensorProductSurface3::RenderData(GLenum render_mode) const {
    if (!_vbo_data)
	return GL_FALSE;
    if (render_mode != GL_LINE_STRIP && render_mode != GL_LINE_LOOP && render_mode != GL_POINTS)
	return GL_FALSE;
    glEnableClientState(GL_VERTEX_ARRAY);
    glBindBuffer(GL_ARRAY_BUFFER, _vbo_data);
    glVertexPointer(3, GL_FLOAT, 0, (const GLvoid*)0);
    GLuint offset=0;
    for(GLuint i=0;i<_data.GetRowCount();++i,offset+=_data.GetColumnCount()){
	glDrawArrays(render_mode, offset, _data.GetColumnCount());
    }
    for(GLuint j=0;j<_data.GetColumnCount();++j,offset+=_data.GetRowCount()){
	glDrawArrays(render_mode, offset, _data.GetRowCount());
    }
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glDisableClientState(GL_VERTEX_ARRAY);
    return GL_TRUE;
}

TensorProductSurface3::TensorProductSurface3(GLdouble u_min, GLdouble u_max, GLdouble v_min, GLdouble v_max, GLuint row_count, GLuint column_count, GLboolean u_closed, GLboolean v_closed):
    _u_min(u_min),_u_max(u_max),_v_min(v_min),_v_max(v_max),//_row_count(row_count),_column_count(column_count),
    _u_closed(u_closed),_v_closed(v_closed)
{
}

//TensorProductSurface3::TensorProductSurface3(const TensorProductSurface3 &surface)
//{

//}

TensorProductSurface3& operator =(const TensorProductSurface3& surface)
{
    this=surface;
    return *this;
}
// destructor
TensorProductSurface3::~TensorProductSurface3()
{
    DeleteVertexBufferObjectsOfData();
}
//TensorProductSurface3::DCoordinate3 operator ()()
