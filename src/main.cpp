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
        if (calories > 0) cout << " (" << calories << " kcal)";
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
    void setServings(int s) { if (s > 0) servings = s; }
    void setRating(int r) { if (r >= 1 && r <= 5) rating = r; }
    void toggleFavorite() { favorite = !favorite; }

    void addTag(string tag) {
        if (!hasTag(tag)) tags.push_back(tag);
    }

    bool hasTag(string tag) {
        for (int i = 0; i < tags.size(); i++) {
            if (tags[i] == tag) return true;
        }
        return false;
    }

    virtual void display() = 0;
};

int main() {
    Ingredient i1("tomatoes", 4, "pcs", 22);
    Ingredient i2("cheese", 200, "g", 264);
    i1.display();
    i2.display();

    Ingredient scaled = i1.scale(2);
    cout << "Scaled:" << endl;
    scaled.display();

    cout << "Base classes ready." << endl;
    return 0;
}
