#include <iostream>
#include <fstream>
#include <stdio.h>
#include <conio.h>
#include <vector>
#include <string>
#include <Windows.h>

#include "time_conv.h"
#include "task_man.h"

// @TODO add date/time of task addition
// @TODO add date/time of task deadline
// https://www.rapidtables.com/code/text/ascii-table.html?viewsel=on

TaskMan taskman;

BOOL WINAPI on_exit_handler(DWORD dwCtrlType)
{
    taskman.close();
    return true;
}
int main()
{
    SetConsoleCtrlHandler(on_exit_handler, true);

    int setup_output = taskman.setup();
    if (setup_output != 0)
    {
        std::cout << "Failed opening the temp file" << std::endl;
        return setup_output;
    }

    taskman.start();
}

