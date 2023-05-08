// El numero es 7442854134

// #include <SoftwareSerial.h> //Se incluye la librería de "SoftwareSerial"
// SoftwareSerial sim900(7, 8); // Se define los pines para la comunicación serial 
// char response = ' ';

// void setup() {
// 	// sim900.begin(9600);   // Configuración de la velocidad en baudios del módulo SIM900
//     // Serial.begin(9600);    // Configuración de la velocidad en baudios del monitor serie (Arduino)
    
// 	// start th serial communication with the host computer
// 	Serial.begin(9600);
// 	while(!Serial);
// 	Serial.println("Arduino with SIM900 is ready");
// 	// start communication with the SIM900A in 9600
// 	sim900.begin(9600); 
// 	Serial.println("SIM900 started at 9600");
// 	delay(1000);
// 	Serial.println("Setup Complete! SIM900 is Ready!");
// }

// void loop () {
// 	// Keep reading from SIM800 and send to Arduino Serial Monitor
// 	if (sim900.available()){ 
// 		response = sim900.read();
// 		Serial.write(c);
// 	}
// 	// Keep reading from Arduino Serial Monitor and send to sim900
// 	if (Serial.available()){ 
// 		c = Serial.read();
// 		sim900.write(c); 
// 	}
// }

// void loop() {
// 	// if (Serial.available()>0) //  Verifica si hay datos en el puerto serie de Arduino.
//    	// switch(Serial.read()){
// 	// 	case 's': // Si hay algo en el monitor serie, si es una letra "s" se ejecutara los comando AT que se definen en la funcion "envio_sms"
// 	// 		envio_sms();
// 	// 		break;
// 	// 	case 'c': // Pero si una letra "c" se ejecutara los comando AT que se definen en la función "hacer_llamada"
// 	// 		hacer_llamada();
// 	// 		break;
// 	// }
// 	if (sim900.available()>0)
// 		Serial.write(sim900.read()); // Imprime los datos recopilados del puerto serie
// }

// void envio_sms() {
// 	Serial.println("Enviado sms...");
// 	sim900.print("AT+CMGF=1\r"); // Establecer en modo envió de sms
// 	delay(300);
// 	/* Se le asigna el numero, debes cambiar la clave de acuerdo al país si eres de México 
// 	solo agrega el numero al que enviaras el sms */
// 	sim900.print("AT+CMGS=\"+527441434451\"\r"); 
// 	delay(600);
// 	sim900.print("Hola, Saludos de TI ISC"); // Contenido que llevara el sms
// 	sim900.print("\r"); 
// 	delay(500);
// 	sim900.print((char)26);
// 	delay(300);
// 	sim900.println();
// 	Serial.println("SMS Enviado...");
// 	delay(500);
// }