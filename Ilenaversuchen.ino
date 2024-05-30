#include "LedControl.h"
LedControl lc=LedControl(12,11,10,4); //lc=variable

int XCoord;
int YCoord;

//Anzahl der bildschirme
const int BILDSCHIRME = 4;
//Alle punkte auf dem display
const int PIXELGESAMT = BILDSCHIRME*8;
//Bewegung pro loop ausführung
const int GESCHWINDIGKEIT = 1;
//Hellugkeit einstellen
const int HELLIGKEIT = 1;
//die Startposition des Punktes vom Spiel
const int STARTPOSITION = 3;

const int LAUFSCHRIFT []= {
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
  1,0,0,0,0,0,1,1,1,1,1,0,1,1,1,1,0,0,1,1,1,1,1,0,1,1,1,1,0,0,0,0,
  1,0,0,0,0,0,1,0,0,0,1,0,1,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,1,0,0,0,
  1,0,0,0,0,0,1,0,0,0,1,0,0,1,0,0,0,0,1,0,0,0,0,0,1,0,0,0,1,0,0,0,
  1,0,0,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,1,1,1,1,0,0,1,1,1,1,0,0,0,0,
  1,0,0,0,0,0,1,0,0,0,1,0,0,0,0,1,0,0,1,0,0,0,0,0,1,0,0,0,1,0,0,0,
  1,1,1,1,1,0,1,1,1,1,1,0,1,1,1,1,0,0,1,1,1,1,1,0,1,0,0,0,0,1,0,0,
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0    //Hinter das letzte Zeichen kommt kein Komma!
};

const int WELLENANIMATIONEN = 4;
byte WELLE[WELLENANIMATIONEN][32] = {
  {
    // Frame 1
    B00000001, B00000010, B00000100, B00001000, B00010000, B00100000, B01000000, B10000000,
    B00000001, B00000010, B00000100, B00001000, B00010000, B00100000, B01000000, B10000000,
    B00000001, B00000010, B00000100, B00001000, B00010000, B00100000, B01000000, B10000000,
    B00000001, B00000010, B00000100, B00001000, B00010000, B00100000, B01000000, B10000000     
  }, {
    // Frame 2
    B00000010, B00000100, B00001000, B00010000, B00100000, B01000000, B10000000, B00000001,
    B00000010, B00000100, B00001000, B00010000, B00100000, B01000000, B10000000, B00000001,
    B00000010, B00000100, B00001000, B00010000, B00100000, B01000000, B10000000, B00000001,
    B00000010, B00000100, B00001000, B00010000, B00100000, B01000000, B10000000, B00000001     
  }, {         
    // Frame 3         
    B00000100, B00001000, B00010000, B00100000, B01000000, B10000000, B00000001, B00000010,
    B00000100, B00001000, B00010000, B00100000, B01000000, B10000000, B00000001, B00000010,
    B00000100, B00001000, B00010000, B00100000, B01000000, B10000000, B00000001, B00000010,
    B00000100, B00001000, B00010000, B00100000, B01000000, B10000000, B00000001, B00000010     
  }, {
    // Frame 4         
    B00001000, B00010000, B00100000, B01000000, B10000000, B00000001, B00000010, B00000100,         
    B00001000, B00010000, B00100000, B01000000, B10000000, B00000001, B00000010, B00000100,         
    B00001000, B00010000, B00100000, B01000000, B10000000, B00000001, B00000010, B00000100,         
    B00001000, B00010000, B00100000, B01000000, B10000000, B00000001, B00000010, B00000100     
  } 
};


//Schreibt die laufschrift auf den Bildschirm
void SchreibLaufschrift(int tempora []){ 
  lc.clearDisplay (1);
  for(int beweg=32; beweg>0; beweg--){
    BildschirmZuruecksetzen();
    for (int i=0; i<255;i++){     // Zusatzaufgabe:Hier wir viel unnötiges gerechnet. Optimieren Sie den Code!
      if(tempora [i]==1){
        lc.setLed(3-(((i+beweg)/8)%4),i/32,(i+beweg)%8,true);  
      }
    } 
    delay(100);
  }
}

//Deaktiviert alle lichter auf dem Bildschirm
void BildschirmZuruecksetzen() {
    lc.clearDisplay (0);
    lc.clearDisplay (1);
    lc.clearDisplay (2);
    lc.clearDisplay (3);
}

void AnimationAnzeigen(byte frames[WELLENANIMATIONEN][PIXELGESAMT]){
  for (int i = 0;i<WELLENANIMATIONEN;i++) {
    AnimationsFrameAnzeigen(frames[i]);
    delay(200);
  }
}

void AnimationsFrameAnzeigen(byte frame[PIXELGESAMT]) {
  for (int col = 0; col < PIXELGESAMT; col++) {
    int device = col / 8;
    int colInDevice = col % 8;
    lc.setColumn(device, colInDevice, frame[col]);
  }
}

//zeichnet den Punkt auf den Bildschrim für das Spiel
//modula 8, damit es immer unter 8 ist und nie darüber geht!
void ZeichnePunkt() {
  lc.setLed(XCoord/8, YCoord, 7 - (XCoord % 8), true);
  delay(100);
  lc.setLed(XCoord/8, YCoord, 7 - (XCoord % 8), false);
}

void GameOver(){
  Serial.println("Game Over!");
  SchreibLaufschrift(LAUFSCHRIFT); // Laufschrift
  delay(1000);
  BildschirmZuruecksetzen();

  AnimationAnzeigen(WELLE); // Animation
  AnimationAnzeigen(WELLE);
  BildschirmZuruecksetzen();

  //nach "LOSER" wird so das Spiel wieder gestartet -> der Punkt wird wieder angezeigt ("zurückgeholt")
  XCoord = STARTPOSITION;
  YCoord = STARTPOSITION;

  Serial.println(analogRead(A2));
  // Solange im kreis drehen bis der stick gedrückt wird (wert von analogRead(A2) tiefer als 20)
  while (digitalRead(A2) != LOW) {
    delay(50);
    ZeichnePunkt();
  }
}

//Arduino funktion die ausgeführt wird beim start EINMAL
void setup() {
 pinMode(A0, INPUT);   //pinMode = wird genutzt um zu bestimmen ob es ein Input oder Output ist
 pinMode(A1, INPUT);
 pinMode(A2,INPUT_PULLUP);
 pinMode(2, OUTPUT);
 digitalWrite(2,HIGH);
 Serial.begin(9600);
  for(int i=0;i<BILDSCHIRME;i++){
	    lc.shutdown(i,false); //macht den Bildschirm an
	    lc.setIntensity(i,HELLIGKEIT); //wie hell es leuchtet
	    lc.clearDisplay(i); //setzt den Bildschirm zurück, so dass es nicht mehr rot ist
  }
}


//Arduino funktion die wird immer wieder ausgeführt
void loop() { 
  int XJoy = analogRead(A1); //Hier wird der Wert welcher der Joystick dauernd an A0 "sendet" in "Zahl" gespeichert
  int YJoy = analogRead(A0);

  if (XJoy < 600){
    XCoord += GESCHWINDIGKEIT;
  }
  else if(XJoy > 350){
    XCoord -= GESCHWINDIGKEIT;
  }

  if (YJoy > 600){
    YCoord += GESCHWINDIGKEIT;
  }
  else if(YJoy < 350){
    YCoord -= GESCHWINDIGKEIT;
  }

  if(YCoord < 0 || YCoord > 7 || XCoord < 0 || XCoord > 32){
    GameOver();
  }

  ZeichnePunkt();
}
