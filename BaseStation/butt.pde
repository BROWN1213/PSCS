Button at_button;  // the button
Button atd_button;  // the button
Button auto_button;  // the button
Button manu_button;  // the button

Button ready_button;  // the button
Button hook_button;  // the button
Button release_button;  // the button
Button left0_button;  // the button
Button left1_button;  // the button
Button center_button;  // the button
Button right0_button;  // the button
Button right1_button;  // the button

void buttonSetup(){
  // create the button object
  at_button = new Button("AT", 20, 20, 40, 40);  
  atd_button = new Button("CNT", 20, 70, 40, 40);
  auto_button = new Button("AUTO", 170, 75, 70, 30); 
  manu_button = new Button("MANUAL", 250, 75, 70, 30); 
  ready_button = new Button("RDY", 70, 20, 40, 25);  
  hook_button = new Button("HOOK", 70, 52, 40, 25);  
  release_button = new Button("RLZ", 70, 84, 40, 25);
  left0_button = new Button("<<", 170, 20, 30, 50);
  left1_button = new Button("<", 205, 20, 30, 50);
  center_button = new Button("O", 240, 20, 30, 50);
  right1_button = new Button(">", 275, 20, 30, 50);  
  right0_button = new Button(">>", 310, 20, 30, 50);

}
void buttonDraw(){
  // draw the button in the window
  at_button.Draw();  
  atd_button.Draw(); 
  auto_button.Draw(); 
  manu_button.Draw(); 
  ready_button.Draw(); 
  hook_button.Draw(); 
  release_button.Draw();
  left0_button.Draw();
  left1_button.Draw();
  center_button.Draw();
  right1_button.Draw();
  right0_button.Draw();  
}
// mouse button clicked
void mousePressed()
{
  if (at_button.MouseIsOver()) {
    // print some text to the console pane if the button is clicked
    String cmd="AT\r\n";
    myPort.write(cmd);
  }
  if (atd_button.MouseIsOver()) {
    // print some text to the console pane if the button is clicked
    String cmd="ATD\r\n";
    myPort.write(cmd);
  }
  if (auto_button.MouseIsOver()) {
    // print some text to the console pane if the button is clicked
    String cmd="4,0;\r\n";
    myPort.write(cmd);
  }  
  if (manu_button.MouseIsOver()) {
    // print some text to the console pane if the button is clicked
    String cmd="4,1;\r\n";
    myPort.write(cmd);
  }   
  if (ready_button.MouseIsOver()) {
    // print some text to the console pane if the button is clicked
    String cmd="2,0;\r\n";
    myPort.write(cmd);
  }   
  if (hook_button.MouseIsOver()) {
    // print some text to the console pane if the button is clicked
    String cmd="2,1;\r\n";
    myPort.write(cmd);
  }   
  if (release_button.MouseIsOver()) {
    // print some text to the console pane if the button is clicked
    String cmd="2,2;\r\n";
    myPort.write(cmd);
  }    
  if (left0_button.MouseIsOver()) {
    // print some text to the console pane if the button is clicked
    String cmd="5,-2;\r\n";
    myPort.write(cmd);
  } 
  if (left1_button.MouseIsOver()) {
    // print some text to the console pane if the button is clicked
    String cmd="5,-1;\r\n";
    myPort.write(cmd);
  } 
  if (center_button.MouseIsOver()) {
    // print some text to the console pane if the button is clicked
    String cmd="5,0;\r\n";
    myPort.write(cmd);
  } 
  if (right1_button.MouseIsOver()) {
    // print some text to the console pane if the button is clicked
    String cmd="5,1;\r\n";
    myPort.write(cmd);
  } 
  if (right0_button.MouseIsOver()) {
    // print some text to the console pane if the button is clicked
    String cmd="5,2;\r\n";
    myPort.write(cmd);
  }   
}

// the Button class
class Button {
  String label; // button label
  float x;      // top left corner x position
  float y;      // top left corner y position
  float w;      // width of button
  float h;      // height of button
  
  // constructor
  Button(String labelB, float xpos, float ypos, float widthB, float heightB) {
    label = labelB;
    x = xpos;
    y = ypos;
    w = widthB;
    h = heightB;
  }
  
  void Draw() {
    fill(218);
    stroke(145);
    rect(x, y, w, h, 8);
    textAlign(CENTER, CENTER);
    fill(0);
    text(label, x + (w / 2), y + (h / 2));
  }
  
  boolean MouseIsOver() {
    if (mouseX > x && mouseX < (x + w) && mouseY > y && mouseY < (y + h)) {
      return true;
    }
    return false;
  }
}