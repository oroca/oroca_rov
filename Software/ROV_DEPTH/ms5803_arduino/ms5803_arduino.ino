#include <Wire.h>
#include "ms5803.h"

void setup() {
  Serial.begin(115200);
  while(!Serial);
  Wire.begin();
  ms5803_init();
}

void loop() {

  ms5803_depth(FRESH_WATER,true);
  
  //  display_onlydepth();  
}

void display_onlydepth(){
  Serial.print("Depth : ");
  Serial.print(ms5803_depth(FRESH_WATER,false));
  Serial.println(" m");
}

