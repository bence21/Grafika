#include "GLWidget.h"
#include <iostream>
using namespace std;
using namespace cagd;
#include <GL/GLU.h>
#include <Core/Exceptions.h>
#include"../Core/Matrices.h"
#include"../Test/TestFunctions.h"
namespace cagd
{

	GLWidget::~GLWidget(){
		for(GLuint i=0;i<_pc.GetColumnCount();++i) {
			if(_pc[i]) {
				delete _pc[i],_pc[i]=0;
			}
		}
		for(GLuint i=0;i<_image_of_pc.GetColumnCount();++i) {
			if(_image_of_pc[i]) {
				delete _image_of_pc[i],_image_of_pc[i]=0;
			}
		}
		for(GLuint i=0;i<_ps.GetColumnCount();++i) {
			if(_image_of_ps[i]) {
				delete _image_of_ps[i],_image_of_ps[i]=0;
			}
			if(_ps[i]) {
				delete _ps[i],_ps[i]=0;
			}
		}
		if(dl) {
			delete dl;
		}
		for(int i=0;i<patchNr;++i){
			if(_before_interpolation[i]) {
				delete _before_interpolation[i],_before_interpolation[i]=0;
			}
			if(_after_interpolation[i]) {
				delete _after_interpolation[i],_after_interpolation[i]=0;
			}
		}
	}


	//--------------------------------
	// special and default constructor
	//--------------------------------
	GLWidget::GLWidget(QWidget *parent, const QGLFormat &format): QGLWidget(format, parent)
	{
		indexX=0;
		indexY=0;
	}

