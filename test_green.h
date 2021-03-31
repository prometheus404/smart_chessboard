void test_green_routine(){
  Serial.println("test_routine");
  for(int row = 0; row < 8; row++){
    for (int col = 0; col < 8; col++){
      if(game.actual[row]&(0x01<<(7-col))){
        Serial.print(row);
        Serial.print(",");
        Serial.println(col);
        SET_LED(LED_GREEN, row, col);
      }
      else if((row+col)%2 == 0)
        SET_LED(LED_BLACK, row, col);
      else
        SET_LED(LED_WHITE, row, col);
    }
  }
}
void test_green_setup(){
  game.state = 0x00;
  game.state_routine = test_green_routine;
}
