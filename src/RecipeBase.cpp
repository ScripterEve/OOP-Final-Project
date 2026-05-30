#include "RecipeBase.h"

using namespace std;

RecipeBase::RecipeBase(string n, int s, string c)
    : name(n), servings(s), category(c), favorite(false), rating(0) {}

RecipeBase::~RecipeBase() {}

string RecipeBase::getName() { return name; }
int RecipeBase::getServings() { return servings; }
string RecipeBase::getCategory() { return category; }
vector<string> RecipeBase::getTags() { return tags; }
bool RecipeBase::isFavorite() { return favorite; }
int RecipeBase::getRating() { return rating; }

void RecipeBase::setName(string n) { name = n; }
void RecipeBase::setCategory(string c) { category = c; }

void RecipeBase::setServings(int s) {
  if (s > 0)
    servings = s;
}

void RecipeBase::setRating(int r) {
  if (r >= 1 && r <= 5)
    rating = r;
}

void RecipeBase::toggleFavorite() { favorite = !favorite; }

void RecipeBase::addTag(string tag) {
  if (!hasTag(tag))
    tags.push_back(tag);
}

bool RecipeBase::hasTag(string tag) {
  for (int i = 0; i < tags.size(); i++) {
    if (tags[i] == tag)
      return true;
  }
  return false;
}