	//--------------------------------------------------------------------------------------
	// this virtual function is called once before the first call to paintGL() or resizeGL()
	//--------------------------------------------------------------------------------------
	void GLWidget::initializeGL()
	{
		// creating a perspective projection matrix
		glMatrixMode(GL_PROJECTION);

		glLoadIdentity();

		_aspect = (float)width() / (float)height();
		_z_near = 1.0;
		_z_far = 1000.0;
		_fovy = 45.0;

		gluPerspective(_fovy, _aspect, _z_near, _z_far);

		// setting the model view matrix
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		_eye[0] = _eye[1] = 0.0, _eye[2] = 6.0;
		_center[0] = _center[1] = _center[2] = 0.0;
		_up[0] = _up[2] = 0.0, _up[1] = 1.0;

		gluLookAt(_eye[0], _eye[1], _eye[2], _center[0], _center[1], _center[2], _up[0], _up[1], _up[2]);

		// enabling depth test
		glEnable(GL_DEPTH_TEST);

		// setting the color of background
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

		// initial values of transformation parameters
		_angle_x = _angle_y = _angle_z = 0.0;
		_trans_x = _trans_y = _trans_z = 0.0;
		_zoom = 1.0;

		try
		{
			// initializing the OpenGL Extension Wrangler library
			GLenum error = glewInit();

			if (error != GLEW_OK)
			{
				throw Exception("Could not initialize the OpenGL Extension Wrangler Library!");
			}

			if (!glewIsSupported("GL_VERSION_2_0"))
			{
				throw Exception("Your graphics card is not compatible with OpenGL 2.0+! "
								"Try to update your driver or buy a new graphics adapter!");
			}

			// create and store your geometry in display lists or vertex buffer objects
			// ...
			//			fstream log("log.txt",ios_base::out);
			//			if(!_shaders.InstallShaders("Shaders/*.vert","Shaders/*.frag",GL_TRUE,log)){
			//				log.close();
			//				throw;
			//			}
			//			log.close();
			//			_shaders.Enable();
			//			_shaders.SetUniformVariable1f("scale_factor",4.0);
			//			//...
			//			_shaders.Disable();

		}
		catch (Exception &e)
		{
			cout << e << endl;
		}


		glEnable(GL_POINT_SMOOTH);
		glHint(GL_POINT_SMOOTH_HINT,GL_NICEST);
		glEnable(GL_LINE_SMOOTH);
		glHint(GL_LINE_SMOOTH_HINT,GL_NICEST);
		glEnable(GL_POLYGON_SMOOTH);
		glHint(GL_POLYGON_SMOOTH_HINT,GL_NICEST);
		glHint(GL_PERSPECTIVE_CORRECTION_HINT,GL_NICEST);
		glEnable(GL_DEPTH);

		glewInit();

		_curve_count=5;
		//_pc= new RowMatrix<>();
		_pc.ResizeColumns(_curve_count);
		_image_of_pc.ResizeColumns(_curve_count);
		_ps.ResizeColumns(_curve_count);
		_image_of_ps.ResizeColumns(_curve_count);
		_pc_index=0;
		RowMatrix<ParametricCurve3::Derivative> derivative(3);
		//int functionIndex=5;
		//_pc=0;
		/*
		switch (functionIndex) {
		case 2:
			derivative(0)=torus_knot::d0;
			derivative(1)=torus_knot::d1;
			derivative(2)=torus_knot::d2;
			_pc=new ParametricCurve3(derivative,torus_knot::u_min,torus_knot::u_max);
			break;
		case 3:
			derivative(0)=epicycloid::d0;
			derivative(1)=epicycloid::d1;
			derivative(2)=epicycloid::d2;
			_pc=new ParametricCurve3(derivative,epicycloid::u_min,epicycloid::u_max);
			break;
		case 4:
			derivative(0)=vivianis::d0;
			derivative(1)=vivianis::d1;
			derivative(2)=vivianis::d2;
			_pc=new ParametricCurve3(derivative,vivianis::u_min,vivianis::u_max);
			break;
		case 5:
			derivative(0)=mobius::d0;
			derivative(1)=mobius::d1;
			derivative(2)=mobius::d2;
			_pc=new ParametricCurve3(derivative,mobius::u_min,mobius::u_max);
			break;
		default:
			derivative(0)=spiral_on_cone::d0;
			derivative(1)=spiral_on_cone::d1;
			derivative(2)=spiral_on_cone::d2;
			_pc=new ParametricCurve3(derivative,spiral_on_cone::u_min,spiral_on_cone::u_max);
			break;
		}*/
		GLint k=0;
		_pc[k]=0;
		derivative(0)=torus_knot::d0;
		derivative(1)=torus_knot::d1;
		derivative(2)=torus_knot::d2;
		_pc[k]=new ParametricCurve3(derivative,torus_knot::u_min,torus_knot::u_max);
		_pc[++k]=0;
		derivative(0)=epicycloid::d0;
		derivative(1)=epicycloid::d1;
		derivative(2)=epicycloid::d2;
		_pc[k]=new ParametricCurve3(derivative,epicycloid::u_min,epicycloid::u_max);
		_pc[++k]=0;
		derivative(0)=vivianis::d0;
		derivative(1)=vivianis::d1;
		derivative(2)=vivianis::d2;
		_pc[k]=new ParametricCurve3(derivative,vivianis::u_min,vivianis::u_max);
		_pc[++k]=0;
		derivative(0)=mobius::d0;
		derivative(1)=mobius::d1;
		derivative(2)=mobius::d2;
		_pc[k]=new ParametricCurve3(derivative,mobius::u_min,mobius::u_max);
		_pc[++k]=0;
		derivative(0)=spiral_on_cone::d0;
		derivative(1)=spiral_on_cone::d1;
		derivative(2)=spiral_on_cone::d2;
		_pc[k]=new ParametricCurve3(derivative,spiral_on_cone::u_min,spiral_on_cone::u_max);

		TriangularMatrix<ParametricSurface3::PartialDerivative> derivs(2);
		derivs(0, 0) = torus::d00;
		derivs(1, 0) = torus::d10;
		derivs(1, 1) = torus::d01;
		k=0;
		_ps[k] = 0;
		_ps[k] = new ParametricSurface3(derivs, torus::u_min, torus::u_max, torus::v_min, torus::v_max);
		derivs(0, 0) = mobiusSurface::d00;
		derivs(1, 0) = mobiusSurface::d10;
		derivs(1, 1) = mobiusSurface::d01;
		_ps[++k] = 0;
		_ps[k] = new ParametricSurface3(derivs, mobiusSurface::u_min, mobiusSurface::u_max, mobiusSurface::v_min, mobiusSurface::v_max);
		derivs(0, 0) = steiner::d00;
		derivs(1, 0) = steiner::d10;
		derivs(1, 1) = steiner::d01;
		_ps[++k] = 0;
		_ps[k] = new ParametricSurface3(derivs, steiner::u_min, steiner::u_max, steiner::v_min, steiner::v_max);
		derivs(0, 0) = dini::d00;
		derivs(1, 0) = dini::d10;
		derivs(1, 1) = dini::d01;
		_ps[++k] = 0;
		_ps[k] = new ParametricSurface3(derivs, dini::u_min, dini::u_max, dini::v_min, dini::v_max);
		derivs(0, 0) = enneper::d00;
		derivs(1, 0) = enneper::d10;
		derivs(1, 1) = enneper::d01;
		_ps[++k] = 0;
		_ps[k] = new ParametricSurface3(derivs, enneper::u_min, enneper::u_max, enneper::v_min, enneper::v_max);
		for(GLint i=0;i<=k;++i){
			if(!_pc[i])
			{
				cerr<<"Error at new ParametricCurve3\n";
			}
			if(!_ps[i])
			{
				cerr<<"Error at new ParametricSurface3\n";
			}
		}
		GLuint div_point_count=200;
		GLenum usage_flag=GL_STATIC_DRAW;

		_image_of_pc.ResizeColumns(_curve_count);
		for(GLuint i=0;i<_curve_count;++i){
			_image_of_pc[i]=0;
			_image_of_pc[i]=_pc[i]->GenerateImage(div_point_count,usage_flag);
			_image_of_ps[i]=0;
			_image_of_ps[i]=_ps[i]->GenerateImage(div_point_count,div_point_count);
			if(!_image_of_pc[i])
			{
				cerr<<"Error at _pc->GenerateImage\n";
			}
			if(!_image_of_ps[i])
			{
				cerr<<"Error at _pc->GenerateImage\n";
			}
			if(i==1){
				if(!_image_of_pc[i]->UpdateVertexBufferObjects(usage_flag,0.1))
					//				if(!_image_of_pc[i]->UpdateVertexBufferObjects(usage_flag))
				{
					cout<<"Could not create the vertex buffer object of the parametric curve!\n";
				}
			} else if(!_image_of_pc[i]->UpdateVertexBufferObjects(usage_flag))
			{
				cout<<"Could not create the vertex buffer object of the parametric curve!\n";
			}
			if (!_image_of_ps[i] -> UpdateVertexBufferObjects(usage_flag))
			{
				cout<<"Could not create the vertex buffer object of the parametric surface!" << endl;
			}
		}
		showCyclicCurves=false;
		showParametricCurves=false;
		showParametricSurface=false;
		showModel=false;
		showPatch=true;
		initCyclicCurves();

		initModel();

		dl =0;
		HCoordinate3 direction(0.0, 0.0, 1.0, 0.0);
		Color4 ambient(0.4, 0.4, 0.4, 1.0);
		Color4 diffuse(0.8, 0.8, 0.8, 1.0);
		Color4 specular(1.0, 1.0, 1.0, 1.0);
		dl = new DirectionalLight(GL_LIGHT0, direction, ambient, diffuse, specular);
		_materials[0] = MatFBBrass;
		_materials[1] = MatFBEmerald;
		_materials[2] = MatFBRuby;
		_materials[3] = MatFBGold;
		_materials[4] = MatFBSilver;
		initBezierPatch();
	}

