#include <iostream>
#include <string>
#include <vector>

#include "Ingredient.h"
#include "Recipe.h"
#include "RecipeManager.h"
#include "WeeklyMenu.h"
#include "Pantry.h"
// Custom exceptions for error handling
#include "Exceptions.h"

using namespace std;

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
    cout << " (~" << list[i].getEstimatedTime() << " min)";
    cout << endl;
  }
}

int main() {
  RecipeManager mgr;
  WeeklyMenu menu;
  Pantry pantry;
  string dataFile = "recipes.txt";
  try {
    mgr.loadFromFile(dataFile);
  } catch (const exception& e) {
    cout << "Warning: " << e.what() << endl;
  }

  if (mgr.getCount() == 0) {
    Recipe r1("Shopska salad", 4, "salad");
    r1.addIngredient(Ingredient("tomatoes", 4, "pcs", 22, 1.1, 4.8, 0.2));
    r1.addIngredient(Ingredient("cucumbers", 2, "pcs", 16, 0.7, 3.6, 0.1));
    r1.addIngredient(Ingredient("cheese", 200, "g", 264, 17.5, 1.3, 21.3));
    r1.addStep("Chop the tomatoes and cucumbers");
    r1.addStep("Add cheese on top");
    r1.addTag("vegetarian");
    r1.addTag("quick");
    r1.setRating(5);
    mgr.addRecipe(r1);

    Recipe r2("Chicken with rice", 2, "main");
    r2.addIngredient(Ingredient("chicken breast", 400, "g", 440, 92, 0, 4));
    r2.addIngredient(Ingredient("rice", 200, "g", 260, 5.3, 56, 0.6));
    r2.addIngredient(Ingredient("butter", 30, "g", 215, 0.1, 0, 24));
    r2.addStep("Boil the rice");
    r2.addStep("Fry the chicken");
    r2.addStep("Mix and serve");
    r2.addTag("quick");
    r2.setRating(4);
    mgr.addRecipe(r2);

    Recipe r3("Banana smoothie", 1, "drink");
    r3.addIngredient(Ingredient("bananas", 2, "pcs", 178, 2.2, 46, 0.6));
    r3.addIngredient(Ingredient("milk", 300, "ml", 186, 10.2, 14.4, 9.6));
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
    cout << "16. Plan weekly menu" << endl;
    cout << "17. View weekly plan" << endl;
    cout << "18. Generate shopping list" << endl;
    cout << "19. Add to pantry" << endl;
    cout << "20. View pantry" << endl;
    cout << "21. Suggest recipes from pantry" << endl;
    cout << "22. Sort by prep time" << endl;
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

    try {
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
        double prot = readDouble("  Protein (g): ");
        double carb = readDouble("  Carbs (g): ");
        double ft = readDouble("  Fat (g): ");
        r.addIngredient(Ingredient(iname, qty, unit, cal, prot, carb, ft));
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
      mgr.removeRecipe(input);
      cout << "Deleted." << endl;

    } else if (choice == 4) {
      vector<Recipe>& allRecipes = mgr.getAllRecipes();
      if (allRecipes.empty()) {
        cout << "No recipes available." << endl;
      } else {
        displayList(allRecipes);
        int recipeChoice;
        while (true) {
          recipeChoice = readInt("Select recipe number: ");
          if (recipeChoice >= 1 && recipeChoice <= (int)allRecipes.size()) {
            break;
          }
          cout << "Invalid number. Please try again." << endl;
        }
        allRecipes[recipeChoice - 1].display();
      }

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
      cout << "Tags (comma-separated): "; getline(cin, input);
      // Split input by commas into a vector
      vector<string> tagList;
      string current;
      for (int i = 0; i < (int)input.size(); i++) {
        if (input[i] == ',') {
          // Trim whitespace from current
          string trimmed;
          int start = 0, end = current.size() - 1;
          while (start <= end && current[start] == ' ') start++;
          while (end >= start && current[end] == ' ') end--;
          if (start <= end)
            trimmed = current.substr(start, end - start + 1);
          if (!trimmed.empty())
            tagList.push_back(trimmed);
          current = "";
        } else {
          current += input[i];
        }
      }
      // Don't forget the last token
      {
        int start = 0, end = current.size() - 1;
        while (start <= end && current[start] == ' ') start++;
        while (end >= start && current[end] == ' ') end--;
        if (start <= end) {
          string trimmed = current.substr(start, end - start + 1);
          if (!trimmed.empty())
            tagList.push_back(trimmed);
        }
      }
      if (tagList.empty()) {
        cout << "No tags entered." << endl;
      } else {
        cout << "Searching for recipes with ALL tags: ";
        for (int i = 0; i < tagList.size(); i++) {
          cout << tagList[i];
          if (i < (int)tagList.size() - 1) cout << ", ";
        }
        cout << endl;
        displayList(mgr.searchByTag(tagList));
      }

    } else if (choice == 9) {
      cout << "Name: "; getline(cin, input);
      Recipe* r = mgr.getRecipe(input);
      int ns = readInt("New servings: ");
      Recipe scaled = r->scaleServings(ns);
      scaled.display();

    } else if (choice == 10) {
      cout << "Name: "; getline(cin, input);
      Recipe* r = mgr.getRecipe(input);
      int rat = readInt("Rating (1-5): ");
      if (rat >= 1 && rat <= 5) {
        r->setRating(rat);
        cout << "Rated!" << endl;
      } else {
        cout << "Invalid rating." << endl;
      }

    } else if (choice == 11) {
      cout << "Name: "; getline(cin, input);
      Recipe* r = mgr.getRecipe(input);
      r->toggleFavorite();
      cout << "Toggled." << endl;

    } else if (choice == 12) {
      displayList(mgr.getFavorites());

    } else if (choice == 13) {
      displayList(mgr.getByRating());

    } else if (choice == 14) {
      cout << "Name: "; getline(cin, input);
      Recipe* r = mgr.getRecipe(input);
      string fn;
      cout << "Filename (e.g. export.txt): "; getline(cin, fn);
      r->exportToFile(fn);

    } else if (choice == 15) {
      mgr.displayStats();

    } else if (choice == 16) {
      // Plan weekly menu
      cout << "Available days:" << endl;
      for (int i = 0; i < menu.getDaysCount(); i++)
        cout << "  " << (i + 1) << ". " << menu.getDayName(i) << endl;
      int dayChoice = readInt("Select day (1-7): ");
      if (dayChoice < 1 || dayChoice > menu.getDaysCount()) {
        cout << "Invalid day." << endl;
      } else {
        string selectedDay = menu.getDayName(dayChoice - 1);
        cout << endl << "Available recipes:" << endl;
        vector<Recipe>& allRecipes = mgr.getAllRecipes();
        if (allRecipes.empty()) {
          cout << "No recipes available. Add some first!" << endl;
        } else {
          for (int i = 0; i < allRecipes.size(); i++) {
            cout << "  " << (i + 1) << ". " << allRecipes[i].getName()
                 << " [" << allRecipes[i].getCategory() << "]" << endl;
          }
          int recipeChoice = readInt("Select recipe: ");
          if (recipeChoice < 1 || recipeChoice > (int)allRecipes.size()) {
            cout << "Invalid selection." << endl;
          } else {
            int servings = readInt("For how many servings are you planning this meal? ");
            if (servings > 0) {
              menu.addRecipeToDay(selectedDay, allRecipes[recipeChoice - 1], servings);
              cout << "Added \"" << allRecipes[recipeChoice - 1].getName()
                   << "\" (" << servings << " servings) to " << selectedDay << "!" << endl;
            } else {
              cout << "Invalid servings." << endl;
            }
          }
        }
      }

    } else if (choice == 17) {
      menu.displayWeeklyPlan();

    } else if (choice == 18) {
      menu.generateShoppingList(pantry);

    } else if (choice == 19) {
      // Add to pantry
      string iname, unit;
      cout << "Ingredient name: "; getline(cin, iname);
      double qty = readDouble("Quantity: ");
      cout << "Unit: "; getline(cin, unit);
      pantry.addItem(iname, qty, unit);
      cout << "Added " << qty << " " << unit << " of " << iname << " to pantry!" << endl;

    } else if (choice == 20) {
      pantry.displayPantry();

    } else if (choice == 21) {
      mgr.suggestRecipes(pantry);

    } else if (choice == 22) {
      cout << "Recipes sorted by estimated prep time:" << endl;
      displayList(mgr.getByTime());

    } else if (choice != 0) {
      cout << "Invalid choice." << endl;
    }
    } catch (const exception& e) {
      cout << "Error: " << e.what() << endl;
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
