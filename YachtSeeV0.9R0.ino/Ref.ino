//String getADC(){
//  Serial.println("Collecting Analog data.");
//
//  double volts;
//  double amps;
//  int motion;
//  String adcPayload = "";
//
//  int16_t adc0, adc1, adc2, adc3;
//
//  //ads1115.setGain(GAIN_ONE);     // 1x gain   +/- 4.096V  1 bit = 2mV
//  ads1115.setGain(GAIN_TWO);     // 2x gain   +/- 2.048V  1 bit = 1mV
//
//  adc0 = ads1115.readADC_SingleEnded(0);
//  adc1 = ads1115.readADC_SingleEnded(1);
//  adc2 = ads1115.readADC_SingleEnded(2);
//  //adc3 = ads1115.readADC_SingleEnded(3);
//  //Serial.print("AIN0: "); Serial.println(adc0);
//  //Serial.print("AIN1: "); Serial.println(adc1);
//  //Serial.print("AIN2: "); Serial.println(adc2);
//  //Serial.print("AIN3: "); Serial.println(adc3);
//  //Serial.println(" ");
//  
//  volts = adc0*0.000685;
//  amps = adc1*0.76309;
//  motion = adc2;
//  
//  adcPayload += ", \"V\":";
//  adcPayload += volts;
//  adcPayload += ", \"BPA\":";
//  adcPayload += amps;
//  adcPayload += ", \"MotSen\":";
//  adcPayload += motion;
//  yield();
//  ESP.wdtFeed();
//  return adcPayload;
//}

