#include <SoftwareSerial.h>

#define Pin_Sensor 3

SoftwareSerial serial1(10, 11); // RX, TX

long int qtdVibracao = 0;
long int TempoInicial = 0;
long int TempoMedicao = 1000; // 1 Segundo
String strComando = "";
String strValor = "";


void setup() {
  Serial.begin(9600);
  serial1.begin(9600);

  pinMode(Pin_Sensor, INPUT);
  attachInterrupt(digitalPinToInterrupt(Pin_Sensor), Intensidade, RISING);

  TempoInicial= millis();
}

void loop() {
  enviarBlutooth();
  lerComando();
  if(strComando == "t_medicao"){
    alteraTempoMedicao(strValor.toInt());
    strComando = "";
    strValor = "";
  }
}

void enviarBlutooth() {
  if((millis() - TempoInicial) >= TempoMedicao && qtdVibracao) {
    serial1.print(qtdVibracao);
    serial1.print("/");
    serial1.print(TempoMedicao/1000);
    serial1.println("s");

    Serial.print(qtdVibracao);
    Serial.print("/");
    Serial.print(TempoMedicao/1000);
    Serial.println("s");
    
    qtdVibracao = 0;
    TempoInicial = millis();
  }
}

void Intensidade() {
  if(digitalRead(Pin_Sensor)) {
    qtdVibracao+= 1;
  }
}

void lerComando() {
  bool stComando = false;
  serial1.listen();
  while (serial1.available() > 0) {
    char inChar = (char)serial1.read();
    
    if(!stComando){
      if(inChar == ' ')
        stComando = true;
      else
        strComando+= inChar;
    }
    else{
      strValor+= inChar;
    }
  }
}

void alteraTempoMedicao(long int valor){
  TempoMedicao = valor * 1000;
  Serial.print("Mudança Tempo de Medição: ");
  Serial.print(valor);
  Serial.println("s");

  serial1.print("Mudança Tempo de Medição: ");
  serial1.print(valor);
  serial1.println("s");
}
