#ifndef __PUJ_CGraf__Controller__h__
#define __PUJ_CGraf__Controller__h__

#include <chrono>
#include <string>
#include <vector>

// -- FORWARD DECLARATIONS --
namespace PUJ_CGraf
{
  class World;
}

namespace PUJ_CGraf
{
  /**
   */
  class Controller
  {
  public:
    using Self = Controller;
    using TReal = float;
    using TNatural = unsigned short;

  public:
    static void set_shade_type_to_flat();
    static void set_shade_type_to_smooth();

    static void init(
        int *argc, char **argv, const std::string &t, int w, int h);
    static void set_world(World *s);
    static void reshape(int w, int h);
    static void click(int b, int s, int x, int y);
    static void active_move(int i, int j);
    static void passive_move(int i, int j);
    static void keyboard(unsigned char k, int x, int y);
    static void draw();
    static void idle();
    static void timer(int v);
    static void loop();

  protected:
    static void _inv_prj();

  protected:
    static World *s_World;

    static int s_ShadeType;
    static int s_MainWindow;
    static int s_ViewPort[4];
    static TReal s_InvPrj[16];

    static std::chrono::high_resolution_clock::time_point s_StartTime;
    static bool s_Animating;

    static int s_StartMouse[2];
    static int s_MouseButton;
    static int s_Articulation;
  };
} // end namespace

#endif // __PUJ_CGraf__Controller__h__

// eof - Controller.h
