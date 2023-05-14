#include <assert.h>

#include <iostream>
#include <string>
#include <math.h>

using namespace std;

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <shader/shader.h>

GLuint drawSpiral() {
  int nPoints = 360 + 2;

  GLfloat allCircleVertices[nPoints * 3];

  float angle = 0.0;
  float deltaAngle = 6 * M_PI / (float)(nPoints - 2);
  float radius = 0.0;
  float radiusIncrement = 0.5 / (float)(nPoints - 2);

  allCircleVertices[0] = 0.0f;
  allCircleVertices[1] = 0.0f;
  allCircleVertices[2] = 0.0f;

  for (int i = 1; i < nPoints;
       i++, angle += deltaAngle, radius += radiusIncrement) {
    allCircleVertices[i * 3] = radius * cos(angle);
    allCircleVertices[i * 3 + 1] = radius * sin(angle);
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

int renderSpiral(GLuint width, GLuint height, void (*loadGlad)(),
                 void (*key_callback)(GLFWwindow *window, int key, int scancode,
                                      int action, int mode),
                 void (*setWindowSize)(GLFWwindow *window, GLuint shaderID)) {
  GLFWwindow *windowSpiral =
      glfwCreateWindow(width, height, "Render Spiral", nullptr, nullptr);

  glfwMakeContextCurrent(windowSpiral);

  glfwSetKeyCallback(windowSpiral, key_callback);

  loadGlad();

  Shader shader("list_a/spiral/vertex-shader.vs",
                "list_a/spiral/fragment-shader.fs");

  GLuint VAO = drawSpiral();

  glUseProgram(shader.ID);

  while (!glfwWindowShouldClose(windowSpiral)) {
    setWindowSize(windowSpiral, shader.ID);

    glfwPollEvents();

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glBindVertexArray(VAO);

    glDrawArrays(GL_LINE_STRIP, 1, ((360 + 2) - 1));

    glBindVertexArray(0);

    glfwSwapBuffers(windowSpiral);
  }

  glDeleteVertexArrays(1, &VAO);
  glfwTerminate();

  return 0;
}