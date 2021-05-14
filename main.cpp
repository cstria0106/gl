#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <cstdio>
#include "number.h"
#include "shader.h"

void error_callback(int code, const char *description) {
    fprintf(stderr, "%s\n", description);
}

void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
}

GLFWwindow *open_window() {
    glfwSetErrorCallback(error_callback);

    if (!glfwInit()) {
        return nullptr;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    glfwWindowHint(GLFW_SAMPLES, 4);

    GLFWwindow *window = glfwCreateWindow(800, 600, "GL", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return nullptr;
    }

    glfwMakeContextCurrent(window);
    glfwSetKeyCallback(window, key_callback);

    glfwSwapInterval(1);

    return window;
}

int main() {
    GLFWwindow *window = open_window();

    // OpenGL은 상태머신이다.
    // Context 안에 모든 정보를 들고있다.

    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        printf("unable to init glew\n");
        return 1;
    }

    if (!GLEW_VERSION_3_3) {
        printf("glew does not support 3.3\n");
        return 1;
    }

    f32 vertices[] = {
            0.0f, 0.5f, 0.0f,
            0.5f, -0.5f, 0.0f,
            -0.5f, -0.5f, 0.0f,
            -0.0f, -0.9f, 0.0f,
    };

    u32 indices[] = {
            0, 1, 2,
            1, 2, 3,
    };

    char error[512];

    i64 vertex_shader_result = load_shader("../shaders/vertex_shader.vert", GL_VERTEX_SHADER, error);
    if (vertex_shader_result == -1) {
        printf("%s\n", error);
        return 1;
    }

    i64 frag_shader_result = load_shader("../shaders/frag_shader.frag", GL_FRAGMENT_SHADER, error);
    if (frag_shader_result == -1) {
        printf("%s\n", error);
        return 1;
    }

    u32 vertex_shader = vertex_shader_result;
    u32 frag_shader = frag_shader_result;

    u32 *shaders[] = {&vertex_shader, &frag_shader, nullptr};
    i64 shader_program_result = link_shaders(shaders, error);

    if (shader_program_result == -1) {
        printf("%s\n", error);
        return 1;
    }

    u32 shader_program = shader_program_result;

    u32 vao;
    u32 vbo;
    u32 ebo;

    // Vertex Array Object 를 생성하고 바인딩한다
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    // Vertex Buffer Object 를 생성하고 바인드 후데이터를 넣는다.
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Element Array Buffer 를 생성하고 바인드 후데이터를 넣는다.
    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // Attribute 의 포인터 속성을 지정한다.
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *) nullptr);

    // Attribute 를 활성화한다.
    glEnableVertexAttribArray(0);

    // 각 속성을 언바인드한다.
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

#ifdef POLYGON
    // 폴리곤 모드
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
#endif

    while (!glfwWindowShouldClose(window)) {
        // 화면의 내용을 지운다.
        glClearColor(0.1, 0.1, 0.1, 0);
        glClear(GL_COLOR_BUFFER_BIT);

        // 쉐이더를 사용하도록 지정한다.
        glUseProgram(shader_program);

        // Vertex Array 를 불러오고 삼각형을 그린다.
        glBindVertexArray(vao);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *) nullptr);

        // Vertex Array 를 언바인드한다.
        glBindVertexArray(0);

        // 쉐이더를 초기화한다.
        glUseProgram(0);

        // 이중 버퍼를 화면에 그린다.
        glfwSwapBuffers(window);

        // 이벤트 버퍼를 확인하고 처리한다.
        // 입력이 있을 때만 처리를 한다면 glfwWaitEvents()를 사용한다.
        glfwPollEvents();
    }

    return 0;
}
