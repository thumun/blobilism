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

  struct MouseInput{
    glm::vec3 * _currColor; 
    int _brushSize;
    float _clickX; 
    float _clickY; 
    float _transparency; // might have to put this elsewhere 

    MouseInput(int brushSize, float x, float y, float transparency){
      _brushSize = brushSize;
      _clickX = x;
      _clickY = y; 
      _transparency = _transparency; 
    }

    void setColor(glm::vec3 * color){
      _currColor = color;
    }

    void increaseBrushSize(){
      _brushSize += 0.05; 
    }

    void decreaseBrushSize(){
      if (_brushSize != 0){
        _brushSize -= 0.05; 
      }
    }

    void increaseTransparency() {
      if (_transparency != 1.0){ 
        _transparency += 0.05; 
      }
    }

    void decreaseTransparency() {
      if (_transparency != 0){
        _transparency -= 0.05; 
      }
    }

    ~MouseInput(){ 
      delete _currColor;
    }
  };

class MyWindow : public Window {
 public:
  MyWindow(int w, int h) : Window(w, h) {

    // probably want to change this ?? --- current: brush size: 50px, coords(0, 0) will set when in motion, 100% opacity 
    // color = null (should code that can't draw anything unless color selected)
    mouseInput = new MouseInput(50, 0, 0, 1);

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

      // if color not selected
      if (mouseInput->color == NULL){
        cout >> "color not selected" << std::endl;
      }
      else{
        // draw info here !!! 
        // put "strokes" into a list ?? so easier to clear 
      }
    }
  }
  
  virtual void mouseDown(int button, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT) {
      // todo: check if user clicked a color 
      float mx = mouseX();  // current mouse pos x
      float my = mouseY();  // current mouse pos y

      float clickPt; 

      for (CircleButton * i : circs){
        clickPt = sqrt(pow((my - i->_x), 2) + pow((mx - i->_y), 2)); 
        if((i->_diameter/2) >= clickPt){
          
        }
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
    delete mouseInput;
  };
 private:

  // todo: create member variables for 
  // current circle size
  // current transparency
  // current color
  // list of circles to draw each frame


  // ARE THESE UNDERSCORES W/ NAMES ?? 
  // mouse inputs/data 
  MouseInput * mouseInput; 

   // color pallet
  vector<CircleButton *> circs;
  
 
};

int main() {
  MyWindow window(500, 500);
  window.run();
}
