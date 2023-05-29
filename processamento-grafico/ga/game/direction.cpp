
#include <GLFW/glfw3.h>

enum Direction { UP, DOWN, STOP };

static Direction p1Direction = STOP;
static Direction p2Direction = STOP;

void gameKeyCallback(GLFWwindow *window, int key, int scancode, int action,
                     int mode) {
  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
    p1Direction = UP;
  } else if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
    p2Direction = UP;
  } else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
    p1Direction = DOWN;
  } else if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
    p2Direction = DOWN;
  } else {
    p1Direction = STOP;
    p2Direction = STOP;
  }
}