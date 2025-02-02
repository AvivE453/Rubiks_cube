#include <Camera.h>

void Camera::SetPerspective(float FOVdegree, float near, float far)
{
    m_Aspect = (float)m_Width / (float)m_Height;
    m_FOVdegree = FOVdegree;
    m_Near = near;
    m_Far = far;

    // Rest Projection and View matrices
    m_Projection = glm::perspective(glm::radians(FOVdegree), m_Aspect, near, far);
    m_View = glm::lookAt(m_Position, m_Position + m_Orientation, m_Up);
}

void Camera::SetOrthographic(float near, float far)
{
    m_Near = near;
    m_Far = far;

    // Rest Projection and View matrices
    m_Projection = glm::ortho(m_Left, m_Right, m_Bottom, m_Top, near, far);
    m_View = lookAt(m_Position, m_Position + m_Orientation, m_Up);
}

void Camera::TranslateProjection(glm::vec3 trans)
{
    m_Projection = glm::translate(m_Projection, trans);
}

void Camera::TranslateView(glm::vec3 trans)
{
    m_View = glm::translate(m_View, trans);
}

void Camera::RotateProjection(float angle, glm::vec3 rot)
{
    m_Projection = glm::rotate(m_Projection, angle, rot);
}

void Camera::RotateView(float angle, glm::vec3 rot)
{
    m_View = glm::rotate(m_View, angle, rot);
}

glm::mat4 Camera::GetViewMatrix() const
{
    return m_View;
}

glm::mat4 Camera::GetProjectionMatrix() const
{
    return m_Projection;
}
/////////////////////////////////////////////
RubiksCube &Camera::getRubiksCube() const
{
    return *rubi;
}

float Camera::getRotAngle() const
{
    return rot_angle;
}

bool Camera::getClockwise() const
{
    return clockwise;
}

void Camera::setRotAngle(float angle)
{
    rot_angle = angle;
}

void Camera::setClockwise(bool flag)
{
    clockwise = flag;
}
///////////////////////////////////////////////
int Camera::getRightState() const
{
    return right_state;
}

void Camera::setRightState(int state)
{
    right_state = state;
}

int Camera::getLeftState() const
{
    return left_state;
}

void Camera::setLeftState(int state)
{
    left_state = state;
}

int Camera::getUpState() const
{
    return up_state;
}

void Camera::setUpState(int state)
{
    up_state = state;
}

int Camera::getDownState() const
{
    return down_state;
}

void Camera::setDownState(int state)
{
    down_state = state;
}

int Camera::getFrontState() const
{
    return front_state;
}

void Camera::setFrontState(int state)
{
    front_state = state;
}

int Camera::getBackState() const
{
    return back_state;
}

void Camera::setBackState(int state)
{
    back_state = state;
}
////////////////////////////////////////////////

float Camera::get_near()
{
    return m_Near;
}

float Camera::get_far()
{
    return m_Far;
}

float Camera::get_width()
{
    return m_Width;
}

float Camera::get_AspectRatio()
{
    return m_Aspect;
}

glm::vec3 &Camera::get_position()
{
    return m_Position;
}

/////////////////////
// Input Callbacks //
/////////////////////