	void GLWidget::initBezierPatch() {
		_joinDirectionValue=0;
#define MAX_DATA_POINTS 100
		_data_points.resize(MAX_DATA_POINTS);
		for(int i=0;i<MAX_DATA_POINTS;++i) {
			_data_points[i] =  cagd::Matrix<cagd::DCoordinate3>(4,4);
		}
		_patch.resize(MAX_DATA_POINTS);
		_before_interpolation.resize(MAX_DATA_POINTS);
		_after_interpolation.resize(MAX_DATA_POINTS);
		int k=0;
		_patch[k].SetData(0,0,-2.0,-2.0,0.0);
		_patch[k].SetData(0,1,-2.0,-1.0,0.0);
		_patch[k].SetData(0,2,-2.0,1.0,0.0);
		_patch[k].SetData(0,3,-2.0,2.0,0.0);

		_patch[k].SetData(1,0,-1.0,-2.0,0.0);
		_patch[k].SetData(1,1,-1.0,-1.0,2.0);
		_patch[k].SetData(1,2,-1.0,1.0,2.0);
		_patch[k].SetData(1,3,-1.0,2.0,0.0);

		_patch[k].SetData(2,0,1.0,-2.0,0.0);
		_patch[k].SetData(2,1,1.0,-1.0,2.0);
		_patch[k].SetData(2,2,1.0,1.0,2.0);
		_patch[k].SetData(2,3,1.0,2.0,0.0);

		_patch[k].SetData(3,0,2.0,-2.0,0.0);
		_patch[k].SetData(3,1,2.0,-1.0,0.0);
		_patch[k].SetData(3,2,2.0,1.0,0.0);
		_patch[k].SetData(3,3,2.0,2.0,0.0);
		_patch[k].UpdateVertexBufferObjectsOfData();

		_before_interpolation[k] = _patch[k].GenerateImage(30,30,GL_STATIC_DRAW);

		if(_before_interpolation[k]) {
			if (!_before_interpolation[k]->UpdateVertexBufferObjects())
			{
				cout << "VBO-err" << endl;
			}
			cout<<"before interpolation\n";
		}

		RowMatrix<GLdouble> u_knot_vector(4);
		u_knot_vector(0) = 0.0;
		u_knot_vector(1) = 1.0/3.0;
		u_knot_vector(2) = 2.0/3.0;
		u_knot_vector(3) = 1.0;

		ColumnMatrix<GLdouble> v_knot_vector(4);
		u_knot_vector(0) = 0.0;
		u_knot_vector(1) = 1.0/3.0;
		u_knot_vector(2) = 2.0/3.0;
		u_knot_vector(3) = 1.0;

		Matrix<DCoordinate3> data_points_to_interpolate(4,4);
		for(GLuint row = 0; row<4;++row)
			for(GLuint column = 0;column<4;++column)
				_patch[k].GetData(row,column,data_points_to_interpolate(row,column));

		if(_patch[k].UpdateDataForInterpolation(u_knot_vector,v_knot_vector, data_points_to_interpolate))
		{
			_after_interpolation[k] = _patch[k].GenerateImage(30,30,GL_STATIC_DRAW);

			if(_after_interpolation[k]) {
				_after_interpolation[k]->UpdateVertexBufferObjects();
				cout<<"after interpolation\n";
			}
		}
		patchNr=0;
	}

	void GLWidget::initModel() {
		//		glFrontFace(GL_CCW);
		//model
		if (_mouse.LoadFromOFF("debug/Models/elephant.off", true))
			//					if (_mouse.LoadFromOFF("debug/Models/dinsr.off", true))
			//		if (_mouse.LoadFromOFF("debug/Models/space.off", true))
			//			if (_mouse.LoadFromOFF("debug/Models/volkswagon.off", true))
			//			if (_mouse.LoadFromOFF("debug/Models/king.off", true))
			//			if (_mouse.LoadFromOFF("debug/Models/cube.off", true))
		{
			if (_mouse.UpdateVertexBufferObjects(GL_DYNAMIC_DRAW))
			{
				_angle = 0.0;
				//				_timer->start();
			}
		}
	}

	//-----------------------
	// the rendering function
	//-----------------------
	void GLWidget::paintGL()
	{
		// clears the color and depth buffers
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		// stores/duplicates the original model view matrix
		glPushMatrix();

		// applying transformations
		glRotatef(_angle_x, 1.0, 0.0, 0.0);
		glRotatef(_angle_y, 0.0, 1.0, 0.0);
		glRotatef(_angle_z, 0.0, 0.0, 1.0);
		glTranslated(_trans_x, _trans_y, _trans_z);
		glScaled(_zoom, _zoom, _zoom);

		// render your geometry (this is oldest OpenGL rendering technique, later we will use some advanced methods)

		//			glColor3f(1.0f, 1.0f, 1.0f);
		//			glBegin(GL_LINES);
		//			glVertex3f(0.0f, 0.0f, 0.0f);
		//			glVertex3f(1.1f, 0.0f, 0.0f);

		//			glVertex3f(0.0f, 0.0f, 0.0f);
		//			glVertex3f(0.0f, 1.1f, 0.0f);

		//			glVertex3f(0.0f, 0.0f, 0.0f);
		//			glVertex3f(0.0f, 0.0f, 1.1f);
		//			glEnd();

		//			glBegin(GL_TRIANGLES);
		//			// attributes
		//			glColor3f(1.0f, 0.0f, 0.0f);
		//			// associated with position
		//			glVertex3f(1.0f, 0.0f, 0.0f);

		//			// attributes
		//			glColor3f(0.0, 1.0, 0.0);
		//			// associated with position
		//			glVertex3f(0.0, 1.0, 0.0);

		//			// attributes
		//			glColor3f(0.0f, 0.0f, 1.0f);
		//			// associated with position
		//			glVertex3f(0.0f, 0.0f, 1.0f);
		//			glEnd();

		// pops the current matrix stack, replacing the current matrix with the one below it on the stack,
		// i.e., the original model view matrix is restored
		//			glPopMatrix();

		//		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
		//		glPushMatrix();
		if(showParametricCurves) {
			if(_image_of_pc[_pc_index]) {
				glColor3f(1.0,0.0,0.0);
				_image_of_pc[_pc_index]->RenderDerivatives(0,GL_LINE_STRIP);

				glPointSize(5.0);
				glColor3f(0.0,0.5,0.0);
				_image_of_pc[_pc_index]->RenderDerivatives(1,GL_LINES);
				_image_of_pc[_pc_index]->RenderDerivatives(1,GL_POINTS);
				glColor3f(0.1f,0.5f,0.9f);
				_image_of_pc[_pc_index]->RenderDerivatives(2,GL_LINES);
				_image_of_pc[_pc_index]->RenderDerivatives(2,GL_POINTS);
				glPointSize(1.0);
			}
		}
		if(showCyclicCurves) {
			cyclicCurvesRender();
		}
		if(showParametricSurface) {
			parametricSurfaceRender();
		}
		if(showModel) {
			_materials[selectedMaterial].Apply();
			modelRender();
		}
		if(showPatch) {
			if(showUV) {
				glDisable(GL_LIGHTING);
				glDisable(GL_NORMALIZE);
				glDisable(GL_LIGHT0);
				glColor3f(1.0f,0.0f,0.0f);
				for(int k=0;k<_u_isoline.size();++k){
					for(int i=0;i<_u_isoline[k]->GetColumnCount();++i)
						(*_u_isoline[k])[i]->RenderDerivatives(0,GL_LINE_STRIP);
				}
				glColor3f(1.0f,1.0f,1.0f);
				for(int k=0;k<_v_isoline.size();++k){
					for(int i=0;i<_v_isoline[k]->GetColumnCount();++i) {
						glColor3f(1.0f,1.0f,1.0f);
						(*_v_isoline[k])[i]->RenderDerivatives(0,GL_LINE_STRIP);

//						glPointSize(5.0);
						glColor4f(0.0,0.5,0.0,0.9);
//						GenericCurve3.RenderDerivatives()
						(*_v_isoline[k])[i]->RenderDerivatives(1,GL_LINES);
						(*_v_isoline[k])[i]->RenderDerivatives(1,GL_POINTS);
//						glColor3f(0.1f,0.5f,0.9f);
//						(*_v_isoline[k])[i]->RenderDerivatives(2,GL_LINES);
//						(*_v_isoline[k])[i]->RenderDerivatives(2,GL_POINTS);
//						glPointSize(1.0);
					}
	//				for(int i=0;i<_u_isoline[k]->GetColumnCount();++i) {
	//					for(int i=0;i<1;++i) {
	//					RowMatrix<GenericCurve3*> * tmp=_u_isoline[k];
	//					GenericCurve3*tmp2;
	//					tmp2=(GenericCurve3*)&tmp[i];
	//					tmp2->RenderDerivatives(0,GL_LINE_STRIP);
	//	//					_u_isoline[k][i]->RenderDerivatives(0,GL_LINE_STRIP);
	//	//					_v_isoline[k][i].RenderDerivatives(0,GL_LINE_STRIP);
	//				}
				}
			}
			_materials[selectedMaterial].Apply();
			glEnable(GL_LIGHTING);
			glEnable(GL_NORMALIZE);
			glEnable(GL_LIGHT0);
			for(int k=0;k<patchNr;++k){
//				BicubicBezierPatch.GenerateUIsoparametricLines()
//				_u_isoline[k]->Render();
				if(showControlNet){
					if (!_patch[k].RenderData())
					{
						cout << "Could not render control net!" << endl;
					}
				}

				if(showBeforeInterpolation){
					if(_before_interpolation[k]) {
						//                    MatFBRuby.Apply();
						//				glPointSize(10.0);
						glColor3f(1.0, 0.0, 0.0);
						_before_interpolation[k]->Render();
					}
				}
				if(showAfterInterpolation){
					if(_after_interpolation[k]) {
						glEnable(GL_BLEND);
						glDepthMask(GL_FALSE);
						glBlendFunc(GL_SRC_ALPHA,GL_ONE);
						//						MatFBSilver.Apply();
						MatFBTurquoise.Apply();
						_after_interpolation[k]->Render();
						glDepthMask(GL_TRUE);
						glDisable(GL_BLEND);
					}
				}
			}
			glDisable(GL_LIGHTING);
			glDisable(GL_NORMALIZE);
			glDisable(GL_LIGHT0);
		}

		glPopMatrix();


		//		MatFB Brass.Apply();
		//		_shaders.Enable();
		//		_model.Render();
		//		_shaders.Disable();



		//(
		//			float color[4*10]={0.1,10.3,30.4,40.5};
		//			_shaders.SetUniformVariable4fv("outline.color[0]",1,color);
		//					...)
	}

