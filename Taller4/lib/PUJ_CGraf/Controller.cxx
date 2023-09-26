#define GL_SILENCE_DEPRECATION
#include <PUJ_CGraf/Controller.h>

#include <PUJ_CGraf/Camera.h>
#include <PUJ_CGraf/World.h>

#include <cmath>

// -------------------------------------------------------------------------
#ifdef __APPLE__
#include <OpenGL/gl.h> // Use this when using mac
#include <GLUT/glut.h> // Use this when using mac
#else
#include <GL/gl.h>   //use this when in linux
#include <GL/glut.h> //use this when in linux
#endif
// -------------------------------------------------------------------------
// #include <GL/gl.h>
// #include <GL/glu.h>
// #include <GL/glut.h>

// -------------------------------------------------------------------------
PUJ_CGraf::World *PUJ_CGraf::Controller::s_World{nullptr};

int PUJ_CGraf::Controller::s_MainWindow{0};
int PUJ_CGraf::Controller::s_ShadeType{GL_FLAT};
bool PUJ_CGraf::Controller::s_Animating{false};
int PUJ_CGraf::Controller::s_MouseButton{-1};
int PUJ_CGraf::Controller::s_Articulation{-1};

int PUJ_CGraf::Controller::s_ViewPort[4]{0};
int PUJ_CGraf::Controller::s_StartMouse[2]{0};

PUJ_CGraf::Controller::TReal PUJ_CGraf::Controller::s_InvPrj[16]{0};
std::chrono::high_resolution_clock::time_point PUJ_CGraf::Controller::s_StartTime;

// -------------------------------------------------------------------------
void PUJ_CGraf::Controller::
    set_shade_type_to_flat()
{
  Self::s_ShadeType = GL_FLAT;
}

// -------------------------------------------------------------------------
void PUJ_CGraf::Controller::
    set_shade_type_to_smooth()
{
  Self::s_ShadeType = GL_SMOOTH;
}

// -------------------------------------------------------------------------
void PUJ_CGraf::Controller::
    init(
        int *argc, char **argv,
        const std::string &t,
        int w, int h)
{
  // Configure GLUT
  glutInit(argc, argv);
  glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);

  // Register window
  glutInitWindowSize(w, h);
  Self::s_MainWindow = glutCreateWindow(t.c_str());

  // Clear all rendering objects
  glClearColor(0, 0, 0, 0);
  PUJ_CGraf::Controller::reshape(w, h);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  // Register events
  glutReshapeFunc(PUJ_CGraf::Controller::reshape);
  glutMouseFunc(PUJ_CGraf::Controller::click);
  glutMotionFunc(PUJ_CGraf::Controller::active_move);
  glutPassiveMotionFunc(PUJ_CGraf::Controller::passive_move);
  glutKeyboardFunc(PUJ_CGraf::Controller::keyboard);
  glutDisplayFunc(PUJ_CGraf::Controller::draw);
  glutIdleFunc(PUJ_CGraf::Controller::idle);
}

// -------------------------------------------------------------------------
void PUJ_CGraf::Controller::
    set_world(PUJ_CGraf::World *s)
{
  Self::s_World = s;
  if (Self::s_World != nullptr)
    Self::s_World->init(Self::s_ShadeType);
}

// -------------------------------------------------------------------------
void PUJ_CGraf::Controller::
    reshape(int w, int h)
{
  if (Self::s_World != nullptr)
    Self::s_World->camera()->viewport(w, h);
}

// -------------------------------------------------------------------------
void PUJ_CGraf::Controller::
    click(int b, int s, int x, int y)
{
  if (s == GLUT_DOWN)
  {
    Self::s_StartMouse[0] = x;
    Self::s_StartMouse[1] = y;
    Self::s_MouseButton = b;
  }
  else if (s == GLUT_UP)
  {
    Self::s_MouseButton = -1;
    if (Self::s_World != nullptr)
      Self::s_World->camera()->save_state();
  } // end if
}

// -------------------------------------------------------------------------
void PUJ_CGraf::Controller::
    active_move(int i, int j)
{
  if (Self::s_World == nullptr)
    return;
  if (Self::s_World->camera() == nullptr)
    return;

  if (Self::s_MouseButton == GLUT_LEFT_BUTTON)
  {
    Self::s_World->camera()->move_delta(
        i - Self::s_StartMouse[0],
        Self::s_StartMouse[1] - j);
    glutPostRedisplay();
  } // end if
}

