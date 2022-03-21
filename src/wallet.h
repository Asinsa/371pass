// -----------------------------------------------------
// CSC371 Advanced Object Oriented Programming (2021/22)
// Department of Computer Science, Swansea University
//
// Author: 850617
//
// Canvas: https://canvas.swansea.ac.uk/courses/24793
// -----------------------------------------------------
// The root object that holds all data for 371pass. This
// class contains Categories, which in turn hold Items,
// which hold a mapping of entries.
// -----------------------------------------------------

#ifndef WALLET_H
#define WALLET_H

#include <string>
#include <unordered_map>

#include "category.h"

class Wallet {
   private:
    std::unordered_map<std::string, Category> walletEntries;

   public:
    Wallet();

    unsigned int size() const;
    bool empty() const;

    Category& newCategory(std::string categoryIdent);
    bool exists(Category category) const;
    bool addCategory(Category category);
    Category& getCategory(std::string categoryIdent);

    bool deleteCategory(std::string categoryIdent);

    bool updateCategory(std::string oldCategoryIdent, std::string newCategoryIdent);

    void load(std::string filename);
    void save(std::string filename);

    std::unordered_map<std::string, Category> getAllEntries() const;

    friend bool operator==(const Wallet& wObj1, const Wallet& wObj2);

    std::string str() const;
};

#endif  // WALLET_H
