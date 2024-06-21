#include <Arduino.h>
#include <Wire.h> 
#include <WiFi.h>
#include <LiquidCrystal_I2C.h>
#include <WebServer.h>
#include <vector>
#include<map>

#include <Form.h>

const int HALL_GPIO = 23;
const float MOMENT_OF_INERTIA = 0.0102;

struct data;
QueueHandle_t TimeQueue;
TaskHandle_t MonitorT;
TaskHandle_t RecordT;
LiquidCrystal_I2C lcd(0x27, 16, 2);  
std::vector<data> Record{} ;
QueueHandle_t DataQueues[3];
bool IsRecording = true;

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

std::map<String, int> GetRepMap(data Data);

int FindMax(String Selected){
  int max = 0;
  for(data Data : Record){
    std::map<String, int> RepMap = GetRepMap(Data);    
    if(RepMap[Selected] > max) {max = RepMap[Selected];}
  }
  return max;
  //return map(Coord,0,max,0,Size);
}

String GetSvg(String selectedX,String selectedY){
  String Svg = SvgTemplate;
  int MaxX = FindMax(selectedX);
  int MaxY = FindMax(selectedY);
  for(data Data : Record){
    std::map<String, int> RepMap = GetRepMap(Data);  
    int x = map(RepMap[selectedX],0,MaxX,0,400);
    int y = 200-map(RepMap[selectedY],0,MaxY,0,200);
    Svg.replace("\"/><!--P-->",String('L')+ x + String(' ') + y + String("\"/><!--P-->"));
  }
  return Svg;
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

WebServer server(80);

std::map<String, int> GetRepMap(data Data){
  std::map<String, int> RepMap;    
  RepMap["T"]=Data.Time;
  RepMap["DT"]=Data.DeltaTime;
  RepMap["RPS"]=Data.RadPS;
  RepMap["RPM"]=Data.RotPM;
  RepMap["ANGACC"]=Data.AngAcc;
  RepMap["MOMENT"]=Data.Moment;
  RepMap["POWER"]=Data.Power;
  return RepMap;
}

void FormReplace(String& Form,data Data, String selectedX,String selectedY, String ChartSvg){
  std::map<String, int> RepMap = GetRepMap(Data);    
  for(auto rep : RepMap){
    Form.replace(String("[")+rep.first+String("]"),String(rep.second));
    Form.replace(String("<!--X")+rep.first+String("-->"),selectedX==rep.first?"selected = \"selected\"":"");
    Form.replace(String("<!--Y")+rep.first+String("-->"),selectedY==rep.first?"selected = \"selected\"":"");
    Form.replace("[SVGREP]", ChartSvg);
  }
}

void handleRoot() {
  static int prevSize = 0;
  String response = MAIN_page;
  data CurData;
  prevSize = Record.size();
  FormReplace(response,CurData,"MOMENT","ANGACC", GetSvg("T","RPM"));
  server.send(200, "text/html", response);
}

void handleNotFound() {
   server.send(404, "text/plain", "Not found");
}

void InitServer()
{
   server.on("/", handleRoot);
   server.onNotFound(handleNotFound);
   server.begin();
   Serial.println("HTTP server started");
}

void FillQueueArr(){
  for(int i = 0; i < sizeof(DataQueues)/sizeof(QueueHandle_t); i++)DataQueues[i] = xQueueCreate(8,sizeof(data));
}

void SendToQueueArr(data Data){
  for(QueueHandle_t Queue : DataQueues)xQueueSend(Queue,&Data,0);
}

void IRAM_ATTR HallISR() {
  int Time = millis();
  xQueueSendFromISR(TimeQueue,&Time,NULL);
}



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
    Output.Time = Time;
    Output.DeltaTime = (Output.Time - Prev.Time)/(float)1000;// перевести в секунды
    Output.RotPM = (float)60/Output.DeltaTime;
    Output.RadPS = 2*PI/Output.DeltaTime;
    Output.AngAcc = (Output.RadPS - Prev.RadPS)/Output.DeltaTime;
    Output.Moment = constrain(MOMENT_OF_INERTIA * Output.AngAcc,0,INFINITY);
    Output.Power = (Output.Moment * Output.RotPM)/9550;
    Serial.println(true);
    Prev = Output;
    SendToQueueArr(Output);
  }
}

void RecordTask(void* params){
  data CurData;
  while(true){
    if(xQueueReceive((QueueHandle_t)params,&CurData,portMAX_DELAY)!=pdTRUE)continue;

    //Record.clear();
    if(!IsRecording){continue;}

    if(Record.size() <= 4000)Record.push_back(CurData);
    else {
      Serial.println("Err:Record list overflov");
      Record.clear();
    }
  }
}

void setup() {
  pinMode(HALL_GPIO,INPUT_PULLUP);
  Serial.begin(9600);
  lcd.init();           
  lcd.backlight();
  ConnectWiFi_AP();
  InitServer();

  /*for(int i = 0; i < 10; i++){
    data D;
    D.Time = map(i,0,10,0,200);
    D.RotPM = D.Time*D.Time;
    Record.push_back(D);
  }*/

  attachInterrupt(digitalPinToInterrupt(HALL_GPIO), HallISR, RISING);
  TimeQueue = xQueueCreate(8,sizeof(int));
  FillQueueArr();
  xTaskCreate(CalcTask, "Calc", 4096, NULL, 1, NULL);
  xTaskCreate(MonitorTask, "Monitor", 4096, DataQueues[0], 1, &MonitorT);
  xTaskCreate(RecordTask, "Record", 4096, DataQueues[1], 1, &RecordT);
  DataQueues[2] = xSemaphoreCreateBinary();
}

void loop() {
  server.handleClient();
}

