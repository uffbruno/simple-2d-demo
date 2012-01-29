#include <fstream>
#include <iostream>

#include "map2d.hpp"

map2d::map2d(const std::string& filename): cells(max_rows * max_cols) {
    std::ifstream file(filename.c_str());
    if (!file) {
        std::cout << "error opening file " << filename << std::endl;
        return;
    }
    
    std::string line;
    
    int pos_x = 0;
    int pos_y = 0;
    while (std::getline(file, line)) {
        for (unsigned int i = 0; i < line.size(); ++i) {
            if (line[i] == '#') {
                bounding_box *bb = new bounding_box(pos_x, pos_y, 
                    cell_size, cell_size, 0, 0);
                bb->set_color(al_map_rgb(0, 255, 0));
                cells.push_back(bb);
            } else {
                cells.push_back(NULL);
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

bounding_box* map2d::get_cell(int row, int col) const{
    unsigned int id = row * max_cols + col;
    if (id >= cells.size()) {
        std::cout << "get_cell: invalid cell id in map 2d: " << id << std::endl;
        return NULL;
    }
    return cells.at(id);
}

void map2d::set_cell(int row, int col, bounding_box *bb) {
    unsigned int id = row * max_cols + col;
    if (id >= cells.size()) {
        std::cout << "set_cell: invalid cell id in map 2d: " << id << std::endl;
    } else if (cells.at(id) != NULL) {
        delete cells[id];
        cells[id] = bb;
    }
}

void map2d::check_collision_with(bounding_box& bb) {
    for (unsigned int i = 0; i < cells.size(); ++i) {
        bb.handle_collide(cells[i]);
    }
}

void map2d::draw() const {
    for (unsigned int i = 0; i < cells.size(); ++i) {
        if (cells.at(i) != NULL) {
            cells[i]->draw();
        }
    }
}