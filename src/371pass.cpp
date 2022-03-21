// -----------------------------------------------------
// CSC371 Advanced Object Oriented Programming (2021/22)
// Department of Computer Science, Swansea University
//
// Author: 850617
//
// Canvas: https://canvas.swansea.ac.uk/courses/24793
// -----------------------------------------------------

#include "371pass.h"
#include "lib_cxxopts.hpp"
#include "wallet.h"

#include <algorithm>
#include <cstdlib>
#include <exception>
#include <iostream>
#include <string>
#include <unordered_map>

// TODO Complete this function. You have been provided some skeleton code which
//  retrieves the database file name from cxxopts.
//  1. Load the database file by calling load() on a Wallet object
//  2. Parse the 'action' argument to decide what action should be taken (
//     create, read, update, or delete). Read is the easiest to implement, and
//     update is the hardest. The details of how these arguments work is in the
//     coursework specification.
//  4. Save the updated Wallet object to the JSON file if there have been
//     changes (calling save() on the Wallet object).
//
// Some commented out code has been provided. Using some of this will be
// demonstrated in the coursework video on Canvas. Remember, this coursework is
// meant to be challenging and testing your understanding of programming in C++.
// Part of the challenge is figuring things out on your own. That is a major
// part of software development.
//
// Example:
//  int main(int argc, char *argv[]) { return App::run(argc, argv); }
int App::run(int argc, char *argv[]) {
    auto options = App::cxxoptsSetup();
    auto args = options.parse(argc, argv);

    // Print the help usage if requested
    if (args.count("help")) {
        std::cout << options.help() << '\n';
        return 0;
    }

    // Open the database and construct the Wallet
    const std::string db = args["db"].as<std::string>();
    Wallet wObj{};
    // Only uncomment this once you have implemented the load function!
    wObj.load(db);

    // Possible actions
    try {
        const Action a = parseActionArgument(args);
        switch (a) {
            case Action::CREATE:
                createAction(db, args, wObj);
                break;

            case Action::READ:
                readAction(args, wObj);
                break;

            case Action::UPDATE:
                updateAction(db, args, wObj);
                break;

            case Action::DELETE:
                deleteAction(db, args, wObj);
                break;

            default:
                throw std::invalid_argument("action");
                return 1;
        }
    } catch (const std::invalid_argument &inv) {
        std::cerr << "Error: invalid " << inv.what() << " argument(s)." << '\n';
        return 1;
    } catch (const std::out_of_range &range) {
        std::cerr << "Error: missing " << range.what() << " argument(s)." << '\n';
        return 1;
    }
    return 0;
}

// Method to split strings using the delimeter provided
std::vector<std::string> App::splitString(const std::string &stringToSplit, char delimeter) {
    std::istringstream stringstream(stringToSplit);
    std::vector<std::string> splitString;
    std::string split;
    while (std::getline(stringstream, split, delimeter)) {
        splitString.push_back(split);
    }
    return splitString;
}

// Action read
void App::readAction(cxxopts::ParseResult &args, Wallet wObj) {
    if (args.count("category") > 0) {
        std::string category = args["category"].as<std::string>();
        // Checks if category exists throws error if not
        if (wObj.exists(category) == false) {
            throw std::invalid_argument("category");
        }
        if (args.count("item") > 0) {
            std::string item = args["item"].as<std::string>();
            // Checks if item exists and throws error if not
            if (wObj.getCategory(category).exists(item) == false) {
                throw std::invalid_argument("item");
            }
            if (args.count("entry") > 0) {
                std::string entry = args["entry"].as<std::string>();
                // Checks if entry exists and throws error if not
                if (wObj.getCategory(category).getItem(item).exists(entry) == false) {
                    throw std::invalid_argument("entry");
                }
                std::cout << wObj.getCategory(category).getItem(item).getEntry(entry) << '\n';
                return exit(EXIT_SUCCESS);
            }
            std::cout << (wObj.getCategory(category).getItem(item)).str() << '\n';
            return exit(EXIT_SUCCESS);
        } else if (args.count("entry") > 0) {
            throw std::out_of_range("item");
        }
        std::cout << (wObj.getCategory(category)).str() << '\n';
        return exit(EXIT_SUCCESS);
    } else if (args.count("item") > 0 || args.count("entry") > 0) {
        throw std::out_of_range("category");
    } else {
        std::cout << wObj.str() << '\n';
    }
}

