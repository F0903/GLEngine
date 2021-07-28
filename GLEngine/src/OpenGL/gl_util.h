#pragma once
#include <GL/glew.h>
#include "../util.h"

#ifdef _DEBUG
#define DEBUG_GL_CALL(call) { call; GLenum err; if((err = glGetError()) != GLEW_OK) DEBUG_ERR("OPENGL ERROR AT LINE " << __LINE__ << " IN " << __FILE__); }
#define DEBUG_GL_CHECK() { GLenum err; while ((err = glGetError()) != GL_NO_ERROR) { DEBUG_ERR("OPENGL ERROR BEFORE LINE " << __LINE__ << " IN " << __FILE__ << " : "  << err); } }
#else
#define DEBUG_GL_CALL(call)
#define DEBUG_GL_CHECK()
#endif //DEBUG
