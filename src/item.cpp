// -----------------------------------------------------
// CSC371 Advanced Object Oriented Programming (2021/22)
// Department of Computer Science, Swansea University
//
// Author: 850617
//
// Canvas: https://canvas.swansea.ac.uk/courses/24793
// -----------------------------------------------------

#include "item.h"

#include <string>
#include <unordered_map>
#include "lib_json.hpp"

// TODO Write a constructor that takes one parameter, a string identifier
//  and initialises the object and member data.
//
// Example:
//  Item iObj{"identIdent"};
Item::Item(std::string identIdent) : identIdent(identIdent) {
}

// TODO Write a function, size, that takes no parameters and returns an unsigned
//  int of the number of entries in the Item contains.
//
// Example:
//  Item iObj{"identIdent"};
//  auto size = iObj.size();
unsigned int Item::size() const {
    return itemEntries.size();
}

// TODO Write a function, empty, that takes no parameters and returns true
//  if the number of entries in the Item is zero, false otherwise.
//
// Example:
//  Item iObj{"identIdent"};
//  auto empty = iObj.empty();
bool Item::empty() {
    return itemEntries.empty();
}

// TODO Write a function, setIdent, that takes one parameter, a string for a new
//  Item identifier, and updates the member variable. It returns nothing.
//
// Example:
//  Item iObj{"identIdent"};
//  iObj.setIdent("identIdent2");
void Item::setIdent(std::string newIdent) {
    this->identIdent = newIdent;
}

// TODO Write a function, getIdent, that returns the identifier for the Item.
//
// Example:
//  Item iObj{"identIdent"};
//  auto ident = iObj.getIdent();
std::string Item::getIdent() {
    return identIdent;
}

// TODO Write a function, addEntry, that takes two parameters, an entry
//  key and value and returns true if the entry was inserted into the
//  container or false if the entry already existed and was replaced.
//
// Example:
//  Item iObj{"identIdent"};
//  iObj.addEntry("key", "value");
bool Item::addEntry(std::string key, std::string value) {
    if (itemEntries.find(key) == itemEntries.end()) {
        itemEntries.insert({key, value});
        return true;
    }
    auto item = itemEntries.find(key);
    item->second = value;
    return false;
}

// TODO Write a function, getEntry, that takes one parameter, an entry
//  key and returns it's value. If no entry exists, throw an appropriate
//  exception.
//
// Example:
//  Item iObj{"identIdent"};
//  iObj.addEntry("key", "value");
//  auto value = iObj.getEntry("key");
std::string Item::getEntry(std::string key) {
    if (itemEntries.find(key) != itemEntries.end()) {
        return itemEntries.at(key);
    }
    else {
        throw std::out_of_range("The entry " + key + " does not exist");
    }
}

// TODO Write a function, deleteEntry, that takes one parameter, an entry
//  key, deletes it from the container, and returns true if the Item was
//  deleted. If no entry exists, throw an appropriate exception.
//
// Example:
//  Item iObj{"identIdent"};
//  iObj.addEntry("key", "value");
//  iObj.deleteEntry("key");
bool Item::deleteEntry(std::string key) {
    if (itemEntries.find(key) != itemEntries.end()) {
        itemEntries.erase(key);
        return true;
    }
    throw std::out_of_range("The entry " + key + " does not exist so cannot be deleted");
    return false;
}

// Method to return all the entries of the item so it can be compared in the operator overload method.
std::unordered_map<std::string, std::string> Item::getAllEntries() {
    return itemEntries;
}

// TODO Write an == operator overload for the Item class, such that two
//  Item objects are equal only if they have the same identifier and same
//  entries.
//
// Example:
//  Item iObj1{"identIdent"};
//  iObj1.addEntry("key", "value");
//  Item iObj2{"identIdent2"};
//  if(iObj1 == iObj2) {
//    ...
//  }
bool operator==(Item iObj1, Item iObj2) {
    if ((iObj1.getIdent() == iObj2.getIdent()) && (iObj1.size() == iObj2.size())) {
        unsigned int count = 0;
        for (auto entry = iObj1.getAllEntries().begin(); entry != iObj1.getAllEntries().end(); ++entry) {
            if (entry->second == iObj2.getEntry(entry->first)) {
                count++;
            }
        }
        if (count == iObj1.size()) {
            return true;
        }
    }
    return false;
}

// TODO Write a function, str, that takes no parameters and returns a
//  std::string of the JSON representation of the data in the Item.
//
// See the coursework specification for how this JSON should look.
//
// Example:
//  Item iObj{"itemIdent"};
//  std::string s = iObj.str();
std::string Item::str() {
    using json = nlohmann::json;
    
    json entries;
    for (auto entry = itemEntries.begin(); entry != itemEntries.end(); ++entry){
        entries[entry->first] = entry->second;
    }

    json jsonRep = {
        {identIdent, entries}
    };

    return jsonRep.dump();
}