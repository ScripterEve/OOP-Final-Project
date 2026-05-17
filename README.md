# Recipe Manager

Console C++ application for managing recipes.

## Build and Run

```
g++ -std=c++17 -o recipe_manager src/main.cpp
./recipe_manager
```

## Implemented Functionalities

1. **CRUD** — add, view and delete recipes with ingredients and preparation steps
2. **Search and filter** — search by name, ingredient, tag or category
3. **Portion scaling** — automatically recalculate ingredient quantities for a different number of servings
4. **Favorite recipes** — mark recipes as favorites and filter by them
5. **Rating** — rate recipes from 1 to 5 and sort by rating
6. **Export** — save a recipe to a .txt file
7. **Statistics** — total recipes, count by category, average rating and top rated
8. **Nutrition info** — track calories, protein, carbs and fat per ingredient with auto-calculation of totals

## Class Diagram

```
+------------------------------+
|        Ingredient            |
+------------------------------+
| - name : string              |
| - quantity : double          |
| - unit : string              |
| - calories : double          |
| - protein : double           |
| - carbs : double             |
| - fat : double               |
+------------------------------+
| + Ingredient(...)            |
| + getName() : string         |
| + getQuantity() : double     |
| + getUnit() : string         |
| + getCalories() : double     |
| + getProtein() : double      |
| + getCarbs() : double        |
| + getFat() : double          |
| + scale(double) : Ingredient |
| + display()                  |
+------------------------------+

+------------------------------------+
|     <<abstract>> RecipeBase        |
+------------------------------------+
| # name : string                   |
| # servings : int                  |
| # category : string               |
| # tags : vector<string>           |
| # favorite : bool                 |
| # rating : int                    |
+------------------------------------+
| + getName() : string              |
| + getCategory() : string          |
| + getServings() : int             |
| + isFavorite() : bool             |
| + getRating() : int               |
| + setRating(int)                  |
| + toggleFavorite()                |
| + hasTag(string) : bool           |
| + virtual display() = 0           |
+------------------------------------+
            ^
            | inherits
            |
+------------------------------------+
|           Recipe                   |
+------------------------------------+
| - ingredients : vector<Ingredient> |
| - steps : vector<string>          |
+------------------------------------+
| + addIngredient(Ingredient)        |
| + removeIngredient(string)         |
| + addStep(string)                  |
| + scaleServings(int) : Recipe      |
| + getTotalCalories() : double      |
| + getTotalProtein() : double       |
| + getTotalCarbs() : double         |
| + getTotalFat() : double           |
| + display() override              |
| + exportToFile(string)             |
+------------------------------------+

+---------------------------------------+
|        RecipeManager                  |
+---------------------------------------+
| - recipes : vector<Recipe>            |
+---------------------------------------+
| + addRecipe(Recipe)                   |
| + removeRecipe(string) : bool         |
| + getRecipe(string) : Recipe*         |
| + searchByName(string) : vector       |
| + searchByTag(string) : vector        |
| + filterByCategory(string) : vector   |
| + searchByIngredient(string) : vector |
| + getFavorites() : vector<Recipe>     |
| + getByRating() : vector<Recipe>      |
| + displayStats()                      |
| + saveToFile(string)                  |
| + loadFromFile(string)                |
+---------------------------------------+
```

### Relationships
- **RecipeBase <- Recipe**: inheritance (Recipe extends the abstract RecipeBase)
- **Recipe -> Ingredient**: composition (a recipe contains a vector of ingredients)
- **RecipeManager -> Recipe**: aggregation (the manager holds a collection of recipes)

## Design Decisions

1. **RecipeBase is abstract** — `display()` is pure virtual so that different recipe types can implement their own representation. This follows the Open/Closed Principle and allows future extensions without modifying existing code.

2. **RecipeBase fields are protected, not private** — subclasses like `Recipe` need direct access to `name`, `servings`, `tags`, etc. Using protected avoids unnecessary getter/setter calls inside the hierarchy while still hiding data from outside code.

3. **Nutrition info is stored directly in Ingredient** — the original plan had a separate `NutritionInfo` class. Keeping calories, protein, carbs and fat as fields in `Ingredient` is simpler while still demonstrating composition and scaling. The totals are calculated by `Recipe` methods.

## Changes from Checkpoint 1

- Removed `NutritionInfo` as a separate class — nutrition fields (calories, protein, carbs, fat) are stored directly in `Ingredient`. This reduced complexity without losing functionality.
- Removed `ISearchable` interface and `SearchableRecipe` wrapper — search logic is handled directly by `RecipeManager`, which is simpler and more practical.
- Removed `WeeklyMenu` and `ShoppingList` — these were planned but not implemented to keep the scope manageable.
- Added file persistence (`saveToFile` / `loadFromFile`) which was not in the original plan.
- Added statistics display as a new feature.
