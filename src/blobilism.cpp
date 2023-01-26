/**
 * This is the main file for Assignment 01: Blobism.
 *
 * This program is a mini drawing program. It allows the user to draw with a circle 
 * shaped brush that they can change the size of using the UP/DOWN arrow keys. They can 
 * also change the opacity of their strokes with the LEFT/RIGHT arrow keys. 
 * They can pick from one of 5 colors to draw with- and pick a color by clicking on it 
 * with the left mouse button. 
 * The canvas can be cleared with the c key. 
 *
 *
 * @author: Neha Thumu
 * @version: January 26, 2023
 */

#include <iostream>
#include "tinygl-cpp.h"
#include <glm/gtc/matrix_transform.hpp>
#include <cmath>
#include <vector>
using namespace tinygl;
using namespace std;

// holds the information for drawn strokes 
struct StrokeSettings
{
  int _x;
  int _y;
  int _diameter;
  float _transparency;
  glm::vec3 *_color;

  StrokeSettings(int x, int y, int diameter, float transparency, glm::vec3 *color)
  {
    _x = x;
    _y = y;
    _diameter = diameter;
    _transparency = transparency;
    _color = color;
  }

  ~StrokeSettings()
  {
    if (_color != NULL)
    {
      delete _color;
    }
  }
};

// the data related to the color palette ('buttons')
struct CircleButton
{
  glm::vec3 *_color;
  int _x;
  int _y;
  int _diameter;

  CircleButton(glm::vec3 *color, int x, int y, int diameter)
  {
    _color = color;
    _x = x;
    _y = y;
    _diameter = diameter;
  }

  ~CircleButton()
  {
    if (_color != NULL)
    {
      delete _color;
    }
  }
};

// has the data for current color, brush settings, mouse position 
// has methods to access/set these variables 
struct MouseInput
{
  glm::vec3 *_currColor = NULL;
  int _brushSize;
  float _posX;
  float _posY;
  float _transparency; 

  MouseInput(int brushSize, float x, float y, float transparency)
  {
    _brushSize = brushSize;
    _posX = x;
    _posY = y;
    _transparency = transparency;
  }

  /** * first deletes existing color and sets a new color 
   * (accessed when color in palette selected)
   * @param color the new color (to be set)
  */
  void setColor(glm::vec3 *color)
  {
    if (_currColor != NULL)
    {
      delete _currColor;
    }
    _currColor = new glm::vec3(color->x, color->y, color->z);
  }

  /* increases the size of the brush (called when UP pressed) */
  void increaseBrushSize()
  {
    _brushSize += 5;
  }


  /** * decreases the size of the brush (called when DOWN pressed) 
   * will not go to size 0 or below 
  */
  void decreaseBrushSize()
  {
    if (_brushSize != 0)
    {
      _brushSize -= 5;
    }
  }

  /** * increases opacity of the stroke (called when RIGHT pressed) 
   * will not above 1 
  */
  void increaseTransparency()
  {
    if (_transparency != 1.0)
    {
      _transparency += 0.05;
    }
  }

  /** * decreases opacity of the stroke (called when LEFT pressed) 
   * will not below 0 
  */
  void decreaseTransparency()
  {
    if (_transparency != 0)
    {
      _transparency -= 0.05;
    }
  }

  ~MouseInput()
  {
    if (_currColor != NULL)
    {
      delete _currColor;
    }
  }
};

class MyWindow : public Window
{
public:
  MyWindow(int w, int h) : Window(w, h)
  {

    // initializng mouse input as "empty" values
    _mouseInput = new MouseInput(50, 0, 0, 1);

    // initializing the color palette 
    _circs.push_back(new CircleButton(new glm::vec3(0.6, 0, 0.45), 35, 35, 50));
    _circs.push_back(new CircleButton(new glm::vec3(1, 0.9, 0.95), 90, 35, 50));
    _circs.push_back(new CircleButton(new glm::vec3(1, 0.86, 0.6), 145, 35, 50));
    _circs.push_back(new CircleButton(new glm::vec3(0, 0, 0), 200, 35, 50));
    _circs.push_back(new CircleButton(new glm::vec3(1, 1, 1), 255, 35, 50));
  }

  void setup() override
  {
    std::cout << "Window size: " << width() << ", " << height() << std::endl;
  }

