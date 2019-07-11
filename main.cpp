#include <string>
#include <stack>
#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include "option.h"

//This function runs first and gathers preferred settings from the user
std::string SetPreferences()
{
  std::string dataType;
  std::string saveOption;
  std::string includeOption;
  std::string returnString;
  bool saveData = false;
  bool includeData = false;

  std::cout << "What are you deciding on?\n";
  getline(std::cin,dataType);
  std::cout << std::endl;

  std::ifstream ifs;
  std::string fileName = dataType + ".csv";

  ifs.open(fileName);
  if (!ifs.is_open())
  {
    std::cout << "There is no data on file for that decision.\n";
  }
  else
  {
    std::cout << "There is previous saved data from a similar decision. Would you like to include it today? (Y/N)\n";
    getline(std::cin, includeOption);
    std::cout << std::endl;
    if (includeOption == "y" || includeOption == "Y")
    {
      includeData = true;
    }
  }
  std::cout << "Would you like to save today's data to help with future decisions? (Y/N)\n";
  getline(std::cin, saveOption);
  std::cout << std::endl;
  if (saveOption == "y" || saveOption == "Y")
  {
    saveData = true;
  }
  if (includeData)
  {
    returnString.append("true ");
  }
  else
  {
    returnString.append("false ");
  }
  if (saveData)
  {
    returnString.append("true ");
  }
  else
  {
    returnString.append("false ");
  }
  returnString.append(fileName);
  return returnString;
}

//A function to check if two words are the same (case-insensitive)
bool SameWord(std::string wordOne, std::string wordTwo)
{
  for (int i = 0; i < wordOne.size(); i++)
  {
    wordOne[i] = tolower(wordOne[i]);
  }
  for (int i = 0; i < wordTwo.size(); i++)
  {
    wordTwo[i] = tolower(wordTwo[i]);
  }
  if (wordOne == wordTwo)
  {
    return true;
  }
  return false;
}

void OpenFile(std::string fileName, std::vector<option> &myVector, std::stack<option> &myStack)
{
  std::ifstream ifs;
  std::string temp;

  ifs.open(fileName);
  if (!ifs.is_open())
  {
    std::cout << "Error occured opening file.\n";
  }
  while (getline(ifs, temp))
  {
    option newOption;
    std::string name;
    std::string wins;
    std::istringstream iss(temp);
    std::getline(iss, name, ',');
    std::getline(iss, wins, ',');
    newOption.SetName(name);
    newOption.SetNumWins(stoi(wins));
    myVector.push_back(newOption);
    myStack.push(newOption);
  }
}

void WriteFile(std::string fileName, std::vector<option> myVector, std::string winnerName)
{
  std::ofstream ofs;

  ofs.open(fileName);
  if (!ofs.is_open())
  {
    std::cout << "Error writing to file\n";
  }
  for (int i = 0; i < myVector.size(); i++)
  {
    if (myVector.at(i).GetName() == winnerName)
    {
      ofs << myVector.at(i).GetName() << "," << std::to_string(myVector.at(i).GetNumWins() + 1) << std::endl;
    }
    else
    {
      ofs << myVector.at(i).GetName() << "," << std::to_string(myVector.at(i).GetNumWins()) << std::endl;
    }
  }
}

int FindMax(std::vector<option> vctr)
{
  int max = 0;
  if (vctr.size() == 0)
  {
    return -1;
  }
  if (vctr.size() == 1)
  {
    return 0;
  }
  for (int i = 1; i < vctr.size(); i++)
  {
    if (vctr.at(i).GetNumWins() > vctr.at(max).GetNumWins())
    {
      max = i;
    }
  }
  return max;
}

//This function collects a list of all bracket options, including from a file.
void CollectOptions(std::stack<option> &stack, std::vector<option> &optionVector, bool useOldDataBool, std::string fileName)
{
  std::string optionName;

  if (useOldDataBool)//If we're reading from a file (per user preferences and file availability)
  {
    OpenFile(fileName, optionVector, stack);
  }

  //Get the list
  while (optionName != "done" && optionName != "Done")
  {
    std::cout << "Enter your options. When finished, enter \"Done\"\n";
    getline(std::cin,optionName);
    std::cout << std::endl;
    bool isSame = false;

    if (optionName == "Done" || optionName == "done" && optionVector.size() == 0)
    {
      std::cout << "Error: you must include at least one option.\n";
      optionName = "";
      continue;
    }

    if (optionName == "Done" || optionName == "done")
    {
      continue;
    }

    for (int i = 0; i < optionVector.size(); i++)
    {
      if (SameWord(optionName, optionVector.at(i).GetName()))
      {
        isSame = true;
      }
    }

    if (!isSame)
    {
      option newOption;
      newOption.SetName(optionName);
      optionVector.push_back(newOption);
      stack.push(newOption);
    }
  }
}

