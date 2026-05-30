#ifndef RECIPE_H
#define RECIPE_H

#include <string>
#include <vector>

#include "Ingredient.h"
#include "RecipeBase.h"

using namespace std;

class Recipe : public RecipeBase {
private:
  vector<Ingredient> ingredients;
  vector<string> steps;

public:
  Recipe(string n, int s, string c);

  void addIngredient(Ingredient ing);
  void removeIngredient(string ingName);
  vector<Ingredient> getIngredients();

  void addStep(string step);
  void removeStep(int index);
  vector<string> getSteps();

  Recipe scaleServings(int newServings);

  double getTotalCalories();
  double getTotalProtein();
  double getTotalCarbs();
  double getTotalFat();

  void display() override;
  void exportToFile(string filename);
};

#endif
