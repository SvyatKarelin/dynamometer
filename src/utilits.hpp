#pragma once
#include <WiFi.h>
#include <LiquidCrystal_I2C.h>
#include <WebServer.h>
#include <vector>
#include <map>

#include "templates.h"

struct data;
extern std::vector<data> Record;
extern QueueHandle_t DataQueues[2];
extern String selectedX, selectedY;

struct data
{
    float DeltaTime = 0, Time = 0,RadPS = 0,RotPM = 0, AngAcc = 0, Moment = 0, Power = 0;
    /*data(float Time, float RadPS, float RotPS, float Power){
      this->Time=Time;
      this->RadPS=RadPS;
      this->RotPS=RotPS;
      this->Power=Power;
    }*/
};

std::map<String, float> GetRepMap(data Data);

int FindMax(String Selected){
  int max = 0;
  for(data Data : Record){
    std::map<String, float> RepMap = GetRepMap(Data);    
    if(RepMap[Selected] > max) {max = RepMap[Selected];}
  }
  return max;
  //return map(Coord,0,max,0,Size);
}


String GetSvg(){
  String Svg = SvgTemplate;
  int MaxX = FindMax(selectedX);
  int MaxY = FindMax(selectedY);
  float GridX = MaxX / 5;
  float GridY = MaxY / 5;
  for(int i = 1; i <= 5; i++){
    String _Vline = VLine;
    String _Hline = HLine;
    _Vline.replace("[VAL]",String(GridX * i));
    _Hline.replace("[VAL]",String(GridY * i));
    _Vline.replace("[X]",String(80 * i));
    _Hline.replace("[Y]",String(40 * i));
    Svg.replace("<!--L-->",_Vline+String('\n')+_Hline+String("<!--L-->"));
  }
  for(data Data : Record){
    std::map<String, float> RepMap = GetRepMap(Data);  
    int x = map(RepMap[selectedX],0,MaxX,0,400);
    int y = 200-map(RepMap[selectedY],0,MaxY,0,200);
    Svg.replace("\"/><!--P-->",String('L')+ x + String(' ') + y + String("\"/><!--P-->"));
  }
  return Svg;
}

bool compareData(data d1, data d2) 
{ 
    std::map<String, float> d1RepMap = GetRepMap(d1), d2RepMap = GetRepMap(d2);
    return (d1RepMap[selectedX] < d2RepMap[selectedX]); 
} 


void ConnectWiFi_AP()
{ 
    Serial.println("");
    WiFi.mode(WIFI_AP);
    while(!WiFi.softAP("Dynamometer", ""))
    {
      Serial.println(".");
      delay(100);
    }

    Serial.println("");
    Serial.print("Started AP:\t");
    Serial.print("IP address:\t");
    Serial.println(WiFi.softAPIP());
}



std::map<String, float> GetRepMap(data Data){
  std::map<String, float> RepMap;    
  RepMap["T"]=Data.Time;
  RepMap["DT"]=Data.DeltaTime;
  RepMap["RPS"]=Data.RadPS;
  RepMap["RPM"]=Data.RotPM;
  RepMap["ANGACC"]=Data.AngAcc;
  RepMap["MOMENT"]=Data.Moment;
  RepMap["POWER"]=Data.Power;
  return RepMap;
}

void FormReplace(String& Form, String selectedX,String selectedY, String ChartSvg){
  std::map<String, float> RepMap = GetRepMap(data());    
  for(auto rep : RepMap){
    Form.replace(String("<!--X")+rep.first+String("-->"),selectedX==rep.first?"selected = \"selected\"":"");
    Form.replace(String("<!--Y")+rep.first+String("-->"),selectedY==rep.first?"selected = \"selected\"":"");
  }
  Form.replace("[SVGREP]", ChartSvg);
}

void FormReplace(String& Form,data Data, String ChartSvg){
  std::map<String, float> RepMap = GetRepMap(Data);    
  for(auto rep : RepMap){
    Form.replace(String("[")+rep.first+String("]"),String(rep.second));
  }
  Form.replace("[SVGREP]", ChartSvg);
}

void FillQueueArr(){
  for(int i = 0; i < sizeof(DataQueues)/sizeof(QueueHandle_t); i++)DataQueues[i] = xQueueCreate(8,sizeof(data));
}

void SendToQueueArr(data Data){
  for(QueueHandle_t Queue : DataQueues)xQueueSend(Queue,&Data,0);
}