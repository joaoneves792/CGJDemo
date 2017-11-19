#include <iostream>
#include <sstream>
#include <cmath>

#include "GL/glew.h"
#include "GL/freeglut.h"

#include "CGJengine.h"
#include "scene.h"

#define CAPTION "CGJDemo"
#define ESCAPE 27

int WinX = 1024, WinY = 1024;
int WindowHandle = 0;
unsigned int FrameCount = 0;

const std::string SCENE_NAME = "main";

/////////////////////////////////////////////////////////////////////// CALLBACKS
void cleanup()
{
    ResourceManager::getInstance()->destroyEverything();
    ResourceManager::getInstance()->deleteInstance();
}

void display()
{
	++FrameCount;
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    FrameBuffer* fb = new FrameBuffer(WinX, WinY);
	fb->bind();
    SceneGraph* scene = ResourceManager::getInstance()->getScene(SCENE_NAME);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	scene->draw();
	fb->unbind();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    scene->draw();
	fb->bindTexture();
	scene = ResourceManager::getInstance()->getScene("hud");
	scene->draw();

	delete fb;
	checkOpenGLError("ERROR: Could not draw scene.");
	glutSwapBuffers();
}

void update(){
    static int lastTime = glutGet(GLUT_ELAPSED_TIME);

    int currentTime = glutGet(GLUT_ELAPSED_TIME);
    int timeDelta = currentTime-lastTime;
    lastTime = currentTime;

	ResourceManager::getInstance()->getScene(SCENE_NAME)->update(timeDelta);
}

void idle()
{
    update();
	glutPostRedisplay();
}

void reshape(int w, int h)
{
	WinX = w;
	WinY = h;
	glViewport(0, 0, WinX, WinY);
    SceneGraph* scene = ResourceManager::getInstance()->getScene(SCENE_NAME);
    scene->getCamera()->perspective((float)M_PI/4, (WinX/WinY), 0.1, 100);
	//scene->getCamera()->ortho(-2, 2, -2, 2, 0, 10);
}

void timer(int value)
{
	std::ostringstream oss;
	oss << CAPTION << ": " << FrameCount << " FPS @ (" << WinX << "x" << WinY << ")";
	std::string s = oss.str();
	glutSetWindow(WindowHandle);
	glutSetWindowTitle(s.c_str());
    FrameCount = 0;
    glutTimerFunc(1000, timer, 0);
}

void keyboard(unsigned char key, int x, int y){
	InputManager::getInstance()->keyDown(key);
}

void keyboardUp(unsigned char key, int x, int y){
    InputManager::getInstance()->keyUp(key);
}

void specialKeyboard(int key, int x, int y){
    InputManager::getInstance()->specialKeyDown(key);
}

void specialKeyboardUp(int key, int x, int y){
    InputManager::getInstance()->specialKeyUp(key);
}

void mouse(int x, int y) {
    InputManager::getInstance()->mouseMovement(x, y);
}

void setupActions(const std::string& sceneName) {
    SceneGraph *scene = ResourceManager::getInstance()->getScene(sceneName);
    InputManager *im = InputManager::getInstance();

    im->setActionInterval(10); //Update every 10ms

    /*Update camera movement*/
    const float movementRate = 0.005; //magic number
    const float cameraRate = (float)-1.0f*(M_PI / (WinX*2));

    im->setMouseAction([=](int x, int y, int timeDelta){
        int deltaX = (WinX/2) - x;
        int deltaY = (WinY/2) - y;
        glutWarpPointer(WinX/2, WinY/2);
        scene->getCamera()->changeOrientation(deltaX*cameraRate, deltaY*cameraRate, 0.0f);
    });

    im->addKeyAction('w', [=](int timeDelta){
        scene->getCamera()->move(0.0f, 0.0f, -(timeDelta * movementRate));
    });
    im->addKeyAction('a', [=](int timeDelta){
        scene->getCamera()->move(-timeDelta * movementRate, 0.0f, 0.0f);
    });
    im->addKeyAction('s', [=](int timeDelta){
        scene->getCamera()->move(0.0f, 0.0f, timeDelta * movementRate);
    });
    im->addKeyAction('d', [=](int timeDelta){
        scene->getCamera()->move(timeDelta * movementRate, 0.0f, 0.0f);
    });
    im->addKeyAction('q', [=](int timeDelta){
        scene->getCamera()->changeOrientation(0.0f, 0.0f, -(timeDelta * movementRate));
    });
    im->addKeyAction('e', [=](int timeDelta){
        scene->getCamera()->changeOrientation(0.0f, 0.0f, (timeDelta * movementRate));
    });
    im->addKeyAction(ESCAPE, [=](int timeDelta){
        glutLeaveMainLoop();
    });

}
/////////////////////////////////////////////////////////////////////// SETUP

void setupCallbacks() 
{
	glutCloseFunc(cleanup);
	glutDisplayFunc(display);
	glutIdleFunc(idle);
	glutReshapeFunc(reshape);
	glutTimerFunc(0,timer,0);
	glutPassiveMotionFunc(mouse);
	glutKeyboardFunc(keyboard);
    glutKeyboardUpFunc(keyboardUp);
    glutSpecialFunc(specialKeyboard);
    glutSpecialUpFunc(specialKeyboardUp);
}

void checkOpenGLInfo()
{
	const GLubyte *renderer = glGetString(GL_RENDERER);
	const GLubyte *vendor = glGetString(GL_VENDOR);
	const GLubyte *version = glGetString(GL_VERSION);
	const GLubyte *glslVersion = glGetString(GL_SHADING_LANGUAGE_VERSION);
	std::cerr << "OpenGL Renderer: " << renderer << " (" << vendor << ")" << std::endl;
	std::cerr << "OpenGL version " << version << std::endl;
	std::cerr << "GLSL version " << glslVersion << std::endl;
}

void setupOpenGL()
{
	checkOpenGLInfo();
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glDepthMask(GL_TRUE);
	glDepthRange(0.0, 1.0);
	glClearDepth(1.0);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);
}

void setupGLEW() 
{
	glewExperimental = GL_TRUE;
	GLenum result = glewInit() ; 
	if (result != GLEW_OK) { 
		std::cerr << "ERROR glewInit: " << glewGetString(result) << std::endl;
		exit(EXIT_FAILURE);
	} 
	//GLenum err_code = glGetError();
}

void setupGLUT(int argc, char* argv[])
{
	glutInit(&argc, argv);
	
	glutInitContextVersion(3, 3);
	glutInitContextFlags(GLUT_FORWARD_COMPATIBLE);
	glutInitContextProfile(GLUT_CORE_PROFILE);

	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE,GLUT_ACTION_GLUTMAINLOOP_RETURNS);
	
	glutInitWindowSize(WinX, WinY);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	WindowHandle = glutCreateWindow(CAPTION);
	if(WindowHandle < 1) {
		std::cerr << "ERROR: Could not create a new rendering window." << std::endl;
		exit(EXIT_FAILURE);
	}
    glutSetCursor(GLUT_CURSOR_NONE);
}

void init(int argc, char* argv[])
{
	setupGLUT(argc, argv);
	setupGLEW();
	setupOpenGL();

    setupScene(SCENE_NAME);
    setupActions(SCENE_NAME);

	setupCallbacks();
}

int main(int argc, char* argv[])
{
	init(argc, argv);
	glutMainLoop();
	exit(EXIT_SUCCESS);
}

///////////////////////////////////////////////////////////////////////
