#include "WeeklyMenu.h"
#include <iostream>

using namespace std;

WeeklyMenu::WeeklyMenu() {
  dayNames[0] = "Monday";
  dayNames[1] = "Tuesday";
  dayNames[2] = "Wednesday";
  dayNames[3] = "Thursday";
  dayNames[4] = "Friday";
  dayNames[5] = "Saturday";
  dayNames[6] = "Sunday";
}

int WeeklyMenu::dayIndex(string day) {
  for (int i = 0; i < DAYS_COUNT; i++) {
    if (dayNames[i] == day)
      return i;
  }
  return -1;
}

bool WeeklyMenu::addRecipeToDay(string day, Recipe recipe) {
  int idx = dayIndex(day);
  if (idx == -1) return false;
  schedule[idx].push_back(recipe);
  return true;
}

bool WeeklyMenu::removeRecipeFromDay(string day, int index) {
  int idx = dayIndex(day);
  if (idx == -1) return false;
  if (index < 0 || index >= (int)schedule[idx].size()) return false;
  schedule[idx].erase(schedule[idx].begin() + index);
  return true;
}

vector<Recipe> WeeklyMenu::getRecipesForDay(string day) {
  int idx = dayIndex(day);
  if (idx == -1) return vector<Recipe>();
  return schedule[idx];
}

string WeeklyMenu::getDayName(int index) {
  if (index >= 0 && index < DAYS_COUNT)
    return dayNames[index];
  return "";
}

int WeeklyMenu::getDaysCount() {
  return DAYS_COUNT;
}

void WeeklyMenu::displayWeeklyPlan() {
  cout << "==========================================" << endl;
  cout << "         WEEKLY MENU PLAN" << endl;
  cout << "==========================================" << endl;
  for (int i = 0; i < DAYS_COUNT; i++) {
    cout << endl << "  " << dayNames[i] << ":" << endl;
    if (schedule[i].empty()) {
      cout << "    (no meals planned)" << endl;
    } else {
      for (int j = 0; j < schedule[i].size(); j++) {
        cout << "    " << (j + 1) << ". " << schedule[i][j].getName()
             << " [" << schedule[i][j].getCategory() << "]" << endl;
      }
    }
  }
  cout << endl << "==========================================" << endl;
}

void WeeklyMenu::clearDay(string day) {
  int idx = dayIndex(day);
  if (idx != -1)
    schedule[idx].clear();
}

void WeeklyMenu::clearAll() {
  for (int i = 0; i < DAYS_COUNT; i++)
    schedule[i].clear();
}
