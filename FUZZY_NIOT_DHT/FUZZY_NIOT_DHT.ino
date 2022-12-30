#include <Fuzzy.h>
#include <DHT.h>

Fuzzy *fuzzy = new Fuzzy();
#define dhtpin    4
#define DHTTYPE   DHT11
DHT     dht(dhtpin, DHTTYPE);

float t;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  dht.begin();
  
  randomSeed(analogRead(4));    // Set a random seed

  FuzzyInput *temp = new FuzzyInput(1);     // Instantiating a FuzzyInput

  FuzzySet *cold = new FuzzySet(15, 20, 20, 25);  // Instantiating a FuzzySet
  FuzzySet *room = new FuzzySet(23, 27, 27, 30);
  FuzzySet *warm = new FuzzySet(28, 33, 33, 38);

  temp->addFuzzySet(cold);    // Including the FuzzySet into FuzzyInput
  temp->addFuzzySet(room);
  temp->addFuzzySet(warm);

  fuzzy->addFuzzyInput(temp);     // Including the FuzzyInput into Fuzzy

  FuzzyOutput *setAC = new FuzzyOutput(1);  // Instantiating a FuzzyOutput

  FuzzySet *setLow = new FuzzySet(16, 18, 18, 20);  // Instantiating a FuzzySet
  FuzzySet *setMid = new FuzzySet(19, 21, 21, 23);
  FuzzySet *setHot = new FuzzySet(22, 24, 24, 26);

  setAC->addFuzzySet(setLow);
  setAC->addFuzzySet(setMid);
  setAC->addFuzzySet(setHot);

  fuzzy->addFuzzyOutput(setAC);


  // Building FuzzyRule "IF temp = cold THEN setAC = hot"
  FuzzyRuleAntecedent *ifTempCold = new FuzzyRuleAntecedent();
  ifTempCold->joinSingle(cold);

  FuzzyRuleConsequent *thenSetHot = new FuzzyRuleConsequent();
  thenSetHot->addOutput(setHot);

  FuzzyRule *fuzzyRule01 = new FuzzyRule(1, ifTempCold, thenSetHot);
  fuzzy->addFuzzyRule(fuzzyRule01);


  // Building FuzzyRule "IF temp = room THEN setAC = mid"
  FuzzyRuleAntecedent *ifTempRoom = new FuzzyRuleAntecedent();
  ifTempRoom->joinSingle(room);

  FuzzyRuleConsequent *thenSetMid = new FuzzyRuleConsequent();
  thenSetMid->addOutput(setMid);

  FuzzyRule *fuzzyRule02 = new FuzzyRule(2, ifTempRoom, thenSetMid);
  fuzzy->addFuzzyRule(fuzzyRule02);


  // Building FuzzyRule "IF temp = warm THEN setAC = low"
  FuzzyRuleAntecedent *ifTempWarm = new FuzzyRuleAntecedent();
  ifTempWarm->joinSingle(warm);

  FuzzyRuleConsequent *thenSetLow = new FuzzyRuleConsequent();
  thenSetLow->addOutput(setLow);

  FuzzyRule *fuzzyRule03 = new FuzzyRule(3, ifTempWarm, thenSetLow);
  fuzzy->addFuzzyRule(fuzzyRule03);
}

void loop() {
  // put your main code here, to run repeatedly:

  //int input = random(18, 34);
  int temp = t;

  Serial.println("\n\n\nEntrance: ");
  Serial.print("\t\tTemperature: ");
  Serial.println(input);

  fuzzy->setInput(1, input);

  fuzzy->fuzzify();

  float output = fuzzy->defuzzify(1);

  Serial.println("Result: ");
  Serial.print("\t\tSet AC Temp: ");
  Serial.println(output);

  delay(12000);

}
