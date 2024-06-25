#pragma once
#include <WiFi.h>
#include <LiquidCrystal_I2C.h>
#include <WebServer.h>
#include <vector>
#include<map>

#include <utilits.hpp>

extern const int HALL_GPIO;
extern const float MOMENT_OF_INERTIA;

extern QueueHandle_t DataQueues[2];
extern QueueHandle_t TimeQueue;
extern TaskHandle_t MonitorT;
extern TaskHandle_t RecordT;
extern LiquidCrystal_I2C lcd;  
extern bool IsRecording;

void MonitorTask(void* params){
  data CurData;
  lcd.setCursor(0,0);
  lcd.print("RPS:");
  lcd.setCursor(0,1);
  lcd.print("Power:");
  while (true)
  {
    if(xQueueReceive((QueueHandle_t)params,&CurData,portMAX_DELAY)!=pdTRUE)continue;
    //data CurData = Record.back();
    //lcd.clear();
    lcd.setCursor(4,0);
    lcd.print(CurData.RotPM);
    lcd.print("           ");
    lcd.setCursor(6,1);
    lcd.print(CurData.Power);
    lcd.print("           ");
  }
}

void CalcTask(void* params){
  int Time;
  data Prev;
  //если время между обнаружениями магнита слишком большое 
  while (true)
  {
    if(xQueueReceive(TimeQueue,&Time,portMAX_DELAY) != pdTRUE)continue;
    //if(dt > 100000) ;
    data Output;
    Output.Time = Time / (float)1000;// перевести в секунды;
    Output.DeltaTime = Output.Time - Prev.Time;
    Output.RotPM = (float)60/Output.DeltaTime;
    Output.RadPS = 2*PI/Output.DeltaTime;
    Output.AngAcc = (Output.RadPS - Prev.RadPS)/Output.DeltaTime;
    Output.Moment = constrain(MOMENT_OF_INERTIA * Output.AngAcc,0,INFINITY);
    Output.Power = (Output.Moment * Output.RotPM*1000)/9550;
    Prev = Output;
    SendToQueueArr(Output);
  }
}

void RecordTask(void* params){
  data CurData;
  while(true){
    if(xQueueReceive((QueueHandle_t)params,&CurData,portMAX_DELAY)!=pdTRUE)continue;

    //Record.clear();
    if(!IsRecording)continue;

    if(Record.size() <= 4000){

      if(Record.size()==0)Record.push_back(CurData);

      //ищем место нового элемента по оси x
      float CurX = GetRepMap(CurData)[selectedX];
      if(GetRepMap(Record.back())[selectedX]<CurX)Record.push_back(CurData);//если больше последнего элемента добавляем в конец
      else for(int i = 0; i < Record.size(); i++){
          std::map<String, float> RepMap = GetRepMap(Record[i]);
          if(RepMap[selectedX]>CurX){Record.insert(Record.begin()+i,CurData);break;}//иначе ищем перед каким элементом нахлдится
      }
      Serial.println(CurX);
    }
    else {
      Serial.println("Err:Record list overflov");
      Record.clear();
    }
  }
}