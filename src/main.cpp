#include <fstream>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

class Ingredient {
private:
  string name;
  double quantity;
  string unit;
  double calories;

public:
  Ingredient(string n, double q, string u, double cal = 0)
      : name(n), quantity(q), unit(u), calories(cal) {}

  string getName() { return name; }
  double getQuantity() { return quantity; }
  string getUnit() { return unit; }
  double getCalories() { return calories; }

  void setQuantity(double q) { quantity = q; }
  void setCalories(double c) { calories = c; }

  Ingredient scale(double factor) {
    return Ingredient(name, quantity * factor, unit, calories * factor);
  }

  void display() {
    cout << "  - " << name << ": " << quantity << " " << unit;
    if (calories > 0)
      cout << " (" << calories << " kcal)";
    cout << endl;
  }
};

class RecipeBase {
protected:
  string name;
  int servings;
  string category;
  vector<string> tags;
  bool favorite;
  int rating;

public:
  RecipeBase(string n, int s, string c)
      : name(n), servings(s), category(c), favorite(false), rating(0) {}
  virtual ~RecipeBase() {}

  string getName() { return name; }
  int getServings() { return servings; }
  string getCategory() { return category; }
  vector<string> getTags() { return tags; }
  bool isFavorite() { return favorite; }
  int getRating() { return rating; }

  void setName(string n) { name = n; }
  void setCategory(string c) { category = c; }
  void setServings(int s) {
    if (s > 0)
      servings = s;
  }
  void setRating(int r) {
    if (r >= 1 && r <= 5)
      rating = r;
  }
  void toggleFavorite() { favorite = !favorite; }

  void addTag(string tag) {
    if (!hasTag(tag))
      tags.push_back(tag);
  }

  bool hasTag(string tag) {
    for (int i = 0; i < tags.size(); i++) {
      if (tags[i] == tag)
        return true;
    }
    return false;
  }

  virtual void display() = 0;
};

class Recipe : public RecipeBase {
private:
  vector<Ingredient> ingredients;
  vector<string> steps;

public:
  Recipe(string n, int s, string c) : RecipeBase(n, s, c) {}

  void addIngredient(Ingredient ing) { ingredients.push_back(ing); }

  void removeIngredient(string ingName) {
    for (int i = 0; i < ingredients.size(); i++) {
      if (ingredients[i].getName() == ingName) {
        ingredients.erase(ingredients.begin() + i);
        return;
      }
    }
  }

  vector<Ingredient> getIngredients() { return ingredients; }

  void addStep(string step) { steps.push_back(step); }

  void removeStep(int index) {
    if (index >= 0 && index < (int)steps.size())
      steps.erase(steps.begin() + index);
  }

  vector<string> getSteps() { return steps; }

