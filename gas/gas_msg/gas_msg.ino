#include <Time.h>
#include <TimeLib.h>
#include <SoftwareSerial.h>
/**
*   Constantes de la comunicacion GSM
*/
SoftwareSerial SIM900A(7,8);

/*
*   Constantes de la lectura de GAS
*/
const int MQ_PIN = A0;      // Pin del sensor
const int RL_VALUE = 5;     // Resistencia RL del modulo en Kilo ohms
const int R0 = 10;          // Resistencia R0 del sensor en Kilo ohms

// Datos para lectura multiple
const int READ_SAMPLE_INTERVAL = 100;    // Tiempo entre muestras
const int READ_SAMPLE_TIMES = 5;       // Numero muestras

// Ajustar estos valores para vuestro sensor según el Datasheet
// (opcionalmente, según la calibración que hayáis realizado)
const int THRESHOLD = 8000;
const float X0 = 200;
const float Y0 = 1.7;
const float X1 = 10000;
const float Y1 = 0.28;

// Puntos de la curva de concentración {X, Y}
const float point0[] = { log10(X0), log10(Y0) };
const float point1[] = { log10(X1), log10(Y1) };

// Calcular pendiente y coordenada abscisas
const float scope = (point1[1] - point0[1]) / (point1[0] - point0[0]);
const float coord = point0[1] - point0[0] * scope;

// Timer
bool bValidSendMessage = false;
bool bValidReceiveMessage = false;
int sizeMessage = 0;
const String PHONE = "+527441057042";
const String TEXT_MESSAGE = "Prueba Arduino";

void setup() {
    Serial.begin(9600);
    setTime(12,0,0,1,1,20);

    SIM900A.begin(9600);  
    Serial.println ("SIM900A Lista!!");
}

void loop() {
    float rs_med = readMQ();
    float concentration = getConcentration(rs_med/R0);
    
    // Mostrar el valor de la concentración por serial
    if( (concentration >= THRESHOLD) && (bValidSendMessage == false) ){
        sendMessage();
        bValidSendMessage = true;
    }else{
        if( (second() == 0) && (bValidSendMessage) )
            bValidSendMessage = false;
        
        Serial.print("Segundos: ");
        Serial.println(second());
        Serial.print("Concentración: ");
        Serial.println(concentration);
    }
    
    // recieveMessage();
}

/**
*   Envia un mensaje utilizando el modulo SIM900 que funciona con GMS
*/
void sendMessage() {
    Serial.println ("Enviando mensaje");
    SIM900A.println("AT+CMGF=1"); //Establece el modulo GSM en modo texto
    delay(1000);
    Serial.println ("Estableciendo el numero al que se enviara el mensaje");
    // SIM900A.println("AT+CMGS=\"+527441055861\"\r");
    SIM900A.println("AT+CMGS=\""+  PHONE + "\"\r");
    delay(1000);
    Serial.println("Estableciendo el contenido del mensaje");
    SIM900A.println(TEXT_MESSAGE);
    delay(100);
    Serial.println ("Finalizado");
    SIM900A.println((char)26);// Codigo ASCII para CTRL+Z
    delay(1000);
    Serial.println ("El mensaje ha sido enviado a este numero: "+ PHONE);
}

/**
*   Recibe un mensaje utilizando el modulo SIM900 que funciona con GMS
*/
void recieveMessage() {
    delay(100);
    while (Serial.available()){
      SIM900A.write(Serial.read());//Forward what Serial received to Software Serial Port
    }
    while(SIM900A.available()){
        Serial.write(SIM900A.read());//Forward what Software Serial received to Serial Port
        updateSettings();
    }
}

