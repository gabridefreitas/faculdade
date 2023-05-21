#include <assert.h>

#include <iostream>
#include <string>
#include <thread>

using namespace std;

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "triangle/main.cpp"
#include "move/main.cpp"
#include "pong/main.cpp"
#include "grid/main.cpp"

GLuint INITIAL_WIDTH = 800, INITIAL_HEIGHT = 600;

void key_callback(GLFWwindow* window, int key, int scancode, int action,
                  int mode) {
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    glfwSetWindowShouldClose(window, GL_TRUE);
}

void config() {
  glfwInit();

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
}

void loadGlad() {
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cout << "Failed to initialize GLAD" << std::endl;
  }

  std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;
  std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;
}

int main() {
  config();

  // renderTriangle(INITIAL_WIDTH, INITIAL_HEIGHT, loadGlad, key_callback);

  // renderMove(INITIAL_WIDTH, INITIAL_HEIGHT, loadGlad);

  // renderPong(INITIAL_WIDTH, INITIAL_HEIGHT, loadGlad, key_callback);

  renderGrid(loadGlad, key_callback);

  return 0;
}