	void GLWidget::initCyclicCurves()
	{
		_cc = nullptr;
		_image_of_cc = nullptr;
		_n = 3;
		_cc = new CyclicCurves3(_n);
		GLdouble step = TWO_PI / (2 * _n + 1);
		for (GLuint i = 0; i <= 2 * _n; i++)
		{
			GLdouble u = i * step;
			DCoordinate3& cp = (*_cc)[i];
			cp[0] = cos(u);
			cp[1] = sin(u);
			cp[2] = cos(u) * i;
		}
		_cc -> UpdateVertexBufferObjectsOfData();
		GLuint _max_order_of_derivatives = 2;
		GLuint _div_point_count = 100;
		_image_of_cc = _cc -> GenerateImage(_max_order_of_derivatives, _div_point_count);
		_image_of_cc -> UpdateVertexBufferObjects();

		_icc = new CyclicCurves3(_n);
		_d.ResizeRows(2 * _n + 1);
		_u_icc.ResizeRows(2 * _n + 1);
		for (GLuint i = 0; i <= 2 * _n; i++)
		{
			_u_icc[i] = i * step;
			DCoordinate3 &p = _d[i];
			p[0] = cos(_u_icc[i]);
			p[1] = sin(_u_icc[i]);
			p[2] = cos(_u_icc[i]) * i;
		}
		if  (!_icc->UpdateDataForInterpolation(_u_icc, _d))
		{
			//error
		}
		_icc->UpdateVertexBufferObjectsOfData();
		_image_of_icc = _icc->GenerateImage(_max_order_of_derivatives, _div_point_count);
		_image_of_icc->UpdateVertexBufferObjects();
	}

	void GLWidget::cyclicCurvesRender()
	{
		glColor3f(0.5, 0.5, 0.5);
		glPointSize(10.0);
		glBegin(GL_POINTS);
		for (GLuint i = 0; i <= 2 * _n; i++)
		{
			glVertex3f(_d[i][0], _d[i][1], _d[i][2]);
		}
		glEnd();
		glPointSize(1.0);

		cyclicCurveRender();
		interpolatingCyclicCurveRender();
		if (_icc)
		{
			glColor3f(1.0, 0.0, 1.0);
			_cc->RenderData(GL_LINE_LOOP);
		}
	}

	void GLWidget::cyclicCurveRender()
	{
		if (_image_of_cc)
		{
			glColor3f(1.0, 0.0, 0.0);
			_image_of_cc -> RenderDerivatives(0, GL_LINE_STRIP);
		}
	}

	void GLWidget::interpolatingCyclicCurveRender()
	{
		if (_image_of_icc) {
			glColor3f(1.0, 1.0, 0.0);
			_image_of_icc -> RenderDerivatives(0, GL_LINE_LOOP);
		}
	}

	void GLWidget::parametricSurfaceRender()
	{
		if (dl)
		{
			glEnable(GL_LIGHTING);
			glEnable(GL_NORMALIZE);
			//glEnable(GL_LIGHT0);
			dl->Enable();
			if (_image_of_ps[_pc_index])
			{
				_image_of_ps[_pc_index]->Render();
			}
			dl->Disable();
			glDisable(GL_NORMALIZE);
			glDisable(GL_LIGHTING);
			//        glDisable(GL_LIGHT0);
		}
	}