  Recipe scaleServings(int newServings) {
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

  double getTotalCalories() {
    double total = 0;
    for (int i = 0; i < ingredients.size(); i++)
      total += ingredients[i].getCalories();
    return total;
  }

  void display() override {
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
    cout << "Total calories: " << getTotalCalories() << " kcal" << endl;
    cout << "==========================================" << endl;
  }

  void exportToFile(string filename) {
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
};

class RecipeManager {
private:
  vector<Recipe> recipes;

public:
  void addRecipe(Recipe r) { recipes.push_back(r); }

  bool removeRecipe(string name) {
    for (int i = 0; i < recipes.size(); i++) {
      if (recipes[i].getName() == name) {
        recipes.erase(recipes.begin() + i);
        return true;
      }
    }
    return false;
  }

  Recipe *getRecipe(string name) {
    for (int i = 0; i < recipes.size(); i++) {
      if (recipes[i].getName() == name)
        return &recipes[i];
    }
    return nullptr;
  }

  vector<Recipe> &getAllRecipes() { return recipes; }
  int getCount() { return recipes.size(); }

  vector<Recipe> searchByName(string query) {
    vector<Recipe> results;
    for (int i = 0; i < recipes.size(); i++) {
      if (recipes[i].getName().find(query) != string::npos)
        results.push_back(recipes[i]);
    }
    return results;
  }

  vector<Recipe> filterByCategory(string cat) {
    vector<Recipe> results;
    for (int i = 0; i < recipes.size(); i++) {
      if (recipes[i].getCategory() == cat)
        results.push_back(recipes[i]);
    }
    return results;
  }

  vector<Recipe> searchByTag(string tag) {
    vector<Recipe> results;
    for (int i = 0; i < recipes.size(); i++) {
      if (recipes[i].hasTag(tag))
        results.push_back(recipes[i]);
    }
    return results;
  }

  vector<Recipe> searchByIngredient(string ing) {
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

  vector<Recipe> getFavorites() {
    vector<Recipe> results;
    for (int i = 0; i < recipes.size(); i++) {
      if (recipes[i].isFavorite())
        results.push_back(recipes[i]);
    }
    return results;
  }

  vector<Recipe> getByRating() {
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

  void displayStats() {
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
  void saveToFile(string filename) {
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
        file << ings[j].getName() << "|" << ings[j].getQuantity() << "|" << ings[j].getUnit() << "|" << ings[j].getCalories() << endl;
      vector<string> steps = recipes[i].getSteps();
      file << steps.size() << endl;
      for (int j = 0; j < steps.size(); j++) file << steps[j] << endl;
    }
    file.close();
  }

  void loadFromFile(string filename) {
    ifstream file(filename);
    if (!file.is_open()) return;
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
        double qty, cal;
        int p1 = ingLine.find('|');
        int p2 = ingLine.find('|', p1+1);
        int p3 = ingLine.find('|', p2+1);
        iname = ingLine.substr(0, p1);
        qty = stod(ingLine.substr(p1+1, p2-p1-1));
        unit = ingLine.substr(p2+1, p3-p2-1);
        cal = stod(ingLine.substr(p3+1));
        r.addIngredient(Ingredient(iname, qty, unit, cal));
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
    file.close();
  }
};

void displayList(vector<Recipe> list) {
  if (list.empty()) {
    cout << "No recipes found." << endl;
    return;
  }
  for (int i = 0; i < list.size(); i++) {
    cout << (i + 1) << ". " << list[i].getName() << " ["
         << list[i].getCategory() << "]";
    if (list[i].isFavorite())
      cout << " [FAV]";
    if (list[i].getRating() > 0)
      cout << " " << list[i].getRating() << "/5";
    cout << endl;
  }
}

int main() {
  RecipeManager mgr;
  string dataFile = "recipes.txt";
  mgr.loadFromFile(dataFile);

  if (mgr.getCount() == 0) {
    Recipe r1("Shopska salad", 4, "salad");
    r1.addIngredient(Ingredient("tomatoes", 4, "pcs", 22));
    r1.addIngredient(Ingredient("cucumbers", 2, "pcs", 16));
    r1.addIngredient(Ingredient("cheese", 200, "g", 264));
    r1.addStep("Chop the tomatoes and cucumbers");
    r1.addStep("Add cheese on top");
    r1.addTag("vegetarian");
    r1.addTag("quick");
    r1.setRating(5);
    mgr.addRecipe(r1);

    Recipe r2("Chicken with rice", 2, "main");
    r2.addIngredient(Ingredient("chicken breast", 400, "g", 440));
    r2.addIngredient(Ingredient("rice", 200, "g", 260));
    r2.addIngredient(Ingredient("butter", 30, "g", 215));
    r2.addStep("Boil the rice");
    r2.addStep("Fry the chicken");
    r2.addStep("Mix and serve");
    r2.addTag("quick");
    r2.setRating(4);
    mgr.addRecipe(r2);

    Recipe r3("Banana smoothie", 1, "drink");
    r3.addIngredient(Ingredient("bananas", 2, "pcs", 178));
    r3.addIngredient(Ingredient("milk", 300, "ml", 186));
    r3.addStep("Peel the bananas");
    r3.addStep("Blend everything");
    r3.addTag("vegetarian");
    r3.addTag("dessert");
    r3.setRating(3);
    mgr.addRecipe(r3);
  }

  auto readInt = [](string prompt) -> int {
    int val;
    while (true) {
      cout << prompt;
      cin >> val;
      if (cin.fail()) {
        cin.clear();
        cin.ignore(1000, '\n');
        cout << "Please enter a number." << endl;
      } else {
        cin.ignore();
        return val;
      }
    }
  };

  auto readDouble = [](string prompt) -> double {
    double val;
    while (true) {
      cout << prompt;
      cin >> val;
      if (cin.fail()) {
        cin.clear();
        cin.ignore(1000, '\n');
        cout << "Please enter a number." << endl;
      } else {
        cin.ignore();
        return val;
      }
    }
  };

  int choice;
  string input;

  do {
    cout << endl;
    cout << "==========================================" << endl;
    cout << "         RECIPE MANAGER" << endl;
    cout << "==========================================" << endl;
    cout << " 1. View all recipes" << endl;
    cout << " 2. Add a recipe" << endl;
    cout << " 3. Delete a recipe" << endl;
    cout << " 4. View recipe details" << endl;
    cout << " 5. Search by name" << endl;
    cout << " 6. Filter by category" << endl;
    cout << " 7. Search by ingredient" << endl;
    cout << " 8. Search by tag" << endl;
    cout << " 9. Scale servings" << endl;
    cout << "10. Rate a recipe" << endl;
    cout << "11. Toggle favorite" << endl;
    cout << "12. View favorites" << endl;
    cout << "13. Sort by rating" << endl;
    cout << "14. Export recipe to file" << endl;
    cout << "15. Statistics" << endl;
    cout << " 0. Exit" << endl;
    cout << "==========================================" << endl;
    cout << "Choice: ";
    cin >> choice;
    if (cin.fail()) {
      cin.clear();
      cin.ignore(1000, '\n');
      cout << "Invalid input." << endl;
      continue;
    }
    cin.ignore();

    if (choice == 1) {
      displayList(mgr.getAllRecipes());

    } else if (choice == 2) {
      string name, cat;
      cout << "Name: "; getline(cin, name);
      cout << "Category: "; getline(cin, cat);
      int srv = readInt("Servings: ");
      Recipe r(name, srv, cat);

      int n = readInt("How many ingredients? ");
      for (int i = 0; i < n; i++) {
        string iname, unit;
        cout << "  Ingredient " << (i+1) << " name: "; getline(cin, iname);
        double qty = readDouble("  Quantity: ");
        cout << "  Unit: "; getline(cin, unit);
        double cal = readDouble("  Calories: ");
        r.addIngredient(Ingredient(iname, qty, unit, cal));
      }

      int ns = readInt("How many steps? ");
      for (int i = 0; i < ns; i++) {
        string step;
        cout << "  Step " << (i+1) << ": "; getline(cin, step);
        r.addStep(step);
      }

      int nt = readInt("How many tags? ");
      for (int i = 0; i < nt; i++) {
        string tag;
        cout << "  Tag " << (i+1) << ": "; getline(cin, tag);
        r.addTag(tag);
      }

      mgr.addRecipe(r);
      cout << "Recipe added!" << endl;

    } else if (choice == 3) {
      cout << "Name: "; getline(cin, input);
      if (mgr.removeRecipe(input)) cout << "Deleted." << endl;
      else cout << "Not found." << endl;

    } else if (choice == 4) {
      cout << "Name: "; getline(cin, input);
      Recipe* r = mgr.getRecipe(input);
      if (r) r->display();
      else cout << "Not found." << endl;

    } else if (choice == 5) {
      cout << "Search: "; getline(cin, input);
      displayList(mgr.searchByName(input));

    } else if (choice == 6) {
      cout << "Category: "; getline(cin, input);
      displayList(mgr.filterByCategory(input));

    } else if (choice == 7) {
      cout << "Ingredient: "; getline(cin, input);
      displayList(mgr.searchByIngredient(input));

    } else if (choice == 8) {
      cout << "Tag: "; getline(cin, input);
      displayList(mgr.searchByTag(input));

    } else if (choice == 9) {
      cout << "Name: "; getline(cin, input);
      Recipe* r = mgr.getRecipe(input);
      if (r) {
        int ns = readInt("New servings: ");
        Recipe scaled = r->scaleServings(ns);
        scaled.display();
      } else cout << "Not found." << endl;

    } else if (choice == 10) {
      cout << "Name: "; getline(cin, input);
      Recipe* r = mgr.getRecipe(input);
      if (r) {
        int rt = readInt("Rating (1-5): ");
        r->setRating(rt);
        cout << "Rated!" << endl;
      } else cout << "Not found." << endl;

    } else if (choice == 11) {
      cout << "Name: "; getline(cin, input);
      Recipe* r = mgr.getRecipe(input);
      if (r) {
        r->toggleFavorite();
        cout << (r->isFavorite() ? "Added to favorites." : "Removed from favorites.") << endl;
      } else cout << "Not found." << endl;

    } else if (choice == 12) {
      displayList(mgr.getFavorites());

    } else if (choice == 13) {
      displayList(mgr.getByRating());

    } else if (choice == 14) {
      cout << "Name: "; getline(cin, input);
      Recipe* r = mgr.getRecipe(input);
      if (r) {
        string fname;
        cout << "Filename: "; getline(cin, fname);
        r->exportToFile(fname);
      } else cout << "Not found." << endl;

    } else if (choice == 15) {
      mgr.displayStats();

    } else if (choice != 0) {
      cout << "Invalid choice." << endl;
    }

    if (choice != 0) {
      cout << endl << "Press Enter to continue...";
      string dummy;
      getline(cin, dummy);
    }

  } while (choice != 0);

  mgr.saveToFile(dataFile);
  cout << "Recipes saved. Goodbye!" << endl;
  return 0;
}

