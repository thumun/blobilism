#include <iostream>
#include "tinygl-cpp.h"
#include <glm/gtc/matrix_transform.hpp>
#include <cmath>
#include <vector>
using namespace tinygl;
using namespace std;

// can know if mouse clicked by position !!! 
struct CircleButton{

    glm::vec3 * _color; 
    int _x; 
    int _y; 
    int _diameter; 

    CircleButton(glm::vec3 * color, int x, int y, int diameter) { 
      _color = color;
      _x = x; 
      _y = y; 
      _diameter = diameter; 
    }

    ~CircleButton(){ 
      delete _color; 
    };

  };

  struct mouseInput{
    glm::vec3 _currColor; 
    int _brushSize;
    int _clickX; // data type?? 
    int _clickY; // data type ?? 
    float _transparency; // might have to put this elsewhere 
  };

class MyWindow : public Window {
 public:
  MyWindow(int w, int h) : Window(w, h) {
    circs.push_back(new CircleButton(new glm::vec3(0.6, 0, 0.45), 35, 35, 50));
    circs.push_back(new CircleButton(new glm::vec3(1, 0.9, 0.95), 90, 35, 50));
    circs.push_back(new CircleButton(new glm::vec3(1, 0.86, 0.6), 145, 35, 50));
    circs.push_back(new CircleButton(new glm::vec3(0, 0, 0), 200, 35, 50));
    circs.push_back(new CircleButton(new glm::vec3(1, 1, 1), 255, 35, 50));

  }

  void setup() override {
    std::cout << "Window size: " << width() << ", " << height() << std::endl;
  }

  virtual void mouseMotion(int x, int y, int dx, int dy) {
    if (mouseIsDown(GLFW_MOUSE_BUTTON_LEFT)) {
      // todo: store a circle with the current color, size, x, y
    }
  }
  
  virtual void mouseDown(int button, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT) {
      // todo: check if user clicked a color 
      float mx = mouseX();  // current mouse pos x
      float my = mouseY();  // current mouse pos y

      for (CircleButton * i : circs){
        
      }
    }
  }

  void keyDown(int key, int mods) {
    if (key == GLFW_KEY_UP) {
      // increase size of circle
      int sizeNum = 0; 

      
      cout << "Pressed UP: Increase point size to " << sizeNum << endl; 
    } 
    else if (key == GLFW_KEY_DOWN) {
      // decrease size of circle
      int sizeNum = 0; 

      cout << "Pressed DOWN: Decrease point size to " << sizeNum << endl;
    }
    else if (key == GLFW_KEY_LEFT) {
      // decrease alpha
      int sizeNum = 0; 

      cout << "Pressed LEFT: Increase point size to " << sizeNum << endl;
    }
    else if (key == GLFW_KEY_RIGHT) {
      // increase alpha
      int sizeNum = 0; 

      cout << "Pressed RIGHT: Decrease point size to " << sizeNum << endl;
    }
    else if (key == GLFW_KEY_C) {
      // clear vector of circles
    }
  }

  void draw() override {
    background(0.95f, 0.95f, 0.95f); // parameters: r, g, b

    // color(1.0f, 0.5f, 0.5f);
    // circle(width() * 0.5f, height() * 0.5, 300);

    // todo : draw pallet
    color(0.1f, 0.1f, 0.1f);
    square(width()/2.0f, 35, width(), 70);

    for (CircleButton * i : circs){
      color(i->_color->x, i->_color->y, i->_color->z);
      circle(i->_x, i->_y, i->_diameter);
    }
  }

  ~MyWindow(){ 
    for (CircleButton * i : circs){
      delete i; 
    }
  };
 private:

  // todo: create member variables for 
  // current circle size
  // current transparency
  // current color
  // list of circles to draw each frame

  // gotta destroy the objects 
  vector<CircleButton *> circs;

  // color pallet
};

int main() {
  MyWindow window(500, 500);
  window.run();
}