void KeyCallback(GLFWwindow *window, int key, int scanCode, int action, int mods)
{
    Camera *camera = (Camera *)glfwGetWindowUserPointer(window);
    if (!camera)
    {
        std::cout << "Warning: Camera wasn't set as the Window User Pointer! KeyCallback is skipped" << std::endl;
        return;
    }

    float sensitivity = camera->m_KeySensitivity;
    RubiksCube &rubi = camera->getRubiksCube();
    std::vector<std::vector<std::vector<Cube *>>> &cube_indexes = rubi.getCubeIndexes();
    std::vector<Cube *> temp;
    temp.resize(9);
    size_t i;
    int loops = 1;

    if (action == GLFW_PRESS || action == GLFW_REPEAT)
    {
        glm::mat3 viewRotateInverse = glm::transpose(glm::mat3(camera->GetViewMatrix()));

        switch (key)
        {
        case GLFW_KEY_P:
            camera->m_picking_mode = !camera->m_picking_mode;
            if (camera->m_picking_mode)
            {
                std::cout << "picking mode is on" << std::endl;
            }
            else
            {
                std::cout << "picking mode is off" << std::endl;
            }
            break;

        case GLFW_KEY_M:
            Camera::RandomMixer(window, camera);
            break;

        case GLFW_KEY_SPACE:
            camera->setClockwise(!camera->getClockwise());
            break;

        case GLFW_KEY_Z:
            if (camera->getRotAngle() > 45.0f)
                camera->setRotAngle(camera->getRotAngle() / 2);
            std::cout << "angle:" << camera->getRotAngle() << std::endl;
            break;

        case GLFW_KEY_A:
            if (camera->getRotAngle() < 180.0f)
                camera->setRotAngle(camera->getRotAngle() * 2);
            std::cout << "angle:" << camera->getRotAngle() << std::endl;
            break;

        case GLFW_KEY_R:
            if (camera->getUpState() == 0 && camera->getDownState() == 0 && camera->getFrontState() == 0 && camera->getBackState() == 0)
            {
                if (camera->getRotAngle() == 180.0f)
                {
                    loops = 4;
                }
                else if (camera->getRotAngle() == 90.0f)
                {
                    loops = 2;
                }

                for (int k = 0; k < loops; k++)
                {
                    temp.clear();
                    // Collect pointers from the last column
                    for (int height = 0; height < 3; height++)
                    {
                        for (int depth = 0; depth < 3; depth++)
                        {
                            temp.push_back(cube_indexes[height][depth][2]);
                        }
                    }

                    // Rotate each cube in temp
                    for (i = 0; i < 9; ++i)
                    {

                        Cube *cube = temp[i];
                        if (camera->getClockwise())
                        {
                            cube->rotateCube(glm::radians(-45.0f), glm::vec3(1.0f, 0.0f, 0.0f));
                        }
                        else
                        {
                            cube->rotateCube(glm::radians(45.0f), glm::vec3(1.0f, 0.0f, 0.0f));
                        }
                    }

                    if (camera->getClockwise())
                    {
                        camera->setRightState(camera->getRightState() + 1);
                    }
                    else
                    {
                        camera->setRightState(camera->getRightState() - 1);
                    }

                    // Update cube_indexes with the rotated positions
                    i = 0;
                    if (camera->getRightState() == 2)
                    {
                        camera->setRightState(0);
                        for (int depth = 2; depth >= 0; depth--)
                        {
                            for (int height = 0; height < 3; height++)
                            {
                                cube_indexes[height][depth][2] = temp[i];
                                i++;
                            }
                        }
                    }
                    else if (camera->getRightState() == -2)
                    {
                        camera->setRightState(0);
                        for (int depth = 0; depth < 3; depth++)
                        {
                            for (int height = 2; height >= 0; height--)
                            {
                                cube_indexes[height][depth][2] = temp[i];
                                i++;
                            }
                        }
                    }
                }
            }
            else
            {
                std::cout << "Cube is locked" << std::endl;
            }
            break;

        case GLFW_KEY_L:
            if (camera->getUpState() == 0 && camera->getDownState() == 0 && camera->getFrontState() == 0 && camera->getBackState() == 0)
            {
                if (camera->getRotAngle() == 180.0f)
                {
                    loops = 4;
                }
                else if (camera->getRotAngle() == 90.0f)
                {
                    loops = 2;
                }

                for (int k = 0; k < loops; k++)
                {
                    temp.clear();
                    for (int height = 0; height < 3; height++)
                    {
                        for (int depth = 0; depth < 3; depth++)
                        {
                            temp.push_back(cube_indexes[height][depth][0]);
                        }
                    }

                    // Rotate each cube in temp
                    for (i = 0; i < 9; ++i)
                    {
                        Cube *cube = temp[i];
                        if (camera->getClockwise())
                        {
                            cube->rotateCube(glm::radians(45.0f), glm::vec3(1.0f, 0.0f, 0.0f));
                        }
                        else
                        {
                            cube->rotateCube(glm::radians(-45.0f), glm::vec3(1.0f, 0.0f, 0.0f));
                        }
                    }

                    if (camera->getClockwise())
                    {
                        camera->setLeftState(camera->getLeftState() + 1);
                    }
                    else
                    {
                        camera->setLeftState(camera->getLeftState() - 1);
                    }

                    // push back the new places
                    i = 0;
                    if (camera->getLeftState() == 2)
                    {
                        camera->setLeftState(0);
                        for (int depth = 0; depth < 3; depth++)
                        {
                            for (int height = 2; height >= 0; height--)
                            {
                                cube_indexes[height][depth][0] = temp[i];
                                i++;
                            }
                        }
                    }
                    else if (camera->getLeftState() == -2)
                    {
                        camera->setLeftState(0);
                        for (int depth = 2; depth >= 0; depth--)
                        {
                            for (int height = 0; height < 3; height++)
                            {
                                cube_indexes[height][depth][0] = temp[i];
                                i++;
                            }
                        }
                    }
                }
            }
            else
            {
                std::cout << "Cube is locked" << std::endl;
            }
            break;

        case GLFW_KEY_F:
            if (camera->getUpState() == 0 && camera->getDownState() == 0 && camera->getRightState() == 0 && camera->getLeftState() == 0)
            {
                if (camera->getRotAngle() == 180.0f)
                {
                    loops = 4;
                }
                else if (camera->getRotAngle() == 90.0f)
                {
                    loops = 2;
                }

                for (int k = 0; k < loops; k++)
                {
                    temp.clear();
                    for (int height = 0; height < 3; height++)
                    {
                        for (int width = 0; width < 3; width++)
                        {
                            temp.push_back(cube_indexes[height][2][width]);
                        }
                    }

                    for (i = 0; i < 9; ++i)
                    {

                        Cube *cube = temp[i];
                        if (camera->getClockwise())
                        {
                            cube->rotateCube(glm::radians(-45.0f), glm::vec3(0.0f, 0.0f, 1.0f));
                        }
                        else
                        {
                            cube->rotateCube(glm::radians(45.0f), glm::vec3(0.0f, 0.0f, 1.0f));
                        }
                    }

                    if (camera->getClockwise())
                    {
                        camera->setFrontState(camera->getFrontState() + 1);
                    }
                    else
                    {
                        camera->setFrontState(camera->getFrontState() - 1);
                    }

                    i = 0;
                    if (camera->getFrontState() == 2)
                    {
                        camera->setFrontState(0);
                        for (int width = 0; width < 3; width++)
                        {
                            for (int height = 2; height >= 0; height--)
                            {
                                cube_indexes[height][2][width] = temp[i];
                                i++;
                            }
                        }
                    }
                    else if (camera->getFrontState() == -2)
                    {
                        camera->setFrontState(0);
                        for (int width = 2; width >= 0; width--)
                        {
                            for (int height = 0; height < 3; height++)
                            {
                                cube_indexes[height][2][width] = temp[i];
                                i++;
                            }
                        }
                    }
                }
            }
            else
            {
                std::cout << "Cube is locked" << std::endl;
            }
            break;

        case GLFW_KEY_B:
            if (camera->getUpState() == 0 && camera->getDownState() == 0 && camera->getRightState() == 0 && camera->getLeftState() == 0)
            {
                if (camera->getRotAngle() == 180.0f)
                {
                    loops = 4;
                }
                else if (camera->getRotAngle() == 90.0f)
                {
                    loops = 2;
                }

                for (int k = 0; k < loops; k++)
                {
                    temp.clear();
                    for (int height = 0; height < 3; height++)
                    {
                        for (int width = 0; width < 3; width++)
                        {
                            temp.push_back(cube_indexes[height][0][width]);
                        }
                    }

                    for (i = 0; i < 9; ++i)
                    {

                        Cube *cube = temp[i];
                        if (camera->getClockwise())
                        {
                            cube->rotateCube(glm::radians(45.0f), glm::vec3(0.0f, 0.0f, 1.0f));
                        }
                        else
                        {
                            cube->rotateCube(glm::radians(-45.0f), glm::vec3(0.0f, 0.0f, 1.0f));
                        }
                    }

                    if (camera->getClockwise())
                    {
                        camera->setBackState(camera->getBackState() + 1);
                    }
                    else
                    {
                        camera->setBackState(camera->getBackState() - 1);
                    }

                    i = 0;
                    if (camera->getBackState() == 2)
                    {
                        camera->setBackState(0);
                        for (int width = 2; width >= 0; width--)
                        {
                            for (int height = 0; height < 3; height++)
                            {
                                cube_indexes[height][0][width] = temp[i];
                                i++;
                            }
                        }
                    }
                    else if (camera->getBackState() == -2)
                    {
                        camera->setBackState(0);
                        for (int width = 0; width < 3; width++)
                        {
                            for (int height = 2; height >= 0; height--)
                            {
                                cube_indexes[height][0][width] = temp[i];
                                i++;
                            }
                        }
                    }
                }
            }
            else
            {
                std::cout << "Cube is locked" << std::endl;
            }
            break;

        case GLFW_KEY_U:
            if (camera->getFrontState() == 0 && camera->getBackState() == 0 && camera->getRightState() == 0 && camera->getLeftState() == 0)
            {
                if (camera->getRotAngle() == 180.0f)
                {
                    loops = 4;
                }
                else if (camera->getRotAngle() == 90.0f)
                {
                    loops = 2;
                }

                for (int k = 0; k < loops; k++)
                {
                    temp.clear();
                    for (int depth = 0; depth < 3; depth++)
                    {
                        for (int width = 0; width < 3; width++)
                        {
                            temp.push_back(cube_indexes[2][depth][width]);
                        }
                    }

                    for (i = 0; i < 9; ++i)
                    {

                        Cube *cube = temp[i];
                        if (camera->getClockwise())
                        {
                            cube->rotateCube(glm::radians(-45.0f), glm::vec3(0.0f, 1.0f, 0.0f));
                        }
                        else
                        {
                            cube->rotateCube(glm::radians(45.0f), glm::vec3(0.0f, 1.0f, 0.0f));
                        }
                    }

                    if (camera->getClockwise())
                    {
                        camera->setUpState(camera->getUpState() + 1);
                    }
                    else
                    {
                        camera->setUpState(camera->getUpState() - 1);
                    }

                    // push back the new places
                    i = 0;
                    if (camera->getUpState() == 2)
                    {
                        camera->setUpState(0);
                        for (int width = 2; width >= 0; width--)
                        {
                            for (int depth = 0; depth < 3; depth++)
                            {
                                cube_indexes[2][depth][width] = temp[i];
                                i++;
                            }
                        }
                    }
                    else if (camera->getUpState() == -2)
                    {
                        camera->setUpState(0);
                        for (int width = 0; width < 3; width++)
                        {
                            for (int depth = 2; depth >= 0; depth--)
                            {
                                cube_indexes[2][depth][width] = temp[i];
                                i++;
                            }
                        }
                    }
                }
            }
            else
            {
                std::cout << "Cube is locked" << std::endl;
            }

            break;

        case GLFW_KEY_D:
            if (camera->getFrontState() == 0 && camera->getBackState() == 0 && camera->getRightState() == 0 && camera->getLeftState() == 0)
            {
                if (camera->getRotAngle() == 180.0f)
                {
                    loops = 4;
                }
                else if (camera->getRotAngle() == 90.0f)
                {
                    loops = 2;
                }

                for (int k = 0; k < loops; k++)
                {
                    temp.clear();
                    for (int depth = 0; depth < 3; depth++)
                    {
                        for (int width = 0; width < 3; width++)
                        {
                            temp.push_back(cube_indexes[0][depth][width]);
                        }
                    }

                    for (i = 0; i < 9; ++i)
                    {

                        Cube *cube = temp[i];
                        if (camera->getClockwise())
                        {
                            cube->rotateCube(glm::radians(45.0f), glm::vec3(0.0f, 1.0f, 0.0f));
                        }
                        else
                        {
                            cube->rotateCube(glm::radians(-45.0f), glm::vec3(0.0f, 1.0f, 0.0f));
                        }
                    }

                    if (camera->getClockwise())
                    {
                        camera->setDownState(camera->getDownState() + 1);
                    }
                    else
                    {
                        camera->setDownState(camera->getDownState() - 1);
                    }

                    i = 0;
                    if (camera->getDownState() == 2)
                    {
                        camera->setDownState(0);
                        for (int width = 0; width < 3; width++)
                        {
                            for (int depth = 2; depth >= 0; depth--)
                            {
                                cube_indexes[0][depth][width] = temp[i];
                                i++;
                            }
                        }
                    }
                    else if (camera->getDownState() == -2)
                    {
                        camera->setDownState(0);
                        for (int width = 2; width >= 0; width--)
                        {
                            for (int depth = 0; depth < 3; depth++)
                            {
                                cube_indexes[0][depth][width] = temp[i];
                                i++;
                            }
                        }
                    }
                }
            }
            else
            {
                std::cout << "Cube is locked" << std::endl;
            }
            break;

        case GLFW_KEY_UP:
            camera->TranslateView(viewRotateInverse * glm::vec3(0.0f, -sensitivity, 0.0f));
            break;
        case GLFW_KEY_DOWN:
            camera->TranslateView(viewRotateInverse * glm::vec3(0.0f, sensitivity, 0.0f));
            break;
        case GLFW_KEY_LEFT:
            camera->TranslateView(viewRotateInverse * glm::vec3(sensitivity, 0.0f, 0.0f));
            break;
        case GLFW_KEY_RIGHT:
            camera->TranslateView(viewRotateInverse * glm::vec3(-sensitivity, 0.0f, 0.0f));
            break;
        default:
            break;
        }
    }
}

