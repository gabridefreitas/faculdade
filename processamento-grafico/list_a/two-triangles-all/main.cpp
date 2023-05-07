#include <assert.h>

#include <iostream>
#include <string>

using namespace std;

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <shader/shader.h>

GLfloat VERTICES_ALL[] = {
    // first triangle
    -1.0f, -1.0f, 0.0f,  // left bottom
    -0.0f, -1.0f, 0.0f,  // right bottom
    -0.5f, 1.0f, 0.0f,   // top middle
    // second triangle
    0.0f, -1.0f, 0.0f,  // left bottom
    1.0f, -1.0f, 0.0,   // right bottom
    0.5f, 1.0f, 0.0f};  // top middle

GLuint drawTrianglesAll() {
  GLuint VAO, VBO;

  glGenBuffers(1, &VBO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(VERTICES_ALL), VERTICES_ALL,
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

int renderTwoTrianglesAll(GLuint width, GLuint height, void (*loadGlad)(),
                          void (*key_callback)(GLFWwindow* window, int key,
                                               int scancode, int action,
                                               int mode),
                          void (*setWindowSize)(GLFWwindow* window)) {
  GLFWwindow* windowTwoTrianglesPoint = glfwCreateWindow(
      width, height, "Render 2 Triangles - All", nullptr, nullptr);

  glfwMakeContextCurrent(windowTwoTrianglesPoint);

  glfwSetKeyCallback(windowTwoTrianglesPoint, key_callback);

  loadGlad();

  Shader shader("two-triangles-all/vertex-shader.vs",
                "two-triangles-all/fragment-shader.fs");

  GLuint VAO = drawTrianglesAll();

  glUseProgram(shader.ID);

  while (!glfwWindowShouldClose(windowTwoTrianglesPoint)) {
    setWindowSize(windowTwoTrianglesPoint);

    glfwPollEvents();

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glPointSize(20);

    glBindVertexArray(VAO);

    glDrawArrays(GL_TRIANGLES, 0, 6);

    glDrawArrays(GL_LINE_LOOP, 0, 3);
    glDrawArrays(GL_LINE_LOOP, 3, 3);

    glDrawArrays(GL_POINT, 0, 6);

    glBindVertexArray(0);

    glfwSwapBuffers(windowTwoTrianglesPoint);
  }

  glDeleteVertexArrays(1, &VAO);
  glfwTerminate();

  return 0;
}