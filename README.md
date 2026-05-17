# Recipe Manager

Console C++ application for managing recipes.

## Functionalities

1. **CRUD** — add, view, edit and delete recipes with ingredients and preparation steps
2. **Search and filter** — search by name, ingredient, tag or category
3. **Portion scaling** — automatically recalculate ingredient quantities for a different number of servings
4. **Favorite recipes** — mark recipes as favorites and filter by them
5. **Rating** — rate recipes from 1 to 5 and sort by rating
6. **Export** — save a recipe to a .txt file

## Class Diagram

```
+------------------------------+
|       NutritionInfo          |
+------------------------------+
| - calories : double          |
| - protein : double           |
| - carbs : double             |
| - fat : double               |
+------------------------------+
| + NutritionInfo()            |
| + getCalories() : double     |
| + getProtein() : double      |
| + getCarbs() : double        |
| + getFat() : double          |
| + operator+(NutritionInfo)   |
| + operator*(double)          |
| + display() const            |
+------------------------------+

+------------------------------+
|        Ingredient            |
+------------------------------+
| - name : string              |
| - quantity : double          |
| - unit : string              |
| - nutrition : NutritionInfo  |
+------------------------------+
| + Ingredient(...)            |
| + getName() : string         |
| + getQuantity() : double     |
| + getUnit() : string         |
| + getNutrition()             |
| + scale(double) : Ingredient |
| + display() const            |
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
| + virtual display() const = 0     |
| + virtual clone() : RecipeBase* =0|
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
| + removeStep(int)                  |
| + scaleServings(int) : Recipe      |
| + getTotalNutrition():NutritionInfo|
| + display() const override        |
| + clone() : RecipeBase* override   |
| + exportToFile(string) const       |
+------------------------------------+

+---------------------------------------+
|        RecipeManager                  |
+---------------------------------------+
| - recipes : vector<Recipe>            |
+---------------------------------------+
| + addRecipe(Recipe)                   |
| + removeRecipe(string) : bool         |
| + getRecipe(string) : Recipe*         |
| + getAllRecipes() : vector<Recipe>&    |
| + searchByName(string) : vector       |
| + searchByTag(string) : vector        |
| + filterByCategory(string) : vector   |
| + searchByIngredient(string) : vector |
| + getFavorites() : vector<Recipe>     |
| + getByRating() : vector<Recipe>      |
+---------------------------------------+
```

### Relationships
- **RecipeBase <- Recipe**: inheritance (Recipe extends the abstract RecipeBase)
- **Recipe -> Ingredient**: composition (a recipe contains a vector of ingredients)
- **Ingredient -> NutritionInfo**: composition (each ingredient has nutrition data)
- **RecipeManager -> Recipe**: aggregation (the manager holds a collection of recipes)

## Design Decisions

1. **RecipeBase is abstract** — `display()` and `clone()` are pure virtual so that different recipe types can implement their own representation. This follows the Open/Closed Principle and allows future extensions without modifying existing code.

2. **RecipeBase fields are protected, not private** — subclasses like `Recipe` need direct access to `name`, `servings`, `tags`, etc. Using protected avoids unnecessary getter/setter calls inside the hierarchy while still hiding data from outside code.

3. **NutritionInfo is a separate class** — nutrition data has its own operations (summing across ingredients, scaling with portions). Extracting it into its own class enables operator overloading (`+` and `*`) and follows the Single Responsibility Principle.

## Build and Run

```
g++ -std=c++17 -o recipe_manager src/main.cpp
./recipe_manager
```
