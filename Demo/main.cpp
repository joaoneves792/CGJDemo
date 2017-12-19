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
	static SceneGraph* pipeline = ResourceManager::getInstance()->getScene(PIPELINE);
	static SceneGraph* creditsHUD = ResourceManager::getInstance()->getScene(CREDITS_HUD);

    static GFrameBuffer* mainFBO = (GFrameBuffer*)ResourceManager::getInstance()->getFrameBuffer(MAIN_FBO);
	static ColorTextureFrameBuffer* sideBuffer1 = (ColorTextureFrameBuffer*)ResourceManager::getInstance()->getFrameBuffer(SIDE_FBO1);
	static ColorTextureFrameBuffer* sideBuffer2 = (ColorTextureFrameBuffer*)ResourceManager::getInstance()->getFrameBuffer(SIDE_FBO2);
    static ColorTextureFrameBuffer* sideBuffer3 = (ColorTextureFrameBuffer*)ResourceManager::getInstance()->getFrameBuffer(SIDE_FBO3);

    static Shader* blit = ResourceManager::getInstance()->getShader(BLIT_SHADER);
    static GLint renderTargetLoc = blit->getUniformLocation("renderTarget");

    static ParticlePool* particlePool = ResourceManager::getInstance()->getParticlePool(POOL);


	++FrameCount;
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    LightsManager::getInstance()->uploadLights();

    /*Draw scene to G-Buffer*/
	mainFBO->bind();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	scene->draw();
	mainFBO->unbind();

    /*Perform SSAO stage*/
	sideBuffer1->bind();
	glActiveTexture(GL_TEXTURE2);
	mainFBO->bindNormals();
	glActiveTexture(GL_TEXTURE0);
	mainFBO->bindDepth();
	pipeline->draw(SSAO_LEVEL);
	sideBuffer1->unbind();
    sideBuffer2->bind();
	glActiveTexture(GL_TEXTURE0);
	sideBuffer1->bindTexture();
	pipeline->draw(SSAO_BLUR_LEVEL);
    sideBuffer2->unbind();

    /*Perform Lighting stage*/
    sideBuffer1->bind();
	glActiveTexture(GL_TEXTURE0);
	mainFBO->bindDiffuse();
	glActiveTexture(GL_TEXTURE1);
    mainFBO->bindSpecular();
	glActiveTexture(GL_TEXTURE2);
    mainFBO->bindDepth();
	glActiveTexture(GL_TEXTURE3);
    mainFBO->bindNormals();
	glActiveTexture(GL_TEXTURE0);
	pipeline->draw(LIGHTS_LEVEL);
    sideBuffer1->unbind();

    /*Blend in the ambient information*/
    sideBuffer3->bind();
    glActiveTexture(GL_TEXTURE0);
    sideBuffer1->bindTexture();
    glActiveTexture(GL_TEXTURE1);
    mainFBO->bindAmbient();
    glActiveTexture(GL_TEXTURE2);
    sideBuffer2->bindTexture();
    glActiveTexture(GL_TEXTURE0);
    pipeline->draw(AMBIENT_LEVEL);
    sideBuffer3->unbind();

    /*Prepare buffer to draw particles*/
    mainFBO->bind();
    blit->use();
    glUniform1i(renderTargetLoc, 4);
    glActiveTexture(GL_TEXTURE0);
    sideBuffer3->bindTexture();
    pipeline->draw(BLIT_LEVEL);

    /*Draw the normal particles*/
    particlePool->draw(DEFAULT_PARTICLES_LEVEL);
    mainFBO->unbind();

    /*Prepare the background for post-FX particles*/
    sideBuffer3->bind();
    blit->use();
    glUniform1i(renderTargetLoc, 0);
    glActiveTexture(GL_TEXTURE0);
    mainFBO->bindParticles();
    pipeline->draw(BLIT_LEVEL);
    sideBuffer3->unbind();

    /*Draw post-FX particles*/
    mainFBO->bind();
    glActiveTexture(GL_TEXTURE0);
    sideBuffer3->bindTexture();
    scene->draw(HEAT_HAZE_LEVEL);
    particlePool->draw(HEAT_HAZE_LEVEL);
    mainFBO->unbind();

    /*Apply FXAA and render to screen*/
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glActiveTexture(GL_TEXTURE0);
    mainFBO->bindParticles();
    pipeline->draw(FXAA_LEVEL);

    //sideBuffer2->blit();

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
    ResourceManager::getInstance()->getFrameBuffer(SIDE_FBO1)->resize(w, h);
    ResourceManager::getInstance()->getFrameBuffer(SIDE_FBO2)->resize(w, h);
    ResourceManager::getInstance()->getFrameBuffer(SIDE_FBO3)->resize(w, h);

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
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glDepthMask(GL_TRUE);
	glDepthRange(0.0, 1.0);
	glClearDepth(1.0);

    /*Set transparency on diffuse render target*/
    glEnablei(GL_BLEND, 0);
    glBlendFunci(0, GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnablei(GL_BLEND, 4);
    //glBlendFunci(4, GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glBlendFuncSeparatei(4, GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE);

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
