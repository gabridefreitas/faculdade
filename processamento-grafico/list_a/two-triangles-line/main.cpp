#include <assert.h>

#include <iostream>
#include <string>

using namespace std;

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <shader/shader.h>

GLfloat VERTICES_LINE[] = {
    // first triangle
    -1.0f, -1.0f, 0.0f,  // left bottom
    -0.0f, -1.0f, 0.0f,  // right bottom
    -0.5f, 1.0f, 0.0f,   // top middle
    // second triangle
    0.0f, -1.0f, 0.0f,  // left bottom
    1.0f, -1.0f, 0.0,   // right bottom
    0.5f, 1.0f, 0.0f};  // top middle

GLuint drawTrianglesLine() {
  GLuint VAO, VBO;

  glGenBuffers(1, &VBO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(VERTICES_LINE), VERTICES_LINE,
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

int renderTwoTrianglesLine(GLuint width, GLuint height, void (*loadGlad)(),
                           void (*key_callback)(GLFWwindow* window, int key,
                                                int scancode, int action,
                                                int mode),
                           void (*setWindowSize)(GLFWwindow* window)) {
  GLFWwindow* windowTwoTrianglesLine = glfwCreateWindow(
      width, height, "Render 2 Triangles - Line", nullptr, nullptr);

  glfwMakeContextCurrent(windowTwoTrianglesLine);

  glfwSetKeyCallback(windowTwoTrianglesLine, key_callback);

  loadGlad();

  Shader shader("two-triangles-line/vertex-shader.vs",
                "two-triangles-line/fragment-shader.fs");

  GLuint VAO = drawTrianglesLine();

  glUseProgram(shader.ID);

  while (!glfwWindowShouldClose(windowTwoTrianglesLine)) {
    setWindowSize(windowTwoTrianglesLine);

    glfwPollEvents();

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glLineWidth(20);

    glBindVertexArray(VAO);

    glDrawArrays(GL_LINE_LOOP, 0, 3);
    glDrawArrays(GL_LINE_LOOP, 3, 3);

    glBindVertexArray(0);

    glfwSwapBuffers(windowTwoTrianglesLine);
  }

  glDeleteVertexArrays(1, &VAO);
  glfwTerminate();

  return 0;
}