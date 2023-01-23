#include <ArxContainer.h>
#include "LedControl.h"

LedControl matrix = LedControl(12, 10, 11, 1);
arx::pair <short, short> dir;
arx::pair <short, short> food;
arx::vector <arx::pair <short, short>> snake;
bool start, moved, matrixB[8][8] = {0}, foodOn = true;
unsigned long delayStart, foodToggle;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  initialize();
}

void loop() {
  // put your main code here, to run repeatedly:
  if(!start && (analogRead(A2) > 1000 || analogRead(A3) > 1000 || analogRead(A3) < 50)) {
    start = true;
  }
  if(start && millis() - delayStart > analogRead(A5)*0.7 + 100) {
    if((snake.back().first + dir.first + 8)%8 != food.first || (snake.back().second + dir.second + 8)%8 != food.second) {
      matrix.setLed(0, snake.front().first, snake.front().second, false);
      matrixB[snake.front().first][snake.front().second] = false;
      snake.erase(snake.begin());
      snake.push_back({snake.back().first + dir.first, snake.back().second + dir.second});
      snake.back().first += 8;
      snake.back().first %= 8;
      snake.back().second += 8;
      snake.back().second %= 8;

    
    }else{
      snake.push_back({snake.back().first + dir.first, snake.back().second + dir.second});
      snake.back().first += 8;
      snake.back().first %= 8;
      snake.back().second += 8;
      snake.back().second %= 8;
      generateFood();
    }

    if(matrixB[snake.back().first][snake.back().second]) {
      gameOver();
    }else {
      matrix.setLed(0, snake.back().first, snake.back().second, true);
      matrixB[snake.back().first][snake.back().second] = true;

      
    }
    moved = true;
    delayStart = millis();  
  }
  
    if(millis() - foodToggle > 50) {
      if(foodOn)
        matrix.setLed(0, food.first, food.second, true);
      else
        matrix.setLed(0, food.first, food.second, false);
      foodOn = !foodOn;
      foodToggle = millis();
    }
  readJoystick();
}

void initialize() {
  generateFood();
  start = false;
  moved = true;
  snake.clear();
  snake.push_back({0, 0});
  snake.push_back({0, 1});
  snake.push_back({0, 2});
  //snake = {{0, 0}, {0, 1}, {0, 2}};
  dir = {0 ,0};
  pinMode(A2, INPUT);
  pinMode(A3, INPUT);
  randomSeed(analogRead(0));
  
  matrix.clearDisplay(0);
  matrix.shutdown(0, false);
  matrix.setIntensity(0, 1);
  matrix.clearDisplay(0);
  for(auto &i : matrixB) {
    for(auto &j : i)
      j = false;
  }
  for(auto i : snake) {
    matrix.setLed(0, i.first, i.second, true);
    matrixB[i.first][i.second] = true;
  }
}

void gameOver() {
  initialize();
}

void readJoystick() {
  if(analogRead(A2) > 1000 && analogRead(A3) > 50 && analogRead(A3) < 1000  && dir.second == 0 && moved) {
    dir.second = 1;
    dir.first = 0;
    moved = false;
  }else if(analogRead(A2) < 50 && analogRead(A3) > 50 && analogRead(A3) < 1000  && dir.second == 0 && moved) {
    dir.second = -1;
    dir.first = 0;
    moved = false;
  }
  else if(analogRead(A3) > 1000 && analogRead(A2) > 50 && analogRead(A2) < 1000  && dir.first == 0 && moved) {
    dir.first = 1;
    dir.second = 0;
    moved = false;
  }else if(analogRead(A3) < 50 && analogRead(A2) > 50 && analogRead(A2) < 1000  && dir.first == 0 && moved) {
    dir.first = -1;
    dir.second = 0;
    moved = false;
  }
  delay(50);
}

void generateFood() {
  arx::vector<arx::pair <short, short>> emp;
  for(int i = 0; i < 8; i++) {
    for(int j = 0; j < 8; j++) {
      if(!matrixB[i][j])
        emp.push_back({i, j});
    }
  }
  food = emp[random(emp.size())];
}