// Action create
void App::createAction(const std::string db, cxxopts::ParseResult &args, Wallet wObj) {
    if (args.count("category") > 0) {
        // Gets the category & makes object
        std::string category = args["category"].as<std::string>();
        Category newCategory(category);
        if (args.count("item") > 0) {
            // Gets the item & makes object
            std::string item = args["item"].as<std::string>();
            Item newItem(item);
            if (args.count("entry") > 0) {
                // Gets entry key and value by splitting the string by the comma
                std::vector<std::string> keyValue = splitString(args["entry"].as<std::string>(), ',');
                newItem.addEntry(keyValue.at(0), keyValue.at(1));
            }
            newCategory.addItem(newItem);
        }
        wObj.addCategory(newCategory);
        wObj.save(db);
        return exit(EXIT_SUCCESS);
    } else {
        throw std::out_of_range("category, item or entry");
    }
}

// Action delete
void App::deleteAction(const std::string db, cxxopts::ParseResult &args, Wallet wObj) {
    if (args.count("category") > 0) {
        std::string category = args["category"].as<std::string>();
        // Checks if category exists and throws error if not
        if (wObj.exists(category) == false) {
            throw std::invalid_argument("category");
        }
        if (args.count("item") > 0) {
            std::string item = args["item"].as<std::string>();
            // Checks if item exists and throws error if not
            if (wObj.getCategory(category).exists(item) == false) {
                throw std::invalid_argument("item");
            }
            if (args.count("entry") > 0) {
                std::string entry = args["entry"].as<std::string>();
                // Checks if entry exists and throws error if not
                if (wObj.getCategory(category).getItem(item).exists(entry) == false) {
                    throw std::invalid_argument("entry");
                }
                wObj.getCategory(category).getItem(item).deleteEntry(entry);
                return exit(EXIT_SUCCESS);
            }
            wObj.getCategory(category).deleteItem(item);
            return exit(EXIT_SUCCESS);
        } else if (args.count("entry") > 0) {
            throw std::out_of_range("item");
        }
        wObj.deleteCategory(category);
        wObj.save(db);
        return exit(EXIT_SUCCESS);
    } else if (args.count("item") > 0 || args.count("entry") > 0) {
        throw std::out_of_range("category");
    } else {
        std::cout << wObj.str() << '\n';
    }
}

