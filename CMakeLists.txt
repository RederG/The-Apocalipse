cmake_minimum_required(VERSION 3.30)

project(The-Apocalipse)

add_executable(THE_APOCALIPSE
    src/main.cpp 
    src/Effects/effect_manager.cpp                      src/Effects/effect.cpp                      src/Effects/healing_effect.cpp      src/Effects/hungry_effect.cpp
    src/Entities/entity.cpp                             src/Entities/player.cpp                     src/Entities/zombie.cpp 
    src/Graphicals/KeysManager.cpp                      src/Graphicals/life_bar.cpp                 src/Graphicals/viewer.cpp 
    src/Help/contain_manager.cpp                        src/Help/debugging.cpp                      src/Help/font_manager.cpp           src/Help/contain_manager.cpp        src/Help/texture_manager.cpp    src/Help/the_apocalipse.cpp     src/Help/thread_manager.cpp
    src/InteractiveObjects/interactive_object.cpp       src/InteractiveObjects/roof.cpp             src/InteractiveObjects/tomb.cpp     src/InteractiveObjects/wall.cpp
    src/Items/item.cpp
    src/Items/Effect_items/effect_item.cpp              src/Items/Effect_items/syringe.cpp 
    src/Items/Foods/artificial_food.cpp                 src/Items/Foods/food.cpp 
    src/Items/Weapons/cartridge.cpp                     src/Items/Weapons/gun.cpp                   src/Items/Weapons/knife.cpp         src/Items/Weapons/weapon.cpp 
    src/Map/Structures/structure_manager.cpp            src/Map/Structures/structure.cpp 
    src/Map/animated_element.cpp                        src/Map/area_group.cpp src/Map/area.cpp     src/Map/animated_element.cpp        src/Map/environment_element.cpp     src/Map/map_element.cpp         src/Map/map_manager.cpp 
    src/Others/bullet.cpp                               src/Others/Collision.cpp                    src/Others/container.cpp            src/Others/pathfinding.cpp          src/Others/perlin_noise.cpp     src/Others/probability.cpp 
    src/window_menu/add_player_window.cpp               src/window_menu/game_over_window.cpp 
    src/window_menu/game_pause_window.cpp               src/window_menu/game_window.cpp 
    src/window_menu/main_menu_window.cpp                src/window_menu/play_window.cpp 
    src/window_menu/player_data_window.cpp              src/window_menu/set_lang_window.cpp 
    src/window_menu/world_settings_window.cpp 
)

target_include_directories(THE_APOCALIPSE
    PUBLIC extern/include
)

target_link_directories(THE_APOCALIPSE
    PUBLIC extern/lib
)

target_link_libraries(THE_APOCALIPSE
    sfml-window
    sfml-system
    sfml-graphics
)