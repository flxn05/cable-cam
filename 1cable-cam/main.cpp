#include <Arduino.h>
#include <WiFi.h>
#include <HTTPServer.hpp> 
#include <HTTPRequest.hpp>
#include <HTTPResponse.hpp>
#include <ResourceNode.hpp>
#include <string>
#include <string_ops.h> 
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
<link rel="icon" type="image/x-icon" href="favicon-cc.png">
<title>Cable Cam Control</title>
<style text="text/css">
.halfspace {
  margin-top: 15vh;
}
.quarterspace {
  margin-top: 8vh;
}
body {
  background-color: rgb(20, 20, 20);
  padding-top: 1vh;
  color: white;
  overflow: hidden;
}
.button-container {
  display: flex;
  justify-content: center;
  margin-bottom: 5vh;
}
.button-container button {
  margin-right: 18px;
}
.btn-row {
  border: solid black 1px;
  border-radius: 12px;
  height: 55px;
  width: 104px;
  font-size: 16px;
  font-family: Arial, Helvetica, sans-serif;
  cursor: pointer;
  transition: all .2s;
}
#stop-btn {
  background-color: rgb(255, 0, 0);
  transform: scale(1.025);
}
#full-btn {
  background-color: rgb(0, 100, 0);
}
#back-btn {
  background-color: #965a00;
}
#minusfivebtn,
#plusfivebtn {
  background-color: #00418b;
}
#plusfivebtn {
  margin-right: 0;
}

.slider {
  -webkit-appearance: none;
  height: 6px;
  border-radius: 5px;
  background-color: red;
  outline: none;
  opacity: 0.7;
  -webkit-transition: .2s;
  flex: 1;
  height: 4px;
  z-index: 1000;
}

.slidecontainer {
  display: flex;
  padding-left: 7.5vw;
  padding-right: 7.5vw;
}

.slider::-webkit-slider-thumb {
  -webkit-appearance: none;
  appearance: none;
  width: 25px;
  height: 25px;
  border-radius: 50%; 
  background: #ffffff;
  cursor: pointer;
}

.slider::-moz-range-thumb {
  width: 8px;
  height: 8px;
  border-radius: 50%;
  background: #ffffff;
  cursor: pointer;
  border-color: white;
}
.inputcontainer {
  display: flex;
  justify-content: center;
}
nav {
  text-align: center;
  position: fixed;
  bottom: 0;
  left: 0;
  right: 0;
  font-family: Arial, Helvetica, sans-serif;
  color: #969ea8;
  padding-bottom: 12px;
  font-size: 10px;
  display: flex;
  justify-content: center;
  z-index: 10;
}
nav div {
  margin-left: 64px;
  font-style: italic;
  z-index: 10;
}
/* :focus */
.btn-row:focus {
  animation: traffic-hover .4s forwards;
  font-weight: bold;
}
#stop-btn:focus {
  color: red;
  background-color: rgba(0, 0, 0, 0.2);
  border: 1px red solid;
}
#full-btn:focus {
  color: green;
  background-color: rgba(0, 0, 0, 0.2);
  border: 1px #006400 solid;
}
#back-btn:focus {
  color: #c98522;
  background-color: rgba(0, 0, 0, 0.2);
  border: 1px #965a00 solid;
}
#plusfivebtn:focus, #minusfivebtn:focus {
  color: rgb(25, 159, 212);
  background-color: rgba(0, 0, 0, 0.2);
  border: 1px #00418b solid;
}
/* /:hover */
.headline-ccc {
  font-size: 1.75rem;
  font-weight: bold;
  font-family: Arial, Helvetica, sans-serif;
  letter-spacing: -2.25px;
  color: red;
}
.container-flex {
  display: flex;
  align-items: center;
  flex-direction: column;
}

.current {
  font-size: 8rem;
  font-weight: bold;
  color: grey;
  font-family: Arial, Helvetica, sans-serif;
}

.tltip-con {
  display: flex;
  justify-content: center;
  align-items: center;
  margin-top: 15vh;
  color: grey;
}
.tltip-con div {
  margin-right: 24px;
  font-family: Arial, Helvetica, sans-serif;
  text-align: center;
  line-height: 2rem;
}

#warning {
  margin-top: 5vh;
  color: grey;
  letter-spacing: 2px;
  font-size: 1.125rem;
  font-family: Arial, Helvetica, sans-serif;
  animation: warning 10ms both 7.5s;
  display: flex;
  justify-content: center;
  align-items: center;
  margin-top: 15vh;
}

@keyframes traffic-hover {
  0% {transform: scale(1);}
  50% {transform: scale(0.97);}
  100% {transform: scale(1);}
}
@keyframes warning {
  /*0% {display: flex;}
  100% {display: none;}*/
  0% {opacity: 1;}
  100% {opacity: 0; display: none;}
}
.headline {
  font-family: Arial, Helvetica, sans-serif;
  font-weight: bold;
  font-size: 5rem;
  letter-spacing: -5px;
  margin-bottom: 3rem;
  color: black;
}
.credits div {
  margin-bottom: 1.5rem;
  font-size: 1.3rem;
  margin-left: 2.5rem;
  color: black;
  font-family: Arial, Helvetica, sans-serif;
  font-style: italic;
  text-align: left;
}

@media (max-width: 740px) {
  .headline-ccc {
    font-size: 1rem !important;
    letter-spacing: 0px !important;
    margin: 0 !important;
  }
  .current {
    font-size: 1.5rem !important;
    margin-top: 4vh;
  }
  .btn-row {
  border: solid black 1px;
  border-radius: 12px;
  height: 36px;
  width: 104px;
  font-size: 1rem !important;
  font-family: Arial, Helvetica, sans-serif;
  }
  #warning {
    text-align: center;
    font-size: .8rem !important;
  }
  nav div {
    font-size: .5rem !important;
    margin: 6px;
    margin-bottom: 0;
    padding-bottom: 0;
  }
  nav {
    position: fixed;
    bottom: 0;
  }
}
</style>
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
    <div class="tltip-con">
      <div class="tltip-con-content"><b>- 5</b> = ArrowLeft</div>
      <div class="tltip-con-content"><b>Back</b> = y</div>
      <div class="tltip-con-content"><b>STOP</b> = [spacebar]</div>
      <div class="tltip-con-content"><b>Full</b> = x</div>
      <div class="tltip-con-content"><b>+ 5</b> = ArrowRight</div>
    </div>
    <div class="tltip-con--ineff" id="warning"><b>DO NOT CHANGE DIRECTIONS QUICKLY TO PREVENT DAMAGE!</b></div>
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

</body>
</html>)rawliteral";




HTTPServer cablecam = HTTPServer();

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