// -------------------------------------------------------------------------
void PUJ_CGraf::Controller::
    passive_move(int i, int j)
{
}

// -------------------------------------------------------------------------
void PUJ_CGraf::Controller::
    keyboard(unsigned char k, int x, int y)
{
  if (k == '+')
  {
    Self::s_World->camera()->move_radius(-5);
    glutPostRedisplay();
  }
  else if (k == '-')
  {
    Self::s_World->camera()->move_radius(5);
    glutPostRedisplay();
  }
  else if (std::isdigit(k) || k == 'x' || k == 'X' || k == 'y' || k == 'Y' || k == 'z' || k == 'Z')
  {
    Self::s_World->keypressed(k);
    // Self::s_Articulation = k - '0'; //c++filt
  }
  else if (k == 27 /*ESC*/)
    if (Self::s_MainWindow != 0)
      glutDestroyWindow(Self::s_MainWindow);
}

// -------------------------------------------------------------------------
void PUJ_CGraf::Controller::
    draw()
{
  // Clear framebuffer and model transform
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  // Prepare next frame
  if (Self::s_World != nullptr)
    Self::s_World->draw();
  glutSwapBuffers();
}

// -------------------------------------------------------------------------
void PUJ_CGraf::Controller::
    timer(int v)
{
  /* TODO
     using _R = std::chrono::milliseconds;

     if( Self::s_Animating )
     {
     if( v == 1 )
     Self::s_StartTime = std::chrono::high_resolution_clock::now( );
     unsigned long long d =
     std::chrono::duration_cast< _R >(
     std::chrono::high_resolution_clock::now( ) - Self::s_StartTime
     ).count( );
     Self::s_World->advance_simulation( d );
     glutPostRedisplay( );
     Self::s_StartTime = std::chrono::high_resolution_clock::now( );
     glutTimerFunc( 20, PUJ_CGraf::Controller::timer, 0 );
     }
     else
     {
     Self::s_World->advance_simulation( 0 );
     glutPostRedisplay( );
     } // end if
  */
}

// -------------------------------------------------------------------------
void PUJ_CGraf::Controller::
    idle()
{
  glutPostRedisplay();
}

// -------------------------------------------------------------------------
void PUJ_CGraf::Controller::
    loop()
{
  glutMainLoop();
}

