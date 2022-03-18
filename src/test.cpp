#include "wallet.h"
#include <iostream>

int main(int argc, char *argv[]) {

    Wallet wObj{};

    /*
    Category cObj("categoryIdent");
    Item iObj("item 1");
    iObj.addEntry("entry 1", "TEST1");
    iObj.addEntry("entry 2", "TEST2");
    Item iObj2("item 2");
    iObj2.addEntry("entry a", "TEST1111");
    iObj2.addEntry("entry b", "TEST2222");
    cObj.addItem(iObj);
    cObj.addItem(iObj2);

    Category cObj1("cat2");
    Item iObj1("item fuck");
    iObj1.addEntry("fuck 1", "FUCK1");
    iObj1.addEntry("fuck 2", "FUCK2");
    Item iObj3("item 2");
    iObj3.addEntry("fucky a", "FUCK1111");
    iObj3.addEntry("fucky b", "FUCK2222");
    cObj1.addItem(iObj1);
    cObj1.addItem(iObj3);

    wObj.addCategory(cObj);
    wObj.addCategory(cObj1);
    */

    wObj.load("database.json");

    //wObj.str();

    //std::cout << "aaaa" << iObj.str() << std::endl;
    //std::cout << wObj.str() << std::endl;
    
}