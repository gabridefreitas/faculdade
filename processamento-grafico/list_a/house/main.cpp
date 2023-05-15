#include <assert.h>

#include <iostream>
#include <string>

using namespace std;

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <shader/shader.h>

GLuint drawRoof() {
  GLfloat VERTICES[] = {
      -0.4, 0.4, 0.0,  // v0
      0.4,  0.4, 0.0,  // v1
      0.0,  0.9, 0.0,  // v2
  };

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

GLuint drawHouse() {
  GLfloat VERTICES[] = {
      -0.4, 0.4,  0.0,  // v0
      -0.4, -0.4, 0.0,  // v1
      0.4,  -0.4, 0.0,  // v2
      0.4,  0.4,  0.0,  // v3
  };

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

GLuint drawDoor() {
  GLfloat VERTICES[] = {
      -0.1, -0.2, 0.0,  // v0
      -0.1, -0.4, 0.0,  // v1
      0.1,  -0.4, 0.0,  // v2
      0.1,  -0.2, 0.0,  // v3
  };

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

GLuint drawWindow() {
  GLfloat VERTICES[] = {
      -0.3, 0.0, 0.0,  // v0
      -0.3, 0.2, 0.0,  // v1
      -0.1, 0.2, 0.0,  // v2
      -0.1, 0.0, 0.0,  // v3
  };

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

GLuint drawFloor() {
  GLfloat VERTICES[] = {
      -1.0, -0.4, 0.0,  // v0
      -1.0, -1.0, 0.0,  // v1
      1.0,  -1.0, 0.0,  // v2
      1.0,  -0.4, 0.0,  // v3
  };

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

int renderHouse(GLuint width, GLuint height, void (*loadGlad)(),
                void (*key_callback)(GLFWwindow *window, int key, int scancode,
                                     int action, int mode),
                void (*setWindowSize)(GLFWwindow *window)) {
  GLFWwindow *windowHouse =
      glfwCreateWindow(width, height, "Reder House", nullptr, nullptr);

  glfwMakeContextCurrent(windowHouse);

  glfwSetKeyCallback(windowHouse, key_callback);

  loadGlad();

  Shader shader("list_a/house/vertex-shader.vs",
                "list_a/house/fragment-shader.fs");

  GLuint roofVAO = drawRoof();
  GLuint houseVAO = drawHouse();
  GLuint doorVAO = drawDoor();
  GLuint windowVAO = drawWindow();
  GLuint floorVAO = drawFloor();

  GLint color = glGetUniformLocation(shader.ID, "vertex_color");

  glUseProgram(shader.ID);

  while (!glfwWindowShouldClose(windowHouse)) {
    setWindowSize(windowHouse);

    glfwPollEvents();

    glClearColor(0.5f, 0.8f, 0.9f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glLineWidth(3);
    glPointSize(10);

    glBindVertexArray(roofVAO);

    glUniform3f(color, 0.8f, 0.0f, 0.0f);
    glDrawArrays(GL_TRIANGLES, 0, 3);

    glBindVertexArray(houseVAO);

    glUniform3f(color, 0.9f, 0.9f, 0.9f);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

    glBindVertexArray(doorVAO);

    glUniform3f(color, 0.7f, 0.4f, 0.2f);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

    glBindVertexArray(windowVAO);

    glUniform3f(color, 1.0f, 1.0f, 0.9f);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

    glBindVertexArray(floorVAO);

    glUniform3f(color, 0.2f, 0.5f, 0.2f);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

    glBindVertexArray(0);

    glfwSwapBuffers(windowHouse);
  }

  glDeleteVertexArrays(1, &roofVAO);
  glDeleteVertexArrays(1, &houseVAO);
  glDeleteVertexArrays(1, &doorVAO);
  glfwTerminate();

  return 0;
}