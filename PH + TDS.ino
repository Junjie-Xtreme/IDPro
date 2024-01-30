#define TdsSensorPin A1
#define VREF 5.0   
#define SCOUNT  30       
#define SensorPin A0 
#define Offset -6.70
int analogBufferTDS[SCOUNT];   
int analogBufferTemp[SCOUNT];
int analogBufferIndex = 0, copyIndex = 0;
float averageVoltage = 0, tdsValue = 0, temperature = 25;
unsigned long int avgValue;

void setup()
{
  Serial.begin(9600);
  pinMode(TdsSensorPin, INPUT);
  pinMode(SensorPin, INPUT);
}

void loop()
{
  static unsigned long analogSampleTimepoint = millis();
  if (millis() - analogSampleTimepoint > 40U) 
  {
    analogSampleTimepoint = millis();
    analogBufferTDS[analogBufferIndex] = analogRead(TdsSensorPin); 
    analogBufferIndex++;
    if (analogBufferIndex == SCOUNT)
      analogBufferIndex = 0;
  }
  static unsigned long printTimepoint = millis();
  if (millis() - printTimepoint > 800U)
  {
    printTimepoint = millis();
    for (copyIndex = 0; copyIndex < SCOUNT; copyIndex++)
      analogBufferTemp[copyIndex] = analogBufferTDS[copyIndex];
    averageVoltage = getMedianNum(analogBufferTemp, SCOUNT) * (float)VREF / 1024.0; 
    float compensationCoefficient = 1.0 + 0.02 * (temperature - 25.0); 
    float compensationVolatge = averageVoltage / compensationCoefficient; 
    tdsValue = (133.42 * compensationVolatge * compensationVolatge * compensationVolatge - 255.86 * compensationVolatge * compensationVolatge + 857.39 * compensationVolatge) * 0.5;
//    Serial.print("TDS----Value:");
//    Serial.print(tdsValue, 1);
//    Serial.println("ppm");
//    Serial.println(" ");
//    Serial.print("TDS----Voltage:");
//    Serial.print(compensationVolatge, 1);
//    Serial.println("V");
  }
  int buf[10]; 
  for(int i=0;i<10;i++) 
  {
    buf[i]=analogRead(SensorPin); 
    delay(10);
  }
  for(int i=0;i<9;i++) 
  {
    for(int j=i+1;j<10;j++)
    {
      if(buf[i]>buf[j]) {
        int temp=buf[i];
        buf[i]=buf[j];
        buf[j]=temp;
      }
    }
  }
  avgValue=0;
  for(int i=2;i<8;i++){
    avgValue+=buf[i];
  }
  float phValue=(float)avgValue*5.0/1024/6; 
  float phValueAdj = -4.0008 * pow(phValue, 6) + 78.312 * pow(phValue, 5) - 612.88 * pow(phValue, 4) + 2411.5 * pow(phValue, 3) - 4852.3 * pow(phValue, 2) + 4296.5 * phValue - 786.88;
  Serial.print("PH----Volt:");
  Serial.print(phValue); 
  Serial.println(" "); 
  Serial.print("PH----ValueAdj:");
  Serial.print(phValueAdj, 2); 
  Serial.println(" "); 
  Serial.println("\n"); 
  delay(800);
}
int getMedianNum(int bArray[], int iFilterLen)
{
  int bTab[iFilterLen];
  for (byte i = 0; i < iFilterLen; i++)
    bTab[i] = bArray[i];
  int i, j, bTemp;
  for (j = 0; j < iFilterLen - 1; j++)
  {
    for (i = 0; i < iFilterLen - j - 1; i++)
    {
      if (bTab[i] > bTab[i + 1])
      {
        bTemp = bTab[i];
        bTab[i] = bTab[i + 1];
        bTab[i + 1] = bTemp;
      }
    }
  }
  if ((iFilterLen & 1) > 0)
    bTemp = bTab[(iFilterLen - 1) / 2];
  else
    bTemp = (bTab[iFilterLen / 2] + bTab[iFilterLen / 2 - 1]) / 2;
  return bTemp;
}
