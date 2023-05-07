#include <assert.h>

#include <iostream>
#include <string>
#include <thread>

using namespace std;

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "two-triangles/main.cpp"
#include "two-triangles-all/main.cpp"
#include "two-triangles-line/main.cpp"
#include "two-triangles-point/main.cpp"

GLuint INITIAL_WIDTH = 720, INITIAL_HEIGHT = 405;

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

void setWindowSize(GLFWwindow* window) {
  int width, height;

  glfwGetFramebufferSize(window, &width, &height);

  glViewport(0, 0, width, height);
}

int main() {
  config();

  renderTwoTriangles(INITIAL_WIDTH, INITIAL_HEIGHT, loadGlad, key_callback,
                     setWindowSize);

  // renderTwoTrianglesLine(INITIAL_WIDTH, INITIAL_HEIGHT, loadGlad,
  // key_callback,
  //                        setWindowSize);

  // renderTwoTrianglesPoint(INITIAL_WIDTH, INITIAL_HEIGHT, loadGlad,
  // key_callback,
  //                         setWindowSize);

  // renderTwoTrianglesAll(INITIAL_WIDTH, INITIAL_HEIGHT, loadGlad,
  // key_callback,
  //                         setWindowSize);

  return 0;
}