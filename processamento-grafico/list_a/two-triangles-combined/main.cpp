#include <assert.h>

#include <iostream>
#include <string>

using namespace std;

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <shader/shader.h>

GLfloat VERTICES_COMBINED[] = {
    // first triangle
    -1.0f, -1.0f, 0.0f,  // left bottom
    -0.0f, -1.0f, 0.0f,  // right bottom
    -0.5f, 1.0f, 0.0f,   // top middle
    // second triangle
    0.0f, -1.0f, 0.0f,  // left bottom
    1.0f, -1.0f, 0.0,   // right bottom
    0.5f, 1.0f, 0.0f};  // top middle

GLuint drawTrianglesCombined() {
  GLuint VAO, VBO;

  glGenBuffers(1, &VBO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(VERTICES_COMBINED), VERTICES_COMBINED,
               GL_STATIC_DRAW);

  glGenVertexArrays(1, &VAO);
  glBindVertexArray(VAO);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat),
                        (GLvoid*)0);

  glEnableVertexAttribArray(0);

  glBindBuffer(GL_ARRAY_BUFFER, 0);

  glBindVertexArray(0);

  return VAO;
}

int renderTwoTrianglesCombined(GLuint width, GLuint height, void (*loadGlad)(),
                               void (*key_callback)(GLFWwindow* window, int key,
                                                    int scancode, int action,
                                                    int mode),
                               void (*setWindowSize)(GLFWwindow* window)) {
  GLFWwindow* windowTwoTrianglesCombined = glfwCreateWindow(
      width, height, "Render 2 Triangles - Combined", nullptr, nullptr);

  glfwMakeContextCurrent(windowTwoTrianglesCombined);

  glfwSetKeyCallback(windowTwoTrianglesCombined, key_callback);

  loadGlad();

  Shader shader("list_a/two-triangles-combined/vertex-shader.vs",
                "list_a/two-triangles-combined/fragment-shader.fs");

  GLuint VAO = drawTrianglesCombined();

  GLint color = glGetUniformLocation(shader.ID, "vertex_color");

  glUseProgram(shader.ID);

  while (!glfwWindowShouldClose(windowTwoTrianglesCombined)) {
    setWindowSize(windowTwoTrianglesCombined);

    glfwPollEvents();

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glLineWidth(20);
    glPointSize(10);

    glBindVertexArray(VAO);

    glUniform3f(color, 1.0f, 0.0f, 0.0f);

    glDrawArrays(GL_TRIANGLES, 0, 6);

    glUniform3f(color, 0.0f, 1.0f, 0.0f);

    glDrawArrays(GL_LINE_LOOP, 0, 3);
    glDrawArrays(GL_LINE_LOOP, 3, 3);

    glUniform3f(color, 0.0f, 0.0f, 1.0f);

    glDrawArrays(GL_POINTS, 0, 6);

    glBindVertexArray(0);

    glfwSwapBuffers(windowTwoTrianglesCombined);
  }

  glDeleteVertexArrays(1, &VAO);
  glfwTerminate();

  return 0;
}