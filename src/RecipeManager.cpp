#include "RecipeManager.h"
#include <fstream>
#include <iostream>

using namespace std;

void RecipeManager::addRecipe(Recipe r) { recipes.push_back(r); }

void RecipeManager::removeRecipe(string name) {
  for (int i = 0; i < recipes.size(); i++) {
    if (recipes[i].getName() == name) {
      recipes.erase(recipes.begin() + i);
      return;
    }
  }
  throw RecipeNotFoundException("Recipe not found: " + name);
}

Recipe* RecipeManager::getRecipe(string name) {
  for (int i = 0; i < recipes.size(); i++) {
    if (recipes[i].getName() == name)
      return &recipes[i];
  }
  throw RecipeNotFoundException("Recipe not found: " + name);
}

vector<Recipe>& RecipeManager::getAllRecipes() { return recipes; }
int RecipeManager::getCount() { return recipes.size(); }

vector<Recipe> RecipeManager::searchByName(string query) {
  vector<Recipe> results;
  for (int i = 0; i < recipes.size(); i++) {
    if (recipes[i].getName().find(query) != string::npos)
      results.push_back(recipes[i]);
  }
  return results;
}

vector<Recipe> RecipeManager::filterByCategory(string cat) {
  vector<Recipe> results;
  for (int i = 0; i < recipes.size(); i++) {
    if (recipes[i].getCategory() == cat)
      results.push_back(recipes[i]);
  }
  return results;
}

vector<Recipe> RecipeManager::searchByTag(vector<string> tags) {
  vector<Recipe> results;
  for (int i = 0; i < recipes.size(); i++) {
    bool hasAll = true;
    for (int j = 0; j < tags.size(); j++) {
      if (!recipes[i].hasTag(tags[j])) {
        hasAll = false;
        break;
      }
    }
    if (hasAll)
      results.push_back(recipes[i]);
  }
  return results;
}

vector<Recipe> RecipeManager::searchByIngredient(string ing) {
  vector<Recipe> results;
  for (int i = 0; i < recipes.size(); i++) {
    vector<Ingredient> ings = recipes[i].getIngredients();
    for (int j = 0; j < ings.size(); j++) {
      if (ings[j].getName() == ing) {
        results.push_back(recipes[i]);
        break;
      }
    }
  }
  return results;
}

vector<Recipe> RecipeManager::getFavorites() {
  vector<Recipe> results;
  for (int i = 0; i < recipes.size(); i++) {
    if (recipes[i].isFavorite())
      results.push_back(recipes[i]);
  }
  return results;
}

vector<Recipe> RecipeManager::getByRating() {
  vector<Recipe> sorted = recipes;
  for (int i = 0; i < sorted.size(); i++) {
    for (int j = i + 1; j < sorted.size(); j++) {
      if (sorted[j].getRating() > sorted[i].getRating()) {
        Recipe temp = sorted[i];
        sorted[i] = sorted[j];
        sorted[j] = temp;
      }
    }
  }
  return sorted;
}

vector<Recipe> RecipeManager::getByTime() {
  vector<Recipe> sorted = recipes;
  for (int i = 0; i < sorted.size(); i++) {
    for (int j = i + 1; j < sorted.size(); j++) {
      if (sorted[j].getEstimatedTime() < sorted[i].getEstimatedTime()) {
        Recipe temp = sorted[i];
        sorted[i] = sorted[j];
        sorted[j] = temp;
      }
    }
  }
  return sorted;
}

void RecipeManager::displayStats() {
  cout << "==========================================" << endl;
  cout << "           STATISTICS" << endl;
  cout << "==========================================" << endl;
  cout << "Total recipes: " << recipes.size() << endl;

  cout << endl << "By category:" << endl;
  vector<string> cats;
  vector<int> catCounts;
  for (int i = 0; i < recipes.size(); i++) {
    string c = recipes[i].getCategory();
    bool found = false;
    for (int j = 0; j < cats.size(); j++) {
      if (cats[j] == c) { catCounts[j]++; found = true; break; }
    }
    if (!found) { cats.push_back(c); catCounts.push_back(1); }
  }
  for (int i = 0; i < cats.size(); i++)
    cout << "  " << cats[i] << ": " << catCounts[i] << endl;

  double totalRating = 0;
  int rated = 0;
  for (int i = 0; i < recipes.size(); i++) {
    if (recipes[i].getRating() > 0) {
      totalRating += recipes[i].getRating();
      rated++;
    }
  }
  if (rated > 0)
    cout << endl << "Average rating: " << (totalRating / rated) << "/5" << endl;

  int favs = 0;
  for (int i = 0; i < recipes.size(); i++)
    if (recipes[i].isFavorite()) favs++;
  cout << "Favorites: " << favs << endl;

  if (rated > 0) {
    cout << endl << "Top rated:" << endl;
    vector<Recipe> top = getByRating();
    int show = 3;
    if ((int)top.size() < show) show = top.size();
    for (int i = 0; i < show; i++) {
      if (top[i].getRating() > 0)
        cout << "  " << top[i].getName() << " - " << top[i].getRating() << "/5" << endl;
    }
  }
  cout << "==========================================" << endl;
}

