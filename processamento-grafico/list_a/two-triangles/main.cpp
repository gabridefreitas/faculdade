#include <assert.h>

#include <iostream>
#include <string>

using namespace std;

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <shader/shader.h>

GLfloat VERTICES[] = {
    // first triangle
    -1.0f, -1.0f, 0.0f,  // left bottom
    -0.0f, -1.0f, 0.0f,  // right bottom
    -0.5f, 1.0f, 0.0f,   // top middle
    // second triangle
    0.0f, -1.0f, 0.0f,  // left bottom
    1.0f, -1.0f, 0.0,   // right bottom
    0.5f, 1.0f, 0.0f};  // top middle

GLuint drawTriangles() {
  GLuint VAO, VBO;

  glGenBuffers(1, &VBO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(VERTICES), VERTICES, GL_STATIC_DRAW);

  glGenVertexArrays(1, &VAO);
  glBindVertexArray(VAO);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat),
                        (GLvoid *)0);

  glEnableVertexAttribArray(0);

  glBindBuffer(GL_ARRAY_BUFFER, 0);

  glBindVertexArray(0);

  return VAO;
}

int renderTwoTriangles(GLuint width, GLuint height, void (*loadGlad)(),
                       void (*key_callback)(GLFWwindow *window, int key,
                                            int scancode, int action, int mode),
                       void (*setWindowSize)(GLFWwindow *window)) {
  GLFWwindow *windowTwoTriangles =
      glfwCreateWindow(width, height, "Render 2 Triangles", nullptr, nullptr);

  glfwMakeContextCurrent(windowTwoTriangles);

  glfwSetKeyCallback(windowTwoTriangles, key_callback);

  loadGlad();

  Shader shader("list_a/two-triangles/vertex-shader.vs",
                "list_a/two-triangles/fragment-shader.fs");

  GLuint VAO = drawTriangles();

  glUseProgram(shader.ID);

  while (!glfwWindowShouldClose(windowTwoTriangles)) {
    setWindowSize(windowTwoTriangles);

    glfwPollEvents();

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glBindVertexArray(VAO);

    glDrawArrays(GL_TRIANGLES, 0, 6);

    glBindVertexArray(0);

    glfwSwapBuffers(windowTwoTriangles);
  }

  glDeleteVertexArrays(1, &VAO);
  glfwTerminate();

  return 0;
}