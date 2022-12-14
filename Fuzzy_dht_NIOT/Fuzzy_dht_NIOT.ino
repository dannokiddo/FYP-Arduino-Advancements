#include <Fuzzy.h>
#include <DHT.h>

Fuzzy   *fuzzy = new Fuzzy();
#define dhtpin      4 
#define DHTTYPE DHT11
DHT     dht(dhtpin, DHTTYPE);      // DHT

float t;

void setup()
{
  Serial.begin(9600);
  dht.begin();

  randomSeed(digitalRead(4));

  FuzzyInput *temp = new FuzzyInput(1);

  FuzzySet *low = new FuzzySet(18, 21, 21, 24);
  FuzzySet *mid = new FuzzySet(23, 26, 26, 29);
  FuzzySet *high = new FuzzySet(28, 31, 31, 34);

  temp -> addFuzzySet(low);
  temp -> addFuzzySet(mid);
  temp -> addFuzzySet(high);

  fuzzy -> addFuzzyInput(temp);

  FuzzyOutput *tempOut = new FuzzyOutput(1);

  FuzzySet *lowSet = new FuzzySet(16, 18, 18, 20);
  FuzzySet *midSet = new FuzzySet(19, 21, 21, 23);
  FuzzySet *highSet = new FuzzySet(22, 24, 24, 26);

  tempOut -> addFuzzySet(lowSet);
  tempOut -> addFuzzySet(midSet);
  tempOut -> addFuzzySet(highSet);

  fuzzy -> addFuzzyOutput(tempOut);

  // Rule 01 - if low, then set high
  FuzzyRuleAntecedent *ifTempLow = new FuzzyRuleAntecedent();
  ifTempLow -> joinSingle(low);

  FuzzyRuleConsequent *thenSetHigh = new FuzzyRuleConsequent();
  thenSetHigh -> addOutput(highSet);

  FuzzyRule *fuzzyRule01 = new FuzzyRule(1, ifTempLow, thenSetHigh);
  fuzzy -> addFuzzyRule(fuzzyRule01);

  // Rule 02 - if mid, then set mid
  FuzzyRuleAntecedent *ifTempMid = new FuzzyRuleAntecedent();
  ifTempLow -> joinSingle(mid);

  FuzzyRuleConsequent *thenSetMid = new FuzzyRuleConsequent();
  thenSetHigh -> addOutput(midSet);

  FuzzyRule *fuzzyRule02 = new FuzzyRule(2, ifTempMid, thenSetMid);
  fuzzy -> addFuzzyRule(fuzzyRule02);

  // Rule 03 - if high, then set low
  FuzzyRuleAntecedent *ifTempHigh = new FuzzyRuleAntecedent();
  ifTempLow -> joinSingle(high);

  FuzzyRuleConsequent *thenSetLow = new FuzzyRuleConsequent();
  thenSetHigh -> addOutput(lowSet);

  FuzzyRule *fuzzyRule03 = new FuzzyRule(3, ifTempHigh, thenSetLow);
  fuzzy -> addFuzzyRule(fuzzyRule03);

}

void loop()
{
  dhtsense();
  //int input = digitalRead(2);//random(18, 34);
  int temp = t;

  // Printing something
  Serial.println("\n\n\nEntrance: ");
  Serial.print("\t\t\tTemperature: ");
  Serial.println(temp);
  // Set the random value as an input
  fuzzy->setInput(1, temp);
  // Running the Fuzzification
  fuzzy->fuzzify();
  // Running the Defuzzification
  float output = fuzzy->defuzzify(1);
  // Printing something
  Serial.println("Result: ");
  Serial.print("\t\t\tTempSet: ");
  Serial.println(output);
  // wait 12 seconds
  delay(12000);
}

//##################################################################################
void dhtsense() { // Temperature & Humidity
  // temp & humid //readcan take 250ms -  2s
  float h = dht.readHumidity();
  t = dht.readTemperature();
  float f = dht.readTemperature(true);
  
  if (isnan(h) || isnan(t) || isnan(f)) { //if read fail then exit
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }

  float hic = dht.computeHeatIndex(t, h, false); //false->notFarhrenheit = Celcius

  Serial.println("Humidity    : " + (String)h + " %");
  //Serial.print("    ");
  Serial.println("Temperature : " + (String)t + " °C");
  //Serial.println("Heat Index  : " + (String)hic + " °C \n");
  delay(500);

  /*Blynk.virtualWrite(V1, h);  //Blynk Input
  Blynk.virtualWrite(V2, t);*/
  return;
}