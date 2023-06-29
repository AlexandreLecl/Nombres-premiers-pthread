cmake_minimum_required(VERSION 3.16)
project(TP1)
include_directories(.)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
add_executable(main main.cpp)

include(GNUInstallDirs)
# ceci met à disposition des variables CMake pour les chemins standard de librairies sous GNU, genre /usr/local/lib
 
find_library(GMP_LIB
            NAMES gmp
            HINTS ${CMAKE_INSTALL_LIBDIR})
 
find_library(GMPXX_LIB
            NAMES gmpxx
            HINTS ${CMAKE_INSTALL_LIBDIR})
# ces deux énoncés recherchent les librairies gmp et gmpxx et mettent leur chemin dans les variables GMP_LIB et GMPXX_LIB, respectivement. Note : gmpxx n'est nécessaire que si ton projet est en C++. CMAKE_INSTALL_LIBDIR est une variable fournie par GNUInstallDirs.
 
find_path(GMP_INCLUDE_DIR
              NAMES gmp.h
              HINTS ${CMAKE_INSTALL_INCLUDEDIR})
 
find_path(GMPXX_INCLUDE_DIR
               NAMES gmpxx.h
               HINTS ${CMAKE_INSTALL_INCLUDEDIR})
# Ces deux énoncés recherchent les headers gmp.h et gmpxx.h et mettent leur chemin dans GMP_INCLUDE_DIR et GMPXX_INCLUDEDIR.
 
target_include_directories(main PRIVATE GMP_INCLUDE_DIR GMPXX_INCLUDE_DIR)
# ceci va ajouter les répertoires de headers de GMP aux chemins recherchés pour les headers lors de la compilation. tp1 est le nom de l'exécutable produit par le projet.
 
target_link_libraries(main PRIVATE gmp gmpxx)
# Ceci va lier les librairies gmp et gmpxx à l'exécutable compilé.

target_link_libraries(main PRIVATE Threads::Threads)