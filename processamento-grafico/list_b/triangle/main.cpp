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
    -0.5 * 100 + 400, -0.5 * 100 + 300, 0.0,  // v0
    0.5 * 100 + 400,  -0.5 * 100 + 300, 0.0,  // v1
    0.0 * 100 + 400,  0.5 * 100 + 300,  0.0,  // v2
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
                                        "Render Triangle", nullptr, nullptr);

  glfwMakeContextCurrent(window);

  glfwSetKeyCallback(window, key_callback);

  loadGlad();

  Shader shader("list_b/triangle/vertex-shader.vs",
                "list_b/triangle/fragment-shader.fs");

  GLuint VAO = drawTriangle();

  GLint projection = glGetUniformLocation(shader.ID, "projection");

  glUseProgram(shader.ID);

  glm::mat4 _projection = glm::mat4(1);

  /**
   * 1 - Modifique a janela do mundo (window/ortho) para os limites:
   * xmin=-10, xmax=10, ymin=-10, ymax=10
   */
  // _projection = glm::ortho(-10.0, 10.0, -10.0, 10.0, -1.0, 1.0);

  /**
   * 2 - Agora modifique para:
   * xmin=0, xmax=800, ymin=600, ymax=0
   */
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

    /**
     * 4 - Modifique o viewport para desenhar a
     * cena apenas no seguinte quadrante da janela da aplicação
     */
    // glViewport(400, 300, width / 2, height / 2);
    // glDrawArrays(GL_TRIANGLES, 0, 3);

    /**
     * 5 - Agora, desenhe a mesma cena nos 4 quadrantes
     */
    for (int i = 0; i < 4; i++) {
      int leftLowerX, leftLowerY;

      switch (i) {
        case 0:
          leftLowerX = 0;
          leftLowerY = 0;
          break;
        case 1:
          leftLowerX = 0;
          leftLowerY = 300;
          break;
        case 2:
          leftLowerX = 400;
          leftLowerY = 0;
          break;
        case 3:
          leftLowerX = 400;
          leftLowerY = 300;
          break;
      }

      glViewport(leftLowerX, leftLowerY, width / 2, height / 2);
      glDrawArrays(GL_TRIANGLES, 0, 3);
    }

    glBindVertexArray(0);

    glfwSwapBuffers(window);
  }

  glDeleteVertexArrays(1, &VAO);
  glfwTerminate();

  return 0;
}