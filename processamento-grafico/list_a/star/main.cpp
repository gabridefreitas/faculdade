#include <assert.h>

#include <iostream>
#include <string>

using namespace std;

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <shader/shader.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

GLfloat TRIANGLE_VERTICES[] = {
    // top triangle
    -0.1f, 0.25f, 0.0f,  // left-bottom
    0.1f, 0.25f, 0.0f,   // right-bottom
    0.0f, 0.65f, 0.0f,   // middle-top
    // left triangle
    -0.1f, 0.25f, 0.0f,  // left-top
    -0.5f, 0.17f, 0.0f,  // left
    -0.2f, 0.0f, 0.0f,   // left-bottom
    // left bottom triangle
    -0.2f, 0.0f, 0.0f,    // left-top
    -0.25f, -0.5f, 0.0f,  // bottom
    -0.0f, -0.17f, 0.0f,  // right-top
    // right bottom triangle
    0.2f, 0.0f, 0.0f,    // right-top
    0.25f, -0.5f, 0.0f,  // bottom
    0.0f, -0.17f, 0.0f,  // left-top
    // right triangle
    0.1f, 0.25f, 0.0f,  // right-top
    0.5f, 0.17f, 0.0f,  // right
    0.2f, 0.0f, 0.0f,   // right-bottom
};

GLuint drawTriangle() {
  GLuint VAO, VBO;

  glGenBuffers(1, &VBO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(TRIANGLE_VERTICES), TRIANGLE_VERTICES,
               GL_STATIC_DRAW);

  glGenVertexArrays(1, &VAO);
  glBindVertexArray(VAO);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat),
                        (GLvoid *)0);

  glEnableVertexAttribArray(0);

  glBindBuffer(GL_ARRAY_BUFFER, 0);

  glBindVertexArray(0);

  return VAO;
}

int renderStar(GLuint width, GLuint height, void (*loadGlad)(),
               void (*key_callback)(GLFWwindow *window, int key, int scancode,
                                    int action, int mode),
               void (*setWindowSize)(GLFWwindow *window, GLuint shaderID)) {
  GLFWwindow *windowStar =
      glfwCreateWindow(width, height, "Render Star", nullptr, nullptr);

  glfwMakeContextCurrent(windowStar);

  glfwSetKeyCallback(windowStar, key_callback);

  loadGlad();

  Shader shader("list_a/star/vertex-shader.vs",
                "list_a/star/fragment-shader.fs");

  GLuint VAO = drawTriangle();

  glUseProgram(shader.ID);

  while (!glfwWindowShouldClose(windowStar)) {
    setWindowSize(windowStar, shader.ID);

    glfwPollEvents();

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glBindVertexArray(VAO);

    glDrawArrays(GL_TRIANGLE_FAN, 0, 15);

    glBindVertexArray(0);

    glfwSwapBuffers(windowStar);
  }

  glDeleteVertexArrays(1, &VAO);
  glfwTerminate();

  return 0;
}