	void GLWidget::modelRender()
	{
		glEnable(GL_LIGHTING);
		glEnable(GL_NORMALIZE);
		glEnable(GL_LIGHT0);

		switch (_pc_index) {
		case 0:
			MatFBBrass.Apply();
			break;
		case 1:
			MatFBRuby.Apply();
			break;
		case 2:
			MatFBPearl.Apply();
			break;
		case 3:
			MatFBTurquoise.Apply();
			break;
		case 4:
			MatFBGold.Apply();
			break;
		case 5:
			MatFBBrass.Apply();
			break;
		default:
			break;
		}
		_mouse.Render();

		glDisable(GL_LIGHTING);
		glDisable(GL_NORMALIZE);
		glDisable(GL_LIGHT0);
	}
	//----------------------------------------------------------------------------
	// when the main window is resized one needs to redefine the projection matrix
	//----------------------------------------------------------------------------
	void GLWidget::resizeGL(int w, int h)
	{
		// setting the new size of the rendering context
		glViewport(0, 0, w, h);

		// redefining the projection matrix
		glMatrixMode(GL_PROJECTION);

		glLoadIdentity();

		_aspect = (float)w / (float)h;

		gluPerspective(_fovy, _aspect, _z_near, _z_far);

		// switching back to the model view matrix
		glMatrixMode(GL_MODELVIEW);

		updateGL();
	}

	//-----------------------------------
	// implementation of the public slots
	//-----------------------------------

	void GLWidget::_animate()
	{
		GLfloat *vertex = _mouse.MapVertexBuffer(GL_READ_WRITE);
		GLfloat *normal = _mouse.MapNormalBuffer(GL_READ_ONLY);

		_angle += DEG_TO_RADIAN;
		if(_angle >= TWO_PI) _angle -= TWO_PI;

		GLfloat scale = sin(_angle) / 3000.0;
		for (GLuint i = 0; i < _mouse.VertexCount(); ++i)
		{
			for (GLuint coordinate = 0; coordinate < 3; ++coordinate, ++vertex, ++normal)
				*vertex += scale * (*normal);
		}

		_mouse.UnmapVertexBuffer();
		_mouse.UnmapNormalBuffer();

		updateGL();
	}

	void GLWidget::set_angle_x(int value)
	{
		if (_angle_x != value)
		{
			_angle_x = value;
			updateGL();
		}
	}

	void GLWidget::set_angle_y(int value)
	{
		if (_angle_y != value)
		{
			_angle_y = value;
			updateGL();
		}
	}

	void GLWidget::set_angle_z(int value)
	{
		if (_angle_z != value)
		{
			_angle_z = value;
			updateGL();
		}
	}

	void GLWidget::set_zoom_factor(double value)
	{
		if (_zoom != value)
		{
			_zoom = value;
			updateGL();
		}
	}

	void GLWidget::set_trans_x(double value)
	{
		if (_trans_x != value)
		{
			_trans_x = value;
			updateGL();
		}
	}

	void GLWidget::set_trans_y(double value)
	{
		if (_trans_y != value)
		{
			_trans_y = value;
			updateGL();
		}
	}

	void GLWidget::set_trans_z(double value)
	{
		if (_trans_z != value)
		{
			_trans_z = value;
			updateGL();
		}
	}

	void GLWidget::setConstant(int value)
	{
		epicycloid::constant2=value;
		vivianis::constant2=value;
		if (mobius::constant2 != value)
		{
			mobius::constant2 = value;
			cout<<mobius::constant2<<"\n";

			delete _pc[1];
			delete _image_of_pc[1];

			_pc[1]=0;
			RowMatrix<ParametricCurve3::Derivative> derivative(3);
			derivative(0)=epicycloid::d0;
			derivative(1)=epicycloid::d1;
			derivative(2)=epicycloid::d2;
			_pc[1]=new ParametricCurve3(derivative,epicycloid::u_min,epicycloid::u_max);
			_image_of_pc[1]=0;
			_image_of_pc[1]=_pc[1]->GenerateImage(200);
			if(!_image_of_pc[1])
			{
				cerr<<"Error at _pc->GenerateImage\n";
			}
			if(!_image_of_pc[1]->UpdateVertexBufferObjects(GL_STATIC_DRAW,0.1))
				//			if(!_image_of_pc[1]->UpdateVertexBufferObjects(GL_STATIC_DRAW))
			{
				cout<<"Could not create the vertex buffer object of the parametric curve!\n";
			}
			updateGL();
		}
	}

	void GLWidget::setParametricCurveIndex(int index){
		if(_pc_index!=index)
		{
			_pc_index=index;
			updateGL();
		}
	}
	void GLWidget::setShowParametricCurve(bool x){
		if(showParametricCurves!=x){
			showParametricCurves=x;
			updateGL();
		}
	}
	void GLWidget::setShowCyclicCurve(bool x){
		if(showCyclicCurves!=x){
			showCyclicCurves=x;
			updateGL();
		}
	}
	void GLWidget::setShowParametricSurface(bool x){
		if(showParametricSurface!=x){
			showParametricSurface=x;
			updateGL();
		}
	}
	void GLWidget::setShowModel(bool x){
		if(showModel!=x){
			showModel=x;
			updateGL();
		}
	}
	void GLWidget::setShowPatch(bool x){
		if(showPatch!=x){
			showPatch=x;
			updateGL();
		}
	}

	void GLWidget::setJoinDirection(int direction) {
		_joinDirectionValue=direction;
	}

