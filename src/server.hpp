#pragma once
#include <WebServer.h>
#include <WiFi.h>
#include <LiquidCrystal_I2C.h>
#include <WebServer.h>
#include <vector>
#include <map>

#include <templates.h>
#include <utilits.hpp>


extern std::vector<data> Record;
extern String selectedX, selectedY;
extern bool IsRecording;

WebServer server(80);

void RedirectTo(String URI){
  server.sendHeader("Location", URI, true);
  server.send ( 302, "text/plain", "");
}

void handleRoot() {
  RedirectTo("/settings");
}

void handleSettings(){
  String response = SETTINGS_page;
  FormReplace(response,selectedX,selectedY,GetSvg());
  server.send(200, "text/html", response);
}

void handleSession(){
  String response = MAIN_page;
  data CurData;
  if(Record.size()>0)CurData = Record.back();
  FormReplace(response,CurData, GetSvg());
  server.send(200, "text/html", response);
}

void handleApplySettings(){
  selectedX = String(server.arg("Axis-X"));
  selectedY = String(server.arg("Axis-Y"));
  if(Record.size()>0)std::sort(Record.begin(), Record.end(), compareData); 
  RedirectTo("/settings");
}

void HandleRecord(){
  IsRecording = server.arg("session").toInt();
  if(!IsRecording){Record.clear(); RedirectTo("/settings");}
  else RedirectTo("/session");
}

void handleNotFound() {
   server.send(404, "text/plain", "Not found");
}

void InitServer()
{
  server.on("/", handleRoot);
  server.on("/settings", handleSettings);
  server.on("/session", handleSession);
  server.on("/apply_settings", handleApplySettings);
  server.on("/rec", HandleRecord);
  server.onNotFound(handleNotFound);
  server.begin();
  Serial.println("HTTP server started");
}