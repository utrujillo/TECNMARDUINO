#define         MQ1                       (0)     //define la entrada analogica para el sensor
#define         RL_VALOR             (5)     //define el valor de la resistencia mde carga en kilo ohms
#define         RAL       (9.83)  // resistencia del sensor en el aire limpio / RO, que se deriva de la                                             tabla de la hoja de datos
#define         GAS_LP                      (0)
String inputstring = "";                                                        //Cadena recibida desde el PC
float           LPCurve[3]  =  {2.3,0.21,-0.47};
float           Ro           =  10;
byte     estado = 0;

void setup()
{
    Serial.begin(9600);                                                                  //Inicializa Serial a 9600 baudios
    Serial.println("Iniciando ...");
    //configuracion del sensor
    Serial.print("Calibrando...\n");
    Ro = Calibracion(MQ1);                        //Calibrando el sensor. Por favor de asegurarse que el sensor se encuentre en una zona de aire limpio mientras se calibra
    Serial.print("Calibracion finalizada...\n");
    Serial.print("Ro=");
    Serial.print(Ro);
    Serial.print("kohm");
    Serial.print("\n");
}
 
void loop()
{
    Serial.print("LP:");
    int gas_ppm = porcentaje_gas(lecturaMQ(MQ1)/Ro,GAS_LP);    
    Serial.print( gas_ppm );
    Serial.print( "ppm" );
    Serial.print("    ");
    Serial.print("\n");
    if( gas_ppm > 0 ){
        Serial.print("Gas detectado!!! encender alarma");
    }

    // if  (porcentaje_gas(lecturaMQ(MQ1)/Ro,GAS_LP)  > 0)
      // zumba(1);
    //  else
    //   zumba(0);
   
    delay(200);
}
 
float calc_res(int raw_adc)
{
    return ( ((float)RL_VALOR*(1023-raw_adc)/raw_adc));
}
 
float Calibracion(float mq_pin)
{
    int i;
    float val=0;
    for (i=0;i<50;i++) {                                                                               //tomar múltiples muestras
        val += calc_res(analogRead(mq_pin));
        delay(500);
    }
    val = val/50;                                                                                         //calcular el valor medio
    val = val/RAL;
    return val;
}
 
float lecturaMQ(int mq_pin)
{
    int i;
    float rs=0;
    for (i=0;i<5;i++)
    {
        rs += calc_res(analogRead(mq_pin));
        delay(50);
    }
    rs = rs/5;
    return rs;
}
 
int porcentaje_gas(float rs_ro_ratio, int gas_id)
{
    if ( gas_id == GAS_LP )
        return calcula_porcentaje_gas(rs_ro_ratio,LPCurve);
    return 0;
}
 
int calcula_porcentaje_gas(float rs_ro_ratio, float *pcurve)
{
    return (pow(10, (((log(rs_ro_ratio)-pcurve[1])/pcurve[2]) + pcurve[0])));
}

// Fin
// byte zumba(estado)
// {
//   if estado == 1
//       analogWrite (5, 250); delay(20);
//   else
//       analogWrite(5, 0); 
// }
