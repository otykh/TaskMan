#pragma once
#include <iostream>
#include <ctime>

struct Task
{
    std::string _name;
    bool _completed;
    std::string _description;
    time_t _expiry_time;

    Task(std::string name, bool completed, std::string description, time_t expiry_time) : _name(name), _completed(completed), _description(description), _expiry_time(expiry_time)
    { }
};

struct SchTask
{
    std::string _name;
    std::string _description;

    Task convert_to_task()
    {
        return Task(_name, false, _description, -1); // @TODO set expiry time (non completed struct SchTask)
    }
};