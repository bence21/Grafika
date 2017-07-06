#include "MainWindow.h"

namespace cagd
{
	MainWindow::MainWindow(QWidget *parent): QMainWindow(parent)
	{
		setupUi(this);

	/*

	  the structure of the main window's central widget

	 *---------------------------------------------------*
	 |                 central widget                    |
	 |                                                   |
	 |  *---------------------------*-----------------*  |
	 |  |     rendering context     |   scroll area   |  |
	 |  |       OpenGL widget       | *-------------* |  |
	 |  |                           | | side widget | |  |
	 |  |                           | |             | |  |
	 |  |                           | |             | |  |
	 |  |                           | *-------------* |  |
	 |  *---------------------------*-----------------*  |
	 |                                                   |
	 *---------------------------------------------------*

	*/
		_side_widget = new SideWidget(this);

		_scroll_area = new QScrollArea(this);
		_scroll_area->setWidget(_side_widget);
		_scroll_area->setSizePolicy(_side_widget->sizePolicy());
		_scroll_area->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);

		_gl_widget = new GLWidget(this);

		centralWidget()->setLayout(new QHBoxLayout());
		centralWidget()->layout()->addWidget(_gl_widget);
		centralWidget()->layout()->addWidget(_scroll_area);

		// creating a signal slot mechanism between the rendering context and the side widget
		connect(_side_widget->rotate_x_slider, SIGNAL(valueChanged(int)), _gl_widget, SLOT(set_angle_x(int)));
		connect(_side_widget->rotate_y_slider, SIGNAL(valueChanged(int)), _gl_widget, SLOT(set_angle_y(int)));
		connect(_side_widget->rotate_z_slider, SIGNAL(valueChanged(int)), _gl_widget, SLOT(set_angle_z(int)));

		connect(_side_widget->zoom_factor_spin_box, SIGNAL(valueChanged(double)), _gl_widget, SLOT(set_zoom_factor(double)));

		connect(_side_widget->trans_x_spin_box, SIGNAL(valueChanged(double)), _gl_widget, SLOT(set_trans_x(double)));
		connect(_side_widget->trans_y_spin_box, SIGNAL(valueChanged(double)), _gl_widget, SLOT(set_trans_y(double)));
		connect(_side_widget->trans_z_spin_box, SIGNAL(valueChanged(double)), _gl_widget, SLOT(set_trans_z(double)));
		connect(_side_widget->spinBox, SIGNAL(valueChanged(int)), _gl_widget, SLOT(setConstant(int)));
		connect(_side_widget->parametricIndexSpinBox, SIGNAL(valueChanged(int)), _gl_widget, SLOT(setParametricCurveIndex(int)));
		connect(_side_widget->parametricRadioButton,SIGNAL(toggled(bool)),_gl_widget,SLOT(setShowParametricCurve(bool)));
		connect(_side_widget->cyclicRadioButton,SIGNAL(toggled(bool)),_gl_widget,SLOT(setShowCyclicCurve(bool)));
		connect(_side_widget->parametricSurfaceRadioButton,SIGNAL(toggled(bool)),_gl_widget,SLOT(setShowParametricSurface(bool)));
		connect(_side_widget->modelRadioButton,SIGNAL(toggled(bool)),_gl_widget,SLOT(setShowModel(bool)));
		connect(_side_widget->bicubicBezierRadioButton,SIGNAL(toggled(bool)),_gl_widget,SLOT(setShowPatch(bool)));

		connect(_side_widget->firstSpinBox,SIGNAL(valueChanged(int)),_gl_widget,SLOT(setFirstToJoin(int)));
		connect(_side_widget->secondSpinBox,SIGNAL(valueChanged(int)),_gl_widget,SLOT(setSecondToJoin(int)));
		connect(_side_widget->joinButton,SIGNAL(clicked()),_gl_widget,SLOT(join()));
		connect(_side_widget->directionComboBox,SIGNAL(currentIndexChanged(int)),_gl_widget,SLOT(setJoinDirection(int)));

		connect(_side_widget->newMeshXSpinBox,SIGNAL(valueChanged(int)),_gl_widget,SLOT(setX(int)));
		connect(_side_widget->newMeshYSpinBox,SIGNAL(valueChanged(int)),_gl_widget,SLOT(setY(int)));
		connect(_side_widget->newMeshButton,SIGNAL(clicked()),_gl_widget,SLOT(addNewMesh()));

		connect(_side_widget->move_down_button,SIGNAL(clicked()),_gl_widget,SLOT(moveDown()));
		connect(_side_widget->move_up_button,SIGNAL(clicked()),_gl_widget,SLOT(moveUp()));
		connect(_side_widget->move_left_button,SIGNAL(clicked()),_gl_widget,SLOT(moveLeft()));
		connect(_side_widget->move_right_button,SIGNAL(clicked()),_gl_widget,SLOT(moveRight()));
		connect(_side_widget->idSpinBox,SIGNAL(valueChanged(int)),_gl_widget,SLOT(setMovingID(int)));

		connect(_side_widget->after_inter_check,SIGNAL(toggled(bool)),_gl_widget,SLOT(set_after_interpolation(bool)));
		connect(_side_widget->before_inter_check,SIGNAL(toggled(bool)),_gl_widget,SLOT(set_before_interpolation(bool)));
		connect(_side_widget->control_net_check,SIGNAL(toggled(bool)),_gl_widget,SLOT(set_show_control_net(bool)));

		connect(_side_widget->material_combo,SIGNAL(currentIndexChanged(int)),_gl_widget,SLOT(setMaterialIndex(int)));

		connect(_side_widget->upPushButton,SIGNAL(clicked()),_gl_widget,SLOT(onUpChangeControl()));
		connect(_side_widget->downPushButton,SIGNAL(clicked()),_gl_widget,SLOT(onDownChangeControl()));
		connect(_side_widget->patchIndexSpinBox,SIGNAL(valueChanged(int)),_gl_widget,SLOT(setChangeControlIndexPatch(int)));
		connect(_side_widget->iSpinBox,SIGNAL(valueChanged(int)),_gl_widget,SLOT(setChangeControlIndexI(int)));
		connect(_side_widget->jSpinBox,SIGNAL(valueChanged(int)),_gl_widget,SLOT(setChangeControlIndexJ(int)));

		connect(_side_widget->showUVCheckBox,SIGNAL(toggled(bool)),_gl_widget,SLOT(setShowUV(bool)));
	}

	//--------------------------------
	// implementation of private slots
	//--------------------------------
	void MainWindow::on_action_Quit_triggered()
	{
		qApp->exit(0);
	}
}
