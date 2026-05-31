#include <iostream>
#include <string>
#include <vector>

#include "Ingredient.h"
#include "Recipe.h"
#include "RecipeManager.h"
#include "WeeklyMenu.h"
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
  string dataFile = "recipes.txt";
  try {
    mgr.loadFromFile(dataFile);
  } catch (const exception& e) {
    cout << "Warning: " << e.what() << endl;
  }

  try {
    menu.loadFromFile("weekly_plan.txt", mgr);
  } catch (const exception& e) {
    cout << "Warning loading weekly plan: " << e.what() << endl;
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

  auto selectRecipeInteractive = [&mgr, &readInt](string action) -> Recipe* {
    string query;
    cout << "Enter part of the recipe name to " << action << ": ";
    getline(cin, query);
    vector<Recipe> matches = mgr.searchByName(query);
    if (matches.empty()) {
      cout << "No recipes match that query." << endl;
      return nullptr;
    }
    if (matches.size() == 1) {
      cout << "Automatically selected: " << matches[0].getName() << endl;
      return mgr.getRecipe(matches[0].getName());
    }
    cout << "Multiple recipes found:" << endl;
    displayList(matches);
    int choice;
    while (true) {
      choice = readInt("Enter the number of the recipe you want to select: ");
      if (choice >= 1 && choice <= (int)matches.size()) break;
      cout << "Invalid number. Please try again." << endl;
    }
    return mgr.getRecipe(matches[choice - 1].getName());
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
    cout << "19. Sort by prep time" << endl;
    cout << "20. Update a recipe" << endl;
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
      mgr.saveToFile(dataFile);
      cout << "Recipe added!" << endl;

    } else if (choice == 3) {
      Recipe* r = selectRecipeInteractive("delete");
      if (r) {
        mgr.removeRecipe(r->getName());
        mgr.saveToFile(dataFile);
        cout << "Deleted." << endl;
      }

    } else if (choice == 4) {
      Recipe* r = selectRecipeInteractive("view details");
      if (r) {
        r->display();
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
      Recipe* r = selectRecipeInteractive("scale servings");
      if (r) {
        int ns = readInt("New servings: ");
        Recipe scaled = r->scaleServings(ns);
        scaled.display();
      }

    } else if (choice == 10) {
      Recipe* r = selectRecipeInteractive("rate");
      if (r) {
        int rat = readInt("Rating (1-5): ");
        if (rat >= 1 && rat <= 5) {
          r->setRating(rat);
          mgr.saveToFile(dataFile);
          cout << "Rated!" << endl;
        } else {
          cout << "Invalid rating." << endl;
        }
      }

    } else if (choice == 11) {
      Recipe* r = selectRecipeInteractive("toggle favorite status");
      if (r) {
        r->toggleFavorite();
        mgr.saveToFile(dataFile);
        cout << "Toggled." << endl;
      }

    } else if (choice == 12) {
      displayList(mgr.getFavorites());

    } else if (choice == 13) {
      displayList(mgr.getByRating());

    } else if (choice == 14) {
      Recipe* r = selectRecipeInteractive("export");
      if (r) {
        string fn;
        cout << "Filename (e.g. export.txt): "; getline(cin, fn);
        r->exportToFile(fn);
      }

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
        Recipe* r = selectRecipeInteractive("plan for " + selectedDay);
        if (r) {
          int servings = readInt("For how many servings are you planning this meal? ");
          if (servings > 0) {
            menu.addRecipeToDay(selectedDay, *r, servings);
            menu.saveToFile("weekly_plan.txt");
            cout << "Added \"" << r->getName() << "\" (" << servings << " servings) to " << selectedDay << "!" << endl;
          } else {
            cout << "Invalid servings." << endl;
          }
        }
      }

    } else if (choice == 17) {
      menu.displayWeeklyPlan();

    } else if (choice == 18) {
      menu.generateShoppingList();

    } else if (choice == 19) {
      cout << "Recipes sorted by estimated prep time:" << endl;
      displayList(mgr.getByTime());

    } else if (choice == 20) {
      Recipe* r = selectRecipeInteractive("update");
      if (r) {
        int subChoice;
        do {
          cout << endl << "==========================================" << endl;
          cout << "       UPDATE RECIPE: " << r->getName() << endl;
          cout << "==========================================" << endl;
          cout << " 1. Edit Name" << endl;
          cout << " 2. Edit Category" << endl;
          cout << " 3. Edit Default Servings" << endl;
          cout << " 4. Add Ingredient" << endl;
          cout << " 5. Remove Ingredient" << endl;
          cout << " 6. Add Step" << endl;
          cout << " 7. Remove Step" << endl;
          cout << " 8. Add Tag" << endl;
          cout << " 0. Finish Editing" << endl;
          cout << "==========================================" << endl;
          subChoice = readInt("Choice: ");

          if (subChoice == 1) {
            string name;
            cout << "New name: "; getline(cin, name);
            if (!name.empty()) r->setName(name);
            cout << "Name updated." << endl;
          } else if (subChoice == 2) {
            string cat;
            cout << "New category: "; getline(cin, cat);
            if (!cat.empty()) r->setCategory(cat);
            cout << "Category updated." << endl;
          } else if (subChoice == 3) {
            int srv = readInt("New default servings: ");
            r->setServings(srv);
            cout << "Servings updated." << endl;
          } else if (subChoice == 4) {
            string iName, unit;
            cout << "Ingredient name: "; getline(cin, iName);
            double qty = readDouble("Quantity: ");
            cout << "Unit: "; getline(cin, unit);
            double cal = readDouble("Calories: ");
            double prot = readDouble("Protein (g): ");
            double carbs = readDouble("Carbs (g): ");
            double fat = readDouble("Fat (g): ");
            r->addIngredient(Ingredient(iName, qty, unit, cal, prot, carbs, fat));
            cout << "Ingredient added." << endl;
          } else if (subChoice == 5) {
            string iName;
            cout << "Name of ingredient to remove: "; getline(cin, iName);
            r->removeIngredient(iName);
            cout << "Ingredient removed (if it existed)." << endl;
          } else if (subChoice == 6) {
            string step;
            cout << "New step: "; getline(cin, step);
            if (!step.empty()) r->addStep(step);
            cout << "Step added." << endl;
          } else if (subChoice == 7) {
            int idx = readInt("Index of step to remove (1-based): ");
            r->removeStep(idx - 1);
            cout << "Step removed (if index was valid)." << endl;
          } else if (subChoice == 8) {
            string tag;
            cout << "New tag: "; getline(cin, tag);
            if (!tag.empty()) r->addTag(tag);
            cout << "Tag added." << endl;
          } else if (subChoice != 0) {
            cout << "Invalid choice." << endl;
          }
        } while (subChoice != 0);
        
        mgr.saveToFile(dataFile);
        cout << "Recipe updated and saved successfully!" << endl;
      }

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
