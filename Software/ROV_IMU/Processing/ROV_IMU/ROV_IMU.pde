import processing.serial.*;

Serial myPort;  // Create object from Serial class



float imu_x;
float imu_y;
float imu_z;

String  inString;  

final int VIEW_SIZE_X = 600, VIEW_SIZE_Y = 600;



void setup() 
{
  size(VIEW_SIZE_X, VIEW_SIZE_Y, P3D);

  println(Serial.list());                      //포트 리스트 출력
  myPort = new Serial(this, "/dev/tty.usbmodemfa131", 115200);  
  myPort.clear();
  myPort.bufferUntil(10);
}




void buildBoxShape() 
{
  //box(60, 10, 40);
  noStroke();
  beginShape(QUADS);
  
  //Z+ (to the drawing area)
  fill(#00ff00);
  vertex(-30, -5, 20);
  vertex(30, -5, 20);
  vertex(30, 5, 20);
  vertex(-30, 5, 20);
  
  //Z-
  fill(#0000ff);
  vertex(-30, -5, -20);
  vertex(30, -5, -20);
  vertex(30, 5, -20);
  vertex(-30, 5, -20);
  
  //X-
  fill(#ff0000);
  vertex(-30, -5, -20);
  vertex(-30, -5, 20);
  vertex(-30, 5, 20);
  vertex(-30, 5, -20);
  
  //X+
  fill(#ffff00);
  vertex(30, -5, -20);
  vertex(30, -5, 20);
  vertex(30, 5, 20);
  vertex(30, 5, -20);
  
  //Y-
  fill(#ff00ff);
  vertex(-30, -5, -20);
  vertex(30, -5, -20);
  vertex(30, -5, 20);
  vertex(-30, -5, 20);
  
  //Y+
  fill(#00ffff);
  vertex(-30, 5, -20);
  vertex(30, 5, -20);
  vertex(30, 5, 20);
  vertex(-30, 5, 20);
  
  endShape();
}


void drawCube() {  
  pushMatrix();
    translate(300, 300, 0);
    scale(4,4,4);
    
    rotateY( -imu_z * PI / 180);
    rotateX( imu_x * PI / 180);
    rotateZ( -imu_y * PI / 180);
    

    buildBoxShape();
    
  popMatrix();
}



void draw() {  
  
  background(#000000);
  fill(#ffffff);

  
  drawCube();
}


//데이터 받아오기
void serialEvent(Serial p) {

  inString = (myPort.readString());
  
  try {
   
    String[] dataStrings = split(inString, ' ');
    imu_x = float(dataStrings[1]);
    imu_y = float(dataStrings[2]);
    imu_z = float(dataStrings[3]);
       
    println( imu_x + " " + imu_y + " " + imu_z );
  } catch (Exception e) {
      println("Caught Exception");
  }
}

