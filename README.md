# 371pass

You should use this README to document things you did that you thing are clever and could get you marks, or things you couldn't get working and perhaps why. It is fully expected that a lot of students will not complete this coursework—it is designed to be challenging. The mark scheme is also designed to be fair—marks are awarded for everything you complete, with some given for the quality of the code you write.


## Things that work
The main functionality of the program generally works, it is possible to create, read, update and delete. And my code passes all of the given unit and input tests. The 4 actions are split into 4 methods for clarity and I have added a few extra methods in the object classes to make it easier to perform some of the actions.

#### Create
 - The category, item or entries can be created & appropriate error messages are outputted to stderr
 - If item to be created already exists nothing happens

#### Read
 - The category, item or entries can be read & appropriate error messages are outputted to stderr
 - Data is outputted to stdout with exit code on 0

#### Update
 - The category, item or entries can be renamed & appropriate error messages are outputted to stderr

#### Delete
 - The category, item or entries can be deleted & appropriate error messages are outputted to stderr
 


## Things I tried to get working but couldn't
I tried to make it so that you could get multiple things to be changed using update, for example:
```/bin/371pass --action update --category Category:NewCategory --item Item:NewItem --entry Entry:TestKey,TestValue```
However my program is only currently able to accept 1 change. This is beacuse of the way my if statements were setup. I spent some time thinking about it but I wasn't sure how to make it less of a mess while still making sure the rest of the program passes the tests. The attempts I made resulted in error outputs getting jumbled and confused.