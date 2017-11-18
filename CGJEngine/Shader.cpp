#define GLEW_STATIC
#include <GL/glew.h>
#include <vector>
#include <iostream>
#include <fstream>

#include "Shader.h"
#include "glerrors.h"
#include "mat.h"

Shader::Shader(const char* path_vert_shader, const char* path_frag_shader){
	create_program(path_vert_shader, path_frag_shader);
    uploadMVPCallback = nullptr;
}


void Shader::clean(){
	glDetachShader(_shaderProgram, _vertexShader);
	glDetachShader(_shaderProgram, _fragmentShader);
	//glDeleteShader(_fragmentShader);
	//glDeleteShader(_vertexShader);
	glDeleteProgram(_shaderProgram);
}

Shader::~Shader(){
	clean();
}

GLuint Shader::getShader(){
	return _shaderProgram;
}

// Read a shader source from a file
// store the shader source in a std::vector<char>
void Shader::read_shader_src(const char *fname, std::vector<char> &buffer) {
        std::ifstream in;
        in.open(fname, std::ios::binary);

        if(in.is_open()) {
                // Get the number of bytes stored in this file
                in.seekg(0, std::ios::end);
                size_t length = (size_t)in.tellg();

                // Go to start of the file
                in.seekg(0, std::ios::beg);

                // Read the content of the file in a buffer
                buffer.resize(length + 1);
                in.read(&buffer[0], length);
                in.close();
                // Add a valid C - string end
                buffer[length] = '\0';
        }
        else {
                std::cerr << "Unable to open " << fname << " I'm out!" << std::endl;
                return;
        }
}


// Compile a shader
GLuint Shader::load_and_compile_shader(const char *fname, GLenum shaderType) {
        // Load a shader from an external file
        std::vector<char> buffer;
        read_shader_src(fname, buffer);
        const char *src = &buffer[0];

        // Compile the shader
        GLuint shader = glCreateShader(shaderType);
        glShaderSource(shader, 1, &src, NULL);
        glCompileShader(shader);
        // Check the result of the compilation
        GLint test;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &test);
        if(!test) {
                std::cerr << "Shader compilation failed with this message:" << std::endl;
                std::vector<char> compilation_log(512);
                glGetShaderInfoLog(shader, compilation_log.size(), NULL, &compilation_log[0]);
                std::cerr << &compilation_log[0] << std::endl;
                return 0;
        }
        return shader;
}

// Create a program from two shaders
void Shader::create_program(const char *path_vert_shader, const char *path_frag_shader) {
        // Load and compile the vertex and fragment shaders
        _vertexShader = load_and_compile_shader(path_vert_shader, GL_VERTEX_SHADER);
        _fragmentShader = load_and_compile_shader(path_frag_shader, GL_FRAGMENT_SHADER);

        // Attach the above shader to a program
        GLuint shaderProgram = glCreateProgram();
        glAttachShader(shaderProgram, _vertexShader);
        glAttachShader(shaderProgram, _fragmentShader);


        // Flag the shaders for deletion
        glDeleteShader(_vertexShader);
        glDeleteShader(_fragmentShader);


        /*glBindAttribLocation(shaderProgram, 0, "position");
        glBindAttribLocation(shaderProgram, 1, "texture_coord");
        glBindAttribLocation(shaderProgram, 2, "normal");
        glBindAttribLocation(shaderProgram, 3, "jointIndex");
        glBindAttribLocation(shaderProgram, 4, "jointWeight");*/
        // Link and use the program
        //glLinkProgram(shaderProgram);

	checkOpenGLError("Failed to load shaders");

        _shaderProgram = shaderProgram;
	
}

void Shader::link(){
        glLinkProgram(_shaderProgram);
	GLint isLinked = 0;
	glGetProgramiv(_shaderProgram, GL_LINK_STATUS, &isLinked);
        if(!isLinked) {

		GLint maxLength = 0;
		glGetProgramiv(_shaderProgram, GL_INFO_LOG_LENGTH, &maxLength);

		//The maxLength includes the NULL character
		std::vector<GLchar> infoLog(maxLength);
		glGetProgramInfoLog(_shaderProgram, maxLength, &maxLength, &infoLog[0]);

		//The program is useless now. So delete it.
		glDeleteProgram(_shaderProgram);

                std::cerr << "Shader link failed with this message:" << std::endl;
                std::cerr << &infoLog[0] << std::endl;
        }
	checkOpenGLError("Failed to load shaders");
}

void Shader::setAttribLocation(const char* name, GLuint position){
        glBindAttribLocation(_shaderProgram, position, name);
}

GLint Shader::getAttribLocation(const char* name){
	return glGetAttribLocation(_shaderProgram, name);
}

GLint Shader::getUniformLocation(const char* name){
	return glGetUniformLocation(_shaderProgram, name);
}

void Shader::use(){
        glUseProgram(_shaderProgram);
}

void Shader::setMVPFunction(std::function<void(Mat4, Mat4, Mat4)> callback) {
    uploadMVPCallback = callback;
}

void Shader::uploadMVP(Mat4 M, Mat4 V, Mat4 P) {
    if(uploadMVPCallback != nullptr)
        uploadMVPCallback(M, V, P);
}