#include <assert.h>

#include <iostream>
#include <string>

using namespace std;

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <shader/shader.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

enum Direction { UP, DOWN, LEFT, RIGHT, STOP };
static Direction direction = STOP;

void directionKeyCallback(GLFWwindow *window, int key, int scancode, int action,
                          int mode) {
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    glfwSetWindowShouldClose(window, GL_TRUE);

  if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS ||
      glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
    direction = RIGHT;
  } else if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS ||
             glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
    direction = LEFT;
  } else if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS ||
             glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
    direction = UP;
  } else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS ||
             glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
    direction = DOWN;
  } else {
    direction = STOP;
  }
}

GLuint drawTriangleMove() {
  GLfloat VERTICES[] = {
      50.0,  100.0, 0.0,  // v0
      100.0, 100.0, 0.0,  // v1
      75.0,  50.0,  0.0,  // v2
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

int renderMove(GLuint init_width, GLuint init_height, void (*loadGlad)()) {
  GLFWwindow *window = glfwCreateWindow(init_width, init_height, "Render Move",
                                        nullptr, nullptr);

  glfwMakeContextCurrent(window);

  glfwSetKeyCallback(window, directionKeyCallback);

  loadGlad();

  Shader shader("list_c/move/vertex-shader.vs",
                "list_c/move/fragment-shader.fs");

  GLuint VAO = drawTriangleMove();

  GLint projection = glGetUniformLocation(shader.ID, "projection");
  GLint model = glGetUniformLocation(shader.ID, "model");

  glUseProgram(shader.ID);

  glm::mat4 _projection = glm::mat4(1);
  _projection = glm::ortho(0.0, 800.0, 600.0, 0.0, -1.0, 1.0);

  glUniformMatrix4fv(projection, 1, GL_FALSE, glm::value_ptr(_projection));

  float positionX = 400.0f, positionY = 300.0f, radius = 50.0f;

  while (!glfwWindowShouldClose(window)) {
    glfwPollEvents();

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    int width, height;

    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0, 0, width, height);

    glBindVertexArray(VAO);

    glm::mat4 _model = glm::mat4(1);

    switch (direction) {
      case LEFT:
        if (positionX > radius) {
          positionX -= 3.0f;
        }
        break;
      case RIGHT:
        if (positionX < width - radius) {
          positionX += 3.0f;
        }
        break;
      case UP:
        if (positionY < height - radius) {
          positionY -= 3.0f;
        }
        break;
      case DOWN:
        if (positionY > radius) {
          positionY += 3.0f;
        }
        break;
      default:
        break;
    }

    _model = glm::translate(_model, glm::vec3(positionX, positionY, 0));
    glUniformMatrix4fv(model, 1, GL_FALSE, glm::value_ptr(_model));

    glDrawArrays(GL_TRIANGLES, 0, 3);

    glBindVertexArray(0);

    glfwSwapBuffers(window);
  }

  glDeleteVertexArrays(1, &VAO);
  glfwTerminate();

  return 0;
}