void CursorPosCallback(GLFWwindow *window, double currMouseX, double currMouseY)
{
    Camera *camera = (Camera *)glfwGetWindowUserPointer(window);
    if (!camera)
    {
        std::cout << "Warning: Camera wasn't set as the Window User Pointer! KeyCallback is skipped" << std::endl;
        return;
    }

    float sensitivity = camera->m_MouseSensitivity;

    camera->m_NewMouseX = camera->m_OldMouseX - currMouseX;
    camera->m_NewMouseY = camera->m_OldMouseY - currMouseY;
    camera->m_OldMouseX = currMouseX;
    camera->m_OldMouseY = currMouseY;

    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
    {
        if (camera->m_picking_mode)
        {
            unsigned char color_buffer[]{0, 0, 0, 0};
            glReadPixels(camera->m_OldMouseX, 800 - camera->m_OldMouseY, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, color_buffer);
            glm::vec3 color = glm::vec3(color_buffer[0] / 255.0f, color_buffer[1] / 255.0f, color_buffer[2] / 255.0f);
            camera->picked_cube = camera->getRubiksCube().cube_with_color(color);
            glReadPixels(currMouseX, 800 - currMouseY, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &camera->m_Depth);

            if (camera->picked_cube)
            {
                Cube *c = camera->picked_cube;
                float m_Near = camera->get_near();
                float m_Far = camera->get_far();
                float m_Width = camera->get_width();
                float m_AspectRatio = camera->get_AspectRatio();

                float z = m_Far + camera->m_Depth * (m_Near - m_Far);
                float x_new = ((camera->m_NewMouseX * m_Far) / (m_Width * z)) * m_Near * 2 * glm::tan(m_AspectRatio);
                float y_new = ((camera->m_NewMouseY * m_Far) / (m_Width * z)) * m_Near * 2 * glm::tan(m_AspectRatio);

                glm::vec3 trans = glm::vec3(-x_new * 0.265, y_new * 0.265, 0.0f);

                c->translateCube(trans, camera->GetViewMatrix());
            }
        }
        else
        {
            glm::mat3 viewRotateInverse = glm::transpose(glm::mat3(camera->GetViewMatrix()));
            camera->TranslateView(viewRotateInverse * glm::vec3((float)camera->m_NewMouseX * sensitivity, (float)-camera->m_NewMouseY * sensitivity, 0.0f));
        }
    }
    else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
    {
        if (camera->m_picking_mode)
        {
            unsigned char color_buffer[]{0, 0, 0, 0};
            glReadPixels(camera->m_OldMouseX, 800 - camera->m_OldMouseY, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, color_buffer);
            glm::vec3 color = glm::vec3(color_buffer[0] / 255.0f, color_buffer[1] / 255.0f, color_buffer[2] / 255.0f);
            camera->picked_cube = camera->getRubiksCube().cube_with_color(color);
            glReadPixels(currMouseX, 800 - currMouseY, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &camera->m_Depth);

            if (camera->picked_cube)
            {
                Cube *c = camera->picked_cube;
                float m_Near = camera->get_near();
                float m_Far = camera->get_far();
                float m_Width = camera->get_width();
                float m_AspectRatio = camera->get_AspectRatio();

                float z = m_Far + camera->m_Depth * (m_Near - m_Far);
                float x_new = ((camera->m_NewMouseX * m_Far) / (m_Width * z)) * m_Near * 2 * glm::tan(m_AspectRatio);
                float y_new = ((camera->m_NewMouseY * m_Far) / (m_Width * z)) * m_Near * 2 * glm::tan(m_AspectRatio);

                // glm::vec3 trans = glm::vec3(-x_new * 0.265, y_new * 0.265, 0.0f);

                c->rotateCubePickingMode(x_new, glm::vec3(1.0f, 0.0f, 0.0f));
                c->rotateCubePickingMode(y_new, glm::vec3(0.0f, 1.0f, 0.0f));
            }
        }
        else
        {
            glm::mat3 viewRotateInverse = glm::transpose(glm::mat3(camera->GetViewMatrix()));
            camera->RotateView((float)camera->m_NewMouseX * sensitivity, viewRotateInverse * glm::vec3(0.0f, -1.0f, 0.0f));
            camera->RotateView((float)camera->m_NewMouseY * sensitivity, viewRotateInverse * glm::vec3(-1.0f, 0.0f, 0.0f));
        }
    }
}

