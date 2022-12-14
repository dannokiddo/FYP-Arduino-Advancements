#include <Fuzzy.h>

Fuzzy  *fuzzy = new Fuzzy();

void setup()
{
  Serial.begin(9600);

  randomSeed(analogRead(2));

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
  int input = random(18, 34);

  // Printing something
  Serial.println("\n\n\nEntrance: ");
  Serial.print("\t\t\tTemperature: ");
  Serial.println(input);
  // Set the random value as an input
  fuzzy->setInput(1, input);
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