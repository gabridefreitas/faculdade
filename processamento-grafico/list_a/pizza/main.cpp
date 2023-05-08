#include <assert.h>

#include <iostream>
#include <string>
#include <math.h>

using namespace std;

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <shader/shader.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

GLuint drawPizzaSlice() {
  GLfloat allCircleVertices[(72 + 2) * 3];

  allCircleVertices[0] = 0.0f;
  allCircleVertices[1] = 0.0f;
  allCircleVertices[2] = 0.0f;

  GLfloat doublePi = 2.0f * M_PI;

  for (int i = 1; i < 72 + 2; i++) {
    allCircleVertices[i * 3] = 0.5f * cos(i * doublePi / 360);
    allCircleVertices[i * 3 + 1] = 0.5f * sin(i * doublePi / 360);
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

GLuint drawPizzaPepperoni() {
  GLfloat allCircleVertices[(360 + 2) * 3];

  allCircleVertices[0] = 0.0f;
  allCircleVertices[1] = 0.0f;
  allCircleVertices[2] = 0.0f;

  GLfloat doublePi = 2.0f * M_PI;

  for (int i = 1; i < 360 + 2; i++) {
    allCircleVertices[i * 3] = 0.05f * cos(i * doublePi / 360);
    allCircleVertices[i * 3 + 1] = 0.05f * sin(i * doublePi / 360);
    allCircleVertices[i * 3 + 2] = 0.0f;
  }

  GLuint VBO, VAO;

  glGenBuffers(1, &VBO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(allCircleVertices), allCircleVertices,
               GL_STATIC_DRAW);

  glGenVertexArrays(1, &VAO);
  glBindVertexArray(VAO);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_TRUE, 3 * sizeof(GLfloat),
                        (GLvoid *)0);

  glEnableVertexAttribArray(0);

  glBindBuffer(GL_ARRAY_BUFFER, 0);

  glBindVertexArray(0);

  return VAO;
}

void renderPepperoni(GLint translate, GLuint VAO, glm::vec3 translateVec) {
  glm::mat4 pepperoni_translate = glm::translate(glm::mat4(1.0f), translateVec);

  glUniformMatrix4fv(translate, 1, GL_FALSE, &pepperoni_translate[0][0]);

  glBindVertexArray(VAO);

  glDrawArrays(GL_TRIANGLE_FAN, 0, (360 + 2) * 3);
}

int renderPizzaSlice(GLuint width, GLuint height, void (*loadGlad)(),
                     void (*key_callback)(GLFWwindow *window, int key,
                                          int scancode, int action, int mode),
                     void (*setWindowSize)(GLFWwindow *window,
                                           GLuint shaderID)) {
  GLFWwindow *windowPizza =
      glfwCreateWindow(width, height, "Render Pizza", nullptr, nullptr);

  glfwMakeContextCurrent(windowPizza);

  glfwSetKeyCallback(windowPizza, key_callback);

  loadGlad();

  Shader shader("list_a/pizza/vertex-shader.vs",
                "list_a/pizza/fragment-shader.fs");

  GLuint pizzaVAO = drawPizzaSlice();
  GLuint pepperoniVAO = drawPizzaPepperoni();

  GLint color = glGetUniformLocation(shader.ID, "vertex_color");
  GLint rotate = glGetUniformLocation(shader.ID, "rotate");
  GLint translate = glGetUniformLocation(shader.ID, "translate");
  GLboolean shouldRotate = glGetUniformLocation(shader.ID, "shouldRotate");
  GLboolean shouldTranslate =
      glGetUniformLocation(shader.ID, "shouldTranslate");

  glUseProgram(shader.ID);

  while (!glfwWindowShouldClose(windowPizza)) {
    setWindowSize(windowPizza, shader.ID);

    glfwPollEvents();

    glUniform1i(shouldRotate, GL_TRUE);

    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glm::mat4 slice_rotate = glm::rotate(glm::mat4(1.0f), glm::radians(45.0f),
                                         glm::vec3(0.0f, 0.0f, 1.0f));

    glUniformMatrix4fv(rotate, 1, GL_FALSE, &slice_rotate[0][0]);

    glBindVertexArray(pizzaVAO);

    glLineWidth(15.0f);
    glUniform3f(color, 1.0f, 1.0f, 0.0f);

    glDrawArrays(GL_TRIANGLE_FAN, 0, (360 + 2) * 3);

    glUniform3f(color, 0.0f, 0.0f, 0.0f);

    glDrawArrays(GL_LINE_LOOP, 0, (360 + 2) * 3);

    glUniform1i(shouldRotate, GL_FALSE);
    glUniform1i(shouldTranslate, GL_TRUE);

    glUniform3f(color, 0.9f, 0.1f, 0.1f);

    renderPepperoni(translate, pepperoniVAO, glm::vec3(0.0f, 0.12f, 0.0f));
    renderPepperoni(translate, pepperoniVAO, glm::vec3(-0.05f, 0.35f, 0.0f));
    renderPepperoni(translate, pepperoniVAO, glm::vec3(0.15f, 0.25f, 0.0f));

    glBindVertexArray(0);

    glfwSwapBuffers(windowPizza);
  }

  glDeleteVertexArrays(1, &pizzaVAO);
  glDeleteVertexArrays(1, &pepperoniVAO);
  glfwTerminate();

  return 0;
}