// -------------------------------------------------------------------------
void PUJ_CGraf::Controller::
    _inv_prj()
{
  TReal inv[16]{0}, det;
  int i;

  inv[0] = Self::s_InvPrj[5] * Self::s_InvPrj[10] * Self::s_InvPrj[15] - Self::s_InvPrj[5] * Self::s_InvPrj[14] * Self::s_InvPrj[11] - Self::s_InvPrj[6] * Self::s_InvPrj[9] * Self::s_InvPrj[15] + Self::s_InvPrj[6] * Self::s_InvPrj[13] * Self::s_InvPrj[11] + Self::s_InvPrj[7] * Self::s_InvPrj[9] * Self::s_InvPrj[14] - Self::s_InvPrj[7] * Self::s_InvPrj[13] * Self::s_InvPrj[10];
  inv[1] = -Self::s_InvPrj[1] * Self::s_InvPrj[10] * Self::s_InvPrj[15] + Self::s_InvPrj[1] * Self::s_InvPrj[14] * Self::s_InvPrj[11] + Self::s_InvPrj[2] * Self::s_InvPrj[9] * Self::s_InvPrj[15] - Self::s_InvPrj[2] * Self::s_InvPrj[13] * Self::s_InvPrj[11] - Self::s_InvPrj[3] * Self::s_InvPrj[9] * Self::s_InvPrj[14] + Self::s_InvPrj[3] * Self::s_InvPrj[13] * Self::s_InvPrj[10];
  inv[2] = Self::s_InvPrj[1] * Self::s_InvPrj[6] * Self::s_InvPrj[15] - Self::s_InvPrj[1] * Self::s_InvPrj[14] * Self::s_InvPrj[7] - Self::s_InvPrj[2] * Self::s_InvPrj[5] * Self::s_InvPrj[15] + Self::s_InvPrj[2] * Self::s_InvPrj[13] * Self::s_InvPrj[7] + Self::s_InvPrj[3] * Self::s_InvPrj[5] * Self::s_InvPrj[14] - Self::s_InvPrj[3] * Self::s_InvPrj[13] * Self::s_InvPrj[6];
  inv[3] = -Self::s_InvPrj[1] * Self::s_InvPrj[6] * Self::s_InvPrj[11] + Self::s_InvPrj[1] * Self::s_InvPrj[10] * Self::s_InvPrj[7] + Self::s_InvPrj[2] * Self::s_InvPrj[5] * Self::s_InvPrj[11] - Self::s_InvPrj[2] * Self::s_InvPrj[9] * Self::s_InvPrj[7] - Self::s_InvPrj[3] * Self::s_InvPrj[5] * Self::s_InvPrj[10] + Self::s_InvPrj[3] * Self::s_InvPrj[9] * Self::s_InvPrj[6];
  inv[4] = -Self::s_InvPrj[4] * Self::s_InvPrj[10] * Self::s_InvPrj[15] + Self::s_InvPrj[4] * Self::s_InvPrj[14] * Self::s_InvPrj[11] + Self::s_InvPrj[6] * Self::s_InvPrj[8] * Self::s_InvPrj[15] - Self::s_InvPrj[6] * Self::s_InvPrj[12] * Self::s_InvPrj[11] - Self::s_InvPrj[7] * Self::s_InvPrj[8] * Self::s_InvPrj[14] + Self::s_InvPrj[7] * Self::s_InvPrj[12] * Self::s_InvPrj[10];
  inv[5] = Self::s_InvPrj[0] * Self::s_InvPrj[10] * Self::s_InvPrj[15] - Self::s_InvPrj[0] * Self::s_InvPrj[14] * Self::s_InvPrj[11] - Self::s_InvPrj[2] * Self::s_InvPrj[8] * Self::s_InvPrj[15] + Self::s_InvPrj[2] * Self::s_InvPrj[12] * Self::s_InvPrj[11] + Self::s_InvPrj[3] * Self::s_InvPrj[8] * Self::s_InvPrj[14] - Self::s_InvPrj[3] * Self::s_InvPrj[12] * Self::s_InvPrj[10];
  inv[6] = -Self::s_InvPrj[0] * Self::s_InvPrj[6] * Self::s_InvPrj[15] + Self::s_InvPrj[0] * Self::s_InvPrj[14] * Self::s_InvPrj[7] + Self::s_InvPrj[2] * Self::s_InvPrj[4] * Self::s_InvPrj[15] - Self::s_InvPrj[2] * Self::s_InvPrj[12] * Self::s_InvPrj[7] - Self::s_InvPrj[3] * Self::s_InvPrj[4] * Self::s_InvPrj[14] + Self::s_InvPrj[3] * Self::s_InvPrj[12] * Self::s_InvPrj[6];
  inv[7] = Self::s_InvPrj[0] * Self::s_InvPrj[6] * Self::s_InvPrj[11] - Self::s_InvPrj[0] * Self::s_InvPrj[10] * Self::s_InvPrj[7] - Self::s_InvPrj[2] * Self::s_InvPrj[4] * Self::s_InvPrj[11] + Self::s_InvPrj[2] * Self::s_InvPrj[8] * Self::s_InvPrj[7] + Self::s_InvPrj[3] * Self::s_InvPrj[4] * Self::s_InvPrj[10] - Self::s_InvPrj[3] * Self::s_InvPrj[8] * Self::s_InvPrj[6];
  inv[8] = Self::s_InvPrj[4] * Self::s_InvPrj[9] * Self::s_InvPrj[15] - Self::s_InvPrj[4] * Self::s_InvPrj[13] * Self::s_InvPrj[11] - Self::s_InvPrj[5] * Self::s_InvPrj[8] * Self::s_InvPrj[15] + Self::s_InvPrj[5] * Self::s_InvPrj[12] * Self::s_InvPrj[11] + Self::s_InvPrj[7] * Self::s_InvPrj[8] * Self::s_InvPrj[13] - Self::s_InvPrj[7] * Self::s_InvPrj[12] * Self::s_InvPrj[9];
  inv[9] = -Self::s_InvPrj[0] * Self::s_InvPrj[9] * Self::s_InvPrj[15] + Self::s_InvPrj[0] * Self::s_InvPrj[13] * Self::s_InvPrj[11] + Self::s_InvPrj[1] * Self::s_InvPrj[8] * Self::s_InvPrj[15] - Self::s_InvPrj[1] * Self::s_InvPrj[12] * Self::s_InvPrj[11] - Self::s_InvPrj[3] * Self::s_InvPrj[8] * Self::s_InvPrj[13] + Self::s_InvPrj[3] * Self::s_InvPrj[12] * Self::s_InvPrj[9];
  inv[10] = Self::s_InvPrj[0] * Self::s_InvPrj[5] * Self::s_InvPrj[15] - Self::s_InvPrj[0] * Self::s_InvPrj[13] * Self::s_InvPrj[7] - Self::s_InvPrj[1] * Self::s_InvPrj[4] * Self::s_InvPrj[15] + Self::s_InvPrj[1] * Self::s_InvPrj[12] * Self::s_InvPrj[7] + Self::s_InvPrj[3] * Self::s_InvPrj[4] * Self::s_InvPrj[13] - Self::s_InvPrj[3] * Self::s_InvPrj[12] * Self::s_InvPrj[5];
  inv[11] = -Self::s_InvPrj[0] * Self::s_InvPrj[5] * Self::s_InvPrj[11] + Self::s_InvPrj[0] * Self::s_InvPrj[9] * Self::s_InvPrj[7] + Self::s_InvPrj[1] * Self::s_InvPrj[4] * Self::s_InvPrj[11] - Self::s_InvPrj[1] * Self::s_InvPrj[8] * Self::s_InvPrj[7] - Self::s_InvPrj[3] * Self::s_InvPrj[4] * Self::s_InvPrj[9] + Self::s_InvPrj[3] * Self::s_InvPrj[8] * Self::s_InvPrj[5];
  inv[12] = -Self::s_InvPrj[4] * Self::s_InvPrj[9] * Self::s_InvPrj[14] + Self::s_InvPrj[4] * Self::s_InvPrj[13] * Self::s_InvPrj[10] + Self::s_InvPrj[5] * Self::s_InvPrj[8] * Self::s_InvPrj[14] - Self::s_InvPrj[5] * Self::s_InvPrj[12] * Self::s_InvPrj[10] - Self::s_InvPrj[6] * Self::s_InvPrj[8] * Self::s_InvPrj[13] + Self::s_InvPrj[6] * Self::s_InvPrj[12] * Self::s_InvPrj[9];
  inv[13] = Self::s_InvPrj[0] * Self::s_InvPrj[9] * Self::s_InvPrj[14] - Self::s_InvPrj[0] * Self::s_InvPrj[13] * Self::s_InvPrj[10] - Self::s_InvPrj[1] * Self::s_InvPrj[8] * Self::s_InvPrj[14] + Self::s_InvPrj[1] * Self::s_InvPrj[12] * Self::s_InvPrj[10] + Self::s_InvPrj[2] * Self::s_InvPrj[8] * Self::s_InvPrj[13] - Self::s_InvPrj[2] * Self::s_InvPrj[12] * Self::s_InvPrj[9];
  inv[14] = -Self::s_InvPrj[0] * Self::s_InvPrj[5] * Self::s_InvPrj[14] + Self::s_InvPrj[0] * Self::s_InvPrj[13] * Self::s_InvPrj[6] + Self::s_InvPrj[1] * Self::s_InvPrj[4] * Self::s_InvPrj[14] - Self::s_InvPrj[1] * Self::s_InvPrj[12] * Self::s_InvPrj[6] - Self::s_InvPrj[2] * Self::s_InvPrj[4] * Self::s_InvPrj[13] + Self::s_InvPrj[2] * Self::s_InvPrj[12] * Self::s_InvPrj[5];
  inv[15] = Self::s_InvPrj[0] * Self::s_InvPrj[5] * Self::s_InvPrj[10] - Self::s_InvPrj[0] * Self::s_InvPrj[9] * Self::s_InvPrj[6] - Self::s_InvPrj[1] * Self::s_InvPrj[4] * Self::s_InvPrj[10] + Self::s_InvPrj[1] * Self::s_InvPrj[8] * Self::s_InvPrj[6] + Self::s_InvPrj[2] * Self::s_InvPrj[4] * Self::s_InvPrj[9] - Self::s_InvPrj[2] * Self::s_InvPrj[8] * Self::s_InvPrj[5];

  det = Self::s_InvPrj[0] * inv[0] + Self::s_InvPrj[4] * inv[1] + Self::s_InvPrj[8] * inv[2] + Self::s_InvPrj[12] * inv[3];
  if (det != TReal(0))
  {
    det = TReal(1.0) / det;
    for (i = 0; i < 16; i++)
      Self::s_InvPrj[i] = inv[i] * det;
  } // end if
}

// eof - Controller.cxx
