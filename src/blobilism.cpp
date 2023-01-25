#include <iostream>
#include "tinygl-cpp.h"
#include <glm/gtc/matrix_transform.hpp>
#include <cmath>
#include <vector>
using namespace tinygl;
using namespace std;

struct Triple{
  int _x; 
  int _y; 
  int _diameter; 
};

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
      if (_color != NULL){
        delete _color; 
      }
    }

  };

  struct MouseInput{
    glm::vec3 * _currColor = NULL; 
    int _brushSize;
    float _posX; 
    float _posY; 
    float _transparency; // might have to put this elsewhere 

    MouseInput(int brushSize, float x, float y, float transparency){
      _brushSize = brushSize;
      _posX = x;
      _posY = y; 
      _transparency = transparency; 
    }

    void setColor(glm::vec3 * color){
      if (_currColor != NULL){
        delete _currColor; 
      } 
      _currColor = new glm::vec3 (color->x, color->y, color->z);
    }

    void increaseBrushSize(){
      _brushSize += 5; 
    }

    void decreaseBrushSize(){
      if (_brushSize != 0){
        _brushSize -= 5; 
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
      if (_currColor != NULL){
        delete _currColor;
      }
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
      if (mouseInput->_currColor == NULL){
        cout << "color not selected" << std::endl;
      }
      else{
        mouseInput->_posX = x; 
        mouseInput->_posY = y; 
        strokes.push_back({(int)mouseInput->_posX, (int)mouseInput->_posY, (int)mouseInput->_brushSize});
        
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

        // cout << i->_x << "," << i->_y << endl; 
        // cout << mx << "," << my << endl; 

        clickPt = sqrt(pow((mx - i->_x), 2) + pow((my - i->_y), 2)); 
        if(clickPt <= (i->_diameter/2)){
          mouseInput->setColor(i->_color);
          cout << "Setting color to " << mouseInput->_currColor->x << " " << mouseInput->_currColor->y << " " << mouseInput->_currColor->z << endl;
          return;
        }
      }
    }
  }

  void keyDown(int key, int mods) {
    if (key == GLFW_KEY_UP) {
      // increase size of circle
      mouseInput->increaseBrushSize(); 
      
      cout << "Pressed UP: Increase point size to " << mouseInput->_brushSize << endl; 
    } 
    else if (key == GLFW_KEY_DOWN) {
      // decrease size of circle
      mouseInput->decreaseBrushSize(); 

      cout << "Pressed DOWN: Decrease point size to " << mouseInput->_brushSize << endl;
    }
    else if (key == GLFW_KEY_LEFT) {
      // decrease alpha
      mouseInput->decreaseTransparency(); 

      cout << "Pressed LEFT: Decrease transparency to " << mouseInput->_transparency << endl;
    }
    else if (key == GLFW_KEY_RIGHT) {
      // increase alpha
      mouseInput->increaseTransparency(); 

      cout << "Pressed RIGHT: Increase transparency to " << mouseInput->_transparency << endl;
    }
    else if (key == GLFW_KEY_C) {
      // clear vector of circles

      // for (Triple i : strokes){

      // }

      cout << "Clear screen" << endl; 
    }
  }

  void draw() override {
    background(0.95f, 0.95f, 0.95f); // parameters: r, g, b

    if (strokes.size() != 0){
      for (Triple i : strokes){
        color(mouseInput->_currColor->x, mouseInput->_currColor->y, mouseInput->_currColor->z, mouseInput->_transparency);
        circle(i._x, i._y, i._diameter);
      }
    }
    

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

    if (mouseInput != NULL){
      delete mouseInput;
    }
  }
 private:

  // todo: create member variables for 
  // current circle size
  // current transparency
  // current color

  // list of circles to draw each frame
  // https://www.geeksforgeeks.org/store-data-triplet-vector-c/ --> used to figure out how to store a triple 
  vector<Triple> strokes; 

  // ARE THESE UNDERSCORES W/ NAMES ?? 
  // mouse inputs/data 
  MouseInput * mouseInput = NULL; 

   // color pallet
  vector<CircleButton *> circs;
  
};

int main() {
  MyWindow window(500, 500);
  window.run();
}
