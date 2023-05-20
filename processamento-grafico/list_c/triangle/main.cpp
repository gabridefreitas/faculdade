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

GLfloat VERTICES[] = {
    50.0,  100.0, 0.0,  // v0
    100.0, 100.0, 0.0,  // v1
    75.0,  50.0,  0.0,  // v2
};

GLuint drawTriangle() {
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

int renderTriangle(GLuint init_width, GLuint init_height, void (*loadGlad)(),
                   void (*key_callback)(GLFWwindow *window, int key,
                                        int scancode, int action, int mode)) {
  GLFWwindow *window = glfwCreateWindow(init_width, init_height,
                                        "Render 3 Triangles - Transformations",
                                        nullptr, nullptr);

  glfwMakeContextCurrent(window);

  glfwSetKeyCallback(window, key_callback);

  loadGlad();

  Shader shader("list_c/triangle/vertex-shader.vs",
                "list_c/triangle/fragment-shader.fs");

  GLuint VAO = drawTriangle();

  GLint projection = glGetUniformLocation(shader.ID, "projection");
  GLint model = glGetUniformLocation(shader.ID, "model");

  glUseProgram(shader.ID);

  glm::mat4 _projection = glm::mat4(1);
  _projection = glm::ortho(0.0, 800.0, 600.0, 0.0, -1.0, 1.0);

  glUniformMatrix4fv(projection, 1, GL_FALSE, glm::value_ptr(_projection));

  while (!glfwWindowShouldClose(window)) {
    glfwPollEvents();

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    int width, height;

    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0, 0, width, height);

    glBindVertexArray(VAO);

    for (int i = 1; i < 4; i++) {
      glm::mat4 _model = glm::mat4(1);

      _model = glm::scale(_model, glm::vec3(i * 1.0, i * 1.0, 1.0));
      _model = glm::translate(_model, glm::vec3(i * 1.0, i * 1.0, 0));
      _model = glm::rotate(_model, glm::radians(i * -5.0f),
                           glm::vec3(0.0f, 0.0f, 1.0f));

      glUniformMatrix4fv(model, 1, GL_FALSE, glm::value_ptr(_model));

      glDrawArrays(GL_TRIANGLES, 0, 3);
    }

    glBindVertexArray(0);

    glfwSwapBuffers(window);
  }

  glDeleteVertexArrays(1, &VAO);
  glfwTerminate();

  return 0;
}