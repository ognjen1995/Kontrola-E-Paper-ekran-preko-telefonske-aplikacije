//Kontrolisanje e-Paper ekran, za prikaz slike i teksta
#include <GxEPD2_BW.h>
//Dodavanje grafičke funkcionalnosti tekstu ili slici na ekranu, poput fonta, crtanje oblika i slično
#include <Adafruit_GFX.h>
//Kreiranje dodatnih serijskih portova za komunikaciju sa  Bluetooth uređajem
#include <SoftwareSerial.h>
//Fontovi za prikaz teksta na ekranu
#include <Fonts/FreeSansBold9pt7b.h>
#include <Fonts/FreeSerifBold12pt7b.h>
#include <Fonts/FreeSerifBold24pt7b.h>

//Definicija e-Paper ekrana sa smanjenim međuspremnikom radi manje upotrebe memorije
GxEPD2_BW<GxEPD2_213_B73, GxEPD2_213_B73::HEIGHT / 4> display(GxEPD2_213_B73(10, 9, 8, 7));
//Bluetooth modul
SoftwareSerial Bluetooth(2, 3);
//Zadnja prikazana poruka
String lastMessage = "";

//Podešavanje prilikom pokretanja
void setup() {
  //Omogućuje serijsku komunikaciju između Arduina i računara putem USB-a
  Serial.begin(9600);
  //Omogućava komunikaciju između Arduina i Bluetooth uređaja putem softverskog serijskog porta
  Bluetooth.begin(9600);
  //Inicijalizacija e-Paper ekrana
  display.init();
  display.setRotation(1);
  display.setTextColor(GxEPD_BLACK);
  // Prikaz početne poruke "START"
  displayMessage("START");
  lastMessage = "START";
  Serial.println("Bluetooth povezan. Sistem spreman.");
}

//Podešavanje rada programa, sve ono što će se izvršavati do završetka rada programa
void loop() {
  //Provjera da li je stigla komanda preko Bluetooth-a
  if (Bluetooth.available()) {
    String command = Bluetooth.readStringUntil('\n'); //Čitanje komande u vidu stringa 
    command.trim(); //Uklanjanje nepotrebnih razmaka ili novih redova
    Serial.println("Primljena komanda: " + command);
    //Odlučivanje koja poruka će biti prikazana
    String messageToShow = "";
    if (command == "1") {
      messageToShow = "OPCIJA 1";
    } else if (command == "2") {
      messageToShow = "OPCIJA 2";
    } else if (command == "3") {
      messageToShow = "OPCIJA 3";
    } else if (command == "4") {
      messageToShow = "OPCIJA 4";
    } else if (command == "5") {
      messageToShow = "OPCIJA 5";
    } else if (command == "6") {
      messageToShow = "OPCIJA 6";
    } else if (command == "A") {
      messageToShow = "APEIRON";
    }else if (command == "IOT") {
      messageToShow = "IOT";
    }else if (command == "+DISC:SUCCESS") {
      messageToShow = "UCITANO";
    }else if (command == "OK") {
      messageToShow = "OK";
    } else {
      messageToShow = command;
    }
    // Prikaz poruke samo ako je različita od prethodne
    if (messageToShow != "" && lastMessage != messageToShow) {
      //Funkcija koja prosljeđen string postavlja na odredjeno mjesto na ekranu pa zatim prikazuje
      displayMessage(messageToShow.c_str());
      lastMessage = messageToShow; //Ažuriranje poslednje prikazane poruke
    }
  }
}
//Funkcija za prikaz poruke na e-Paper ekranu
void displayMessage(const char* message) {
  display.setFullWindow(); // Postavljanje cijelog ekrana za prikaz
  display.firstPage();
  do {
    display.clearScreen();//Čišćenje prethodne poruke sa ekrana
    display.fillScreen(GxEPD_WHITE); //Postavljanje bijele pozadine
    //display.setFont(&FreeSansBold9pt7b); //Korišćenje malog fonta
    //display.setFont(&FreeSerifBold12pt7b); //Korišćenje srednjeg fonta
    display.setFont(&FreeSerifBold24pt7b); //Korišćenje velikog fonta
    // Proračunavanje dimenzija za centriranje teksta
    int16_t x1, y1; //Koordinate gornjeg lijevog ugla teksta
    uint16_t w, h;  //Širina i visina teksta
    display.getTextBounds(message, 0, 0, &x1, &y1, &w, &h);
    // Izračunavanje pozicije za centriranje
    int16_t centerX = (display.width() - w) / 2;
    int16_t centerY = (display.height() + h) / 2;
    display.setCursor(centerX, centerY); //Postavljanje kursora na centriranu poziciju
    display.print(message); //Prikaz poruke
  } while (display.nextPage());
}