// Action update
void App::updateAction(const std::string db, cxxopts::ParseResult &args, Wallet wObj) {
    if (args.count("category") > 0) {
        std::vector<std::string> categoryValues = splitString(args["category"].as<std::string>(), ':');
        // Checks if category exists and throws error if not
        if (wObj.exists(categoryValues.at(0)) == false) {
            throw std::invalid_argument("category");
        } else if (categoryValues.size() == 2) {
            wObj.updateCategory(categoryValues.at(0), categoryValues.at(1));
            wObj.save(db);
            return exit(EXIT_SUCCESS);
        }
        if (args.count("item") > 0) {
            std::vector<std::string> itemValues = splitString(args["item"].as<std::string>(), ':');
            // Checks if item exists and throws error if not
            if (wObj.getCategory(categoryValues.at(0)).exists(itemValues.at(0)) == false) {
                throw std::invalid_argument("item");
            } else if (itemValues.size() == 2) {
                wObj.getCategory(categoryValues.at(0)).updateItem(itemValues.at(0), itemValues.at(1));
                wObj.save(db);
                return exit(EXIT_SUCCESS);
            }
            if (args.count("entry") > 0) {
                std::vector<std::string> entryValues = splitString(args["entry"].as<std::string>(), ':');
                if (entryValues.size() == 2 || entryValues.size() == 1) {
                    std::vector<std::string> keyValues = splitString(entryValues.at(entryValues.size() - 1), ',');
                    if (entryValues.size() == 2 && keyValues.size() == 2) {
                        // Checks if entry exists and throws error if not
                        if (wObj.getCategory(categoryValues.at(0)).getItem(itemValues.at(0)).exists(entryValues.at(0)) == false) {
                            throw std::invalid_argument("entry");
                        } else {  // If argument in form NAME:PERSON,ALBERT program will update entry with key NAME to PERSON with value ALBERT.
                            wObj.getCategory(categoryValues.at(0)).getItem(itemValues.at(0)).deleteEntry(entryValues.at(0));
                            wObj.getCategory(categoryValues.at(0)).getItem(itemValues.at(0)).addEntry(keyValues.at(0), keyValues.at(1));
                            wObj.save(db);
                            return exit(EXIT_SUCCESS);
                        }
                    } else if (entryValues.size() == 1 && keyValues.size() == 2) {
                        // Checks if entry exists and throws error if not
                        if (wObj.getCategory(categoryValues.at(0)).getItem(itemValues.at(0)).exists(keyValues.at(0)) == false) {
                            throw std::invalid_argument("entry");
                        } else {  // If argument in form NAME,ALBERT program will update entry with key NAME to have value ALBERT.
                            std::string existingEntryValue = wObj.getCategory(categoryValues.at(0)).getItem(itemValues.at(0)).getEntry(keyValues.at(0));
                            wObj.getCategory(categoryValues.at(0)).getItem(itemValues.at(0)).deleteEntry(keyValues.at(0));
                            wObj.getCategory(categoryValues.at(0)).getItem(itemValues.at(0)).addEntry(keyValues.at(1), existingEntryValue);
                            wObj.save(db);
                            return exit(EXIT_SUCCESS);
                        }
                    } else if (entryValues.size() == 2 && keyValues.size() == 1) {
                        // Checks if entry exists and throws error if not
                        if (wObj.getCategory(categoryValues.at(0)).getItem(itemValues.at(0)).exists(entryValues.at(0)) == false) {
                            throw std::invalid_argument("entry");
                        } else {  // If argument in form NAME:PERSON program will update entry with key NAME to PERSON.
                            wObj.getCategory(categoryValues.at(0)).getItem(itemValues.at(0)).addEntry(entryValues.at(0), entryValues.at(1));
                            wObj.save(db);
                            return exit(EXIT_SUCCESS);
                        }
                    } else {
                        throw std::invalid_argument("entry");
                    }
                } else {
                    throw std::invalid_argument("entry");
                }
            }
        } else if (args.count("entry") > 0) {
            throw std::out_of_range("item");
        }
    } else if (args.count("item") > 0 || args.count("entry") > 0) {
        throw std::out_of_range("category");
    } else {
        std::cout << wObj.str() << '\n';
    }
}

// Create a cxxopts instance. You do not need to modify this function.
//
// Example:
//  auto options = App::cxxoptsSetup();
//  auto args = options.parse(argc, argv);
cxxopts::Options App::cxxoptsSetup() {
    cxxopts::Options cxxopts("371pass", "Student ID: " + STUDENT_NUMBER + "\n");

    cxxopts.add_options()(
        "db", "Filename of the 371pass database",
        cxxopts::value<std::string>()->default_value("database.json"))(

        "action", "Action to take, can be: 'create', 'read', 'update', 'delete'.",
        cxxopts::value<std::string>())(

        "category",
        "Apply action to a category (e.g., if you want to add a category, set the"
        " action argument to 'add' and the category argument to your chosen"
        " category identifier).",
        cxxopts::value<std::string>())(

        "item",
        "Apply action to an item (e.g., if you want to add an item, set the "
        "action argument to 'add', the category argument to your chosen category "
        "identifier and the item argument to the item identifier).",
        cxxopts::value<std::string>())(

        "entry",
        "Apply action to an entry (e.g., if you want to add an entry, set the "
        "action argument to 'add', the category argument to your chosen category "
        "identifier, the item argument to your chosen item identifier, and the "
        "entry argument to the string 'key,value'). If there is no comma, an "
        "empty entry is inserted. If you are simply retrieving an entry, set the "
        "entry argument to the 'key'. If you are updating an entry key, use a : "
        "e.g., oldkey:newkey,newvalue.",
        cxxopts::value<std::string>())(

        "h,help", "Print usage.");

    return cxxopts;
}

