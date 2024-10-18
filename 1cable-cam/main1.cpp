#include <Arduino.h>
#include <WiFi.h>
#include <HTTPServer.hpp> 
#include <HTTPRequest.hpp>
#include <HTTPResponse.hpp>
#include <ResourceNode.hpp>
#include <string>
#include <string_ops.h> 
#include <SPI.h>
using namespace httpsserver;
using namespace std;
const char *ssid = "Cable-Cam";
const char *password = "cablecam";
const char * html = R"rawliteral(<!doctype html>
<html lang="en">
<head>
<meta charset="utf-8">
<meta name="viewport" content="width=device-width, initial-scale=1">
<link rel="shortcut icon" href="data:">
<title>Cable Cam Control</title>
</head>
<body>
  <main>
    <div class="container-flex">
      <p class="headline-ccc">Cable Cam Control</p>
      <div class="current"></div>
    </div>
    <div class="button-container">
      <div>
        <button onclick="gleft()" class="btn-row" id="minusfivebtn">- 5</button>
      </div>
      <div>
        <button onclick="ofull()" class="btn-row" id="back-btn">&#8672; Back</button>
      </div>
      <div>
        <button onclick="stop()" class="btn-row" id="stop-btn"><b>STOP</b></button>
      </div>
      <div>
        <button onclick="full()" class="btn-row" id="full-btn">Full &#8674;</button>
      </div>
      <div>
        <button onclick="gright()" class="btn-row" id="plusfivebtn">+ 5</button>
      </div>     
    </div>
    <div class="slidecontainer">
      <input type="range" min="-99" max="99" value="0" class="slider" id="myRange">
    </div>

<div class="start-calibrating"><button>Advanced</button></div>

    <div class="button-container advanced">
      <div>
        <button onclick="calib()" class="btn-row" id="calibrate">Start Calibrating</button>
      </div>
      <div>
        <button onclick="stop()" id="stop-calibrating"><b>STOP</b></button>
      </div>
      <input type="number" id="distance" placeholder="Distance (m)">
      <input type="number" id="speed" placeholder="Speed (rel)">
      <input type="number" id="calibration_speed" placeholder="Calibration-Speed (rel)">
      <input type="number" id="rope_length" placeholder="Ropelength in m">
      <input type="number" id="acceleration_time" placeholder="Accelerationtime in s">
      <div>
        <button onclick="preset()" class="btn-row" id="preset">RUN</button>
      </div>
    </div>
    <div class="calibrating-info"><div>(i) Make sure the Cable Cam is positioned at the bottom end of the line.</div></div>
    <div class="tltip-con">
      <div class="tltip-con-content"><b>- 5</b> = ArrowLeft</div>
      <div class="tltip-con-content"><b>Back</b> = y</div>
      <div class="tltip-con-content"><b>STOP</b> = [spacebar]</div>
      <div class="tltip-con-content"><b>Full</b> = x</div>
      <div class="tltip-con-content"><b>+ 5</b> = ArrowRight</div>
    </div>
    <!--<div class="tltip-con--ineff" id="warning"><b>DO NOT CHANGE DIRECTIONS QUICKLY TO PREVENT DAMAGE!</b></div>-->
  </main>
