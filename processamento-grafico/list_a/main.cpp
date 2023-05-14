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

#include "two-triangles/main.cpp"
#include "two-triangles-line/main.cpp"
#include "two-triangles-point/main.cpp"
#include "two-triangles-combined/main.cpp"
#include "circle/main.cpp"
#include "octagon/main.cpp"
#include "pentagon/main.cpp"
#include "pacman/main.cpp"
#include "pizza/main.cpp"
#include "star/main.cpp"
#include "spiral/main.cpp"

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

void setWindowSize(GLFWwindow* window, GLuint shaderID) {
  int width, height;

  glfwGetFramebufferSize(window, &width, &height);

  glViewport(0, 0, width, height);

  if (width >= height) {
    GLfloat ratio = width / (GLfloat)height;
    glUniform1f(glGetUniformLocation(shaderID, "ratio"), ratio);
  } else {
    GLfloat ratio = height / (GLfloat)width;
    glUniform1f(glGetUniformLocation(shaderID, "ratio"), ratio);
  }
}

int main() {
  config();

  // renderTwoTriangles(INITIAL_WIDTH, INITIAL_HEIGHT, loadGlad, key_callback,
  //                    setWindowSize);

  // renderTwoTrianglesLine(INITIAL_WIDTH, INITIAL_HEIGHT, loadGlad,
  // key_callback,
  //                        setWindowSize);

  // renderTwoTrianglesPoint(INITIAL_WIDTH, INITIAL_HEIGHT, loadGlad,
  // key_callback,
  //                         setWindowSize);

  // renderTwoTrianglesCombined(INITIAL_WIDTH, INITIAL_HEIGHT, loadGlad,
  //                            key_callback, setWindowSize);

  // renderCircle(INITIAL_WIDTH, INITIAL_HEIGHT, loadGlad, key_callback,
  //              setWindowSize);

  // renderOctagon(INITIAL_WIDTH, INITIAL_HEIGHT, loadGlad, key_callback,
  //               setWindowSize);

  // renderPentagon(INITIAL_WIDTH, INITIAL_HEIGHT, loadGlad, key_callback,
  //                setWindowSize);

  // renderPacman(INITIAL_WIDTH, INITIAL_HEIGHT, loadGlad, key_callback,
  //              setWindowSize);

  // renderPizzaSlice(INITIAL_WIDTH, INITIAL_HEIGHT, loadGlad, key_callback,
  //                  setWindowSize);

  // renderStar(INITIAL_WIDTH, INITIAL_HEIGHT, loadGlad, key_callback,
  //            setWindowSize);

  renderSpiral(INITIAL_WIDTH, INITIAL_HEIGHT, loadGlad, key_callback,
               setWindowSize);

  return 0;
}