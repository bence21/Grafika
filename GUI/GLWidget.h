#pragma once

#include <GL/glew.h>
#include <QGLWidget>
#include <QGLFormat>
#include"../Parametric/ParametricCurves3.h"
#include"../Parametric/ParametricSurfaces3.h"
//#include"../Core/ShaderPrograms.h"
#include"../Cyclic/CyclicCurves3.h"
#include"../Core/Constants.h"
#include"../Core/Lights.h"
#include"../Core/TriangulatedMeshes3.h"
#include"../Bezier/BicubicBezierPatches.h"
namespace cagd
{
	class GLWidget: public QGLWidget
	{
		Q_OBJECT
		//	protected:
		//		ShaderProgram _shaders;
	private:

		// variables defining the projection matrix
		float       _aspect;            // aspect ratio of the rendering window
		float       _fovy;              // field of view in direction y
		float       _z_near, _z_far;    // distance of near and far clipping planes

		// variables defining the model-view matrix
		float       _eye[3], _center[3], _up[3];

		// variables needed by transformations
		int         _angle_x, _angle_y, _angle_z;
		double      _zoom;
		double      _trans_x, _trans_y, _trans_z;

		// your other declarations
		GLuint _curve_count;
		RowMatrix<ParametricCurve3*> _pc;
		RowMatrix<GenericCurve3*>    _image_of_pc;
		GLint _pc_index;

		bool showParametricCurves;
		bool showParametricSurface;
		RowMatrix<ParametricSurface3*> _ps;
		RowMatrix<TriangulatedMesh3*> _image_of_ps;
		DirectionalLight *dl;

		bool showCyclicCurves;

		ColumnMatrix<DCoordinate3> _d;
		ColumnMatrix<GLdouble> _u_icc;

		CyclicCurves3* _cc;
		GenericCurve3* _image_of_cc;
		GLuint _n;
		//        GLuint _max_number_of_derivatives;
		CyclicCurves3* _icc;
		GenericCurve3* _image_of_icc;

		bool showModel;
		TriangulatedMesh3 _mouse;
		GLfloat _angle;

		bool showPatch;
		BicubicBezierPatch _patch;
		TriangulatedMesh3 *_before_interpolation,*_after_interpolation;

	public:
		// special and default constructor
		// the format specifies the properties of the rendering window
		GLWidget(QWidget* parent = 0, const QGLFormat& format = QGL::Rgba | QGL::DepthBuffer | QGL::DoubleBuffer);

		// redeclared virtual functions
		void initializeGL();
		void paintGL();
		void resizeGL(int w, int h);

		void initCyclicCurves();
		void cyclicCurvesRender();
		void cyclicCurveRender();
		void interpolatingCyclicCurveRender();
		void parametricSurfaceRender();
		void initModel();
		void modelRender();

		void initBezierPatch();

		virtual ~GLWidget();

	public slots:
		void _animate();
		// public event handling methods/slots
		void set_angle_x(int value);
		void set_angle_y(int value);
		void set_angle_z(int value);

		void set_zoom_factor(double value);

		void set_trans_x(double value);
		void set_trans_y(double value);
		void set_trans_z(double value);
		void setConstant(int value);
		void setParametricCurveIndex(int index);
		void setShowParametricCurve(bool x);
		void setShowCyclicCurve(bool x);
		void setShowParametricSurface(bool x);
		void setShowModel(bool x);
		void setShowPatch(bool x);

        void join();
	};
}
