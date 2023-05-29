#include <assert.h>

#include <iostream>
#include <string>

using namespace std;

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <shader/shader.h>
#include <stb_image/stb_image.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "direction.cpp"

GLuint drawBall() {
  GLfloat VERTICES[] = {
      0.0,  50.0, 0.0,  // v0
      50.0, 50.0, 0.0,  // v1
      25.0, 0.0,  0.0,  // v2
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

GLuint drawBar() {
  GLfloat VERTICES[] = {
      // t1
      0.0, 0.0, 0.0,     // v0
      0.0, 100.0, 0.0,   // v1
      20.0, 100.0, 0.0,  // v2
      // t2
      0.0, 0.0, 0.0,     // v0
      20.0, 100.0, 0.0,  // v2
      20.0, 0.0, 0.0,    // v3
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

int renderGame(GLuint init_width, GLuint init_height, void (*loadGlad)()) {
  GLFWwindow *window =
      glfwCreateWindow(init_width, init_height, "Goal2Goal", nullptr, nullptr);

  glfwMakeContextCurrent(window);

  glfwSetKeyCallback(window, gameKeyCallback);

  loadGlad();

  Shader shader("ga/vertex-shader.vs", "ga/fragment-shader.fs");

  GLuint VAO = drawBall();
  GLuint p1VAO = drawBar();
  GLuint p2VAO = drawBar();

  GLint projection = glGetUniformLocation(shader.ID, "projection");
  GLint model = glGetUniformLocation(shader.ID, "model");

  glUseProgram(shader.ID);

  std::srand(std::time(nullptr));

  int paddingX = 20, barWidth = 20, barHeight = 100, ballSpeed = 5,
      barSpeed = 5, p1Score = 0, p2Score = 0;

  float centerX = init_width / 2;
  float centerY = init_height / 2;
  float barInitialY = centerY - 50.0f;

  float positionX = centerX, positionY = centerY, size = 50.0f,
        translateX = (float)ballSpeed, translateY = (float)ballSpeed;

  float p1PositionY = barInitialY, p2PositionY = barInitialY;

  while (!glfwWindowShouldClose(window)) {
    if (p1Score == 2 || p2Score == 2) {
      break;
    }

    glm::mat4 _model = glm::mat4(1);
    glm::mat4 _projection = glm::mat4(1);

    glfwPollEvents();

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    int width, height;

    glfwGetFramebufferSize(window, &width, &height);

    glViewport(0, 0, width, height);

    _projection =
        glm::ortho(0.0f, (float)width, 0.0f, (float)height, -1.0f, 1.0f);

    glUniformMatrix4fv(projection, 1, GL_FALSE, glm::value_ptr(_projection));

    // Draw player 1 bar
    glBindVertexArray(p1VAO);

    switch (p1Direction) {
      case UP:
        if (p1PositionY < height - size) {
          p1PositionY += barSpeed;
        }
        break;
      case DOWN:
        if (p1PositionY > size) {
          p1PositionY -= barSpeed;
        }
        break;
      default:
        break;
    }

    _model = glm::translate(_model, glm::vec3(paddingX, p1PositionY, 0));
    glUniformMatrix4fv(model, 1, GL_FALSE, glm::value_ptr(_model));
    glDrawArrays(GL_TRIANGLES, 0, 6);

    // Draw player 2 bar
    glBindVertexArray(p2VAO);

    switch (p2Direction) {
      case UP:
        if (p2PositionY < height - size) {
          p2PositionY += barSpeed;
        }
        break;
      case DOWN:
        if (p2PositionY > size) {
          p2PositionY -= barSpeed;
        }
        break;
      default:
        break;
    }

    _model = glm::mat4(1);
    _model = glm::translate(
        _model, glm::vec3(width - barWidth - paddingX, p2PositionY, 0));
    glUniformMatrix4fv(model, 1, GL_FALSE, glm::value_ptr(_model));
    glDrawArrays(GL_TRIANGLES, 0, 6);

    // Draw ball
    glBindVertexArray(VAO);

    if (positionX <= 0 || positionX >= width) {
      if (positionX <= 0) {
        p2Score++;
      } else {
        p1Score++;
      }

      positionX = centerX;
      positionY = centerY;
    }

    if (positionY <= 0 || positionY >= height - size) {
      translateY *= -1;
    }

    if  // if hit player 1 bar
        (positionX <= paddingX + barWidth && positionY >= p1PositionY - size &&
         positionY <= p1PositionY + barHeight) {
      translateX *= -1.05;
      translateY *= -1.1;
    } else if  // hit player 2 bar
        (positionX >= width - paddingX - barWidth - size &&
         positionY >= p2PositionY - size &&
         positionY <= p2PositionY + barHeight) {
      translateX *= -1.05;
      translateY *= ((std::rand() % 10) > 5) ? -1.1 : 1.1;
    }

    positionX += translateX;
    positionY += translateY;

    _model = glm::mat4(1);
    _model = glm::translate(_model, glm::vec3(positionX, positionY, 0));
    glUniformMatrix4fv(model, 1, GL_FALSE, glm::value_ptr(_model));

    glDrawArrays(GL_TRIANGLES, 0, 3);

    glBindVertexArray(0);
    glfwSwapBuffers(window);
  }

  glDeleteVertexArrays(1, &VAO);
  glDeleteVertexArrays(1, &p1VAO);
  glDeleteVertexArrays(1, &p2VAO);
  glfwTerminate();

  return 0;
}

// int getScoreTextureId(int p1Score, int p2Score, int &width, int &height) {
//   std::string filePath = "ga/assets/match" + std::to_string(p1Score) +
//                          std::to_string(p2Score) + ".png";

//   return generateTextureId(filePath, width, height);
// }
