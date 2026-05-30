#include "Pantry.h"
#include <iostream>
#include <iomanip>

using namespace std;

void Pantry::addItem(string name, double quantity, string unit) {
  // If same name+unit exists, add to existing quantity
  for (int i = 0; i < names.size(); i++) {
    if (names[i] == name && units[i] == unit) {
      quantities[i] += quantity;
      return;
    }
  }
  // Otherwise add new entry
  names.push_back(name);
  quantities.push_back(quantity);
  units.push_back(unit);
}

bool Pantry::removeItem(string name) {
  for (int i = 0; i < names.size(); i++) {
    if (names[i] == name) {
      names.erase(names.begin() + i);
      quantities.erase(quantities.begin() + i);
      units.erase(units.begin() + i);
      return true;
    }
  }
  return false;
}

bool Pantry::hasEnough(string name, double quantity, string unit) {
  for (int i = 0; i < names.size(); i++) {
    if (names[i] == name && units[i] == unit) {
      return quantities[i] >= quantity;
    }
  }
  return false;
}

double Pantry::getQuantity(string name, string unit) {
  for (int i = 0; i < names.size(); i++) {
    if (names[i] == name && units[i] == unit)
      return quantities[i];
  }
  return 0;
}

int Pantry::getCount() {
  return names.size();
}

void Pantry::displayPantry() {
  cout << "==========================================" << endl;
  cout << "         YOUR PANTRY" << endl;
  cout << "==========================================" << endl;

  if (names.empty()) {
    cout << "  Pantry is empty." << endl;
    cout << "==========================================" << endl;
    return;
  }

  // Calculate column widths
  int nameWidth = 4;
  int qtyWidth = 8;
  int unitWidth = 4;

  for (int i = 0; i < names.size(); i++) {
    if ((int)names[i].size() > nameWidth)
      nameWidth = names[i].size();
    string qtyStr = to_string(quantities[i]);
    int dotPos = qtyStr.find('.');
    if (dotPos != (int)string::npos) {
      int last = qtyStr.size() - 1;
      while (last > dotPos && qtyStr[last] == '0') last--;
      if (last == dotPos) last--;
      qtyStr = qtyStr.substr(0, last + 1);
    }
    if ((int)qtyStr.size() > qtyWidth)
      qtyWidth = qtyStr.size();
    if ((int)units[i].size() > unitWidth)
      unitWidth = units[i].size();
  }

  nameWidth += 2;
  qtyWidth += 2;

  // Header
  cout << "  ";
  cout.width(nameWidth); cout << left << "Name";
  cout.width(qtyWidth);  cout << left << "Quantity";
  cout << "Unit" << endl;

  cout << "  ";
  for (int i = 0; i < nameWidth + qtyWidth + unitWidth; i++)
    cout << "-";
  cout << endl;

  // Rows
  for (int i = 0; i < names.size(); i++) {
    string qtyStr = to_string(quantities[i]);
    int dotPos = qtyStr.find('.');
    if (dotPos != (int)string::npos) {
      int last = qtyStr.size() - 1;
      while (last > dotPos && qtyStr[last] == '0') last--;
      if (last == dotPos) last--;
      qtyStr = qtyStr.substr(0, last + 1);
    }

    cout << "  ";
    cout.width(nameWidth); cout << left << names[i];
    cout.width(qtyWidth);  cout << left << qtyStr;
    cout << units[i] << endl;
  }

  cout << "==========================================" << endl;
}

void Pantry::clear() {
  names.clear();
  quantities.clear();
  units.clear();
}