// TODO Rewrite this function so that it works. This function should
//  case-insensitively check the action argument retrieved from cxxopts and
//  convert this to a value from the ACTION enum. If an invalid value is given
//  in a string, throw an std::invalid_argument exception.
//
// Example:
//  auto options = App::cxxoptsSetup();
//  auto args = options.parse(argc, argv);
//  App::Action action = parseActionArgument(args);
App::Action App::parseActionArgument(cxxopts::ParseResult &args) {
    std::string input = args["action"].as<std::string>();
    transform(input.begin(), input.end(), input.begin(), ::tolower);
    if (input == "create") {
        return Action::CREATE;
    } else if (input == "read") {
        return Action::READ;
    } else if (input == "update") {
        return Action::UPDATE;
    } else if (input == "delete") {
        return Action::DELETE;
    } else {
        throw std::invalid_argument("action");
    }
}

// TODO Write a function, getJSON, that returns a std::string containing the
//  JSON representation of a Wallet object.
//
// This function has been implemented for you, but you may wish to adjust it.
// You will have to uncomment the code, which has been left commented to ensure
// the coursework framework compiles (i.e., it calls functions that you must
// implement, once you have implemented them you may uncomment this function).
//
// Example:
//  Wallet wObj{};
//  std::cout << getJSON(wObj);
std::string App::getJSON(Wallet &wObj) {
    // return "{}";
    // Only uncomment this once you have implemented the functions used!
    return wObj.str();
}

// TODO Write a function, getJSON, that returns a std::string containing the
//  JSON representation of a specific Category in a Wallet object.
//
// This function has been implemented for you, but you may wish to adjust it.
// You will have to uncomment the code, which has been left commented to ensure
// the coursework framework compiles (i.e., it calls functions that you must
// implement, once you have implemented them you may uncomment this function).
//
// Example:
//  Wallet wObj{};
//  std::string c = "category argument value";
//  std::cout << getJSON(wObj, c);
std::string App::getJSON(Wallet &wObj, const std::string &c) {
    // return "{}";
    // Only uncomment this once you have implemented the functions used!
    auto cObj = wObj.getCategory(c);
    return cObj.str();
}

// TODO Write a function, getJSON, that returns a std::string containing the
//  JSON representation of a specific Item in a Wallet object.
//
// This function has been implemented for you, but you may wish to adjust it.
// You will have to uncomment the code, which has been left commented to ensure
// the coursework framework compiles (i.e., it calls functions that you must
// implement, once you have implemented them you may uncomment this function).
//
// Example:
//  Wallet wObj{};
//  std::string c = "category argument value";
//  std::string i = "item argument value";
//  std::cout << getJSON(wObj, c, i);
std::string App::getJSON(Wallet &wObj, const std::string &c,
                         const std::string &i) {
    // return "{}";
    // Only uncomment this once you have implemented the functions used!
    auto cObj = wObj.getCategory(c);
    const auto iObj = cObj.getItem(i);
    return iObj.str();
}

// TODO Write a function, getJSON, that returns a std::string containing the
//  JSON representation of a specific Entry in a Wallet object.
//
// This function has been implemented for you, but you may wish to adjust it.
// You will have to uncomment the code, which has been left commented to ensure
// the coursework framework compiles (i.e., it calls functions that you must
// implement, once you have implemented them you may uncomment this function).
//
// Example:
//  Wallet wObj{};
//  std::string c = "category argument value";
//  std::string i = "item argument value";
//  std::string e = "entry argument value";
//  std::cout << getJSON(wObj, c, i, e);
std::string App::getJSON(Wallet &wObj, const std::string &c,
                         const std::string &i, const std::string &e) {
    // return "{}";
    // Only uncomment this once you have implemented the functions used!
    auto cObj = wObj.getCategory(c);
    auto iObj = cObj.getItem(i);
    return iObj.getEntry(e);
}