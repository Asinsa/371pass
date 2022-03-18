// -----------------------------------------------------
// CSC371 Advanced Object Oriented Programming (2021/22)
// Department of Computer Science, Swansea University
//
// Author: 850617
//
// Canvas: https://canvas.swansea.ac.uk/courses/24793
// -----------------------------------------------------

#include "wallet.h"

#include <string>
#include <unordered_map>
#include <iostream>
#include <fstream>
#include <iomanip>
#include "lib_json.hpp"
using nlohmann::json;

#include <iostream>

// TODO Write a Wallet constructor that takes no parameters and constructs an
//  empty wallet.
//
// Example:
//  Wallet wObj{};
Wallet::Wallet() {
}

// TODO Write a function, size, that takes no parameters and returns an unsigned
//  int of the number of categories in the Wallet contains.
//
// Example:
//  Wallet wObj{};
//  auto size = wObj.size();
unsigned int Wallet::size() const {
    return walletEntries.size();
}

// TODO Write a function, empty, that takes no parameters and returns true
//  if the number of categories in the Wallet is zero, false otherwise.
//
// Example:
//  Wallet wwObj{};
//  auto isEmpty = wObj.empty();
bool Wallet::empty() {
    return walletEntries.empty();
}

// TODO Write a function, newCategory, that takes one parameter, a category
//  identifier, and returns the Category object as a reference. If an object
//  with the same identifier already exists, then the existing object should be
//  returned. Throw a std::runtime_error if the Category object cannot be
//  inserted into the container.
//
// Example:
//  Wallet wObj{};
//  wObj.newCategory("categoryIdent");
Category& Wallet::newCategory(std::string categoryIdent) {
    if (walletEntries.find(categoryIdent) == walletEntries.end()) {
        try {
            walletEntries.insert({categoryIdent, Category(categoryIdent)});
        } catch (...) {
            throw std::runtime_error("Item object " + categoryIdent + "cannot be inserted into container");
        }
    }
    return walletEntries.at(categoryIdent);
}

// TODO Write a function, addCategory, that takes one parameter, a Category
//  object, and returns true if the object was successfully inserted. If an
//  object with the same identifier already exists, then the contents should be
//  merged and then return false. Throw a std::runtime_error if the Category
//  object cannot be inserted into the container for whatever reason.
//
// Example:
//  Wallet wObj{};
//  Category cObj{"categoryIdent"};
//  wObj.addCategory(cObj);
bool Wallet::addCategory(Category category) {
    if (walletEntries.find(category.getIdent()) != walletEntries.end()) {
        // MIGHT BE WRONG PLZ LOOK OVER!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
        walletEntries.at(category.getIdent()).getAllEntries().insert(category.getAllEntries().begin(), category.getAllEntries().end());
        return false;
    }
    walletEntries.insert({category.getIdent(), category});
    return true;
}

// TODO Write a function, getCategory, that takes one parameter, a Category
//  identifier and returns the Category. If no Category exists, throw an
//  appropriate exception.
//
// Example:
//  Wallet wObj{};
//  wObj.newCategory("categoryIdent");
//  auto cObj = wObj.getCategory("categoryIdent");
Category& Wallet::getCategory(std::string categoryIdent) {
    if (walletEntries.find(categoryIdent) != walletEntries.end()) {
        return walletEntries.at(categoryIdent);
    }
    else {
        throw std::out_of_range("The item with identifier " + categoryIdent + " does not exist");
    }
}

// TODO Write a function, deleteCategory, that takes one parameter, a Category
//  identifier, and deletes it from the container, and returns true if the
//  Category was deleted. If no Category exists, throw an appropriate exception.
//
// Example:
//  Wallet wObj{};
//  wObj.newCategory("categoryIdent");
//  wObj.deleteCategory("categoryIdent");
bool Wallet::deleteCategory(std::string categoryIdent) {
    if (walletEntries.find(categoryIdent) != walletEntries.end()) {
        walletEntries.erase(categoryIdent);
        return true;
    }
    throw std::out_of_range("The entry " + categoryIdent + " does not exist so cannot be deleted");
    return false;
}