<div class="quarterspace"></div>
  <nav>
    <div><b>UI DESIGN</b> FELIX NOWAK</div>
    <div><b>CONTROLLER PROGRAMMING</b> DAVID OTT</div>
    <div><b>ELECTRONICAL ENGINEERING</b> MICHAEL DANISCH AND JONATHAN WALLASCH</div>
  </nav>
  <script>
    const slider = document.getElementById('myRange');
    const currentDiv = document.querySelector('.current');

    function sleep(ms) {
    return new Promise(resolve => setTimeout(resolve, ms));
}
    
    function updateCurrentValue() {
      currentDiv.textContent = slider.value;
    }
    
    
    function gleft(){
          if(slider.value == 99){
                  slider.value = Number(slider.value) -4;}
              else{    
              slider.value =Number(slider.value) -5;}
          updateCurrentValue();
      }
    function gright(){
          if(slider.value == -99){
                  slider.value = Number(slider.value) +4;}
              else{    
              slider.value =Number(slider.value) +5;}
          updateCurrentValue();
      }
      document.addEventListener('keydown', function(event) {
          if (event.key === 'ArrowRight') {
              if(slider.value == -99){
                  slider.value = Number(slider.value) +4;}
              else{    
              slider.value =Number(slider.value) +5;}
          updateCurrentValue();
      }
      });  
      document.addEventListener('keydown', function(event) {
          if (event.key === 'ArrowLeft') {
              if(slider.value == 99){
                  slider.value = Number(slider.value) -4;}
              else{    
              slider.value =Number(slider.value) -5;}
          updateCurrentValue();
      }
      });    
    function stop() {
      slider.value = 0;
      updateCurrentValue();
    }
    
    function full() {
      if(slider.value < 0){
        alert("Don't change direction so fast");}
      else{
          slider.value = 99;
          updateCurrentValue();}
    }
    
    function ofull() {
          if(slider.value > 0){
        alert("Don't change direction so fast");}
      else{
          slider.value = -99;
          updateCurrentValue();}
    }

    function preset(){
    const meters = document.getElementById("distance").value;
    const pwm = document.getElementById("speed").value;
            const xhr = new XMLHttpRequest();
            xhr.open('GET', `/meters?=${meters}=${pwm}=`, true);
            xhr.send();
    }
    
    document.getElementById("rope_length").value = 40;
    document.getElementById("calibration_speed").value = 50;
    document.getElementById("acceleration_time").value = 1;
    function cal(){
        let m = document.getElementById("rope_length").value;
        slider.value = document.getElementById("calibration_speed").value;
        let acceleration_time = (document.getElementById("acceleration_time").value)*1000;
        updateCurrentValue();
        const start = Date.now();
        const interval = setInterval(() => {
        if (slider.value == 0) {
            const end = Date.now();
            const duration = end - start; 
            clearInterval(interval); 
            console.log(duration);
            const xhr = new XMLHttpRequest();
            xhr.open('GET', `/dur?=${duration}=${m}=${acceleration_time}=${document.getElementById("calibration_speed").value}=`, true);
            xhr.send();
        }
    }, 100);
}
    
    
    function calib(){
        setTimeout(cal, 0);
    }

      document.addEventListener('keydown', function(event) {
          if (event.key === 'y') {
            ofull();
      }
      });
      document.addEventListener('keydown', function(event) {
          if (event.key === ' ') {
               stop()    
      }
      });
    slider.addEventListener('input', updateCurrentValue);
    document.addEventListener('keydown', function(event) {
      if (event.key === 'x') {
             full()
      }
      });
    document.addEventListener('DOMContentLoaded', (event) => {
      updateCurrentValue();
      let revalue = 0;
      function sendSliderValue() {
        const value = slider.value;
        if (value != revalue) {
          const xhr = new XMLHttpRequest();
          xhr.open('GET', `/value?=${value}=`, true);
          xhr.send();
          revalue = value;
        }
      }
      setInterval(sendSliderValue, 69);
    });
  </script>


<script>
  // Select the button, the advanced section, and the calibrating info div
  const startCalibratingButton = document.querySelector('.start-calibrating');
  const advancedSection = document.querySelector('.advanced');
  const calibratingInfo = document.querySelector('.calibrating-info');

  // Add a click event listener to the button
  startCalibratingButton.addEventListener('click', () => {
      // Display the advanced section as flex
      advancedSection.style.display = 'flex';
      
      // Display the calibrating info div
      calibratingInfo.style.display = 'flex';
      
      // Hide the start-calibrating button
      startCalibratingButton.style.display = 'none';
  });
</script>

<script>
  document.getElementById('stop-calibrating').addEventListener('click', function() {
      const button = this;
      button.textContent = 'SAVED';
      
      setTimeout(function() {
          button.textContent = 'STOP';
      }, 3000); // Change text back to 'STOP' after 2 seconds
  });
</script>


</body>
</html>)rawliteral";



HTTPServer cablecam = HTTPServer();

TaskHandle_t TaskHandler = NULL;

int meters;
int duration_ms;
int acc_time2;
int cal_pwm2;
//int meter = duration_ms/meters;
int task_duration;

void run_motor(int pwm){
  Serial.println(pwm);
  if(pwm > 0){
    digitalWrite(25, HIGH);
    analogWrite(27, 0);
    analogWrite(26, 0.01*pwm*255);
  }
  else if(pwm == 0){
    digitalWrite(25, LOW);
    analogWrite(27, 0);
    analogWrite(26, 0);
  }
  else if(pwm < 0){
    digitalWrite(25, HIGH);
    analogWrite(26, 0);
    analogWrite(27, 0.01*(-pwm)*255);
  }
}

void run_timer(void *pvParameters){
  Serial.println("Task created");
  vTaskDelay(pdMS_TO_TICKS(task_duration));
  run_motor(0);
  Serial.println("Task stopped"); 
  vTaskDelete(NULL);
}

