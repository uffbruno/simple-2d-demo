#ifndef MAP2D_HPP
#define MAP2D_HPP

#include <vector>
#include <string>

#include "character.hpp"

const int cell_size = 32;
const int max_rows = 15;
const int max_cols = 15;

typedef std::vector<character*> cell_collection;

enum cell_id {
    CELL_NONE = '_',
    CELL_BLOCK = '#'
};

typedef enum cell_id cell_id;

class map2d {
    public:
        map2d(const std::string& filename);
        ~map2d();
        
        void set_cell(unsigned int id, character* bb);
        void check_collision_with(character* bb);
        void draw() const;
    
    private:
        cell_collection cells;
        
        map2d(const map2d& other);
        map2d& operator=(const map2d& other);
        
        
};

#endif //MAP2D_HPP