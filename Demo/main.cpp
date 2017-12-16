#include <iostream>
#include <sstream>
#include <cmath>

#include "GL/glew.h"
#include "GL/freeglut.h"

#include "CGJengine.h"
#include "scene.h"
#include "actions.h"
#include "Constants.h"

#define CAPTION "CGJDemo"


int WinX = WIN_X;
int WinY = WIN_Y;
int WindowHandle = 0;
unsigned int FrameCount = 0;

/////////////////////////////////////////////////////////////////////// CALLBACKS
void cleanup()
{
    ResourceManager::getInstance()->destroyEverything();
    ResourceManager::getInstance()->deleteInstance();
}


void display()
{
	static SceneGraph* scene = ResourceManager::getInstance()->getScene(SCENE);
	static SceneGraph* creditsHUD = ResourceManager::getInstance()->getScene(CREDITS_HUD);
    static SceneGraph* ssao = ResourceManager::getInstance()->getScene(SSAO);
	static SceneGraph* ssaoApply = ResourceManager::getInstance()->getScene(SSAO_APPLY);

    static DoubleColorMSFrameBuffer* mainFBO = (DoubleColorMSFrameBuffer*)ResourceManager::getInstance()->getFrameBuffer(MAIN_FBO);
    static ColorTextureFrameBuffer* SceneColorFBO = (ColorTextureFrameBuffer*)ResourceManager::getInstance()->getFrameBuffer(SCENE_COLOR_FBO);
    static DepthTextureFrameBuffer* depthFBO = (DepthTextureFrameBuffer*)ResourceManager::getInstance()->getFrameBuffer(DEPTH_FBO);
    static ColorTextureFrameBuffer* normalZFBO = (ColorTextureFrameBuffer*)ResourceManager::getInstance()->getFrameBuffer(NORMALZ_FBO);
	static ColorTextureFrameBuffer* ssaoFBO = (ColorTextureFrameBuffer*)ResourceManager::getInstance()->getFrameBuffer(SSAO_FBO);
    static ColorTextureFrameBuffer* finalFBO = (ColorTextureFrameBuffer*)ResourceManager::getInstance()->getFrameBuffer(FINAL_FBO);
	static FrameBuffer* reflectionFBO = ResourceManager::getInstance()->getFrameBuffer(REFLECTION_FBO);

    static ParticlePool* particlePool = ResourceManager::getInstance()->getParticlePool(POOL);


	++FrameCount;
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    LightsManager::getInstance()->uploadLights();

	/*Draw Reflection*/
	reflectionFBO->bind();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	scene->getCamera()->setReflection(true, 0.0f, 1.0f, 0.0f, scene->findNode("reflection")->getPositionWorldspace());//, GROUND_LEVEL);
    glFrontFace(GL_CW);
	scene->draw();
    glFrontFace(GL_CCW);
	reflectionFBO->unbind();
    scene->getCamera()->setReflection(false, 0.0f, 1.0f, 0.0f, Vec3(0.0f, 0.0f, 0.0f));

    /*Draw scene to fbo*/
	mainFBO->bind();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	scene->draw();
    mainFBO->blitDepth(depthFBO); /*Blit depth before drawing particles*/
    mainFBO->blitSecondColor(normalZFBO);
    mainFBO->bind();
	scene->draw(REFLECTIONS_LEVEL);
    particlePool->draw(DEFAULT_PARTICLES_LEVEL);


	/*Copy fbo to texture and use the copy for post-processing*/
	mainFBO->blit(SceneColorFBO);
    mainFBO->bind();
    scene->draw(HEAT_HAZE_LEVEL);
	particlePool->draw(HEAT_HAZE_LEVEL);
    mainFBO->blit(SceneColorFBO);

    ssaoFBO->bind();
    ssao->draw();
    ssaoFBO->unbind();
    ssaoApply->draw();
    creditsHUD->draw();

	checkOpenGLError("ERROR: Could not draw scene.");
	glutSwapBuffers();
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

void idle()
{
    update();
	glutPostRedisplay();
}

void reshape(int w, int h)
{
	WinX = w;
	WinY = h;
	ResourceManager::getInstance()->getCamera(FREE_CAM)->resize(w, h);
	ResourceManager::getInstance()->getCamera(BOTTOM_RIGHT_CAM)->resize(w, h);

    ResourceManager::getInstance()->getFrameBuffer(MAIN_FBO)->resize(w, h);
    ResourceManager::getInstance()->getFrameBuffer(SCENE_COLOR_FBO)->resize(w, h);
    ResourceManager::getInstance()->getFrameBuffer(NORMALZ_FBO)->resize(w, h);
    ResourceManager::getInstance()->getFrameBuffer(DEPTH_FBO)->resize(w, h);
    ResourceManager::getInstance()->getFrameBuffer(SSAO_FBO)->resize(w, h);
    ResourceManager::getInstance()->getFrameBuffer(FINAL_FBO)->resize(w, h);

	glViewport(0, 0, w, h);
}

void timer(int value)
{
	std::ostringstream oss;
	oss << CAPTION << ": " << FrameCount << " FPS";
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


void flames(int value){
	((ParticleEmitterNode*)ResourceManager::getInstance()->getScene(SCENE)->findNode(RIGHT_EXHAUST))->emmit();
	((ParticleEmitterNode*)ResourceManager::getInstance()->getScene(SCENE)->findNode(LEFT_EXHAUST))->emmit();
	unsigned int nextInterval = (unsigned int)(((float)std::rand()/(float)RAND_MAX)*5000);
	glutTimerFunc(nextInterval, flames, 0);
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
	glutTimerFunc(5000, flames, 0);
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
    if(GLEW_EXT_texture_filter_anisotropic) {
		GLfloat largestAnisotropic;
		glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &largestAnisotropic);
		std::cerr << "Using anisotropic x" << largestAnisotropic << std::endl;
	}else{
		std::cerr << "Using linear filtering" << std::endl;
	}
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
	glEnable(GL_MULTISAMPLE);
	glEnable(GL_STENCIL_TEST);
	glStencilMask(0xFF);
	glClearStencil(0);
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

void init(int argc, char* argv[])
{
	setupGLUT(argc, argv);
	setupGLEW();
	setupOpenGL();

    setupScene();
    setupActions();

	setupCallbacks();
}

int main(int argc, char* argv[])
{
	init(argc, argv);
	glutMainLoop();
	exit(EXIT_SUCCESS);
}

///////////////////////////////////////////////////////////////////////
