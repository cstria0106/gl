//
// Created by Connple on 2021/05/14.
//

#include "shader.h"
#include "file.h"
#include<cstdio>
#include<GL/glew.h>

i64 link_shaders(u32 **shaders, char *error) {
    size_t i = 0;
    i64 shader_program = glCreateProgram();

    while (shaders[i] != nullptr) {
        glAttachShader(shader_program, *shaders[i]);
        i++;
    }

    glLinkProgram(shader_program);

    int success;
    glGetProgramiv(shader_program, GL_LINK_STATUS, &success);

    if (!success) {
        glGetProgramInfoLog(shader_program, 512, nullptr, error);
        return -1;
    }

    i = 0;
    while (shaders[i] != nullptr) {
        glDeleteShader(*shaders[i]);
        i++;
    }

    return shader_program;
}

i64 load_shader(const char *path, GLenum type, char *error) {
    char *code = load_file(path);
    if (!code) {
        sprintf(error, "could not read file %s: %s", path, error);
        return -1;
    }

    u32 shader = glCreateShader(type);
    glShaderSource(shader, 1, &code, nullptr);
    glCompileShader(shader);

    int success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

    if (!success) {
        glGetShaderInfoLog(shader, 512, nullptr, error);
        return -1;
    }

    return (i64) shader;
}