// TODO Write a function, load, that takes one parameter, a std::string,
//  containing the filename for the database. Open the file, read the contents,
//  and populates the container for this Wallet. If the file does open throw an
//  appropriate exception (either std::runtime_error or a derived class).
//
// A note on clashes:
//  If you encounter two categories with the same key, the categories should be
//  merged (not replaced!). If you encounter two items with the same key in the
//  same category, the items should be merged (not replaced!). If you encounter
//  two entries with the same key in the same item, the entries should be merged
//  (undefined as to which value is picked). Two items in different categories
//  can have the same key, as can two entries in different items.
//
// JSON formatting:
//  The JSON file has the following format (see the sample database.json file
//  also provided with the coursework framework):
//    {
//      "Category 1" : {
//        "Item 1":  {
//          "detail 1 key": "detail 1 value",
//          "detail 2 key": "detail 2 value",
//          ...
//        },
//        "Item 2":  {
//          "detail 1 key": "detail 1 value",
//          "detail 2 key": "detail 2 value"
//        },
//        ...
//      },
//      "Category 2": {
//        "Item 1": {
//          "detail 1 key": "detail 1 value"
//        }
//        ...
//      }
//    }
//
// More help:
//  To help you with this function, I've selected the nlohmann::json
//  library that you must use for your coursework. Read up on how to use it
//  here: https://github.com/nlohmann/json. You may not use any other external
//  library other than the one I have provided. You may choose to process the
//  JSON yourself without the help of the library but I guarantee this will be
//  more work.
//
//  Understanding how to use external libraries is part of this coursework! You
//  will need to use this file to deserialize the JSON from string
//  to the JSON object provided by this library. Don't just look at the code
//  provided below, or in the README on the GitHub and despair. Google search,
//  look around, try code out in a separate file to all figure out how to use
//  this library.
//
//  Once you have deserialized the JSON string into an object, you will need to
//  loop through this object, constructing Category and Item objects according
//  to the JSON data in the file.
//
// Example:
//  Wallet wObj{};
//  wObj.load("database.json");
void Wallet::load(std::string filename) {
    try {
        std::ifstream input(filename);
        json jsonWallet;
        input >> jsonWallet;
        
        for (auto category : jsonWallet.items()) {
            Category newCategory(category.key());
            for (auto item : category.value().items()) {
                Item newItem(item.key());
                for (auto& entry : item.value().items()) {
                    newItem.addEntry(entry.key(), entry.value());
                }
                newCategory.addItem(newItem);
            }
            addCategory(newCategory);
        }
    } catch (...) {
        std::runtime_error("File " + filename + "cannot be opened");
    }

}


// TODO Write a function ,save, that takes one parameter, the path of the file
//  to write the database to. The function should serialise the Wallet object
//  as JSON.
//
// Example:
//  Wallet wObj{};
//  wObj.load("database.json");
//  ...
//  wObj.save("database.json");
void Wallet::save(std::string filename) {
    std::ofstream output(filename);

    json wallet = json::parse(str());

    output << std::setw(0) << wallet << std::endl;
}


// TODO Write an == operator overload for the Wallet class, such that two
//  Wallet objects are equal only if they have the exact same data.
//
// Example:
//  Wallet wObj1{};
//  Wallet wObj2{};
//  if(wObj1 == wObj2) {
//    ...
//  }
bool operator==(const Wallet& wObj1, const Wallet& wObj2) {
    return (wObj1.walletEntries == wObj2.walletEntries);
}

// TODO Write a function, str, that takes no parameters and returns a
//  std::string of the JSON representation of the data in the Wallet.
//
// Hint:
//  See the coursework specification for how this JSON should look.
//
// Example:
//  Wallet wObj{};
//  std::string s = wObj.str();
std::string Wallet::str() {
    json categories;
    for(auto category = walletEntries.begin(); category != walletEntries.end(); category++) {
        std::unordered_map<std::string, Item> categoryEntries = category->second.getAllEntries();
        json items;
        for(auto item = categoryEntries.begin(); item != categoryEntries.end(); item++) {
            std::unordered_map<std::string, std::string> itemEntries = item->second.getAllEntries();
            json entries;
            for(auto entry = itemEntries.begin(); entry != itemEntries.end(); entry++) {
                entries[entry->first] = entry->second;
            }
            items[item->first] = entries;
        }
        categories[category->first] = items;
    }
    return categories.dump();
}