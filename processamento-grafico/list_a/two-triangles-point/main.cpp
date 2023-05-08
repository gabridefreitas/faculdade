#include <assert.h>

#include <iostream>
#include <string>

using namespace std;

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <shader/shader.h>

GLfloat VERTICES_POINT[] = {
    // first triangle
    -1.0f, -1.0f, 0.0f,  // left bottom
    -0.0f, -1.0f, 0.0f,  // right bottom
    -0.5f, 1.0f, 0.0f,   // top middle
    // second triangle
    0.0f, -1.0f, 0.0f,  // left bottom
    1.0f, -1.0f, 0.0,   // right bottom
    0.5f, 1.0f, 0.0f};  // top middle

GLuint drawTrianglesPoint() {
  GLuint VAO, VBO;

  glGenBuffers(1, &VBO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(VERTICES_POINT), VERTICES_POINT,
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

int renderTwoTrianglesPoint(GLuint width, GLuint height, void (*loadGlad)(),
                            void (*key_callback)(GLFWwindow* window, int key,
                                                 int scancode, int action,
                                                 int mode),
                            void (*setWindowSize)(GLFWwindow* window)) {
  GLFWwindow* windowTwoTrianglesPoint = glfwCreateWindow(
      width, height, "Render 2 Triangles - Point", nullptr, nullptr);

  glfwMakeContextCurrent(windowTwoTrianglesPoint);

  glfwSetKeyCallback(windowTwoTrianglesPoint, key_callback);

  loadGlad();

  Shader shader("list_a/two-triangles-point/vertex-shader.vs",
                "list_a/two-triangles-point/fragment-shader.fs");

  GLuint VAO = drawTrianglesPoint();

  glUseProgram(shader.ID);

  while (!glfwWindowShouldClose(windowTwoTrianglesPoint)) {
    setWindowSize(windowTwoTrianglesPoint);

    glfwPollEvents();

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glPointSize(10);

    glBindVertexArray(VAO);

    glDrawArrays(GL_POINTS, 0, 6);

    glBindVertexArray(0);

    glfwSwapBuffers(windowTwoTrianglesPoint);
  }

  glDeleteVertexArrays(1, &VAO);
  glfwTerminate();

  return 0;
}