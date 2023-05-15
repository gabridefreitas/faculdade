#include <assert.h>

#include <iostream>
#include <string>

using namespace std;

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <shader/shader.h>

GLfloat TRIANGLE_POINTS_VERTICES[] = {
    // first triangle
    0.0,  0.6,  0.0,  // left bottom
    -0.6, -0.5, 0.0,  // right bottom
    0.6,  -0.3, 0.0,  // top middle
    0.0,  0.0,  0.0,  // center
};

GLuint drawTrianglePoints() {
  GLuint VAO, VBO;

  glGenBuffers(1, &VBO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(TRIANGLE_POINTS_VERTICES),
               TRIANGLE_POINTS_VERTICES, GL_STATIC_DRAW);

  glGenVertexArrays(1, &VAO);
  glBindVertexArray(VAO);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat),
                        (GLvoid *)0);

  glEnableVertexAttribArray(0);

  glBindBuffer(GL_ARRAY_BUFFER, 0);

  glBindVertexArray(0);

  return VAO;
}

int renderTrianglePoints(GLuint width, GLuint height, void (*loadGlad)(),
                         void (*key_callback)(GLFWwindow *window, int key,
                                              int scancode, int action,
                                              int mode),
                         void (*setWindowSize)(GLFWwindow *window)) {
  GLFWwindow *windowTwoTriangles = glfwCreateWindow(
      width, height, "Reder Triangle Points", nullptr, nullptr);

  glfwMakeContextCurrent(windowTwoTriangles);

  glfwSetKeyCallback(windowTwoTriangles, key_callback);

  loadGlad();

  Shader shader("list_a/triangle-points/vertex-shader.vs",
                "list_a/triangle-points/fragment-shader.fs");

  GLuint VAO = drawTrianglePoints();

  GLint color = glGetUniformLocation(shader.ID, "vertex_color");

  glUseProgram(shader.ID);

  while (!glfwWindowShouldClose(windowTwoTriangles)) {
    setWindowSize(windowTwoTriangles);

    glfwPollEvents();

    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glLineWidth(3);
    glPointSize(10);

    glBindVertexArray(VAO);

    glUniform3f(color, 1.0f, 0.0f, 0.0f);
    glDrawArrays(GL_POINTS, 0, 1);

    glUniform3f(color, 0.0f, 0.5f, 0.0f);
    glDrawArrays(GL_POINTS, 1, 1);

    glUniform3f(color, 0.4f, 0.6f, 0.9f);
    glDrawArrays(GL_POINTS, 2, 1);

    glUniform3f(color, 0.0f, 0.0f, 0.0f);
    glDrawArrays(GL_POINTS, 3, 1);

    glUniform3f(color, 0.25f, 0.4f, 0.9f);
    glDrawArrays(GL_LINE_LOOP, 0, 3);

    glBindVertexArray(0);

    glfwSwapBuffers(windowTwoTriangles);
  }

  glDeleteVertexArrays(1, &VAO);
  glfwTerminate();

  return 0;
}