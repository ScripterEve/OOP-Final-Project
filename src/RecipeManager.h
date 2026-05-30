#ifndef RECIPEMANAGER_H
#define RECIPEMANAGER_H

#include <string>
#include <vector>

#include "Recipe.h"
#include "Pantry.h"
#include "Exceptions.h"

using namespace std;

class RecipeManager {
private:
  vector<Recipe> recipes;

public:
  void addRecipe(Recipe r);
  void removeRecipe(string name);
  Recipe* getRecipe(string name);
  vector<Recipe>& getAllRecipes();
  int getCount();

  vector<Recipe> searchByName(string query);
  vector<Recipe> filterByCategory(string cat);
  vector<Recipe> searchByTag(vector<string> tags);
  vector<Recipe> searchByIngredient(string ing);
  vector<Recipe> getFavorites();
  vector<Recipe> getByRating();
  vector<Recipe> getByTime();

  void displayStats();
  void suggestRecipes(Pantry& pantry);
  void saveToFile(string filename);
  void loadFromFile(string filename);
};

#endif