  /** * Draws a stroke (series of circles) when LEFT mouse btn pressed and in motion 
   * Will not draw on top of the panel for the color palette 
   * (param info in tinygl-cpp.h)
  */
  virtual void mouseMotion(int x, int y, int dx, int dy) override
  {
    if (mouseIsDown(GLFW_MOUSE_BUTTON_LEFT))
    {
      // if color not selected yet 
      if (_mouseInput->_currColor == NULL)
      {
        cout << "color not selected" << std::endl;
      }
      else
      {
        _mouseInput->_posX = x;
        _mouseInput->_posY = y;
        // adds stroke as an object stored in stroked vector to be drawn 
        _strokes.push_back(new StrokeSettings(_mouseInput->_posX, _mouseInput->_posY,
                                             _mouseInput->_brushSize, _mouseInput->_transparency, 
                                             new glm::vec3(_mouseInput->_currColor->x, 
                                             _mouseInput->_currColor->y, _mouseInput->_currColor->z)));
      }
    }
  }

  /** * When mouse LEFT btn clicked on a color in the palette, 
   * will save that color (to be used as stroke/drawing color)
   * (param info in tinygl-cpp.h)
  */
  virtual void mouseDown(int button, int mods) override
  {
    if (button == GLFW_MOUSE_BUTTON_LEFT)
    {
      float mx = mouseX(); // current mouse pos x
      float my = mouseY(); // current mouse pos y

      float clickPt;

      // process for finding which color was selected 
      // cycles through the color options to see if mouse was pressed on a color option 
      for (CircleButton *i : _circs)
      {
        clickPt = sqrt(pow((mx - i->_x), 2) + pow((my - i->_y), 2));
        if (clickPt <= (i->_diameter / 2))
        {
          _mouseInput->setColor(i->_color);
          cout << "Setting color to " << _mouseInput->_currColor->x 
            << " " << _mouseInput->_currColor->y << " " << _mouseInput->_currColor->z << endl;
          return;
        }
      }
    }
  }

  /** Operations for when certain keys is pressed 
   * (param info in tinygl-cpp.h)
  */
  void keyDown(int key, int mods) override
  {
    // increase size of brush 
    if (key == GLFW_KEY_UP)
    {
      _mouseInput->increaseBrushSize();
      cout << "Pressed UP: Increase point size to " << _mouseInput->_brushSize << endl;
    }
    // decrease size of brush
    else if (key == GLFW_KEY_DOWN)
    {
      _mouseInput->decreaseBrushSize();
      cout << "Pressed DOWN: Decrease point size to " << _mouseInput->_brushSize << endl;
    }
    // decrease opacity 
    else if (key == GLFW_KEY_LEFT)
    {
      _mouseInput->decreaseTransparency();
      cout << "Pressed LEFT: Decrease transparency to " << _mouseInput->_transparency << endl;
    }
    // increase opacity 
    else if (key == GLFW_KEY_RIGHT)
    {
      _mouseInput->increaseTransparency();
      cout << "Pressed RIGHT: Increase transparency to " << _mouseInput->_transparency << endl;
    }
    // clear all the drawings on the canvas 
    else if (key == GLFW_KEY_C)
    {
      _strokes.clear();
      cout << "Clear screen" << endl;
    }
  }

  /** * Draws background (canvas), panel for palette, palette color options, and drawings made 
   * (param info in tinygl-cpp.h)
  */
  void draw() override
  {
    background(0.95f, 0.95f, 0.95f); // parameters: r, g, b

    // draws all the strokes made 
    if (_strokes.size() != 0)
    {
      for (StrokeSettings *i : _strokes)
      {
        color(i->_color->x, i->_color->y, i->_color->z, i->_transparency);
        circle(i->_x, i->_y, i->_diameter);
      }
    }

    // panel for color palette 
    color(0.1f, 0.1f, 0.1f);
    square(width() / 2.0f, 35, width(), 70);

    // draws all the color options
    for (CircleButton *i : _circs)
    {
      color(i->_color->x, i->_color->y, i->_color->z);
      circle(i->_x, i->_y, i->_diameter);
    }
  }

  ~MyWindow()
  {
    for (CircleButton *i : _circs)
    {
      delete i;
    }

    if (_mouseInput != NULL)
    {
      delete _mouseInput;
    }
  }

private:
  // list of circles to draw each frame
  vector<StrokeSettings *> _strokes;

  // mouse inputs/data
  MouseInput * _mouseInput = NULL;

  // color pallet
  vector<CircleButton *> _circs;
};

int main()
{
  MyWindow window(500, 500);
  window.run();
}
