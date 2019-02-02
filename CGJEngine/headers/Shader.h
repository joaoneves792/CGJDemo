#ifndef SHADER_H_
#define SHADER_H_

#include <vector>
#include <functional>
#define GLEW_STATIC
#include <GL/glew.h>
#include "glm_wrapper.h"

class Shader{
private:
	GLuint _shaderProgram;
	GLuint _vertexShader;
	GLuint _fragmentShader;

    std::function<void(const Mat4&, const Mat4&, const Mat4&)> _uploadMVPCallback;

public:
	Shader();
	virtual ~Shader();

	void loadFromFiles(const char* path_vert_shader, const char* path_frag_shader);
	void loadFromString(const char* vert_shader, const char* frag_shader);
	void link();
	void use();
	void clean();
	GLuint getShader();
	GLint getAttribLocation(const char* name);
	GLint getUniformLocation(const char* name);
	void setAttribLocation(const char* name, GLuint position);
	void setFragOutputLocation(const char* name, GLuint position);
    void setMVPFunction(std::function<void(const Mat4&, const Mat4&, const Mat4&)> callback);
    void uploadMVP(const Mat4& M, const Mat4& V, const Mat4& P);
private:
	GLuint load_shader_from_file(const char *fname, GLenum shaderType);
	GLuint compile_shader(const char* glsl, GLenum shaderType);
	void read_shader_src(const char *fname, std::vector<char> &buffer);
};


#endif /* SHADER_H_*/

