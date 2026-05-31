#ifndef WEEKLYMENU_H
#define WEEKLYMENU_H

#include <string>
#include <vector>

#include "Recipe.h"

using namespace std;

class RecipeManager;

class WeeklyMenu {
public:
  struct PlannedMeal {
    Recipe recipe;
    int targetServings;
  };

private:
  static const int DAYS_COUNT = 7;
  string dayNames[DAYS_COUNT];
  vector<PlannedMeal> schedule[DAYS_COUNT];

  int dayIndex(string day);

public:
  WeeklyMenu();

  bool addRecipeToDay(string day, Recipe recipe, int targetServings);
  bool removeRecipeFromDay(string day, int index);
  vector<PlannedMeal> getRecipesForDay(string day);
  string getDayName(int index);
  int getDaysCount();

  void saveToFile(string filename);
  void loadFromFile(string filename, RecipeManager& mgr);

  void displayWeeklyPlan();
  void generateShoppingList();
  void clearDay(string day);
  void clearAll();
};

#endif
