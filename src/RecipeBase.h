#ifndef RECIPEBASE_H
#define RECIPEBASE_H

#include <string>
#include <vector>

using namespace std;

class RecipeBase {
protected:
  string name;
  int servings;
  string category;
  vector<string> tags;
  bool favorite;
  int rating;

public:
  RecipeBase(string n, int s, string c);
  virtual ~RecipeBase();

  string getName();
  int getServings();
  string getCategory();
  vector<string> getTags();
  bool isFavorite();
  int getRating();

  void setName(string n);
  void setCategory(string c);
  void setServings(int s);
  void setRating(int r);
  void toggleFavorite();

  void addTag(string tag);
  bool hasTag(string tag);

  virtual void display() = 0;
};

#endif
