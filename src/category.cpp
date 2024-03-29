// -----------------------------------------------------
// CSC371 Advanced Object Oriented Programming (2021/22)
// Department of Computer Science, Swansea University
//
// Author: 850617
//
// Canvas: https://canvas.swansea.ac.uk/courses/24793
// -----------------------------------------------------

#include "category.h"

#include <string>
#include <unordered_map>

#include "lib_json.hpp"
using nlohmann::json;

#include <iostream>

// TODO Write a constructor that takes one parameter, a string identifier
//  and initialises the object and member data.
//
// Example:
//  Category c{"categoryIdent"};
Category::Category(std::string categoryIdent) : categoryIdent(categoryIdent) {
}

// TODO Write a function, size, that takes no parameters and returns an unsigned
//  int of the number of Items in the Category contains.
//
// Example:Item
//  Category c{"categoryIdent"};
//  auto size = c.size();
unsigned int Category::size() const {
    return categoryEntries.size();
}

// TODO Write a function, empty, that takes no parameters and returns true
//  if the number of Items in the Category is zero, false otherwise.
//
// Example:
//  Category c{"categoryIdent"};
//  auto empty = c.empty();
bool Category::empty() const {
    return categoryEntries.empty();
}

// TODO Write a function, getIdent, that returns the identifier for the
//  Category.
//
// Example:
//  Category cObj{"categoryIdent"};
//  auto ident = cObj.getIdent();
std::string Category::getIdent() const {
    return categoryIdent;
}

// TODO Write a function, setIdent, that takes one parameter, a string for a new
//  Category identifier, and updates the member variable. It returns nothing.
//
// Example:
//  Category cObj{"categoryIdent"};
//  cObj.setIdent("categoryIdent2");
void Category::setIdent(std::string newCategoryIdent) {
    this->categoryIdent = newCategoryIdent;
}

// TODO Write a function, newItem, that takes one parameter, an Item identifier,
//  (a string) and returns the Item object as a reference. If an object with the
//  same identifier already exists, then the existing object should be returned.
//  Throw a std::runtime_error if the Item object cannot be inserted into the
//  container for whatever reason.
//
// Example:
//  Category cObj{"categoryIdent"};
//  cObj.newItem("itemIdent");
Item& Category::newItem(std::string itemIdent) {
    if (categoryEntries.find(itemIdent) == categoryEntries.end()) {
        try {
            categoryEntries.insert({itemIdent, Item(itemIdent)});
        } catch (...) {
            throw std::runtime_error("Item object " + itemIdent + "cannot be inserted into container");
        }
    }
    return categoryEntries.at(itemIdent);
}

// Method to check if an item exists in the category
bool Category::exists(Item item) const {
    if (categoryEntries.find(item.getIdent()) != categoryEntries.end()) {
        return true;
    }
    return false;
}

// TODO Write a function, addItem, that takes one parameter, an Item object,
//  and returns true if the object was successfully inserted. If an object with
//  the same identifier already exists, then the contents should be merged and
//  return false.
//
// Example:
//  Category cObj{"categoryIdent"};
//  Item iObj{"itemIdent"};
//  cObj.addItem(iObj);
bool Category::addItem(Item item) {
    if (exists(item)) {
        if (!(item == getItem(item.getIdent()))) {
            for (auto entry : item.getAllEntries()) {
                categoryEntries.at(item.getIdent()).addEntry(entry.first, entry.second);
            }
        }
        return false;
    }
    categoryEntries.insert({item.getIdent(), item});
    return true;
}

// TODO Write a function, getItem, that takes one parameter, an Item
//  identifier (a string) and returns the Item as a reference. If no Item
//  exists, throw an appropriate exception.
//
// Hint:
//  See the test scripts for the exception expected.
//
// Example:
//  Category cObj{"categoryIdent"};
//  cObj.newItem("itemIdent");
//  auto iObj = cObj.getItem("itemIdent");
Item& Category::getItem(std::string itemIdent) {
    if (categoryEntries.find(itemIdent) != categoryEntries.end()) {
        return categoryEntries.at(itemIdent);
    } else {
        throw std::out_of_range("The item with identifier " + itemIdent + " does not exist");
    }
}

// TODO Write a function, deleteItem, that takes one parameter, an Item
//  identifier (a string), deletes it from the container, and returns true if
//  the Item was deleted. If no Item exists, throw an appropriate exception.
//
// Example:
//  Category cObj{"categoryIdent"};
//  cObj.newItem("itemIdent");
//  bool result = cObj.deleteItem("itemIdent");
bool Category::deleteItem(std::string itemIdent) {
    if (categoryEntries.find(itemIdent) != categoryEntries.end()) {
        categoryEntries.erase(itemIdent);
        return true;
    }
    throw std::out_of_range("The entry " + itemIdent + " does not exist so cannot be deleted");
    return false;
}

// Method to update a item ident
bool Category::updateItem(std::string oldItemIdent, std::string newItemIdent) {
    if (exists(oldItemIdent)) {
        auto itemEntries = categoryEntries.find(oldItemIdent);  // Iterator of items in category
        Item item = itemEntries->second;                        // Save current item object
        item.setIdent(newItemIdent);                            // Rename item
        deleteItem(oldItemIdent);                               // Delete current item from map
        addItem(item);                                          // Add category object into new item ident key
        return true;
    } else {
        return false;
    }
}

// Method to return all the entries of the category
std::unordered_map<std::string, Item> Category::getAllEntries() const {
    return categoryEntries;
}

// TODO Write an == operator overload for the Category class, such that two
//  Category objects are equal only if they have the same identifier and same
//  Items.
//
// Example:
//  Category cObj1{"categoryIdent1"};
//  cObj1.newItem("itemIdent");
//  Category cObj2{"categoryIdent2"};
//  if(cObj1 == cObj2) {
//    ...
//  }
bool operator==(const Category& cObj1, const Category& cObj2) {
    return ((cObj1.categoryIdent == cObj2.categoryIdent) && (cObj1.categoryEntries == cObj2.categoryEntries));
}

// TODO Write a function, str, that takes no parameters and returns a
//  std::string of the JSON representation of the data in the Category.
//
// See the coursework specification for how this JSON should look.
//
// Example:
//  Category cObj{"categoryIdent"};
//  std::string s = cObj.str();
std::string Category::str() const {
    json items;
    for (auto item = categoryEntries.begin(); item != categoryEntries.end(); item++) {
        std::unordered_map<std::string, std::string> itemEntries = item->second.getAllEntries();
        json entries;
        for (auto entry = itemEntries.begin(); entry != itemEntries.end(); entry++) {
            entries[entry->first] = entry->second;
        }
        items[item->first] = entries;
    }
    return items.dump();
}
