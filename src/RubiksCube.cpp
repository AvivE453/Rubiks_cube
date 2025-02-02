#include <RubiksCube.h>

RubiksCube::RubiksCube(int _cube_size)
{

    // allocates  memory for the cube indexes vector
    cube_indexes.resize(3);
    for (int height = 0; height < 3; height++)
    {
        cube_indexes[height].resize(3);
        for (int depth = 0; depth < 3; depth++)
        {
            cube_indexes[height][depth].resize(3, nullptr);
        }
    }
    int id = 0;
    // allocates the cubes, and place them the cube in the cube indexes vector
    for (int height = -1; height <= 1; height++)
    {
        for (int depth = -1; depth <= 1; depth++)
        {
            for (int width = -1; width <= 1; width++)
            {
                Cube *cube = new Cube((float)width, (float)height, (float)depth, id);
                cube_indexes[height + 1][depth + 1][width + 1] = cube;
                id++;
            }
        }
    }
}

std::vector<std::vector<std::vector<Cube *>>> &RubiksCube::getCubeIndexes()
{
    return cube_indexes;
}

Cube *RubiksCube::cube_at_index(int i, int j, int k)
{
    if (0 <= i && i < 3 && 0 <= j && j < 3 && 0 <= k && k < 3)
    {
        return cube_indexes[i][j][k];
    }
    return nullptr;
}

Cube *RubiksCube::cube_at_ID(int id)
{
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            for (int k = 0; k < 3; k++)
            {
                Cube *c = cube_indexes[i][j][k];
                if (c->id == id)
                {
                    return c;
                }
            }
        }
    }
    return nullptr;
}

Cube *RubiksCube::cube_with_color(glm::vec3 color)
{
    int r = static_cast<int>(color[0] * 255.0f);
    int g = static_cast<int>(color[1] * 255.0f);
    int b = static_cast<int>(color[2] * 255.0f);

    // Calculate id
    int id = r + (g << 8) + (b << 16) - 1;

    return cube_at_ID(id);
}