#define GL_SILENCE_DEPRECATION
#include <cmath>
#include <iostream>

#include <OpenGL/gl.h>
#include <GLUT/glut.h>

// -------------------------------------------------------------------------

int main_window = 0;
int window_width = 0;
int window_height = 0;

double world_left = -1.5;
double world_right = 1.5;
double world_top = 1.5;
double world_bottom = -1.5;

// -------------------------------------------------------------------------
// Our particle will have certain position (x,y) and velocity (vx,vy)
struct Particle
{
    double x, y;
    double vx, vy;
};

Particle p;
double g_x = 0, g_y = 9;
bool isLaunched = false;

void InitializeParticle(double x, double y, double vx, double vy)
{
    p.x = x;
    p.y = y;
    p.vx = vx;
    p.vy = vy;
    isLaunched = false;
}
// p.x = 0;
// p.y = 0;
// p.vx = 1;
// p.vy = 1;

void UpdateParticle(double dt)
{
    if (isLaunched)
    {
        // Predecir la próxima posición
        double nextX = p.x + p.vx * dt;
        double nextY = p.y + p.vy * dt;
        // Revisar colisiones con las paredes
        if (nextX < world_left || nextX > world_right)
        {
            p.vx *= -1; // Invertir la velocidad en x
        }
        if (nextY < world_bottom || nextY > world_top)
        {
            p.vy *= -1; // Invertir la velocidad en y
        }
        // Avanzamos medio paso en velocidad
        p.vx += 0.5 * g_x * dt;
        p.vy += 0.5 * g_y * dt;
        // Avanzamos un paso en posicion
        p.x += p.vx * dt;
        p.y += p.vy * dt;
        // Avanzamos el medio paso restante en velocidad
        p.vx += 0.5 * g_x * dt;
        p.vy += 0.5 * g_y * dt;
    }
}

// -------------------------------------------------------------------------

void circle(double radius = 1, int mode = GL_LINE_LOOP, unsigned int samples = 30)
{
    double off = 8.0 * std::atan(1) / double(samples);
    glBegin(mode);
    for (unsigned int i = 0; i < samples; ++i)
    {
        double w = double(i) * off;
        glVertex2f(radius * std::cos(w), radius * std::sin(w));
    } // end for
    glEnd();
}

// -------------------------------------------------------------------------

int init(const std::string &title, int width, int height)
{
    window_width = width;
    window_height = height;
    glutInitWindowSize(window_width, window_height);
    int wnd = glutCreateWindow(title.c_str());
    // Set clear color
    glClearColor(0, 0, 0, 0);
    // Prepare viewport
    glViewport(0, 0, window_width, window_height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    // Keep the aspect of the viz
    double aspect = (double)window_width / (double)window_height;
    glOrtho(aspect * world_left, aspect * world_right, world_bottom, world_top, -1, 1);
    //
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    return (wnd);
}

// -------------------------------------------------------------------------

void reshape(int width, int height)
{
    window_width = width;
    window_height = height;
    // Set the viewport to cover the new window
    glViewport(0, 0, window_width, window_height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    // Keep the aspect ratio of the viz
    double aspect = (double)window_width / (double)window_height;
    glOrtho(aspect * world_left, aspect * world_right, world_bottom, world_top, -1, 1);
}

// -------------------------------------------------------------------------

void draw()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(p.x, p.y, 0);
    circle(0.05,GL_POLYGON);
    glutSwapBuffers();
}

// -------------------------------------------------------------------------

void idle()
{
    // std::cout << "idle" << std::endl;
    UpdateParticle(0.03); //
    glutPostRedisplay();
}

// -------------------------------------------------------------------------

void click(int button, int state, int x, int y)
{
    bool click=false;
    std::cout << "click: " << button << " " << state << " " << x << " " << y << std::endl;

    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
    {
        //tranformation to the coordinates of the world
        double worldX = ((double)x / window_width) * (world_right - world_left) + world_left;
        double worldY = ((double)(window_height - y) / window_height) * (world_top - world_bottom) + world_bottom;
        p.x= worldX;
        p.y= worldY;
        draw();
        isLaunched = true;
    }
}

// -------------------------------------------------------------------------

void active_move(int x, int y)
{
    if (isLaunched)
    {
        // Transforma las coordenadas de la ventana a las del mundo
        double worldX = ((double)x / window_width) * (world_right - world_left) + world_left;
        double worldY = ((double)(window_height - y) / window_height) * (world_top - world_bottom) + world_bottom;
        // Configura la velocidad inicial en función de la distancia al clic del ratón
        p.vx = (worldX - p.x) * 0.1;
        p.vy = (worldY - p.y) * 0.1;
    }
}

// -------------------------------------------------------------------------

void passive_move(int x, int y)
{
    // this particular setup is not interesting here
    // std::cout << "passive: " << x << " " << y << std::endl;
}

// -------------------------------------------------------------------------

void keyboard(unsigned char key, int x, int y)
{
    if (key == 27 /*ESC*/)
        if (main_window != 0)
            glutDestroyWindow(main_window);
}

// -------------------------------------------------------------------------

int main(int argc, char **argv)
{
    if (argc != 5)
    {
        std::cout << "Use: ./program.out W H g_x g_y\n";
        return 1;
    }
    double W = atof(argv[1]);
    double H = atof(argv[2]);
    g_x = atof(argv[3]);
    g_y = atof(argv[4]);

    world_left = -W / 2;
    world_right = W / 2;
    world_bottom = -H / 2;
    world_top = H / 2;

    InitializeParticle(0, 0, 0, 0);

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
    main_window = init("Simulador de Angry birds", 700, 700 );
    // View functions
    glutReshapeFunc(reshape);
    glutDisplayFunc(draw);
    glutIdleFunc(idle);
    glutMouseFunc(click);
    glutKeyboardFunc(keyboard);
    glutMotionFunc(active_move);
    glutPassiveMotionFunc(passive_move);
    glutMainLoop();

    return (EXIT_SUCCESS);
}