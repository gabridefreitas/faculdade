#include <assert.h>

#include <iostream>
#include <string>
#include <math.h>

using namespace std;

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <shader/shader.h>

GLuint drawOctagon() {
  GLfloat allCircleVertices[(8 + 2) * 3];

  allCircleVertices[0] = 0.0f;
  allCircleVertices[1] = 0.0f;
  allCircleVertices[2] = 0.0f;

  GLfloat doublePi = 2.0f * M_PI;

  for (int i = 1; i < 8 + 2; i++) {
    allCircleVertices[i * 3] = 0.5f * cos(i * doublePi / 8);
    allCircleVertices[i * 3 + 1] = 0.5f * sin(i * doublePi / 8);
    allCircleVertices[i * 3 + 2] = 0.0f;
  }

  GLuint VBO, VAO;

  glGenBuffers(1, &VBO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(allCircleVertices), allCircleVertices,
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

int renderOctagon(GLuint width, GLuint height, void (*loadGlad)(),
                  void (*key_callback)(GLFWwindow *window, int key,
                                       int scancode, int action, int mode),
                  void (*setWindowSize)(GLFWwindow *window, GLuint shaderID)) {
  GLFWwindow *windowOctagon =
      glfwCreateWindow(width, height, "Render Octagon", nullptr, nullptr);

  glfwMakeContextCurrent(windowOctagon);

  glfwSetKeyCallback(windowOctagon, key_callback);

  loadGlad();

  Shader shader("list_a/octagon/vertex-shader.vs",
                "list_a/octagon/fragment-shader.fs");

  GLuint VAO = drawOctagon();

  glUseProgram(shader.ID);

  while (!glfwWindowShouldClose(windowOctagon)) {
    setWindowSize(windowOctagon, shader.ID);

    glfwPollEvents();

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glBindVertexArray(VAO);

    glDrawArrays(GL_TRIANGLE_FAN, 0, (8 + 2) * 3);

    glBindVertexArray(0);

    glfwSwapBuffers(windowOctagon);
  }

  glDeleteVertexArrays(1, &VAO);
  glfwTerminate();

  return 0;
}