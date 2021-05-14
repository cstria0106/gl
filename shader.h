//
// Created by Connple on 2021/05/14.
//

#ifndef GL_SHADER_H
#define GL_SHADER_H

#include "number.h"
#include <GL/glew.h>

i64 link_shaders(u32 **shaders, char *error);

i64 load_shader(const char *path, GLenum type, char *error);

#endif //GL_SHADER_H
