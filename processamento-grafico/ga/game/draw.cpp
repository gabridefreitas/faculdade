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

struct ScoreTexture {
  GLuint textureId;
  string id;
};

GLuint generateTextureId(string filePath, int &width, int &height) {
  GLuint texID;

  glGenTextures(1, &texID);
  glBindTexture(GL_TEXTURE_2D, texID);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  int nrChannels;
  unsigned char *data =
      stbi_load(filePath.c_str(), &width, &height, &nrChannels, 0);

  if (data) {
    // jpg, bmp
    if (nrChannels == 3) {
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
                   GL_UNSIGNED_BYTE, data);
    }
    // png
    else {
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA,
                   GL_UNSIGNED_BYTE, data);
    }
    glGenerateMipmap(GL_TEXTURE_2D);
  } else {
    std::cout << "Failed to load texture" << std::endl;
  }

  stbi_image_free(data);
  glBindTexture(GL_TEXTURE_2D, 0);

  return texID;
}

GLuint *getBallTextures(int &width, int &height) {
  static GLuint ballTextures[3] = {
      generateTextureId("ga/assets/ball0.png", width, height),
      generateTextureId("ga/assets/ball1.png", width, height),
      generateTextureId("ga/assets/ball2.png", width, height)};

  return ballTextures;
}

GLuint *getBarTextures(int &width, int &height) {
  static GLuint barTextures[3] = {
      generateTextureId("ga/assets/bar0.png", width, height),
      generateTextureId("ga/assets/bar1.png", width, height),
      generateTextureId("ga/assets/bar2.png", width, height)};

  return barTextures;
}

ScoreTexture *getScoreTextures(int &width, int &height) {
  static ScoreTexture scoreTextures[8] = {
      {generateTextureId("ga/assets/match00.png", width, height), "00"},
      {generateTextureId("ga/assets/match01.png", width, height), "01"},
      {generateTextureId("ga/assets/match02.png", width, height), "02"},
      {generateTextureId("ga/assets/match10.png", width, height), "10"},
      {generateTextureId("ga/assets/match11.png", width, height), "11"},
      {generateTextureId("ga/assets/match12.png", width, height), "12"},
      {generateTextureId("ga/assets/match20.png", width, height), "20"},
      {generateTextureId("ga/assets/match21.png", width, height), "21"},
  };

  return scoreTextures;
}

ScoreTexture getScoreTexture(ScoreTexture *scoreTextures, int p1Score,
                             int p2Score) {
  string id = to_string(p1Score) + to_string(p2Score);

  for (int i = 0; i < 8; i++) {
    if (scoreTextures[i].id == id) {
      return scoreTextures[i];
    }
  }

  return scoreTextures[0];
}

