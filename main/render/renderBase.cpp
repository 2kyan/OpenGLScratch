#include "renderBase.h"
#include "rslib.h"
#include "camera.h"
#include "config.h"

#include <iostream>
#include <fstream>
#include <streambuf>
#include <string>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

RenderBase::RenderBase()
{
    auto config = RSLib::instance()->getConfig();
    m_scr_width = config->width();
    m_scr_height = config->height();
    m_camera = std::make_shared<Camera>(glm::vec3(0.5f, 0.5f, 3.0f), glm::vec3(0.0f, 1.0f, 0.0f), float(m_scr_width)/m_scr_height);

    m_lastX = m_scr_width / 2.0f;
    m_lastY = m_scr_height / 2.0f;

    m_firstMouse = true;
    m_pressedMouseButton = 0;
    m_show_fhps = false;

    // timing
    m_deltaTime = 0.0f;	// time between current frame and last frame
    m_lastFrame = 0.0f;
}

RenderBase::~RenderBase()
{
}

void RenderBase::fb_resize_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void RenderBase::mouse_click_callback(GLFWwindow* window, int button, int action, int mode)
{
    printf("Button: %x %x %x\n", button, action, mode);
    if (action) {
        m_pressedMouseButton = 1 << button;
    } else {
        m_pressedMouseButton = 0;
    }
}

void RenderBase::mouse_move_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (m_firstMouse) {
        m_lastX = float(xpos);
        m_lastY = float(ypos);
        m_firstMouse = false;
    }

    float xoffset = float(xpos) - m_lastX;
    float yoffset = m_lastY - float(ypos); // reversed since y-coordinates go from bottom to top

    m_lastX = float(xpos);
    m_lastY = float(ypos);

    if (m_pressedMouseButton == 1) {
        m_camera->Rotate(int(xpos), int(ypos), int(xoffset), int(yoffset), m_scr_width, m_scr_height); 
    } else {
        m_camera->ProcessMouseMovement(xoffset, yoffset);
    }
}

void RenderBase::scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    m_camera->ProcessMouseScroll(float(yoffset));
}

void RenderBase::processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        m_camera->ProcessKeyboard(CameraMovement::FORWARD, m_deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        m_camera->ProcessKeyboard(CameraMovement::BACKWARD, m_deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        m_camera->ProcessKeyboard(CameraMovement::LEFT, m_deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        m_camera->ProcessKeyboard(CameraMovement::RIGHT, m_deltaTime);
}
int RenderBase::run()
{
    init();
    prepare();
    render();
    cleanup();

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

int RenderBase::init()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
#endif

    // glfw window creation
    // --------------------
    m_window = glfwCreateWindow(m_scr_width, m_scr_height, "OpenGL", NULL, NULL);
    if (m_window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(m_window);

    glfwSetWindowUserPointer(m_window, reinterpret_cast<void*>(this));

    auto resize_cb = [](GLFWwindow* w, int width, int height) {
        reinterpret_cast<RenderBase*>(glfwGetWindowUserPointer(w))->fb_resize_callback(w, width, height);
    };

    glfwSetFramebufferSizeCallback(m_window, resize_cb);

    auto mouse_clk_cb = [](GLFWwindow* w, int button, int action, int mode) {
        reinterpret_cast<RenderBase*>(glfwGetWindowUserPointer(w))->mouse_click_callback(w, button, action, mode);
    };
    glfwSetMouseButtonCallback(m_window, mouse_clk_cb);

    auto mouse_mv_cb = [](GLFWwindow* w, double xpos, double ypos) {
        reinterpret_cast<RenderBase*>(glfwGetWindowUserPointer(w))->mouse_move_callback(w, xpos, ypos);
    };
    glfwSetCursorPosCallback(m_window, mouse_mv_cb);

    auto scroll_cb = [](GLFWwindow* w, double xoffset, double yoffset) {
        reinterpret_cast<RenderBase*>(glfwGetWindowUserPointer(w))->scroll_callback(w, xoffset, yoffset);
    };
    glfwSetScrollCallback(m_window, scroll_cb);

    // tell GLFW to capture our mouse
    glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader(( GLADloadproc) glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    return 0;
}

int RenderBase::clear_exit(std::string message)
    {
    std::cout << message << std::endl;
    glfwTerminate();
    return -1;
}


