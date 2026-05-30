#ifndef PANTRY_H
#define PANTRY_H

#include <string>
#include <vector>

#include "Ingredient.h"

using namespace std;

class Pantry {
private:
  vector<string> names;
  vector<double> quantities;
  vector<string> units;

public:
  void addItem(string name, double quantity, string unit);
  bool removeItem(string name);
  bool hasEnough(string name, double quantity, string unit);
  double getQuantity(string name, string unit);
  int getCount();

  void displayPantry();
  void clear();
};

#endif
