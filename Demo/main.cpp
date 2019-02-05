#include <iostream>
#include <sstream>
#include <cmath>

#include "GL/glew.h"
#include "GL/freeglut.h"

#include "CGJengine.h"
#include "scene.h"
#include "actions.h"
#include "constants.h"
#include "pipeline.h"

#define CAPTION "CGJDemo"

int WinX = WIN_X;
int WinY = WIN_Y;
int WindowHandle = 0;
unsigned int FrameCount = 0;
bool inVR = false;

/////////////////////////////////////////////////////////////////////// CALLBACKS
void cleanup(){
    ResourceManager::getInstance()->destroyEverything();
    ResourceManager::getInstance()->deleteInstance();
}


void display(){
	++FrameCount;

	if(inVR){
		static VRCamera* cam = (VRCamera*)ResourceManager::getInstance()->getCamera(SPHERE_CAM);
		static ColorTextureFrameBuffer* leftFBO =
				(ColorTextureFrameBuffer*)ResourceManager::getInstance()->getFrameBuffer(LEFT_FBO_RENDER);
		static ColorTextureFrameBuffer* rightFBO =
				(ColorTextureFrameBuffer*)ResourceManager::getInstance()->getFrameBuffer(RIGHT_FBO_RENDER);

		cam->updatePose();
		cam->setCurrentEye(EYE_LEFT);
        executePipeline(leftFBO);
		cam->setCurrentEye(EYE_RIGHT);
		executePipeline(rightFBO);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		cam->submit(leftFBO, rightFBO);
        //executePipeline(nullptr);

	}else {
        executePipeline(nullptr);
    }
	checkOpenGLError("ERROR: Could not draw scene.");
	glutSwapBuffers();

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glClearColor( 0, 0, 0, 1 );
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    glFlush();
    glFinish();
}

void update(){
    static int lastTime = glutGet(GLUT_ELAPSED_TIME);
	static SceneGraph* scene = ResourceManager::getInstance()->getScene(SCENE);
	static ParticlePool* particlePool = ResourceManager::getInstance()->getParticlePool(POOL);

    int currentTime = glutGet(GLUT_ELAPSED_TIME);
    int timeDelta = currentTime-lastTime;
    lastTime = currentTime;
	scene->update(timeDelta);
	particlePool->update(timeDelta);


}

void idle(){
    update();
	glutPostRedisplay();
}

void resizeFBOs(int w, int h){
    ResourceManager::getInstance()->getCamera(SPHERE_CAM)->resize(w, h);
    ResourceManager::getInstance()->getCamera(BOTTOM_RIGHT_CAM)->resize(w, h);

    ResourceManager::getInstance()->getFrameBuffer(MAIN_FBO)->resize(w, h);
    ResourceManager::getInstance()->getFrameBuffer(SIDE_FBO1)->resize(w, h);
    ResourceManager::getInstance()->getFrameBuffer(SIDE_FBO2)->resize(w, h);
    ResourceManager::getInstance()->getFrameBuffer(SIDE_FBO3)->resize(w, h);
    ResourceManager::getInstance()->getFrameBuffer(SHADOW_FBO)->resize(w, h);
}
void reshape(int w, int h){
    WinX = w;
    WinY = h;
    if(!inVR) {
        resizeFBOs(w, h);
	}
	glViewport(0, 0, w, h);
}


void timer(int value){
	std::ostringstream oss;
	oss << CAPTION << ": " << FrameCount << " FPS";
	std::string s = oss.str();
	glutSetWindow(WindowHandle);
	glutSetWindowTitle(s.c_str());
    FrameCount = 0;
    glutTimerFunc(1000, timer, 0);
}

/////////////////////////////////////////////////////////////////////// INPUT
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

void m(int v){
    InputManager::update();
    glutTimerFunc(10, m, 0);
}
/////////////////////////////////////////////////////////////////////// SETUP

void setupCallbacks(){
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
	glutTimerFunc(5000, flames, 0);
	glutTimerFunc(0, m, 0);
}

void checkOpenGLInfo(){
	const GLubyte *renderer = glGetString(GL_RENDERER);
	const GLubyte *vendor = glGetString(GL_VENDOR);
	const GLubyte *version = glGetString(GL_VERSION);
	const GLubyte *glslVersion = glGetString(GL_SHADING_LANGUAGE_VERSION);
	std::cerr << "OpenGL Renderer: " << renderer << " (" << vendor << ")" << std::endl;
	std::cerr << "OpenGL version " << version << std::endl;
	std::cerr << "GLSL version " << glslVersion << std::endl;
    if(GLEW_EXT_texture_filter_anisotropic) {
		GLfloat largestAnisotropic;
		glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &largestAnisotropic);
		std::cerr << "Using anisotropic x" << largestAnisotropic << std::endl;
	}else{
		std::cerr << "Using linear filtering" << std::endl;
	}
}

void setupOpenGL(){
	checkOpenGLInfo();
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glDepthMask(GL_TRUE);
	glDepthRange(0.0, 1.0);
	glClearDepth(1.0);

    /*Set transparency on diffuse render target*/
    glEnablei(GL_BLEND, 0);
    glBlendFunci(0, GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	/*Set transparency on particles render target*/
    glEnablei(GL_BLEND, 4);
    glBlendFuncSeparatei(4, GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE);

    glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);
}

void setupGLEW() {
	glewExperimental = GL_TRUE;
	GLenum result = glewInit() ;
	if (result != GLEW_OK) {
		std::cerr << "ERROR glewInit: " << glewGetString(result) << std::endl;
		exit(EXIT_FAILURE);
	}
	//GLenum err_code = glGetError();
}

void setupGLUT(int argc, char* argv[]){
	glutInit(&argc, argv);

	glutInitContextVersion(3, 3);
	glutInitContextFlags(GLUT_FORWARD_COMPATIBLE);
	glutInitContextProfile(GLUT_CORE_PROFILE);


	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE,GLUT_ACTION_GLUTMAINLOOP_RETURNS);

	glutInitWindowSize(WIN_X, WIN_Y);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	WindowHandle = glutCreateWindow(CAPTION);
	if(WindowHandle < 1) {
		std::cerr << "ERROR: Could not create a new rendering window." << std::endl;
		exit(EXIT_FAILURE);
	}
    glutSetCursor(GLUT_CURSOR_NONE);
	glutWarpPointer(WIN_X/2, WIN_Y/2);
}

void setupVR(){
	VRCamera* cam = (VRCamera*)ResourceManager::getInstance()->getCamera(SPHERE_CAM);

	ResourceManager::Factory::createColorTextureFrameBuffer(LEFT_FBO_RENDER,
			cam->getRecommendedWidth(), cam->getRecommendedHeight());

	ResourceManager::Factory::createColorTextureFrameBuffer(RIGHT_FBO_RENDER,
			cam->getRecommendedWidth(), cam->getRecommendedHeight());

	resizeFBOs(cam->getRecommendedWidth(), cam->getRecommendedHeight());

}

void init(int argc, char* argv[]){
    if(argc == 2 && argv[1][0] == '-' && argv[1][1] == '-' && argv[1][2] == 'v' && argv[1][3] == 'r') {
        inVR = true;
    }

	setupGLUT(argc, argv);
	setupGLEW();
	setupOpenGL();

    setupScene();
	setupPipeline();
    setupActions();

	setupCallbacks();

	if(inVR){
		setupVR();
	}
}

int main(int argc, char* argv[]){
	init(argc, argv);
	glutMainLoop();
	exit(EXIT_SUCCESS);
}

///////////////////////////////////////////////////////////////////////
