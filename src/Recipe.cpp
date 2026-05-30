#include "Recipe.h"
#include <fstream>
#include <iostream>

using namespace std;

Recipe::Recipe(string n, int s, string c) : RecipeBase(n, s, c) {}

void Recipe::addIngredient(Ingredient ing) { ingredients.push_back(ing); }

void Recipe::removeIngredient(string ingName) {
  for (int i = 0; i < ingredients.size(); i++) {
    if (ingredients[i].getName() == ingName) {
      ingredients.erase(ingredients.begin() + i);
      return;
    }
  }
}

vector<Ingredient> Recipe::getIngredients() { return ingredients; }

void Recipe::addStep(string step) { steps.push_back(step); }

void Recipe::removeStep(int index) {
  if (index >= 0 && index < (int)steps.size())
    steps.erase(steps.begin() + index);
}

vector<string> Recipe::getSteps() { return steps; }

Recipe Recipe::scaleServings(int newServings) {
  double factor = (double)newServings / servings;
  Recipe scaled(name, newServings, category);
  scaled.tags = tags;
  scaled.favorite = favorite;
  scaled.rating = rating;
  scaled.steps = steps;
  for (int i = 0; i < ingredients.size(); i++)
    scaled.addIngredient(ingredients[i].scale(factor));
  return scaled;
}

double Recipe::getTotalCalories() {
  double total = 0;
  for (int i = 0; i < ingredients.size(); i++)
    total += ingredients[i].getCalories();
  return total;
}

double Recipe::getTotalProtein() {
  double total = 0;
  for (int i = 0; i < ingredients.size(); i++)
    total += ingredients[i].getProtein();
  return total;
}

double Recipe::getTotalCarbs() {
  double total = 0;
  for (int i = 0; i < ingredients.size(); i++)
    total += ingredients[i].getCarbs();
  return total;
}

double Recipe::getTotalFat() {
  double total = 0;
  for (int i = 0; i < ingredients.size(); i++)
    total += ingredients[i].getFat();
  return total;
}

int Recipe::getEstimatedTime() {
  return steps.size() * 5;
}

void Recipe::display() {
  cout << "==========================================" << endl;
  cout << "  " << name;
  if (favorite)
    cout << " [FAVORITE]";
  cout << endl;
  cout << "==========================================" << endl;
  cout << "Category: " << category << endl;
  cout << "Servings: " << servings << endl;
  if (rating > 0)
    cout << "Rating: " << rating << "/5" << endl;
  if (!tags.empty()) {
    cout << "Tags: ";
    for (int i = 0; i < tags.size(); i++) {
      cout << tags[i];
      if (i < tags.size() - 1)
        cout << ", ";
    }
    cout << endl;
  }
  cout << endl << "Ingredients:" << endl;
  for (int i = 0; i < ingredients.size(); i++)
    ingredients[i].display();
  cout << endl << "Steps:" << endl;
  for (int i = 0; i < steps.size(); i++)
    cout << "  " << (i + 1) << ". " << steps[i] << endl;
  cout << endl << "Nutrition info:" << endl;
  cout << "  Calories: " << getTotalCalories() << " kcal" << endl;
  cout << "  Protein: " << getTotalProtein() << " g" << endl;
  cout << "  Carbs: " << getTotalCarbs() << " g" << endl;
  cout << "  Fat: " << getTotalFat() << " g" << endl;
  cout << endl << "Estimated prep time: " << getEstimatedTime() << " min" << endl;
  cout << "==========================================" << endl;
}

void Recipe::exportToFile(string filename) {
  ofstream file(filename);
  if (!file.is_open()) {
    cout << "Error opening file." << endl;
    return;
  }
  file << name << endl;
  file << "Category: " << category << endl;
  file << "Servings: " << servings << endl;
  if (rating > 0)
    file << "Rating: " << rating << "/5" << endl;
  file << endl << "Ingredients:" << endl;
  for (int i = 0; i < ingredients.size(); i++)
    file << "  - " << ingredients[i].getName() << ": "
         << ingredients[i].getQuantity() << " " << ingredients[i].getUnit()
         << endl;
  file << endl << "Steps:" << endl;
  for (int i = 0; i < steps.size(); i++)
    file << "  " << (i + 1) << ". " << steps[i] << endl;
  file.close();
  cout << "Exported to " << filename << endl;
}
