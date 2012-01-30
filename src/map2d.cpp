#include <fstream>
#include <iostream>

#include "map2d.hpp"
#include "char_2d.hpp"

map2d::map2d(const std::string& filename): cells(max_rows * max_cols) {
    std::ifstream file(filename.c_str());
    if (!file) {
        std::cout << "error opening file " << filename << std::endl;
        return;
    }
    
    std::string line;
    
    float pos_x = 0;
    float pos_y = 0;
    while (std::getline(file, line)) {
        for (unsigned int i = 0; i < line.size(); ++i) {
            if (line[i] == '#') {
                character *c = new char_2d(pos_x, pos_y, 0, 0, "resources//map");
                c->init();
                cells.push_back(c);
            }
            pos_x += cell_size;
        }
        pos_x = 0;
        pos_y += cell_size;
    }
    
    file.close();
}

map2d::~map2d() {
    for (unsigned int i = 0; i < cells.size(); ++i) {
        if (cells[i] != NULL) {
            delete cells[i];
        }
    }
}

void map2d::set_cell(unsigned int id, character *c) {
    if (id >= cells.size()) {
        std::cout << "set_cell: invalid cell id in map 2d: " << id << std::endl;
    } else if (cells.at(id) != NULL) {
        delete cells[id];
        cells[id] = c;
    }
}

void map2d::check_collision_with(character *c) {
	if (c == NULL) return;

    for (unsigned int i = 0; i < cells.size(); ++i) {
        c->handle_collision(cells[i]);
    }
}

void map2d::draw() const {
    for (unsigned int i = 0; i < cells.size(); ++i) {
        if (cells.at(i) != NULL) {
            cells[i]->draw();
        }
    }
}