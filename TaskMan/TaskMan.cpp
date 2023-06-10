#include <iostream>
#include <stdio.h>
#include <conio.h>
#include <vector>
#include <string>

//@TODO make sub tasks
// https://www.rapidtables.com/code/text/ascii-table.html?viewsel=on

enum Key
{
    None = 0,
    ArrowKeyUp = -72,
    ArrowKeyDown = -80,
    ArrowKeyLeft = -75,
    ArrowKeyRight = -77,
    Submit = 13,
    Space = 32,
    ESC = 27,
    BCS = 8,
    E = 101,
    N = 110,
};
struct Task
{
    std::string _name;
    bool _completed;
    std::string _description;

    Task(std::string name, bool completed, std::string description) : _name(name), _completed(completed), _description(description)
    { }
};

bool inspecting;
bool editing;
bool making_new;
int selected_task_index;
std::vector<Task> task_arr;

void draw_console(std::vector<Task>& arr)
{
    if (inspecting)
    {
        if (arr.size() == 0) { inspecting = false; } // if the array is zero and user wants to read the task contents, then the program should falsify the insection
        else
        {
            std::cout << arr[selected_task_index]._name << std::endl << std::endl;
            std::cout << "Completed: ";
            if (arr[selected_task_index]._completed) {
                std::cout << "Yes";
            }
            else {
                std::cout << "No";
            }
            std::cout << std::endl;
            std::cout << arr[selected_task_index]._description << std::endl; 
        }
    }
    else
    {
        if (arr.size() == 0)
        {
            std::cout << "No Tasks" << std::endl;
            return;
        }
        for (int i = 0; i < arr.size(); i++)
        {
            std::cout << arr[i]._name << ":\t\t[";
            if (selected_task_index == i)
            {
                if (arr[i]._completed)
                {
                    std::cout << (char)219 << ']';
                }
                else
                {
                    std::cout << (char)177 << ']'; // the cursor sign
                }
            }
            else if (arr[i]._completed)
            {
                std::cout << "x]";
            }
            else
            {
                std::cout << " ]";
            }
            std::cout << std::endl;
        }
    }
}
int main()
{
    inspecting = false;
    editing = false;

    /*task_arr.push_back(Task("Task 1", false, "Description"));
    task_arr.push_back(Task("Task 2", false, "BIG DESRCIPTION GALHAGAOGALGAHJLGHAG"));
    task_arr.push_back(Task("Task 3", false, "d"));*/

    draw_console(task_arr);

    Key currentKey = Key::None;
    bool arrow_wait = false;
    while (1)
    {
        bool should_update = false;

        if (editing)
        {
            should_update = true;

            std::string new_name;
            std::string new_desc;

            Task& task = task_arr[selected_task_index];
            system("CLS");
            std::cout << "Edit mode enabled" << std::endl;
            std::cout << "Previous name of the task: " << task._name << std::endl;
            std::cout << "New name: ";
            std::getline(std::cin, new_name);
            std::cout << std::endl;
            system("CLS");
            std::cout << "Previous description: " << std::endl << task._description << std::endl << std::endl;
            std::cout << "New description" << std::endl;
            std::getline(std::cin, new_desc);
            
            if (new_name.empty() == false)
            {
                task._name = new_name;
            }
            if (new_desc.empty() == false)
            {
                task._description = new_desc;
            }

            editing = false;
        }
        else if(making_new)
        {
            should_update = true;

            std::string new_name;
            std::string new_desc;

            system("CLS");
            std::cout << "Creating new" << std::endl;
            std::cout << "New name: ";
            std::getline(std::cin, new_name);
            std::cout << std::endl;

            if (new_name.empty()) { making_new = false; }
            else
            {
                system("CLS");
                std::cout << "New description" << std::endl;
                std::getline(std::cin, new_desc);

                Task new_task = Task(new_name, false, new_desc);

                task_arr.push_back(new_task);

                making_new = false;
            }
        }
        else
        {
            int ch = _getwch(); // get character

            // handle arrow input
            if (ch == 224)
            {
                arrow_wait = true;
                continue;
            }
            else if (arrow_wait)
            {
                currentKey = (Key)(ch * -1);
                arrow_wait = false;
            }
            else
            {
                currentKey = (Key)ch;
            }

            if (!inspecting)
            {
                // handle input
                switch (currentKey)
                {
                case Key::ArrowKeyDown:
                    selected_task_index++;
                    if (selected_task_index >= task_arr.size()) { selected_task_index = task_arr.size() - 1; }
                    should_update = true;
                    break;
                case Key::ArrowKeyUp:
                    selected_task_index--;
                    if (selected_task_index <= 0) { selected_task_index = 0; }
                    should_update = true;
                    break;
                case Key::Space:
                case Key::Submit:
                    if (task_arr.size() == 0) { break; }
                    should_update = true;
                    task_arr[selected_task_index]._completed = !task_arr[selected_task_index]._completed;
                    break;
                case Key::E:
                    should_update = true;
                    inspecting = true;
                    break;
                case Key::N:
                    making_new = true;
                    break;
                case Key::BCS:
                    if (task_arr.size() != 0) {
                        std::cout << "Are you sure you want to remove " << task_arr[selected_task_index]._name << " (y/n) ";
                        char ans = std::getchar();
                        if (ans == 'y' || ans == 'Y')
                        {
                            task_arr.erase(task_arr.begin() + selected_task_index);
                            selected_task_index = task_arr.size() - 1;
                            should_update = true;
                        }
                    }
                    break;
                }
            }
            else if (inspecting)
            {
                if (currentKey == Key::ESC)
                {
                    inspecting = false;
                    should_update = true;
                }
                else if (currentKey == Key::E)
                {
                    editing = true;
                }
            }
        }

        if (should_update)
        {
            system("CLS"); // clear the screen
            draw_console(task_arr);
        }
    };
}

