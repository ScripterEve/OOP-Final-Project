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

void WeeklyMenu::generateShoppingList() {
  // Parallel vectors for aggregated ingredients
  vector<string> names;
  vector<double> quantities;
  vector<string> units;

  // Iterate all days and all recipes
  for (int i = 0; i < DAYS_COUNT; i++) {
    for (int j = 0; j < schedule[i].size(); j++) {
      vector<Ingredient> ings = schedule[i][j].getIngredients();
      for (int k = 0; k < ings.size(); k++) {
        string ingName = ings[k].getName();
        string ingUnit = ings[k].getUnit();
        double ingQty = ings[k].getQuantity();

        // Check if this name+unit combo already exists
        bool found = false;
        for (int m = 0; m < names.size(); m++) {
          if (names[m] == ingName && units[m] == ingUnit) {
            quantities[m] += ingQty;
            found = true;
            break;
          }
        }
        if (!found) {
          names.push_back(ingName);
          quantities.push_back(ingQty);
          units.push_back(ingUnit);
        }
      }
    }
  }

  cout << "==========================================" << endl;
  cout << "         SHOPPING LIST" << endl;
  cout << "==========================================" << endl;

  if (names.empty()) {
    cout << "  No meals planned yet." << endl;
    cout << "==========================================" << endl;
    return;
  }

  // Calculate column widths for alignment
  int nameWidth = 4; // minimum: "Name"
  int qtyWidth = 8;  // minimum: "Quantity"
  int unitWidth = 4;  // minimum: "Unit"

  for (int i = 0; i < names.size(); i++) {
    if ((int)names[i].size() > nameWidth)
      nameWidth = names[i].size();
    // Convert quantity to string to measure width
    string qtyStr = to_string(quantities[i]);
    // Trim trailing zeros after decimal point
    int dotPos = qtyStr.find('.');
    if (dotPos != (int)string::npos) {
      int last = qtyStr.size() - 1;
      while (last > dotPos && qtyStr[last] == '0') last--;
      if (last == dotPos) last--; // remove the dot too if no decimals
      qtyStr = qtyStr.substr(0, last + 1);
    }
    if ((int)qtyStr.size() > qtyWidth)
      qtyWidth = qtyStr.size();
    if ((int)units[i].size() > unitWidth)
      unitWidth = units[i].size();
  }

  // Add padding
  nameWidth += 2;
  qtyWidth += 2;

  // Print header
  cout << "  ";
  cout.width(nameWidth); cout << left << "Name";
  cout.width(qtyWidth);  cout << left << "Quantity";
  cout << "Unit" << endl;

  cout << "  ";
  for (int i = 0; i < nameWidth + qtyWidth + unitWidth; i++)
    cout << "-";
  cout << endl;

  // Print rows
  for (int i = 0; i < names.size(); i++) {
    // Format quantity: trim trailing zeros
    string qtyStr = to_string(quantities[i]);
    int dotPos = qtyStr.find('.');
    if (dotPos != (int)string::npos) {
      int last = qtyStr.size() - 1;
      while (last > dotPos && qtyStr[last] == '0') last--;
      if (last == dotPos) last--;
      qtyStr = qtyStr.substr(0, last + 1);
    }

    cout << "  ";
    cout.width(nameWidth); cout << left << names[i];
    cout.width(qtyWidth);  cout << left << qtyStr;
    cout << units[i] << endl;
  }

  cout << "==========================================" << endl;
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