void ScrollCallback(GLFWwindow *window, double scrollOffsetX, double scrollOffsetY)
{
    Camera *camera = (Camera *)glfwGetWindowUserPointer(window);
    if (!camera)
    {
        std::cout << "Warning: Camera wasn't set as the Window User Pointer! ScrollCallback is skipped" << std::endl;
        return;
    }

    float sensitivity = camera->m_ScrollSensitivity;
    glm::mat3 viewRotateInverse = glm::transpose(glm::mat3(camera->GetViewMatrix()));
    camera->TranslateView(viewRotateInverse * glm::vec3(0.0f, 0.0f, scrollOffsetY * sensitivity));
}

void Camera::EnableInputs(GLFWwindow *window)
{
    // Set camera as the user pointer for the window
    glfwSetWindowUserPointer(window, this);

    // Handle key inputs
    glfwSetKeyCallback(window, (void (*)(GLFWwindow *, int, int, int, int))KeyCallback);

    // Handle cursor postion and inputs
    glfwSetCursorPosCallback(window, (void (*)(GLFWwindow *, double, double))CursorPosCallback);

    // Handle scroll inputs
    glfwSetScrollCallback(window, (void (*)(GLFWwindow *, double, double))ScrollCallback);
}

void Camera::RandomMixer(GLFWwindow *window, Camera *camera)
{
    // Array of possible actions corresponding to keys
    const int actions[] = {GLFW_KEY_R, GLFW_KEY_L, GLFW_KEY_B, GLFW_KEY_F, GLFW_KEY_U, GLFW_KEY_D};
    const size_t actionSize = sizeof(actions) / sizeof(actions[0]);

    srand(time(0));
    // Simulate 100 random actions
    for (int i = 0; i < 100; ++i)
    {
        int randomNum = rand() % actionSize;
        // Pick a random action
        int randomAction = actions[randomNum];

        // Simulate key press and release
        KeyCallback(window, randomAction, 0, GLFW_PRESS, 0);
        KeyCallback(window, randomAction, 0, GLFW_RELEASE, 0);
    }
}