void RecipeManager::suggestRecipes(Pantry& pantry) {
  cout << "==========================================" << endl;
  cout << "       RECIPE SUGGESTIONS" << endl;
  cout << "==========================================" << endl;

  if (recipes.empty()) {
    cout << "  No recipes available." << endl;
    cout << "==========================================" << endl;
    return;
  }

  if (pantry.getCount() == 0) {
    cout << "  Your pantry is empty. Add some items first!" << endl;
    cout << "==========================================" << endl;
    return;
  }

  vector<string> canMake;
  vector<string> almostCanMake;
  vector<vector<string>> missingDetails;

  for (int i = 0; i < recipes.size(); i++) {
    vector<Ingredient> ings = recipes[i].getIngredients();
    bool allAvailable = true;
    vector<string> missing;

    for (int j = 0; j < ings.size(); j++) {
      if (!pantry.hasEnough(ings[j].getName(), ings[j].getQuantity(), ings[j].getUnit())) {
        allAvailable = false;
        double have = pantry.getQuantity(ings[j].getName(), ings[j].getUnit());
        string detail = "    - " + ings[j].getName() + ": need " +
            to_string(ings[j].getQuantity()) + " " + ings[j].getUnit() +
            ", have " + to_string(have) + " " + ings[j].getUnit();
        // Trim trailing zeros from the numbers in the detail string
        missing.push_back(detail);
      }
    }

    if (allAvailable) {
      canMake.push_back(recipes[i].getName());
    } else if ((int)missing.size() <= 2) {
      // "Almost" means missing at most 2 ingredients
      almostCanMake.push_back(recipes[i].getName());
      missingDetails.push_back(missing);
    }
  }

  // Recipes you CAN make
  cout << endl << "  You can make (" << canMake.size() << "):" << endl;
  if (canMake.empty()) {
    cout << "    (none)" << endl;
  } else {
    for (int i = 0; i < canMake.size(); i++)
      cout << "    " << (i + 1) << ". " << canMake[i] << endl;
  }

  // Recipes you ALMOST can make
  if (!almostCanMake.empty()) {
    cout << endl << "  Almost there (missing 1-2 ingredients):" << endl;
    for (int i = 0; i < almostCanMake.size(); i++) {
      cout << "    " << almostCanMake[i] << endl;
      cout << "    Missing:" << endl;
      for (int j = 0; j < missingDetails[i].size(); j++)
        cout << "  " << missingDetails[i][j] << endl;
    }
  }

  cout << "==========================================" << endl;
}

void RecipeManager::saveToFile(string filename) {
  ofstream file(filename);
  for (int i = 0; i < recipes.size(); i++) {
    file << "---" << endl;
    file << recipes[i].getName() << endl;
    file << recipes[i].getServings() << endl;
    file << recipes[i].getCategory() << endl;
    file << recipes[i].getRating() << endl;
    file << (recipes[i].isFavorite() ? 1 : 0) << endl;
    vector<string> tags = recipes[i].getTags();
    file << tags.size() << endl;
    for (int j = 0; j < tags.size(); j++) file << tags[j] << endl;
    vector<Ingredient> ings = recipes[i].getIngredients();
    file << ings.size() << endl;
    for (int j = 0; j < ings.size(); j++)
      file << ings[j].getName() << "|" << ings[j].getQuantity() << "|" << ings[j].getUnit() << "|" << ings[j].getCalories() << "|" << ings[j].getProtein() << "|" << ings[j].getCarbs() << "|" << ings[j].getFat() << endl;
    vector<string> steps = recipes[i].getSteps();
    file << steps.size() << endl;
    for (int j = 0; j < steps.size(); j++) file << steps[j] << endl;
  }
  file.close();
}

void RecipeManager::loadFromFile(string filename) {
  ifstream file(filename);
  if (!file.is_open()) return;
  try {
    string line;
    while (getline(file, line)) {
      if (line != "---") continue;
      string name, cat;
      int srv, rat, fav;
      getline(file, name);
      file >> srv; file.ignore();
      getline(file, cat);
      file >> rat; file.ignore();
      file >> fav; file.ignore();
      Recipe r(name, srv, cat);
      r.setRating(rat);
      if (fav == 1) r.toggleFavorite();
      int tagCount;
      file >> tagCount; file.ignore();
      for (int i = 0; i < tagCount; i++) {
        string tag;
        getline(file, tag);
        r.addTag(tag);
      }
      int ingCount;
      file >> ingCount; file.ignore();
      for (int i = 0; i < ingCount; i++) {
        string ingLine;
        getline(file, ingLine);
        string iname, unit;
        double qty, cal, prot = 0, carb = 0, ft = 0;
        int p1 = ingLine.find('|');
        int p2 = ingLine.find('|', p1+1);
        int p3 = ingLine.find('|', p2+1);
        int p4 = ingLine.find('|', p3+1);
        int p5 = ingLine.find('|', p4+1);
        int p6 = ingLine.find('|', p5+1);
        iname = ingLine.substr(0, p1);
        qty = stod(ingLine.substr(p1+1, p2-p1-1));
        unit = ingLine.substr(p2+1, p3-p2-1);
        cal = stod(ingLine.substr(p3+1, p4-p3-1));
        if (p4 != (int)string::npos) prot = stod(ingLine.substr(p4+1, p5-p4-1));
        if (p5 != (int)string::npos) carb = stod(ingLine.substr(p5+1, p6-p5-1));
        if (p6 != (int)string::npos) ft = stod(ingLine.substr(p6+1));
        r.addIngredient(Ingredient(iname, qty, unit, cal, prot, carb, ft));
      }
      int stepCount;
      file >> stepCount; file.ignore();
      for (int i = 0; i < stepCount; i++) {
        string step;
        getline(file, step);
        r.addStep(step);
      }
      recipes.push_back(r);
    }
  } catch (const exception& e) {
    throw FileLoadException("Failed to load recipes from file: invalid format.");
  }
  file.close();
}