void run_ms(int pwm, int length){
  run_motor(pwm);
  Serial.print("Pwm to drive:");
  Serial.println(pwm);
  Serial.print("Meters to drive: ");
  Serial.println(length);
  Serial.print("Measured distance ms: ");
  Serial.println(duration_ms);
  Serial.print("Length of rope: ");
  Serial.println(meters);
  //task_duration = ((0.8*duration_ms*(100/pwm))/meters)*length;
  //Serial.println(((((100-pwm)*0.01)*2)+((100-pwm)*0.01)));
  task_duration = (((duration_ms/(100/cal_pwm2))/length)*(100/pwm))+acc_time2; 
  Serial.print("Time it should drive: ");
  Serial.println(task_duration);
  xTaskCreate(run_timer, "Run_Length", 4096, NULL, 1, &TaskHandler);
}

void handleRoot(HTTPRequest *req, HTTPResponse *res) {
	res->setHeader("Content-Type", "text/html");
	res->println(html);
  }

void handlePwmVal(HTTPRequest *req, HTTPResponse *res){
	std::string answer = req->getRequestString();
  std::string number = read_from_char(answer, '=');
  std::string number1 = read_until_char(number, '=');
  int number2 = stoi(number1);
  Serial.println(number1.c_str());
  run_motor(number2);
}

void handleF(HTTPRequest *req, HTTPResponse *res){
  std::string answer = req->getRequestString();
  Serial.println(ESP.getFreeHeap());
  std::string meters = read_from_char(answer, '=');
  std::string meters1 = read_until_char(meters, '=');
  std::string pwm = read_from_char(meters, '=');
  std::string pwm1 = read_until_char(pwm, '=');
  Serial.println(ESP.getFreeHeap());
  int meters2 = stoi(meters1);
  int pwm2 = stoi(pwm1);
  run_ms(pwm2, meters2);
}

void handleM(HTTPRequest *req, HTTPResponse *res){
  	std::string answer = req->getRequestString();///dur?=${duration}=${m}=
    std::string dur = read_from_char(answer, '=');
    std::string dur1 = read_until_char(dur, '=');//duration
    std::string m = read_from_char(dur, '=');
    std::string m1 = read_until_char(m, '=');
    std::string acc_time = read_from_char(m, '=');
  std::string acc_time1 = read_until_char(acc_time, '=');
  std::string cal_pwm = read_from_char(acc_time, '=');
  std::string cal_pwm1 = read_until_char(cal_pwm, '=');
  Serial.println(ESP.getFreeHeap());
  int acc_time2 = stoi(acc_time1);
  int cal_pwm2 = stoi(cal_pwm1);
    duration_ms = stoi(dur1);
    meters = stoi(m1);
    Serial.println(dur1.c_str());
    Serial.println(m1.c_str());
}

void setup(){
	Serial.begin(115200);
  pinMode(0, OUTPUT);//led2-grün
  pinMode(25, OUTPUT);//led-grün(wenn motor)
  pinMode(13, OUTPUT);//led-rot(wenn disconnected)
  pinMode(12, OUTPUT);//enable1
  pinMode(14, OUTPUT);//enable2
  pinMode(27, OUTPUT);//pwm1
  pinMode(26, OUTPUT);//pwm2
  digitalWrite(25, LOW);
  digitalWrite(13, HIGH);
  digitalWrite(0, HIGH);
  digitalWrite(12, HIGH);
  digitalWrite(14, HIGH);
  digitalWrite(27, LOW);
  digitalWrite(26, LOW);
  WiFi.mode(WIFI_STA);
  WiFi.hostname("cable_cum");
  WiFi.begin(ssid, password);
  Serial.print("Connecting to ");
  Serial.print(ssid);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print('.');
    delay(1000);
  }
  digitalWrite(13, LOW);
  Serial.print("\nConnected to ");
  Serial.print(ssid);
  Serial.print(" with IP: ");
  Serial.println(WiFi.localIP());
	ResourceNode * nodeRoot = new ResourceNode("/", "GET", &handleRoot);
  cablecam.registerNode(nodeRoot);
  ResourceNode *nodePwm = new ResourceNode("/value", "GET", &handlePwmVal);
  cablecam.registerNode(nodePwm);
  ResourceNode *nodeM = new ResourceNode("/dur", "GET", &handleM);
  cablecam.registerNode(nodeM);
  ResourceNode *nodeF = new ResourceNode("/meters", "GET", &handleF);
  cablecam.registerNode(nodeF);
  cablecam.start();
}

void loop(){
  cablecam.loop();
    if(WiFi.status() != WL_CONNECTED){
      digitalWrite(13, HIGH);
      run_motor(0);
      Serial.println("Connection lost. Try to reconnect to");
      Serial.print(ssid);
        while (WiFi.status() != WL_CONNECTED) {
            Serial.print('.');
            delay(1000);
      }
      digitalWrite(13, LOW);
    Serial.print("Connected to");
    Serial.print(ssid);
    Serial.print("with IP: ");
    Serial.println(WiFi.localIP());
  
    }
}

