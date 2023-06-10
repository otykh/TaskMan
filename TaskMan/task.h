#pragma once
#include <iostream>

struct Task
{
    std::string _name;
    bool _completed;
    std::string _description;

    Task(std::string name, bool completed, std::string description) : _name(name), _completed(completed), _description(description)
    { }
};