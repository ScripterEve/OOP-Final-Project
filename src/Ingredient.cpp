#include "Ingredient.h"
#include <iostream>

using namespace std;

Ingredient::Ingredient(string n, double q, string u, double cal, double p, double c, double f)
    : name(n), quantity(q), unit(u), calories(cal), protein(p), carbs(c), fat(f) {}

string Ingredient::getName() { return name; }
double Ingredient::getQuantity() { return quantity; }
string Ingredient::getUnit() { return unit; }
double Ingredient::getCalories() { return calories; }
double Ingredient::getProtein() { return protein; }
double Ingredient::getCarbs() { return carbs; }
double Ingredient::getFat() { return fat; }

Ingredient Ingredient::scale(double factor) {
  return Ingredient(name, quantity * factor, unit, calories * factor,
                    protein * factor, carbs * factor, fat * factor);
}

void Ingredient::display() {
  cout << "  - " << name << ": " << quantity << " " << unit;
  if (calories > 0)
    cout << " (" << calories << " kcal)";
  cout << endl;
}
