#ifndef WEEKLYMENU_H
#define WEEKLYMENU_H

#include <string>
#include <vector>

#include "Recipe.h"

using namespace std;

class WeeklyMenu {
private:
  static const int DAYS_COUNT = 7;
  string dayNames[DAYS_COUNT];
  vector<Recipe> schedule[DAYS_COUNT];

  int dayIndex(string day);

public:
  WeeklyMenu();

  bool addRecipeToDay(string day, Recipe recipe);
  bool removeRecipeFromDay(string day, int index);
  vector<Recipe> getRecipesForDay(string day);
  string getDayName(int index);
  int getDaysCount();

  void displayWeeklyPlan();
  void clearDay(string day);
  void clearAll();
};

#endif
