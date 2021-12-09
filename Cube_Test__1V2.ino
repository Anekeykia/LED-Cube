// inslude the SPI library:
#include <SPI.h>
#include <math.h>


//unsigned int count = 0xffff;
const int cube_size = 4;
int plane[cube_size];
int fps = 60;
const long frame_time = 1000/fps;//Milliseconds
int cube[cube_size][cube_size][cube_size];


void setup() {

  clearCube();
  for(int i=0; i<cube_size; i++){
      plane[i] = i + 2;
      pinMode(plane[i], OUTPUT);
  }
 /* 
int plane[4] = {2, 3, 5, 6};
  for(int i=0; i<cube_size; i++){
      plane[i] = i + 2;
      pinMode(plane[i], OUTPUT);
  }
  */
 // Serial.begin(9600);
  SPI.begin();
}

void loop() {
rainEffect(600);
fillCube();
phaseEffect(300);
sinEffect(600);
ledTest();

/*
setPoint(0,0,0,1);
setPoint(1,1,1,1);
setPoint(2,2,2,1);
setPoint(3,3,3,1);
frameUpdate(60);
clearCube();
frameUpdate(60);
*/
//transferShift();
//planeShift(0);


 
  //  count<<=1;
  /*  if (count == 0x5555){
      count = 0xaaaa;
    }
    else{
      count = 0x5555;
    }
*/
}








//User Defined Functions

void transferShift(unsigned int x){
    //Calls for SPI transfer to LED CUBE. Input bit register per plane
    SPI.transfer(x & 0xff);
    SPI.transfer(x >> 8);
}



void planeShift(int x){
  //Isolates GND to a single Plane on cube. Input plane # from [0 cube_size].Input -1 to switch off Transistors
  if(x == -1){
     for(int i=0;i<cube_size;i++){
        digitalWrite(plane[i], LOW);
     }
  }
  else{
    for(int i=0;i<cube_size;i++){
      digitalWrite(plane[i], LOW);
      } 
    digitalWrite(plane[x], HIGH);
  }
}



void clearCube(){
  //Sets all elements to 0
  for(int k=0;k<cube_size;k++){
    for(int i=0;i<cube_size;i++){
      for(int j=0;j<cube_size;j++){
        cube[i][j][k] = 0;
      }
    }
  }
}


void fillCube(){
  //Sets all elements to 0
  for(int k=0;k<cube_size;k++){
    for(int i=0;i<cube_size;i++){
      for(int j=0;j<cube_size;j++){
        cube[i][j][k] = 1;
      }
    }
  }
}


void clearPlane(int k){
  //Sets specified plane elements to 0
    for(int i=0;i<cube_size;i++){
      for(int j=0;j<cube_size;j++){
        cube[i][j][k] = 0;
      }
    }
}



void setPoint(int i,int j,int k,int n){
  //Sets element in array to specified integer
  cube[i][j][k]=n;
}



unsigned int plane2Bits(int plane_num){
  //Converts cube matrix into bits[cube_size] ready for transferShift
          unsigned int sum = 0;
          for(int i=0;i<cube_size;i++){
              for(int j=0;j<cube_size;j++){
                  if(cube[i][j][plane_num]==1){
                    unsigned int num = (1<<(i+(j*cube_size)));
                    sum = sum + num;
                  }
              } 
          } 
  return sum;
}



void frameUpdate(int frames){
  //Displays cube in state 'cube' for a specified number of frames 
  unsigned long timer;
  unsigned int bits[cube_size];
    
  for(int i=0;i<cube_size;i++){
    bits[i] = plane2Bits(i);
  }
  int i = 0;
  timer = millis();
  while(millis() < (frames*frame_time)+timer){
    planeShift(-1);
    transferShift(bits[i]);
    planeShift(i);
    if(i == cube_size-1){
      i = 0;
    }
    else{
      i++;
    }
  } 
}



void rainEffect(int frames){
    //Rain Animation!! Pixels spawn randomly in sets of cube_size and fall to the ground!
    unsigned long raintimer = millis();
    clearCube();
    for(int i = 0; i<cube_size;i++){
      setPoint(int(random(0,cube_size)),int(random(0,cube_size)),0,1);        
    }
    
    frameUpdate(8);
  
    while(millis() < (frames*frame_time)+raintimer){

    for(int k = cube_size-1;k>0;k--){
      
      for(int i=0;i<cube_size;i++){
        for(int j=0;j<cube_size;j++){
          cube[i][j][k] = cube[i][j][k-1];
        }
      }
      
    }
    clearPlane(0);
    for(int i = 0; i<4;i++){
      setPoint(int(random(0,cube_size)),int(random(0,cube_size)),0,1);        
    }
    frameUpdate(8);
    }
}


void planeOutline(char planes, int pos){
  //highlights specified cube plane
  if(planes=='i'){
    for(int i=0;i<cube_size;i++){
      for(int j=0;j<cube_size;j++){
        cube[i][j][pos]=1;
      }
    }
  }
  if(planes=='j'){
    for(int j=0;j<cube_size;j++){
      for(int k=0;k<cube_size;k++){
        cube[pos][j][k]=1;
      }
    } 
  }
  if(planes=='k'){
    for(int k=0;k<cube_size;k++){
      for(int i=0;i<cube_size;i++){
        cube[i][pos][k]=1;
      }
    } 
  }
}


void phaseEffect(int frames){
  //frameUpdates cube in distinct layers bottom to top
  unsigned long phaseTimer = millis();
  int placehold[cube_size][cube_size][cube_size];
  for(int k=0;k<cube_size;k++){
    for(int i=0;i<cube_size;i++){
      for(int j=0;j<cube_size;j++){
        placehold[i][j][k] = cube[i][j][k];
      }
    }
  }
  while(millis() < phaseTimer + (frame_time*frames)){
    for(int k = cube_size-1; k>=0 ; k--){
      clearCube();
      for(int i=0;i<cube_size;i++){
        for(int j=0;j<cube_size;j++){
        cube[i][j][k] = placehold[i][j][k];
        }
      }
      frameUpdate(6);
    }
  }  
}


void sinEffect(int frames){
  //double 
  unsigned long rippleTimer = millis();
/*  while(millis()<=rippleTimer+(frame_time*frames)){
    double tick = millis();
    clearCube();
    planeOutline('i',2*(sin(tick/100)+1));
    frameUpdate(3);
  }
*/
  while(millis()<=rippleTimer+(frame_time*frames)){
    double tick = millis();
    clearCube();
    for(int i = 0;i<cube_size;i++){
      for(int j = 0;j<cube_size;j++){
        setPoint(i,j,2*(sin((tick/100)+i+j-cube_size-1)+1),1);
      }
    }
    frameUpdate(4);
  }
}

void ledTest(){
  clearCube();
  for(int k = 0;k<cube_size;k++){
    for(int j = 0;j<cube_size;j++){
      for(int i = 0;i<cube_size;i++){
        setPoint(i,j,k,1);
        frameUpdate(fps*0.125);
        }
      }
    }
}
