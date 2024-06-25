#include <WiFi.h>
#include <LiquidCrystal_I2C.h>
#include <WebServer.h>
#include <vector>
#include <map>

#include <utilits.hpp>
#include <server.hpp>
#include <tasks.hpp>

const int HALL_GPIO = 23;
const float MOMENT_OF_INERTIA = 0.0102;

QueueHandle_t TimeQueue;
TaskHandle_t MonitorT;
TaskHandle_t RecordT;
LiquidCrystal_I2C lcd(0x27, 16, 2);  
std::vector<data> Record{};
QueueHandle_t DataQueues[2];
String selectedX = "T", selectedY = "RPM";
bool IsRecording = false;

void IRAM_ATTR HallISR() {
  int Time = millis();
  xQueueSendFromISR(TimeQueue,&Time,NULL);
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
}

void loop() {
  server.handleClient();
}

