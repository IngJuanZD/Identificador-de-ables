/*IDC  2020
   (Identificador de cableado)
   Este proyecto esta dividiodo en dos diferentes arduinos
   Maestro y Esclavo

   Maestro:
   Este solo se encargara de seleccionar el numero de lineas a identificar
   el cual devera ser el mismo en el esclavo, se controla por 3 botones:
   [bMas] [bMenos]  [bInico]

   [bMas] Aumentar el numero de salidas
   [bMenos] Disminuye el numero de salidas
   [bInico] Inicia la secuencia

   Simplemente se envia una secuencia asendente que sera la referencia,
   para el orden de los cables, el esclavo se encargara de leer las entradas
   y dar el orden corrento segun esten los cables conecatdo.

   Ejemplo:
   Master [ 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13]
   [ El oden de los cables del esclavo dependera de como se conecte ejemplo]
   Esclavo [ 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13]==>(entradas de conección)
   y debera mandar la secunecia correcta para ordenarlos cables
   Esclavo [ 3, 6, 7, 9, 5, 11 , 10, 2, 4, 8, 13, 12]==>(orden correcto)

*/


#include <Wire.h>
#include "DFRobot_LCD.h"

//Numero de salidas.
int pinSalida[12] = { 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13};

//Botons de control.
const int bMas = A0;
const int bMenos = A1;
const int bInico = A2;

//Configuracion de LCD.
DFRobot_LCD lcd(16, 2);

//Simbolos personalisados para LCD.
byte SyNum[8] = {
  0b11111,
  0b10001,
  0b10001,
  0b11111,
  0b00000,
  0b00000,
  0b00000,
  0b00000
};

// Variable Auxuliar.
int contador = 0;

//Botones de control [+],[-].
int estado_bMas = 0;
int ultimo_estado_bMas = 0;
int estado_bMenos = 0;
int ultimo_estado_bMenos = 0;
int estado_bInico = 0;
int ultimo_estado_bInico = 0;

void setup() {
  //Declaracion de simbolo(s).
  lcd.customSymbol(0, SyNum);

  //Se puede eliminar el uso del Puerto Serial.
  Serial.begin(9600);

  pinMode(bMas, INPUT);
  Serial.print ("Pin: ");
  Serial.print (bMas);
  Serial.println (" como Entrada");

  pinMode(bMenos, INPUT);
  Serial.print ("Pin: ");
  Serial.print (bMenos);
  Serial.println (" como Entrada");

  pinMode(bInico, INPUT);
  Serial.print ("Pin: ");
  Serial.print (bInico);
  Serial.println (" como Entrada");

  //Se inicia la LCD.
  lcd.init();

  //Mensaje de entrada/presentacion.
  lcd.setCursor(4, 0);
  lcd.print("IDC  2020");
  lcd.setCursor(0, 1);
  lcd.print("LOADING");
  for (int i = 7 ; i <= 15; i++) {
    lcd.setCursor(i, 1);
    lcd.print(".");
    delay(100);
  }

  for (int i = 0; i < 12; i++) {
    pinMode(pinSalida[i], OUTPUT);
    Serial.print ("Pin: ");
    Serial.print (pinSalida[i]);
    Serial.println (" como Salida");
  }

  lcd.setCursor(0, 0);    //se posiciona el cursor en la LCD.
  lcd.print("HECHO EN MEXICO");
  lcd.setCursor(0, 1);
  lcd.print("@ing.juan.z");
  delay(2000);
  lcd.clear();            //limpia la pantalla

  lcd.setCursor(0, 0);    //se posiciona el cursor en la LCD.
  lcd.print("MAESTRO");
  delay(1000);
  lcd.clear();            //limpia la pantalla

}

void loop() {
  lcd.setCursor(0, 0);
  lcd.print("N");
  lcd.write((unsigned char)0);    //inserta el symbolo personalisados
  lcd.print(" de salidas:");
  lcd.print(contador);


  estado_bMas = digitalRead(bMas);
  Serial.print("Valor de Boton mas: "); //Se puede borrar
  Serial.println(estado_bMas);
  Serial.print("Ultimo Valor de Boton mas: ");
  Serial.println(ultimo_estado_bMas);

  estado_bMenos = digitalRead(bMenos);
  Serial.print("Valor de Boton menos: ");
  Serial.println(estado_bMenos);
  Serial.print("Ultimo Valor de Boton menos: ");
  Serial.println(ultimo_estado_bMenos);

  estado_bInico = digitalRead(bInico);
  Serial.print("Valor de Boton inico: ");
  Serial.println(estado_bInico);
  Serial.print("Ultimo Valor de Boton Inico: ");
  Serial.println(ultimo_estado_bInico);

  /////////////////////////////////////////////////
  //bMas
  if (estado_bMas != ultimo_estado_bMas) {
    if (contador >= 12) {
      Serial.print("Todas las salidas activadas: ");
      Serial.println(contador);
      Serial.print("pin: ");
      Serial.println(pinSalida[contador - 1]);
      estado_bMas = 0;
      lcd.setCursor(0, 1);
      lcd.print("ALL OUTPUTS ON");
      //delay(5000);
    }
    if (estado_bMas == 1) {
      if (contador < 12) {
        //digitalWrite(pinSalida[contador], HIGH);
        Serial.print("Prender pin: ");
        Serial.println(pinSalida[contador]);
        contador++;
        Serial.print("Contador en: ");
        Serial.println(contador);
        lcd.setCursor(0, 1);
        lcd.print("BOTON [+]       ");
        delay(100);
        lcd.setCursor(0, 1);
        lcd.print("                ");
      }
    }
  }
  /////////////////////////////////////////////////
  //bMenos
  if (estado_bMenos != ultimo_estado_bMenos) {
    if (contador <= 0) {
      Serial.print("Salidas desactivadas");
      Serial.println(contador);
      estado_bMenos = 0;
      lcd.setCursor(0, 1);
      lcd.print("ALL OUTPUTS OFF");
    }
    if (estado_bMenos == 1) {
      //digitalWrite(pinSalida[contador - 1], LOW);
      Serial.print("Apaga pin: ");
      Serial.println(contador + 1);
      contador--;
      Serial.print("Contador en: ");
      Serial.println(contador);
      lcd.setCursor(15, 0);
      lcd.print(" ");
      lcd.setCursor(0, 1);
      lcd.print("BOTON [-]       ");
      delay(100);
      lcd.setCursor(0, 1);
      lcd.print("                ");
    }
  }
  /////////////////////////////////////////////////
  // bInicio
  if (estado_bInico != ultimo_estado_bInico) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("ENVIANDO ----> ");
    for (int i = 0; i < contador; i++) {
      lcd.setCursor(0, 1);
      lcd.print("Salida ");
      lcd.print(i + 1);
      digitalWrite(pinSalida[i], HIGH);
      delay(1000);
      digitalWrite(pinSalida[i], LOW);
    }
    lcd.clear();
  }
}
