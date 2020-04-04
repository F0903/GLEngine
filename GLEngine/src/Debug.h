#pragma once

#include <iostream>
#include <GL/glew.h>

#ifndef NDEBUG
#define SBOX_DEBUG
#endif //NDEBUG

#ifdef SBOX_DEBUG
#define GLE_GL_DETECT_ERROR() { unsigned int err; if ((err = glGetError()) != 0) std::cerr << "GL Error " << err << " detected before line " << __LINE__ << " in " << __FILE__; }
#define GLE_GL_DEBUG_CALL(x) { x; unsigned int eFirst, eLast; if((eFirst = glGetError()) != 0){ x; if((eLast = glGetError()) != 0){ std::cerr << "OPENGL ERROR: " << eLast << '\n'; __debugbreak(); } else std::cerr << "OPENGL ERROR: Error detected from previous call: " << eFirst << '\n'; } }
#else
#define GLE_GL_DETECT_ERROR()
#define GLE_GL_DEBUG_CALL(x) x;
#endif //SBOX_DEBUG