// -----------------------------------------------------
// CSC371 Advanced Object Oriented Programming (2021/22)
// Department of Computer Science, Swansea University
//
// Author: 850617
//
// Canvas: https://canvas.swansea.ac.uk/courses/24793
// -----------------------------------------------------
// An Item class contains multiple 'entries' as
// key/value pairs (e.g., a key might be 'username'
// and a value would be the username, another might be
// 'url' and the value is the website address the
// username is for.
// -----------------------------------------------------

#ifndef ITEM_H
#define ITEM_H

#include <string>
#include <unordered_map>

class Item {
    private:
    std::unordered_map<std::string, std::string> itemEntries;
    std::string identIdent;

    public:
    Item(std::string identIdent);
    
    unsigned int size() const;
    bool empty();

    void setIdent(std::string newIdent);
    std::string getIdent();

    bool addEntry(std::string key, std::string value);
    std::string getEntry(std::string key);

    bool deleteEntry(std::string key);

    std::unordered_map<std::string, std::string> getAllEntries();
    friend bool operator==(const Item& iObj1, const Item& iObj2);

    std::string str();
};

#endif // ITEM_H
