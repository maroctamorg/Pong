#ifndef START_MENU_HPP
#define START_MENU_HPP

#include "ui.hpp"
#include "../services/client.hpp"

class Start_Menu {
public:
    // MENU
    static std::shared_ptr<Layout> main_layout;
    static std::shared_ptr<Menu> menu;

    // PANNELS
    static std::shared_ptr<Layout> join_random_pannel_layout;
    static std::shared_ptr<Layout> join_custom_pannel_layout;
    static std::shared_ptr<Layout> create_session_pannel_layout;

    static std::shared_ptr<Pannel> join_random_pannel;
    static std::shared_ptr<Pannel> join_custom_pannel;
    static std::shared_ptr<Pannel> create_session_pannel;

    // BUTTONS
    static std::shared_ptr<Layout> join_random_button_layout;
    static std::shared_ptr<Layout> join_custom_button_layout;
    static std::shared_ptr<Layout> create_session_button_layout;

    static std::shared_ptr<Button> join_random_button;
    static std::shared_ptr<Button> join_custom_button;
    static std::shared_ptr<Button> create_session_button;
    static std::shared_ptr<Button> quit_button;




};

#endif