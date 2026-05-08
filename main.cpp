#include <math.h>
#include <stdlib.h>
#include <GL/glut.h>

/*
** 押し出し形状
*/
#include "extrusion.h"

/* 軸 */
static double spine[][3] = {
  { 0.0, 0.0, 0.0 },
  { 0.0, 1.0, 0.0 },
  { 2.0, 1.0, 0.0 },
  { 2.0, -1.0, 0.0 },
  { 2.0, -1.0, 2.0 },
};
#define NS (sizeof(spine) / (sizeof(double) * 3))

/* 断面*/
static double crossSection[][2] = {
  { -0.7, -0.7 },
  {  0.7, -0.7 },
  {  0.7,  0.7 },
  {  0.4,  0.7 },
  {  0.4, -0.4 },
  { -0.4, -0.4 },
  { -0.4,  0.7 },
  { -0.7,  0.7 },
  { -0.7, -0.7 },
};
#define NC (sizeof(crossSection) / (sizeof(double) * 2))

/* 物体の色 */
static GLfloat red[] = { 0.8f, 0.2f, 0.2f, 1.0f };

/*
** トラックボール
*/
#include "quaternion.h"

/* ドラッグ開始位置 */
static int cx, cy;

/* マウスの絶対位置→ウィンドウ内での相対位置の換算係数 */
static double sx, sy;

/* マウスの相対位置→回転角の換算係数 */
#define SCALE (2.0 * 3.14159265358979323846)

/* 回転の初期値 (クォータニオン) */
static double cq[4] = { 1.0, 0.0, 0.0, 0.0 };

/* ドラッグ中の回転 (クォータニオン) */
static double tq[4];

/* 回転の変換行列 */
static double rt[16];

/*
** その他設定
*/

/* 光源の位置 */
static GLfloat pos[] = { 0.0, 0.0, 1.0, 0.0 };

/* 視点の位置 */
static GLdouble ex = 0.0, ey = 0.0, ez = 10.0;

/* 目標点の位置 */
static GLdouble tx = 0.0, ty = 0.0, tz = 0.0;

void display()
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  /* モデルビュー変換行列の初期化 */
  glLoadIdentity();

  /* 視点の移動 */
  gluLookAt(ex, ey, ez, tx, ty, tz, 0.0, 1.0, 0.0);

  /* 光源の位置を設定 */
  glLightfv(GL_LIGHT0, GL_POSITION, pos);

  /* 回転 */
  glMultMatrixd(rt);

  /* 描画 */
  glMaterialfv(GL_FRONT, GL_DIFFUSE, red);
  extrusion(crossSection, NC, spine, NS);

  glutSwapBuffers();
}

void resize(int w, int h)
{
  /* マウスポインタ位置のウィンドウ内の相対的位置への換算用 */
  sx = 1.0 / (double)w;
  sy = 1.0 / (double)h;

  /* ウィンドウ全体をビューポートにする */
  glViewport(0, 0, w, h);

  /* 透視変換行列の指定 */
  glMatrixMode(GL_PROJECTION);

  /* 透視変換行列の初期化 */
  glLoadIdentity();
  gluPerspective(30.0, (double)w / (double)h, 1.0, 100.0);

  /* モデルビュー変換行列の指定 */
  glMatrixMode(GL_MODELVIEW);
}

void idle()
{
  glutPostRedisplay();
}

void mouse(int button, int state, int x, int y)
{
  switch (button) {
  case GLUT_LEFT_BUTTON:
    switch (state) {
    case GLUT_DOWN:
      /* ドラッグ開始点を記録 */
      cx = x;
      cy = y;
      /* アニメーション開始 */
      glutIdleFunc(idle);
      break;
    case GLUT_UP:
      /* アニメーション終了 */
      glutIdleFunc(0);
      /* 回転の保存 */
      cq[0] = tq[0];
      cq[1] = tq[1];
      cq[2] = tq[2];
      cq[3] = tq[3];
      break;
    default:
      break;
    }
    break;
  default:
    break;
  }
}

void motion(int x, int y)
{
  double dx, dy, a;

  /* マウスポインタの位置のドラッグ開始位置からの変位 */
  dx = (x - cx) * sx;
  dy = (y - cy) * sy;

  /* マウスポインタの位置のドラッグ開始位置からの距離 */
  a = sqrt(dx * dx + dy * dy);

  if (a != 0.0) {
    double ar = a * SCALE * 0.5;
    double as = sin(ar) / a;
    double dq[4] = { cos(ar), dy * as, dx * as, 0.0 };
    
    /* クォータニオンを掛けて回転を合成 */
    qmul(tq, dq, cq);
    /* クォータニオンから回転の変換行列を求める */
    qrot(rt, tq);
  }
}

void keyboard(unsigned char key, int x, int y)
{
  /* q, Q あるいは ESC をタイプしたら終了 */
  switch (key) {
  case 'q':
  case 'Q':
  case '\033':
    exit(0);
  }
}

void init()
{
  /* 初期設定 */
  glClearColor(1.0, 1.0, 1.0, 0.0);
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);
  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);

  /* 回転行列の初期化 */
  qrot(rt, cq);
}

int main(int argc, char *argv[])
{
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
  glutCreateWindow(argv[0]);
  glutDisplayFunc(display);
  glutReshapeFunc(resize);
  glutMouseFunc(mouse);
  glutMotionFunc(motion);
  glutKeyboardFunc(keyboard);
  init();
  glutMainLoop();
  return 0;
}
