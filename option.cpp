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

std::string option::GetTags() const
{
  std::stringstream ss;
  for (int i = 0; i < m_tags.size(); i++)
  {
    ss << m_tags.at(i) << " ";
  }
  return ss.str();
}

void option::SetTags(std::string newTag)
{
  m_tags.push_back(newTag);
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
