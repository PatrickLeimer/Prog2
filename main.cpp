#include <iostream>
#include "ABS.h"
#include "hidden.h"
using namespace std;

int main()
{
  int option = 0;
  cin >> option;
  if(option == 1){
    if (testDefaultConstructorAndGetSize()) {
          std::cout << "Default constructor and getSize() PASSED" << std::endl;;
      } else {
          std::cout << "Default constructor and getSize() FAILED" << std::endl;
      }
  }
  if(option == 2){
    if (testParameterizedConstructor()) {
        std::cout << "Parameterized constructor test PASSED" << std::endl;
    } else {
        std::cout << "Parameterized constructor test FAILED" << std::endl;
    }
  }
  if(option == 3){
    if (testResizingOnPush()) {
        std::cout << "Resizing on push test PASSED" << std::endl;
    } else {
        std::cout << "Resizing on push test FAILED" << std::endl;
    }
  }
  if(option == 4){
    if (testPoppingFromABS()) {
        std::cout << "Popping from ABS test PASSED" << std::endl;
    } else {
        std::cout << "Popping from ABS test FAILED" << std::endl;
    }
  }
  if(option == 5){
    if (testPoppingFromEmptyABS()) {
        std::cout << "Popping from empty ABS test PASSED" << std::endl;
    } else {
        std::cout << "Popping from empty ABS test FAILED" << std::endl;
    }
  }
  if(option == 6){
    if (testPeekingAtEmptyABS()) {
        std::cout << "Peeking at an empty ABS test PASSED" << std::endl;
    } else {
        std::cout << "Peeking at an empty ABS test FAILED" << std::endl;
    }
  }
  if(option == 7){
    if (testPeekingAtContents()) {
        std::cout << "Peeking at contents of ABS test PASSED" << std::endl;
    } else {
        std::cout << "Peeking at contents of ABS test FAILED" << std::endl;
    }
  }
  if(option == 8){
    if (testProperResizingAfterPopping()) {
        std::cout << "Proper resizing after popping test PASSED" << std::endl;
    } else {
        std::cout << "Proper resizing after popping test FAILED" << std::endl;
    }
  }
	if(option == 9){
    cout << "Making integer ABS...\n";
    ABS<int> intABS;
    cout << "Size: " << intABS.getSize() << endl;
    cout << "Max Capacity: " << intABS.getMaxCapacity() << endl;
    cout << "\nPushing items...\n";
    for (int i = 1; i < 10; i++)
    {
      intABS.push(i);
      cout << "\nPushed " << intABS.peek() << endl;
      cout << "New Size: " << intABS.getSize() << endl;
      cout << "New Max Capacity: " << intABS.getMaxCapacity() << endl;
    }

    cout << "\nPopping items...\n";
    for (int i = 1; i < 10; i++)
    {
      cout << "\nPopped " << intABS.pop() << endl;
      cout << "New Size: " << intABS.getSize() << endl;
      cout << "New Max Capacity: " << intABS.getMaxCapacity() << endl;
    }


    cout << "\nMaking float ABS...\n";
    ABS<float> floatABS(10);
    cout << "Size: " << floatABS.getSize() << endl;
    cout << "Max Capacity: " << floatABS.getMaxCapacity() << endl;
    cout << "\nPushing items...\n";
    for (float i = 1; i < 5; i += 0.5f)
    {
      floatABS.push(i);
      cout << "\nPushed " << floatABS.peek() << endl;
      cout << "New Size: " << floatABS.getSize() << endl;
      cout << "New Max Capacity: " << floatABS.getMaxCapacity() << endl;
    }

    cout << "\nPopping items...\n";
    for (float i = 1; i < 5; i += 0.5f)
    {
      cout << "\nPopped " << floatABS.pop() << endl;
      cout << "New Size: " << floatABS.getSize() << endl;
      cout << "New Max Capacity: " << floatABS.getMaxCapacity() << endl;
    }
  }

	return 0;
}