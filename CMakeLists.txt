cmake_minimum_required(VERSION 3.19)
project(systeme_AS_2021_master C)

set(CMAKE_C_STANDARD 11)

include_directories(TD1)

add_executable(systeme_AS_2021_master
        TD1/exo0.c
        TD1/exo1.c
        TD1/exo2.c
        TD1/exo3.c
        TD1/exo4.c
        TD1/exo5.c
        TD1/exo6.c
        TD1/exo7.c
        TD1/exo8.c
        TD1/exo8.h
        TD2/exo1.c
        TD2/exo2.c
        TD2/exo3.c
        TD2/exo4.c
        TD2/exo5.c
        TD2/exo6.c
        TD2/exo7.c
        TD2/exo8.c
        TD2/exo9.c
        TD3/exo1.c
        TD3/exo2.c
        TD3/exo3.c
        TD3/exo4.c
        TD3/exo5a.c
        TD3/exo5b.c
        TD3/exo6.c
        TD3/exo7.c
        TD3/exo8.c
        TD3/exo9.c
        TD4/exo1.c
        TD4/exo10a.c
        TD4/exo10b.c
        TD4/exo2.c
        TD4/exo3.c
        TD4/exo4.c
        TD4/exo5.c
        TD4/exo6.c
        TD4/exo7.c
        TD4/exo8.c
        TD4/exo9a.c
        TD4/exo9b.c
        TD5/exo1/client.c
        TD5/exo1/serveur.c
        TD5/exo2/client.c
        TD5/exo2/serveur.c
        TD5/exo3/client.c
        TD5/exo3/serveur.c
        TD5/exo4/client.c
        TD5/exo4/serveur.c
        TD5/exo5/client.c
        TD5/exo5/serveur.c Projet/serveur.c Projet/client.c Projet/test.c)