int setupSprite() {
  GLfloat vertices[] = {
      // x   y     z    r    g    b     s     t
      // T1
      0.0, 1.0, 0.0, 1.0, 0.0, 0.0, 0.0, 1.0,  // v0 superior esq
      0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0,  // v1 inferior esq
      1.0, 1.0, 0.0, 0.0, 0.0, 1.0, 1.0, 1.0,  // v2 superior dir
      // T2
      0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0,  // v1 inferior esq
      1.0, 1.0, 0.0, 0.0, 0.0, 1.0, 1.0, 1.0,  // v2 superior dir
      1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 1.0, 0.0   // v3 inferior dir
  };

  GLuint VBO, VAO;

  glGenBuffers(1, &VBO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  glGenVertexArrays(1, &VAO);
  glBindVertexArray(VAO);

  // position
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat),
                        (GLvoid *)0);
  glEnableVertexAttribArray(0);

  // fragColor
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat),
                        (GLvoid *)(3 * sizeof(GLfloat)));
  glEnableVertexAttribArray(1);

  // textureCoordinates
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat),
                        (GLvoid *)(6 * sizeof(GLfloat)));
  glEnableVertexAttribArray(2);

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

  GLuint ballVAO = setupSprite();
  GLuint p1VAO = setupSprite();
  GLuint p2VAO = setupSprite();
  GLuint backgroundVAO = setupSprite();
  GLuint fieldVAO = setupSprite();
  GLuint scoreVAO = setupSprite();

  int backgroundTextureWidth, backgroundTextureHeight, fieldTextureWidth,
      fieldTextureHeight, ballTextureWidth, ballTextureHeight, barTextureWidth,
      barTextureHeight, scoreTextureWidth, scoreTextureHeight;

  GLuint backgroundTextureId =
      generateTextureId("ga/assets/background.png", backgroundTextureWidth,
                        backgroundTextureHeight);
  GLuint fieldTextureId = generateTextureId(
      "ga/assets/field.png", fieldTextureWidth, fieldTextureHeight);
  GLuint *ballTextureId = getBallTextures(ballTextureWidth, ballTextureHeight);
  GLuint *barTextureId = getBarTextures(barTextureWidth, barTextureHeight);
  ScoreTexture *scoreTextureId =
      getScoreTextures(scoreTextureWidth, scoreTextureHeight);

  GLint projection = glGetUniformLocation(shader.ID, "projection");
  GLint model = glGetUniformLocation(shader.ID, "model");

  glUseProgram(shader.ID);

  std::srand(std::time(nullptr));

  glActiveTexture(GL_TEXTURE0);

  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_ALWAYS);

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  int padding = 20, barWidth = 20, barHeight = 100, ballWidth = 100,
      ballHeight = 50, ballSpeed = 5, barSpeed = 5, p1Score = 0, p2Score = 0,
      ballDirection = 1, textureIndex = 0;

  float centerX = init_width / 2;
  float centerY = init_height / 2;
  float barInitialY = centerY - (barHeight / 2);

  float positionX = centerX, positionY = centerY, size = 50.0f,
        translateX = (float)ballSpeed, translateY = (float)ballSpeed;

  float p1PositionY = barInitialY, p2PositionY = barInitialY;

  // Game loop
  while (!glfwWindowShouldClose(window)) {
    if (p1Score == 2 || p2Score == 2) {
      break;
    }

    if (textureIndex == 3) {
      textureIndex = 0;
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

    // Draw background
    glBindVertexArray(backgroundVAO);

    _model = glm::mat4(1);
    _model = glm::scale(_model, glm::vec3(width, height, 1.0));
    glUniformMatrix4fv(model, 1, GL_FALSE, glm::value_ptr(_model));

    glBindTexture(GL_TEXTURE_2D, backgroundTextureId);
    glDrawArrays(GL_TRIANGLES, 0, 6);

    // Draw field
    glBindVertexArray(fieldVAO);

    _model = glm::mat4(1);
    _model = glm::scale(_model, glm::vec3(width, height, 1.0));
    glUniformMatrix4fv(model, 1, GL_FALSE, glm::value_ptr(_model));

    glBindTexture(GL_TEXTURE_2D, fieldTextureId);
    glDrawArrays(GL_TRIANGLES, 0, 6);

    // Draw score
    glBindVertexArray(scoreVAO);

    _model = glm::mat4(1);

    double scoreWidth = scoreTextureWidth * 0.3,
           scoreHeight = scoreTextureHeight * 0.3,
           scoreX = (width / 2) - (scoreWidth / 2),
           scoreY = height - scoreHeight - padding;

    _model = glm::translate(_model, glm::vec3(scoreX, scoreY, 0));
    _model = glm::scale(_model, glm::vec3(scoreWidth, scoreHeight, 1.0));
    glUniformMatrix4fv(model, 1, GL_FALSE, glm::value_ptr(_model));

    glBindTexture(GL_TEXTURE_2D,
                  getScoreTexture(scoreTextureId, p1Score, p2Score).textureId);
    glDrawArrays(GL_TRIANGLES, 0, 6);

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

    _model = glm::mat4(1);
    _model = glm::translate(_model, glm::vec3(padding, p1PositionY, 0));
    _model = glm::scale(_model, glm::vec3(barWidth, barHeight, 1.0));
    _model = glm::rotate(_model, glm::radians(-90.0f), glm::vec3(0, 0, 1));
    glUniformMatrix4fv(model, 1, GL_FALSE, glm::value_ptr(_model));

    glBindTexture(GL_TEXTURE_2D, barTextureId[textureIndex]);
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
        _model, glm::vec3(width - barWidth - padding, p2PositionY, 0));
    _model = glm::scale(_model, glm::vec3(barWidth, barHeight, 1.0));
    _model = glm::rotate(_model, glm::radians(90.0f), glm::vec3(0, 0, 1));
    glUniformMatrix4fv(model, 1, GL_FALSE, glm::value_ptr(_model));

    glBindTexture(GL_TEXTURE_2D, barTextureId[textureIndex]);
    glDrawArrays(GL_TRIANGLES, 0, 6);

    // Draw ball
    glBindVertexArray(ballVAO);

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
        (positionX <= padding + barWidth && positionY >= p1PositionY - size &&
         positionY <= p1PositionY + barHeight) {
      translateX *= -1.005;
      translateY *= ((std::rand() % 10) > 5) ? -1.005 : 1.005;
      ballDirection = 1;
    } else if  // hit player 2 bar
        (positionX >= width - padding - barWidth - size &&
         positionY >= p2PositionY - size &&
         positionY <= p2PositionY + barHeight) {
      translateX *= -1.005;
      translateY *= ((std::rand() % 10) > 5) ? -1.005 : 1.005;
      ballDirection = -1;
    }

    positionX += translateX;
    positionY += translateY;

    _model = glm::mat4(1);
    _model = glm::translate(_model, glm::vec3(positionX, positionY, 0));
    _model = glm::scale(_model,
                        glm::vec3(ballWidth * ballDirection, ballHeight, 1.0));
    glUniformMatrix4fv(model, 1, GL_FALSE, glm::value_ptr(_model));

    glBindTexture(GL_TEXTURE_2D, ballTextureId[textureIndex]);
    glDrawArrays(GL_TRIANGLES, 0, 6);

    glBindVertexArray(0);
    glfwSwapBuffers(window);

    textureIndex++;
  }

  // Result loop
  while (!glfwWindowShouldClose(window)) {
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

    // Draw background
    glBindVertexArray(backgroundVAO);

    _model = glm::mat4(1);
    _model = glm::scale(_model, glm::vec3(width, height, 1.0));
    glUniformMatrix4fv(model, 1, GL_FALSE, glm::value_ptr(_model));

    glBindTexture(GL_TEXTURE_2D, backgroundTextureId);
    glDrawArrays(GL_TRIANGLES, 0, 6);

    // Draw field
    glBindVertexArray(fieldVAO);

    _model = glm::mat4(1);
    _model = glm::scale(_model, glm::vec3(width, height, 1.0));
    glUniformMatrix4fv(model, 1, GL_FALSE, glm::value_ptr(_model));

    glBindTexture(GL_TEXTURE_2D, fieldTextureId);
    glDrawArrays(GL_TRIANGLES, 0, 6);

    // Draw score
    glBindVertexArray(scoreVAO);

    _model = glm::mat4(1);

    double scoreWidth = scoreTextureWidth * 0.8,
           scoreHeight = scoreTextureHeight * 0.8,
           scoreX = (width / 2) - (scoreWidth / 2),
           scoreY = (height / 2) - (scoreHeight / 2);

    _model = glm::translate(_model, glm::vec3(scoreX, scoreY, 0));
    _model = glm::scale(_model, glm::vec3(scoreWidth, scoreHeight, 1.0));
    glUniformMatrix4fv(model, 1, GL_FALSE, glm::value_ptr(_model));

    glBindTexture(GL_TEXTURE_2D,
                  getScoreTexture(scoreTextureId, p1Score, p2Score).textureId);
    glDrawArrays(GL_TRIANGLES, 0, 6);

    glBindVertexArray(0);
    glfwSwapBuffers(window);
  }

  glDeleteVertexArrays(1, &backgroundVAO);
  glDeleteVertexArrays(1, &fieldVAO);
  glDeleteVertexArrays(1, &scoreVAO);
  glDeleteVertexArrays(1, &ballVAO);
  glDeleteVertexArrays(1, &p1VAO);
  glDeleteVertexArrays(1, &p2VAO);
  glfwTerminate();

  return 0;
}