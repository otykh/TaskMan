#include <iostream>
#include <fstream>
#include <stdio.h>
#include <conio.h>
#include <vector>
#include <string>

#include "task_man.h"

// @BUG when pressed E on empty task the program freezes
// @TODO make sub tasks
// @TODO make auto save
// @TODO add auto expire task (with three Enter/Submit pressed the task will be removed)
// @TODO add remove feature
// @TODO add date/time of task addition
// @TODO add date/time of task deadline
// https://www.rapidtables.com/code/text/ascii-table.html?viewsel=on

int main()
{
    TaskMan taskman;

    int setup_output = taskman.setup();
    if (setup_output != 0)
    {
        std::cout << "Failed opening the temp file" << std::endl;
        return setup_output;
    }

    taskman.start();
}

