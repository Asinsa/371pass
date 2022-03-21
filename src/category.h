// -----------------------------------------------------
// CSC371 Advanced Object Oriented Programming (2021/22)
// Department of Computer Science, Swansea University
//
// Author: 850617
//
// Canvas: https://canvas.swansea.ac.uk/courses/24793
// -----------------------------------------------------
// A category contains one or more Items, each with
// their own identifier ('ident').
// -----------------------------------------------------

#ifndef CATEGORY_H
#define CATEGORY_H

#include <string>
#include <unordered_map>

#include "item.h"

class Category {
   private:
    std::unordered_map<std::string, Item> categoryEntries;
    std::string categoryIdent;

   public:
    Category(std::string categoryIdent);

    unsigned int size() const;
    bool empty() const;

    std::string getIdent() const;
    void setIdent(std::string newIdent);

    Item& newItem(std::string itemIdent);
    bool exists(Item item) const;
    bool addItem(Item item);
    Item& getItem(std::string itemIdent);

    bool deleteItem(std::string itemIdent);

    bool updateItem(std::string oldItemIdent, std::string newItemIdent);

    std::unordered_map<std::string, Item> getAllEntries() const;
    friend bool operator==(const Category& cObj1, const Category& cObj2);

    std::string str() const;
};

#endif  // CATEGORY_H
