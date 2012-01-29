#ifndef MAP2D_HPP
#define MAP2D_HPP

#include <vector>
#include <string>

#include "bounding_box.hpp"

const int cell_size = 32;
const int max_rows = 15;
const int max_cols = 15;

typedef std::vector<bounding_box*> cell_collection;

enum cell_id {
    CELL_NONE = ' ',
    CELL_BLOCK = '#'
};

typedef enum cell_id cell_id;

class map2d {
    public:
        map2d(const std::string& filename);
        ~map2d();
        
        bounding_box* get_cell(int row, int column) const;
        void set_cell(int row, int column, bounding_box* bb);
        void check_collision_with(bounding_box& bb);
        void draw() const;
    
    private:
        cell_collection cells;
        
        map2d(const map2d& other);
        map2d& operator=(const map2d& other);
        
        
};

#endif //MAP2D_HPP