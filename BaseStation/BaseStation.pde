// Processing application that demonstrates the Button class by creating a button
// Draws a square in the window when the mouse cursor is over the button
// Writes to the Processing IDE console pane when the button is clicked
// 3 July 2015    http://startingelectronics.org


void setup() {
  size (400, 150);
  smooth();
  buttonSetup();
  serialSetup();

}

void draw() {

    // hide the square if the mouse cursor is not over the button
  background(0);
  
  buttonDraw();
  serialReceive();
}