void PrintStack(std::stack<option> stack)
{
  std::cout << "You entered:\n\n";
  while (stack.size() != 0)
  {
    std::cout << stack.top().GetName() << "\n";
    stack.pop();
  }
  std::cout << "\n";
}

void ClearStack(std::stack<option> &stack, std::vector<option> &optionVector)
{
  while (stack.size() != 0)
  {
    stack.pop();
  }
  while (optionVector.size() > 0)
  {
    optionVector.pop_back();
  }
}

//This is where the actual decision happens
bool PickFavorites(std::vector<option> vctr, std::string &winnerName)
{
  int max = FindMax(vctr);
  std::string userChoice;
  if (vctr.at(max).GetNumWins() > 0)
  {
    std::cout << "You have a history of choosing " << vctr.at(max).GetName() << ". Would you like to choose that again? (Y/N)\n";
    getline(std::cin, userChoice);
    std::cout << std::endl;
    if (userChoice == "Y" || userChoice == "y")
    {
      winnerName = vctr.at(max).GetName();
      return true;
    }
  }
  return false;
}

void BracketOptions(std::stack<option> stack, std::string &winnerName)
{
  std::stack<option> tempStack;
  if (stack.size() == 1)
  {
    winnerName = stack.top().GetName();
    std::cout << "The winner is: " << winnerName << "\n";
    return;
  }
  while (stack.size() >= 2)
  {
    option numOne = stack.top();
    stack.pop();
    option numTwo = stack.top();
    stack.pop();

    int userChoice = 0;

    std::cout << "Which would you prefer? (1 or 2)\n";
    std::cout << "\t Option 1: " << numOne.GetName() << std::endl;
    std::cout << "\t Option 2: " << numTwo.GetName() << std::endl;
    std::cin >> userChoice;
    std::cout << std::endl;

    while (userChoice != 1 && userChoice != 2)
    {
      std::cout << "Invalid input. Please enter \"1\" or \"2\".\n";
      std::cin >> userChoice;
      std::cout << std::endl;
    }

    if (userChoice == 1)
    {
      tempStack.push(numOne);
    }
    else
    {
      tempStack.push(numTwo);
    }
  }
  while (stack.size() > 0)
  {
    tempStack.push(stack.top());
    stack.pop();
  }
  BracketOptions(tempStack, winnerName);
}

bool VerifyStack(std::stack<option> &stack)
{
  std::string isCorrect;
  std::cout << "Is that correct? Y/N\n";
  getline(std::cin, isCorrect);
  std::cout << std::endl;
  if (isCorrect == "n" || isCorrect == "N")
  {
    /*ClearStack(stack);
    CollectOptions(stack);
    PrintStack(stack);
    VerifyStack(stack);*/
    return false;
  }
  return true;
}

int main()
{
  std::stack<option> optionStack;
  std::vector<option> optionVector;
  std::string userPreferences = SetPreferences();
  std::string saveData;
  std::string useOldData;
  std::string fileName;
  std::string winnerName;
  std::stringstream ss(userPreferences);
  bool saveDataBool = false;
  bool useOldDataBool = false;
  bool isCorrect;
  bool favoritism = false;

  ss >> useOldData >> saveData >> fileName;
  if (saveData == "true")
  {
    saveDataBool = true;
  }
  if (useOldData == "true")
  {
    useOldDataBool = true;
  }
  do
  {
    ClearStack(optionStack, optionVector);
    CollectOptions(optionStack, optionVector, useOldDataBool, fileName);

    PrintStack(optionStack);
    isCorrect = VerifyStack(optionStack);
  } while (!isCorrect);
  favoritism = PickFavorites(optionVector, winnerName);
  if (!favoritism)
  {
    BracketOptions(optionStack, winnerName);
  }
  if (saveDataBool)//If the user wants to save the results of this decision
  {
    WriteFile(fileName, optionVector, winnerName);
  }

  return 0;
}
