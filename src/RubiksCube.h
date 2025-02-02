#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include <Cube.h>

class RubiksCube
{
public:
    RubiksCube(int cube_size);

    std::vector<std::vector<std::vector<Cube *>>> &getCubeIndexes();
    Cube *cube_at_index(int i, int j, int k);
    Cube *cube_at_ID(int id);
    Cube *cube_with_color(glm::vec3 cube_color);
    int cube_size;

private:
    std::vector<std::vector<std::vector<Cube *>>> cube_indexes;
};