#ifndef INGREDIENT_H
#define INGREDIENT_H

#include <string>

using namespace std;

class Ingredient {
private:
  string name;
  double quantity;
  string unit;
  double calories;
  double protein;
  double carbs;
  double fat;

public:
  Ingredient(string n, double q, string u, double cal = 0, double p = 0, double c = 0, double f = 0);

  string getName();
  double getQuantity();
  string getUnit();
  double getCalories();
  double getProtein();
  double getCarbs();
  double getFat();

  Ingredient scale(double factor);
  void display();
};

#endif
