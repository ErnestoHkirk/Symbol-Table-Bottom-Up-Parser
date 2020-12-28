#include "Memory.hpp"

int Memory::get_value_at_address(int address)
{
  if(this->mem.find(address) != this->mem.end())
    return this->mem[address];
  else
    return 0;
}

bool Memory::update_value_at_address(int address, int value)
{
  if(this->mem.find(address) != this->mem.end())
  {
    this->mem[address] = value;
    return true;
  }
  else
    return false;
}

bool Memory::set_value_at_address(int address, int value)
{
  this->mem[address] = value;
  return true;
}