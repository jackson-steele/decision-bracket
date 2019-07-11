#include "option.h"

option::option()
{

}

option::~option()
{

}

std::string option::GetName() const
{
  return m_name;
}

void option::SetName(std::string name)
{
  m_name = name;
}

int option::GetNumWins() const
{
  return m_numWins;
}

void option::SetNumWins(int numWins)
{
  m_numWins = numWins;
}

void option::IncrementNumWins()
{
  m_numWins += 1;
}
