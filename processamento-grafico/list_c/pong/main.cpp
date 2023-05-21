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

GLuint drawTrianglePong() {
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

int renderPong(GLuint init_width, GLuint init_height, void (*loadGlad)(),
               void (*key_callback)(GLFWwindow *window, int key, int scancode,
                                    int action, int mode)) {
  GLFWwindow *window = glfwCreateWindow(init_width, init_height, "Render Pong",
                                        nullptr, nullptr);

  glfwMakeContextCurrent(window);

  glfwSetKeyCallback(window, key_callback);

  loadGlad();

  Shader shader("list_c/pong/vertex-shader.vs",
                "list_c/pong/fragment-shader.fs");

  GLuint VAO = drawTrianglePong();

  GLint projection = glGetUniformLocation(shader.ID, "projection");
  GLint model = glGetUniformLocation(shader.ID, "model");

  glUseProgram(shader.ID);

  float positionX = init_width / 2, positionY = init_height / 2, size = 50.0f,
        speed = 3.0f, translateX = speed, translateY = speed;

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

    if (positionX <= 0 || positionX >= width - size) {
      translateX = translateX * -1;
    }

    if (positionY <= 0 || positionY >= height - size) {
      translateY = translateY * -1;
    }

    positionX += translateX;
    positionY += translateY;

    glm::mat4 _model = glm::mat4(1);
    _model = glm::translate(_model, glm::vec3(positionX, positionY, 0));
    glUniformMatrix4fv(model, 1, GL_FALSE, glm::value_ptr(_model));

    glDrawArrays(GL_TRIANGLES, 0, 3);

    glViewport(0, 0, width, height);

    glBindVertexArray(0);
    glfwSwapBuffers(window);
  }

  glDeleteVertexArrays(1, &VAO);
  glfwTerminate();

  return 0;
}