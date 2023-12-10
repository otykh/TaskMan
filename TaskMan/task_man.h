#pragma once
#include <iostream>
#include "task.h"
#include "keys.h"
#include "task_man_eror_codes.h"
#include <vector>
#include <fstream>
#include <ostream>

class TaskMan
{
private:
    bool arrow_wait;
    bool inspecting;
    bool editing;
    bool making_new;
    int selected_task_index;
    std::vector<Task> task_arr;
    std::ofstream save_file_input;
   
public:
    void start();
    int setup();
    void close();
    bool handle_input(Key);
    bool inspect_handle_input(Key);
    void mark_as_complete(int);
    void add_selected_task_index(int);
    void read_tasks_from_file(std::ifstream& file);
    void remove_expired_tasks();
    void draw_console(std::vector<Task>& arr);
    void edit_page();
    void add_new_page();
    void remove_task();
    inline void clear_screen();
    Key get_key();
    time_t ask_user_time();
};