	void GLWidget::addNewBicubicBezierSurface(GLint x,GLint y){
		bool was=false;
		if(lastX!=-1&&x==0&&y==0) {
			was=true;
			if(direction==1) {
				if(-lastX+3==lastY){
					direction=2;
					x=lastX+3;
					y=lastY;
				}else{
					x=lastX;
					y=lastY+3;
				}
			}
			else if(direction==2) {
				if(lastX==lastY){
					direction=3;
					x=lastX;
					y=lastY-3;
				}else{
					x=lastX+3;
					y=lastY;
				}
			}
			else if(direction==3) {
				if(lastX==-lastY){
					direction=4;
					x=lastX-3;
					y=lastY;
				}else{
					x=lastX;
					y=lastY-3;
				}
			}
			else if(direction==4) {
				if(lastX==lastY){
					direction=1;
					x=lastX;
					y=lastY+3;
				}else{
					x=lastX-3;
					y=lastY;
				}
			}
		}
		lastX=x;
		lastY=y;
		patchNr++;
		_patch.resize(patchNr);
		_before_interpolation.resize(patchNr);
		_after_interpolation.resize(patchNr);

		GLuint i = patchNr-1;
		//		_patch[i].SetData(0,0,x,y,0.0);
		//		_patch[i].SetData(0,1,x,y+1.0,0.0);
		//		_patch[i].SetData(0,2,x,y+2.0,0.0);
		//		_patch[i].SetData(0,3,x,y+3.0,0.0);

		//		_patch[i].SetData(1,0,x+1.0,y,0.0);
		//		_patch[i].SetData(1,1,x+1.0,y+1.0,1.0);
		//		_patch[i].SetData(1,2,x+1.0,y+2.0,0.0);
		//		_patch[i].SetData(1,3,x+1.0,y+3.0,2.0);

		//		_patch[i].SetData(2,0,x+2.0,y,0.0);
		//		_patch[i].SetData(2,1,x+2.0,y+1.0,0.0);
		//		_patch[i].SetData(2,2,x+2.0,y+2.0,1.0);
		//		_patch[i].SetData(2,3,x+2.0,y+3.0,0.0);

		//		_patch[i].SetData(3,0,x+3.0,y,0.0);
		//		_patch[i].SetData(3,1,x+3.0,y+1.0,0.0);
		//		_patch[i].SetData(3,2,x+3.0,y+2.0,0.0);
		//		_patch[i].SetData(3,3,x+3.0,y+3.0,0.0);

		//Random
		int z;
		int q=3,p=1;
		z=rand()%q-p;
		_patch[i].SetData(0,0,x,y,z);
		z=rand()%q-p;
		_patch[i].SetData(0,1,x,y+1.0,z);
		z=rand()%q-p;
		_patch[i].SetData(0,2,x,y+2.0,z);
		z=rand()%q-p;
		_patch[i].SetData(0,3,x,y+3.0,z);

		z=rand()%q-p;
		_patch[i].SetData(1,0,x+1.0,y,z);
		z=rand()%q-p;
		_patch[i].SetData(1,1,x+1.0,y+1.0,z);
		z=rand()%q-p;
		_patch[i].SetData(1,2,x+1.0,y+2.0,z);
		z=rand()%q-p;
		_patch[i].SetData(1,3,x+1.0,y+3.0,z);

		z=rand()%q-p;
		_patch[i].SetData(2,0,x+2.0,y,z);
		z=rand()%q-p;
		_patch[i].SetData(2,1,x+2.0,y+1.0,z);
		z=rand()%q-p;
		_patch[i].SetData(2,2,x+2.0,y+2.0,z);
		z=rand()%q-p;
		_patch[i].SetData(2,3,x+2.0,y+3.0,z);

		z=rand()%q-p;
		_patch[i].SetData(3,0,x+3.0,y,z);
		z=rand()%q-p;
		_patch[i].SetData(3,1,x+3.0,y+1.0,z);
		z=rand()%q-p;
		_patch[i].SetData(3,2,x+3.0,y+2.0,z);
		z=rand()%q-p;
		_patch[i].SetData(3,3,x+3.0,y+3.0,z);

		_patch[i].UpdateVertexBufferObjectsOfData();

		moveBicubicBezierSurface(4,i);

		if(was&&i>0) {
			int tmp1=firstToJoin;
			int tmp2=secondToJoin;
			int tmp3=_joinDirectionValue;
			secondToJoin=i;
			int g=sqrt(i);
			if(g*g!=i&&i!=g*g+g){
				if(direction%2==1){
					firstToJoin=(g-1)*(g-1)-((g+1)*(g+1)-i-1);
					if(direction==3){
						_joinDirectionValue=2;
					}
					else if(direction==1){
						_joinDirectionValue=3;
					}
					join();
				}
				else {
					firstToJoin=(g-1)*(g-1)-((g+1)*(g+1)-i-1)+2;
					if(direction==4){
						_joinDirectionValue=1;
					}
					else if(direction==2){
						_joinDirectionValue=0;
					}
					join();
				}
				if(direction==3){
					_joinDirectionValue=1;
				}
				else if(direction==4){
					_joinDirectionValue=3;
				}
				else if(direction==1){
					_joinDirectionValue=0;
				}
				else if(direction==2){
					_joinDirectionValue=2;
				}
				firstToJoin=i-1;
				join();
			}
			else {
				if(direction==3){
					_joinDirectionValue=1;
				}
				else if(direction==4){
					_joinDirectionValue=3;
				}
				else if(direction==1){
					_joinDirectionValue=0;
				}
				else if(direction==2){
					_joinDirectionValue=2;
				}
				firstToJoin=i-1;
				join();
			}
			firstToJoin=tmp1;
			secondToJoin=tmp2;
			_joinDirectionValue=tmp3;
		}
		_u_isoline.resize(patchNr);
		_v_isoline.resize(patchNr);
//		BicubicBezierPatch.GenerateUIsoparametricLines()
		_u_isoline[i]=_patch[i].GenerateUIsoparametricLines(30,1,30);
		_v_isoline[i]=_patch[i].GenerateVIsoparametricLines(25,1,30);

		for(int k=0;k<_u_isoline[i]->GetColumnCount();++k) {
			(*_u_isoline[i])[k]->UpdateVertexBufferObjects();
		}
		for(int k=0;k<_v_isoline[i]->GetColumnCount();++k) {
			(*_v_isoline[i])[k]->UpdateVertexBufferObjects(GL_STATIC_DRAW,0.1);
		}
	}

	void GLWidget::setX(int x){
		this->indexX = x;
	}

	void GLWidget::setY(int y){
		this->indexY=y;
	}

	void GLWidget::addNewMesh(){
		addNewBicubicBezierSurface(indexX,indexY);
	}

	void GLWidget::set_show_control_net(bool value){
		showControlNet=value;
		updateGL();
	}

	void GLWidget::set_after_interpolation(bool value){
		showAfterInterpolation=value;
		updateGL();
	}

	void GLWidget::set_before_interpolation(bool value){
		showBeforeInterpolation=value;
		updateGL();
	}

	void GLWidget::setFirstToJoin(int first){
		firstToJoin=first;
	}

	void GLWidget::setSecondToJoin(int second){
		secondToJoin=second;
	}

	void GLWidget::setMovingID(int mID){
		movingID=mID;
	}

	void GLWidget::moveLeft(){

		moveBicubicBezierSurface(0,movingID);

		updateGL();
	}
	void GLWidget::moveRight(){

		moveBicubicBezierSurface(1,movingID);

		updateGL();
	}

	void GLWidget::moveDown(){

		moveBicubicBezierSurface(2,movingID);

		updateGL();
	}

