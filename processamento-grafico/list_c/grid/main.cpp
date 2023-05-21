#include <assert.h>

#include <iostream>
#include <string>
#include <random>
#include <ctime>

using namespace std;

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <shader/shader.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

GLuint INITIAL_GRID_WIDTH = 600, INITIAL_GRID_HEIGHT = 600;

GLuint drawSquare() {
  GLfloat VERTICES[] = {
      // first triangle
      0.0, 0.0, 0.0,  // v0
      0.0, 1.0, 0.0,  // v1
      1.0, 1.0, 0.0,  // v2
      // second triangle
      0.0, 0.0, 0.0,  // v0
      1.0, 1.0, 0.0,  // v2
      1.0, 0.0, 0.0,  // v3
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

int renderGrid(void (*loadGlad)(),
               void (*key_callback)(GLFWwindow *window, int key, int scancode,
                                    int action, int mode)) {
  GLFWwindow *window = glfwCreateWindow(INITIAL_GRID_WIDTH, INITIAL_GRID_HEIGHT,
                                        "Render Grid", nullptr, nullptr);

  glfwMakeContextCurrent(window);

  glfwSetKeyCallback(window, key_callback);

  loadGlad();

  Shader shader("list_c/grid/vertex-shader.vs",
                "list_c/grid/fragment-shader.fs");

  GLuint VAO = drawSquare();

  GLint projection = glGetUniformLocation(shader.ID, "projection");
  GLint model = glGetUniformLocation(shader.ID, "model");
  GLint color = glGetUniformLocation(shader.ID, "color");

  glUseProgram(shader.ID);

  while (!glfwWindowShouldClose(window)) {
    glfwPollEvents();

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    int width, height;

    glfwGetFramebufferSize(window, &width, &height);

    glm::mat4 _projection = glm::mat4(1);
    _projection =
        glm::ortho(0.0f, (float)width, (float)height, 0.0f, -1.0f, 1.0f);

    glUniformMatrix4fv(projection, 1, GL_FALSE, glm::value_ptr(_projection));

    glBindVertexArray(VAO);

    float squareSize = width / 6;

    std::mt19937 rng(std::time(nullptr));
    std::uniform_real_distribution<float> dist(0.0f, 1.0f);

    for (int i = 0, j = 0; i <= 6; i++) {
      if (i > 0 && i % 6 == 0) {
        i = 0;
        j++;

        if (j > 6) {
          break;
        }
      }

      glm::mat4 _model = glm::mat4(1);

      _model = glm::scale(_model, glm::vec3(squareSize, squareSize, 1.0));
      _model = glm::translate(_model, glm::vec3(i * 1.0, j * 1.0, 0));

      glUniformMatrix4fv(model, 1, GL_FALSE, glm::value_ptr(_model));

      glUniform3f(color, dist(rng), dist(rng), dist(rng));

      glDrawArrays(GL_TRIANGLES, 0, 6);
    }

    glViewport(0, 0, width, height);

    glBindVertexArray(0);
    glfwSwapBuffers(window);
  }

  glDeleteVertexArrays(1, &VAO);
  glfwTerminate();

  return 0;
}