#include "WeeklyMenu.h"
#include <iostream>
#include <algorithm>
#include <cctype>
#include <fstream>
#include <iomanip>
#include "Exceptions.h"
#include "RecipeManager.h"

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

bool WeeklyMenu::addRecipeToDay(string day, Recipe recipe, int targetServings) {
  int idx = dayIndex(day);
  if (idx == -1) return false;
  schedule[idx].push_back({recipe, targetServings});
  return true;
}

bool WeeklyMenu::removeRecipeFromDay(string day, int index) {
  int idx = dayIndex(day);
  if (idx == -1) return false;
  if (index < 0 || index >= (int)schedule[idx].size()) return false;
  schedule[idx].erase(schedule[idx].begin() + index);
  return true;
}

vector<WeeklyMenu::PlannedMeal> WeeklyMenu::getRecipesForDay(string day) {
  int idx = dayIndex(day);
  if (idx == -1) return vector<PlannedMeal>();
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
      for (int j = 0; j < (int)schedule[i].size(); j++) {
        cout << "    " << (j + 1) << ". " << schedule[i][j].recipe.getName()
             << " [" << schedule[i][j].recipe.getCategory() << "] - " 
             << schedule[i][j].targetServings << " servings" << endl;
      }
    }
  }
  cout << endl << "==========================================" << endl;
}

void WeeklyMenu::saveToFile(string filename) {
  ofstream file(filename);
  if (!file.is_open()) {
    throw FileLoadException("Failed to open file for saving: " + filename);
  }
  for (int i = 0; i < DAYS_COUNT; i++) {
    file << schedule[i].size() << endl;
    for (int j = 0; j < (int)schedule[i].size(); j++) {
      file << schedule[i][j].recipe.getName() << "|" << schedule[i][j].targetServings << endl;
    }
  }
  file.close();
}

void WeeklyMenu::loadFromFile(string filename, RecipeManager& mgr) {
  ifstream file(filename);
  if (!file.is_open()) return;
  try {
    string line;
    for (int i = 0; i < DAYS_COUNT; i++) {
      if (!getline(file, line)) break;
      int count = stoi(line);
      for (int j = 0; j < count; j++) {
        if (!getline(file, line)) break;
        int p = line.find('|');
        if (p != (int)string::npos) {
          string recipeName = line.substr(0, p);
          int servings = stoi(line.substr(p + 1));
          try {
            Recipe* r = mgr.getRecipe(recipeName);
            if (r) {
              schedule[i].push_back({*r, servings});
            }
          } catch (...) {
             // Recipe was deleted, ignore it
          }
        }
      }
    }
  } catch (...) {
    throw FileLoadException("Error parsing weekly menu file: " + filename);
  }
  file.close();
}

void WeeklyMenu::generateShoppingList() {
  // Parallel vectors for aggregated ingredients
  vector<string> names;
  vector<double> quantities;
  vector<string> units;

  // Iterate all days and all recipes
  for (int i = 0; i < DAYS_COUNT; i++) {
    for (int j = 0; j < (int)schedule[i].size(); j++) {
      // Scale the recipe to the planned target servings
      Recipe scaledRecipe = schedule[i][j].recipe.scaleServings(schedule[i][j].targetServings);
      vector<Ingredient> ings = scaledRecipe.getIngredients();
      for (int k = 0; k < (int)ings.size(); k++) {
        string ingName = ings[k].getName();
        string ingUnit = ings[k].getUnit();
        transform(ingName.begin(), ingName.end(), ingName.begin(), ::tolower);
        transform(ingUnit.begin(), ingUnit.end(), ingUnit.begin(), ::tolower);
        double ingQty = ings[k].getQuantity();

        // Check if this name+unit combo already exists
        bool found = false;
        for (int m = 0; m < (int)names.size(); m++) {
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

  ofstream outFile("shopping_list.txt");
  bool canWrite = outFile.is_open();

  if (canWrite) {
    outFile << "==========================================" << endl;
    outFile << "         SHOPPING LIST" << endl;
    outFile << "==========================================" << endl;
  }

  if (names.empty()) {
    cout << "  Nothing to buy - pantry has everything!" << endl;
    cout << "==========================================" << endl;
    if (canWrite) {
      outFile << "  Nothing to buy - pantry has everything!" << endl;
      outFile << "==========================================" << endl;
      outFile.close();
    }
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

  if (canWrite) {
    outFile << "  ";
    outFile.width(nameWidth); outFile << left << "Name";
    outFile.width(qtyWidth);  outFile << left << "Quantity";
    outFile << "Unit" << endl;
  }

  cout << "  ";
  if (canWrite) outFile << "  ";
  for (int i = 0; i < nameWidth + qtyWidth + unitWidth; i++) {
    cout << "-";
    if (canWrite) outFile << "-";
  }
  cout << endl;
  if (canWrite) outFile << endl;

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

    if (canWrite) {
      outFile << "  ";
      outFile.width(nameWidth); outFile << left << names[i];
      outFile.width(qtyWidth);  outFile << left << qtyStr;
      outFile << units[i] << endl;
    }
  }

  cout << "==========================================" << endl;
  if (canWrite) {
    outFile << "==========================================" << endl;
    outFile.close();
    cout << "  [Exported to shopping_list.txt successfully!]" << endl;
  }
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