/**
*   Actualiza la configuracion consumiendo el Endpoint
*/
void updateSettings(){
    Serial.println("HTTP get method :");
    Serial.print("AT\\r\\n");
    SIM900A.println("AT");	/* Check Communication */
    delay(5000);
    ShowSerialData();	/* Print response on the serial monitor */
    delay(5000);
    /* Configure bearer profile 1 */
    Serial.print("AT+SAPBR=3,1,\"CONTYPE\",\"GPRS\"\\r\\n");		
    SIM900A.println("AT+SAPBR=3,1,\"CONTYPE\",\"GPRS\"");	/* Connection type GPRS */
    delay(5000);
    ShowSerialData();
    delay(5000);
    Serial.print("AT+SAPBR=3,1,\"APN\",\"internet.itelcel.com\"\\r\\n");	
    SIM900A.println("AT+SAPBR=3,1,\"APN\",\"internet.itelcel.com\"");	/* APN of the provider */
    delay(5000);
    ShowSerialData();
    delay(5000);
    Serial.print("AT+SAPBR=1,1\\r\\n");
    SIM900A.println("AT+SAPBR=1,1");	/* Open GPRS context */
    delay(5000);
    ShowSerialData();
    delay(5000);
    Serial.print("AT+SAPBR=2,1\\r\\n");
    SIM900A.println("AT+SAPBR=2,1");	/* Query the GPRS context */
    delay(5000);
    ShowSerialData();
    delay(5000);
    Serial.print("AT+HTTPINIT\\r\\n");
    SIM900A.println("AT+HTTPINIT"); /* Initialize HTTP service */
    delay(5000); 
    ShowSerialData();
    delay(5000);
    Serial.print("AT+HTTPPARA=\"CID\",1\\r\\n");
    SIM900A.println("AT+HTTPPARA=\"CID\",1");	/* Set parameters for HTTP session */
    delay(5000);
    ShowSerialData();
    delay(5000);
    Serial.print("AT+HTTPPARA=\"URL\",\"https://63e53871c04baebbcdb64c42.mockapi.io/api/v1/settings\"\\r\\n");
    SIM900A.println("AT+HTTPPARA=\"URL\",\"https://63e53871c04baebbcdb64c42.mockapi.io/api/v1/settings\"");	/* Set parameters for HTTP session */
    delay(5000);
    ShowSerialData();
    delay(5000);
    Serial.print("AT+HTTPACTION=0\\r\\n");
    SIM900A.println("AT+HTTPACTION=0");	/* Start GET session */
    delay(10000);
    ShowSerialData();
    delay(10000);
    Serial.print("AT+HTTPREAD\\r\\n");
    SIM900A.println("AT+HTTPREAD");	/* Read data from HTTP server */
    delay(8000);
    ShowSerialData();
    delay(8000);
    Serial.print("AT+HTTPTERM\\r\\n");  
    SIM900A.println("AT+HTTPTERM");	/* Terminate HTTP service */
    delay(5000);
    ShowSerialData();
    delay(5000);
    Serial.print("AT+SAPBR=0,1\\r\\n");
    SIM900A.println("AT+SAPBR=0,1"); /* Close GPRS context */
    delay(5000);
    ShowSerialData();
    delay(5000);
}

/**
*   Muestra la informacion de los datos recibidos desde el endpoint
*/
void ShowSerialData(){
  while(SIM900A.available()!=0)	/* If data is available on serial port */
  Serial.write(char (SIM900A.read()));	/* Print character received on to the serial monitor */
}

/**
*   Obtiene la lectura del sensor analogo en el pin especificado
*   @return Devuelve el promedio de las lecturas del sensor MQ en un intervalo de tiempo
*/
float readMQ() {
   float rs = 0;
   for (int i = 0;i<READ_SAMPLE_TIMES;i++) {
      rs += getMQResistance(analogRead(MQ_PIN));
      delay(READ_SAMPLE_INTERVAL);
   }
   return rs / READ_SAMPLE_TIMES;
}

/**
*   De la lectura analoga, se calcula la calidad del aire, basada en la resistencia
*   @return Devuelve un valor flotante del calculo de la calidad de aire
*/
float getMQResistance(int raw_adc) {
    return (((float)RL_VALUE / 1000.0*(1023 - raw_adc) / raw_adc));
}

/**
*   Obtiene la concentracion de gas, basado en la formula 10^(coord + scope * log (rs/r0)
*   @return Devuelve un valor flotante del calculo de la concentracion
*/
float getConcentration(float rs_ro_ratio) {
   return pow(10, coord + scope * log(rs_ro_ratio));
}