	void GLWidget::moveUp(){

		moveBicubicBezierSurface(3,movingID);

		updateGL();
	}

	void GLWidget::moveBicubicBezierSurface(int dir, GLint i){
		if(i>=patchNr) {
			return;
		}
		switch(dir){
		case 0:
			for(GLuint row=0;row<4;++row){
				for(GLuint column=0;column<4;++column){
					GLdouble xx,yy,zz;
					_patch[i].GetData(row,column,xx,yy,zz);
					_patch[i].SetData(row,column,xx-1,yy,zz);
				}
			}
			break;
		case 1:
			for(GLuint row=0;row<4;++row){
				for(GLuint column=0;column<4;++column){
					GLdouble xx,yy,zz;
					_patch[i].GetData(row,column,xx,yy,zz);
					_patch[i].SetData(row,column,xx+1,yy,zz);
				}
			}
			break;
		case 2:
			for(GLuint row=0;row<4;++row){
				for(GLuint column=0;column<4;++column){
					GLdouble xx,yy,zz;
					_patch[i].GetData(row,column,xx,yy,zz);
					_patch[i].SetData(row,column,xx,yy-1,zz);
				}
			}
			break;
		case 3:
			for(GLuint row=0;row<4;++row){
				for(GLuint column=0;column<4;++column){
					GLdouble xx,yy,zz;
					_patch[i].GetData(row,column,xx,yy,zz);
					_patch[i].SetData(row,column,xx,yy+1,zz);
				}
			}
			break;
		}

		_patch[i].UpdateVertexBufferObjectsOfData();

		//generatethemeshofthesurface_patch
		_before_interpolation[i]=_patch[i].GenerateImage(30,30,GL_STATIC_DRAW);

		if(_before_interpolation[i])
			_before_interpolation[i]->UpdateVertexBufferObjects();

		//defineaninterpolationproblem:
		//1:createaknotvectorinu-direction
		RowMatrix<GLdouble> u_knot_vektor(4);
		u_knot_vektor(0)=0.0;
		u_knot_vektor(1)=1.0/3.0;
		u_knot_vektor(2)=2.0/3.0;
		u_knot_vektor(3)=1.0;

		//2:createaknotvectorinv-direction
		ColumnMatrix<GLdouble>v_knot_vektor(4);
		v_knot_vektor(0)=0.0;
		v_knot_vektor(1)=1.0/3.0;
		v_knot_vektor(2)=2.0/3.0;
		v_knot_vektor(3)=1.0;

		//3:defineamatrixofdata_points,e.}.setthemtotheoriginalcontrolpoints
		Matrix<DCoordinate3> data_points_to_interpolate(4,4);
		for(GLuint row=0;row<4;++row)
			for(GLuint column=0;column<4;++column){
				_patch[i].GetData(row,column,data_points_to_interpolate(row,column));
				_patch[i].GetData(row,column,_data_points[i](row,column));
			}

		//4:solvetheinterpolationproblemandgeneratethemeshoftheinterpolating_patch
		if(_patch[i].UpdateDataForInterpolation(u_knot_vektor,v_knot_vektor,data_points_to_interpolate))
		{
			_after_interpolation[i] = _patch[i].GenerateImage(30,30,GL_STATIC_DRAW);

			if(_after_interpolation[i])
				_after_interpolation[i]->UpdateVertexBufferObjects();
		}

		for(GLuint row=0;row<4;++row)
			for(GLuint column=0;column<4;++column){
				_patch[i].SetData(row,column,_data_points[i](row,column));
			}
		updateGL();
	}

	void GLWidget::join(){
		int first = firstToJoin, second =secondToJoin;
		join(first,second,_joinDirectionValue);
	}

	void GLWidget::join(int first,int second,int joinDirectionValue){

		for (int i = 0; i < 4; ++i)
		{
			switch (joinDirectionValue)
			{
			case 0:
				//North
				_patch[second](i, 1) = 2*_patch[first](i, 3) - _patch[first](i, 2);
				_patch[second](i, 0) = _patch[first](i, 3);
				_patch[first].north=second;
				_patch[second].south=first;
				break;
			case 1:
				//South
				_patch[second](i, 2) = 2*_patch[first](i, 0) - _patch[first](i, 1);
				_patch[second](i, 3) = _patch[first](i, 0);
				_patch[first].south=second;
				_patch[second].north=first;
				break;
			case 2:
				//East
				_patch[second](1, i) = 2*_patch[first](3, i) - _patch[first](2, i);
				_patch[second](0, i) = _patch[first](3, i);
				_patch[first].east=second;
				_patch[second].west=first;
				break;
			case 3:
				//West
				_patch[second](2, i) = 2*_patch[first](0, i) - _patch[first](1, i);
				_patch[second](3, i) = _patch[first](0, i);
				_patch[first].west=second;
				_patch[second].east=first;
				break;
			default:
				return;
			}
		}
		//		patchUpdate(first);
		patchUpdate(second);
	}

	void GLWidget::patchUpdate(int i) {
		_patch[i].UpdateVertexBufferObjectsOfData();
		//generatethemeshofthesurface_patch
		_before_interpolation[i]=_patch[i].GenerateImage(30,30,GL_STATIC_DRAW);

		if(_before_interpolation[i])
			_before_interpolation[i]->UpdateVertexBufferObjects();

		//defineaninterpolationproblem:
		//1:createaknotvectorinu-direction
		RowMatrix<GLdouble> u_knot_vektor(4);
		u_knot_vektor(0)=0.0;
		u_knot_vektor(1)=1.0/3.0;
		u_knot_vektor(2)=2.0/3.0;
		u_knot_vektor(3)=1.0;

		//2:createaknotvectorinv-direction
		ColumnMatrix<GLdouble>v_knot_vektor(4);
		v_knot_vektor(0)=0.0;
		v_knot_vektor(1)=1.0/3.0;
		v_knot_vektor(2)=2.0/3.0;
		v_knot_vektor(3)=1.0;

		//3:defineamatrixofdata_points,e.}.setthemtotheoriginalcontrolpoints
		Matrix<DCoordinate3> data_points_to_interpolate(4,4);
		for(GLuint row=0;row<4;++row)
			for(GLuint column=0;column<4;++column){
				_patch[i].GetData(row,column,data_points_to_interpolate(row,column));
				_patch[i].GetData(row,column,_data_points[i](row,column));
			}

		//4:solvetheinterpolationproblemandgeneratethemeshoftheinterpolating_patch
		if(_patch[i].UpdateDataForInterpolation(u_knot_vektor,v_knot_vektor,data_points_to_interpolate))
		{

			_after_interpolation[i] = _patch[i].GenerateImage(30,30,GL_STATIC_DRAW);

			if(_after_interpolation[i])
				_after_interpolation[i]->UpdateVertexBufferObjects();
		}
		for(GLuint row=0;row<4;++row)
			for(GLuint column=0;column<4;++column){
				_patch[i].SetData(row,column,_data_points[i](row,column));
			}
		updateGL();
	}

