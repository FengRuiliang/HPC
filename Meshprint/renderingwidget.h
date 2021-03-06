#ifndef RENDERINGWIDGET_H
#define RENDERINGWIDGET_H
//////////////////////////////////////////////////////////////////////////
#include <QOpenGLWidget>
#include "globalFunctions.h"
#include <QEvent>
#include "HE_mesh/Vec.h"
#include "HE_mesh/Mesh3D.h"
using trimesh::vec;
using trimesh::point;
typedef trimesh::vec3  Vec3f;
class Meshprint;
class MainWindow;
class CArcBall;
class Mesh3D;

class RenderingWidget : public QOpenGLWidget
{
	Q_OBJECT

public:
	Meshprint* parent;

	RenderingWidget(QWidget *parent, MainWindow* mainwindow=0);
	~RenderingWidget();

protected:
	void initializeGL();
	void resizeGL(int w, int h);
	void paintGL();
	void timerEvent(QTimerEvent *e);

	// mouse events
	void mousePressEvent(QMouseEvent *e);
	void mouseMoveEvent(QMouseEvent *e);
	void mouseReleaseEvent(QMouseEvent *e);
	void mouseDoubleClickEvent(QMouseEvent *e);
	void wheelEvent(QWheelEvent *e);

public:
	void keyPressEvent(QKeyEvent *e);
	void keyReleaseEvent(QKeyEvent *e);
private:
	void Render();
	void SetLight();
public slots:
	void SetBackground();
	void ReadMesh();
	void WriteMesh();
	void CheckDrawPoint();
	void CheckDrawEdge();
	void CheckDrawFace();
	void CheckLight();
	void CheckGrid();
	void CheckDrawTexture();
private:
	void DrawAxes(bool bv);
	void DrawPoints(bool);
	void DrawEdge(bool);
	void DrawFace(bool);
	void DrawTexture(bool);
	void DrawGrid(bool bV);
public:
	MainWindow					*ptr_mainwindow_;
	CArcBall					*ptr_arcball_;
	Mesh3D						*ptr_mesh_;
	bool						isAddPoint;
	bool						isAddLine;
	bool						isDelete;
	std::vector<int>			delete_points_;
	std::vector<Vec3f>			line_points_;

	// Texture
	GLuint						texture_[1];
	bool						is_load_texture_;

	// eye
	GLfloat						eye_distance_;
	point						eye_goal_;
	vec							eye_direction_;
	QPoint						current_position_;

	// Render information
	bool						is_draw_point_;
	bool						is_draw_edge_;
	bool						is_move_module_;
	bool						is_draw_face_;
	bool						is_draw_texture_;
	bool						has_lighting_;
	bool						is_draw_axes_;
	bool						is_draw_grid_;
	bool						is_draw_cutpieces_;
	bool						is_select_face;
	bool						is_draw_hatch_;
	bool						is_show_all;
};

#endif // RENDERINGWIDGET_H
