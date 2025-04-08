cmake_minimum_required(VERSION 3.30)
project(philosophers C)
set(CMAKE_C_STANDARD 99)

add_compile_options(-fsanitize=address)
add_link_options(-fsanitize=address)

add_executable(philo
        main.c
        parsing.c
        actions.c
        simulation.c
        utils.c
        ft_list.c
        queue.c
)
