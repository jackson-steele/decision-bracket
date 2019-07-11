#pragma once
#include <vector>
#include <string>
#include <sstream>

class option
{
public:
  option();
  ~option();
  std::string GetName() const;
  void SetName(std::string name);
  int GetNumWins() const;
  void SetNumWins(int numWins);
  void IncrementNumWins();
protected:
  std::string m_name;
  int m_numWins = 0;
  std::vector<std::string> m_tags;
};
