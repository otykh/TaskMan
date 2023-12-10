#include "task_man.h"
#include "time_conv.h"

void TaskMan::start()
{
    inspecting = false;
    editing = false;

    draw_console(task_arr);

    while (1)
    {
        bool should_update = false;

        if (editing)
        {
            should_update = true;
            edit_page();
        }
        else if (making_new)
        {
            should_update = true;
            add_new_page();
        }
        else
        {
            Key current_key = get_key();
            if (current_key == Key::None) { continue; }

            if (!inspecting)
            {
                should_update = handle_input(current_key);
            }
            else if (inspecting)
            {
                should_update = inspect_handle_input(current_key);
            }
        }

        if (should_update)
        {
            clear_screen();
            draw_console(task_arr);
        }
    };
}
int TaskMan::setup()
{
    std::ifstream save_file_output;
    save_file_output.open("tmsavefile.txt");
    if (!save_file_output) { return (int)ErrorCode::CANNOT_READ_FILE; }

    read_tasks_from_file(save_file_output);
    remove_expired_tasks();

    save_file_output.close();

    save_file_input.open("tmsavefile.txt");
    if (!save_file_input) { return (int)ErrorCode::CANNOT_WRITE_FILE; }

    return 0;
};
void TaskMan::close()
{
    if (save_file_input.is_open())
    {
        for (int i = 0; i < task_arr.size(); i++)
        {
            save_file_input << task_arr[i]._name << '|' << (task_arr[i]._completed ? '1' : '0') << '|' << task_arr[i]._description << '|' << task_arr[i]._expiry_time << '|' << std::endl;
        }
        save_file_input.close();
    }
}
bool TaskMan::handle_input(Key current_key)
{
    switch (current_key)
    {
        case Key::ArrowKeyDown:
            add_selected_task_index(1);
            return true;

        case Key::ArrowKeyUp:
            add_selected_task_index(-1);
            return true;

        case Key::Space:
        case Key::Submit:
            mark_as_complete(selected_task_index);
            return true;

        case Key::E:
            if (task_arr.size() == 0) { break; }
            inspecting = true;
            return true;

        case Key::N:
            making_new = true;
            break;

        case Key::BCS:
            remove_task();
            return true;
    }

    return false;
}
bool TaskMan::inspect_handle_input(Key current_key)
{
    if (current_key == Key::ESC)
    {
        inspecting = false;
        return true;
    }
    else if (current_key == Key::E)
    {
        editing = true;
    }
    return false;
}
void TaskMan::mark_as_complete(int task_index)
{
    if (task_arr.size() == 0) { return; }
    task_arr[task_index]._completed = !task_arr[task_index]._completed;
}
void TaskMan::add_selected_task_index(int value)
{
    selected_task_index += value;

    if (selected_task_index >= task_arr.size()) {
        selected_task_index = task_arr.size() - 1;
    }

    if (selected_task_index <= 0) {
        selected_task_index = 0;
    }
}
void TaskMan::read_tasks_from_file(std::ifstream& file)
{
    std::string line;
    int field_count = 0;
    while (std::getline(file, line))
    {
        std::string temp_task_name;
        bool temp_task_is_completed = false;
        std::string temp_task_description;
        time_t temp_task_exp_time = -1;

        field_count = 0;
        std::string accum;
        for (int i = 0; i < line.size(); i++)
        {
            if (line[i] == '|') {
                switch (field_count)
                {
                case 0:
                    temp_task_name = accum;
                    break;
                case 1:
                    temp_task_is_completed = accum[0] == '1';
                    break;
                case 2:
                    temp_task_description = accum;
                    break;
                case 3:
                    if (accum.empty()) { // compatability from previous version @TODO remove for the future versions after migration
                        temp_task_exp_time = -1;
                    }
                    else {
                        temp_task_exp_time = std::stoll(accum);
                    }
                    break;
                }
                field_count++;
                accum.clear();
            }
            else
            {
                accum += line[i];
            }
        }

        Task new_task(temp_task_name, temp_task_is_completed, temp_task_description, temp_task_exp_time); // @TODO save/load of the time
        task_arr.push_back(new_task);
    }
}
void TaskMan::remove_expired_tasks()
{
    time_t time_now = TimeConverter::GetTimeNow();

    std::vector<Task>::iterator iter;
    for (iter = task_arr.begin(); iter != task_arr.end(); ) {
        if (iter->_expiry_time != -1 && time_now > iter->_expiry_time)
            iter = task_arr.erase(iter);
        else
            ++iter;
    }
}
void TaskMan::draw_console(std::vector<Task>& arr)
{
    if (inspecting)
    {
        if (arr.size() == 0) { inspecting = false; } // if the array is zero and user wants to read the task contents, then the program should falsify the insection
        else
        {
            std::cout << arr[selected_task_index]._name << std::endl << std::endl;
            std::cout << "Completed: " << (arr[selected_task_index]._completed ? "Yes" : "No") << std::endl;
            std::cout << "Expiry: " << TimeConverter::TimeToString(arr[selected_task_index]._expiry_time) << std::endl;
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
void TaskMan::edit_page()
{
    std::string new_name;
    std::string new_desc;

    Task& task = task_arr[selected_task_index];
    clear_screen();
    std::cout << "Edit mode enabled" << std::endl;
    std::cout << "Previous name of the task: " << task._name << std::endl;
    std::cout << "New name: ";
    std::getline(std::cin, new_name);
    std::cout << std::endl;
    clear_screen();
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
void TaskMan::add_new_page()
{
    std::string new_name;
    std::string new_desc;

    clear_screen();
    std::cout << "Creating new" << std::endl;
    std::cout << "New name: ";
    std::getline(std::cin, new_name);
    std::cout << std::endl;

    if (new_name.empty()) { making_new = false; }
    else
    {
        clear_screen();
        std::cout << "New description" << std::endl;
        std::getline(std::cin, new_desc);

        clear_screen();
        std::cout << "Set expiration date" << std::endl;
        time_t exp_date = ask_user_time();

        Task new_task = Task(new_name, false, new_desc, exp_date);

        task_arr.push_back(new_task);

        making_new = false;
    }
}
void TaskMan::remove_task()
{
    if (task_arr.size() != 0) {
        std::cout << "Are you sure you want to remove " << task_arr[selected_task_index]._name << " (y/n) ";
        char ans = std::getchar();
        if (ans == 'y' || ans == 'Y')
        {
            task_arr.erase(task_arr.begin() + selected_task_index);
            selected_task_index = task_arr.size() - 1;
        }
    }
}
inline void TaskMan::clear_screen()
{
    system("CLS");
}
Key TaskMan::get_key()
{
    int ch = _getwch(); // get character

    // handle arrow input
    if (ch == 224)
    {
        arrow_wait = true;
        return Key::None;
    }
    else if (arrow_wait)
    {
        arrow_wait = false;
        return (Key)(ch * -1);
    }
    else
    {
        return (Key)ch;
    }
}
time_t TaskMan::ask_user_time()
{
    std::string temp;
    time_t time_now;

    int day = 0;
    int month = 0;
    int year = 0;

    std::cout << "Note: First, write day, then month, then year. Put '-' to skip (The task will not expire)" << std::endl;
    std::cout << "D: ";
    std::cin >> temp;

    char first = temp.at(0);

    std::cout << (first != '+');
    if ((first < '0' || first > '9') && first != '-' && first != '+') // fail save, check if the character that, the user input is not +, -, or something else to skip the date selection stage
    {
        return -1;
    }

    day = std::stoi(temp);
    if (temp.at(0) == '+' || temp.at(0) == '-')
    {
        day += TimeConverter::GetDayI();
    }

    clear_screen();
    std::cout << "D: " << day << "/";
    std::cin >> temp;
    month = std::stoi(temp);
    if (temp.at(0) == '+' || temp.at(0) == '-')
    {
        month += TimeConverter::GetMonthI();
    }

    clear_screen();
    std::cout << "D: " << day << "/" << month << "/";
    std::cin >> temp;
    year = std::stoi(temp);
    if (temp.at(0) == '+' || temp.at(0) == '-')
    {
        year += TimeConverter::GetYearI();
    }

    return TimeConverter::RawToTime(day, month, year);
}