	void GLWidget::setMaterialIndex(int value) {
		if(selectedMaterial!=value){
			selectedMaterial=value;
			updateGL();
		}
	}

	void GLWidget::setChangeControlIndexPatch(int i){
		changeControlIndexPatch=i;
	}

	void GLWidget::setChangeControlIndexI(int i){
		changeControlIndexI=i;
	}

	void GLWidget::setChangeControlIndexJ(int j){
		changeControlIndexJ=j;
	}

	void GLWidget::onUpChangeControl() {
		modifyZ(0.1,true);
	}

	void GLWidget::onDownChangeControl() {
		modifyZ(-0.1,true);
	}

	void GLWidget::modifyZ(GLdouble z,bool first){
		GLint i = changeControlIndexPatch;
		GLdouble xx, yy, zz;

		_patch[i].GetData(changeControlIndexI,changeControlIndexJ,xx,yy,zz);
		_patch[i].SetData(changeControlIndexI,changeControlIndexJ,xx,yy,zz+z);
		if(first){
			if(changeControlIndexI==0) {
				if(_patch[i].west!=-1) {
					changeControlIndexPatch=_patch[i].west;
					changeControlIndexI=3;
					modifyZ(z,false);
					changeControlIndexI=0;
					changeControlIndexPatch=i;
				}
			}
			if(changeControlIndexJ==0) {
				if(_patch[i].south!=-1) {
					changeControlIndexPatch=_patch[i].south;
					changeControlIndexJ=3;
					modifyZ(z,false);
					join(i,changeControlIndexPatch,1);
					changeControlIndexJ=0;
					changeControlIndexPatch=i;
				}
			}
			if(changeControlIndexI==3) {
				if(_patch[i].east!=-1) {
					changeControlIndexI=0;
					changeControlIndexPatch=_patch[i].east;
					modifyZ(z,false);
//					join(i,changeControlIndexPatch,2);
					changeControlIndexI=3;
					changeControlIndexPatch=i;
				}
			}
			if(changeControlIndexJ==3) {
				if(_patch[i].north!=-1) {
					changeControlIndexPatch=_patch[i].north;
					changeControlIndexJ=0;
					modifyZ(z,false);
//					join(i,changeControlIndexPatch,0);
					changeControlIndexJ=3;
					changeControlIndexPatch=i;
				}
			}
			if(_patch[i].west!=-1) {
				join(i,_patch[i].west,3);
			}
			if(_patch[i].north!=-1) {
				join(i,_patch[i].north,0);
			}
			if(_patch[i].south!=-1) {
				join(i,_patch[i].south,1);
			}
			if(_patch[i].east!=-1) {
				join(i,_patch[i].east,2);
			}
		}
		//		else {

		//			if(changeControlIndexI==0) {
		//				if(_patch[i].west!=-1) {
		//					changeControlIndexPatch=_patch[i].west;
		//					join(i,changeControlIndexPatch,3);
		//					changeControlIndexPatch=i;
		//				}
		//			}
		//			if(changeControlIndexJ==0) {
		//				if(_patch[i].south!=-1) {
		//					changeControlIndexPatch=_patch[i].south;
		//					join(i,changeControlIndexPatch,1);
		//					changeControlIndexPatch=i;
		//				}
		//			}
		//			if(changeControlIndexI==3) {
		//				if(_patch[i].east!=-1) {
		//					changeControlIndexPatch=_patch[i].east;
		//					join(i,changeControlIndexPatch,2);
		//					changeControlIndexPatch=i;
		//				}
		//			}
		//			if(changeControlIndexJ==3) {
		//				if(_patch[i].north!=-1) {
		//					changeControlIndexPatch=_patch[i].north;
		//					join(i,changeControlIndexPatch,0);
		//					changeControlIndexPatch=i;
		//				}
		//			}
		//		}
		_patch[i].UpdateVertexBufferObjectsOfData();


		//generatethemeshofthesurface_patch
		_before_interpolation[i]=_patch[i].GenerateImage(30,30,GL_STATIC_DRAW);

		if(_before_interpolation[i])
			_before_interpolation[i]->UpdateVertexBufferObjects();

		//defineaninterpolationproblem:
		//1:createaknotvectorinu-direction
		RowMatrix<GLdouble> u_knot_vektor(4);
		u_knot_vektor(0)=0.0;
		u_knot_vektor(1)=1.0/3.0;
		u_knot_vektor(2)=2.0/3.0;
		u_knot_vektor(3)=1.0;

		//2:createaknotvectorinv-direction
		ColumnMatrix<GLdouble>v_knot_vektor(4);
		v_knot_vektor(0)=0.0;
		v_knot_vektor(1)=1.0/3.0;
		v_knot_vektor(2)=2.0/3.0;
		v_knot_vektor(3)=1.0;

		//3:defineamatrixofdata_points,e.}.setthemtotheoriginalcontrolpoints
		Matrix<DCoordinate3> data_points_to_interpolate(4,4);
		for(GLuint row=0;row<4;++row)
			for(GLuint column=0;column<4;++column){
				_patch[i].GetData(row,column,data_points_to_interpolate(row,column));
				_patch[i].GetData(row,column,_data_points[i](row,column));
			}

		//4:solvetheinterpolationproblemandgeneratethemeshoftheinterpolating_patch
		if(_patch[i].UpdateDataForInterpolation(u_knot_vektor,v_knot_vektor,data_points_to_interpolate))
		{


			_after_interpolation[i] = _patch[i].GenerateImage(30,30,GL_STATIC_DRAW);

			if(_after_interpolation[i])
				_after_interpolation[i]->UpdateVertexBufferObjects();
		}

		for(GLuint row=0;row<4;++row)
			for(GLuint column=0;column<4;++column){
				_patch[i].SetData(row,column,_data_points[i](row,column));
			}
		if(first){
			updateGL();
		}
	}

	void GLWidget::setShowUV(bool value) {
		showUV=value;
		updateGL();
	}
}
