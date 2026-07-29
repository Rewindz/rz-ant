#define GLAD_GL_IMPLEMENTATION
#include <glad/glad.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>

#include <cstdint>
#include <algorithm>

#include "Ant.hpp"
#include "Menu.hpp"

constexpr int WIND_W = 800;
constexpr int WIND_H = 600;
constexpr const char* WIND_TITLE = "Langton's Ant";

constexpr int GRID_W = 250;
constexpr int GRID_H = 250;

constexpr const char vertShader_src[] = {
    #embed "shaders/shader.vert"
};

constexpr const char fragShader_src[] = {
    #embed "shaders/shader.frag"
};

void resize_CB(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

static float zoomLevel = 1.0f;
void scroll_CB(GLFWwindow* window, double xoffset, double yoffset)
{
    zoomLevel += float(yoffset) * 0.1f;
    zoomLevel = std::clamp(zoomLevel, 0.1f, 10.f);
}

static float panX = 0.0f;
static float panY = 0.0f;
static bool isDragging = false;
static double lastX = 0.0;
static double lastY = 0.0;

static bool showMenu = true;

void mousebutton_CB(GLFWwindow* window, int button, int action, int mods)
{
    if(ImGui::GetIO().WantCaptureMouse) return;
    if(button == GLFW_MOUSE_BUTTON_LEFT){
        if(action == GLFW_PRESS) {
            isDragging = true;
            glfwGetCursorPos(window, &lastX, &lastY);
        } else if (action == GLFW_RELEASE) {
            isDragging = false;
        }
    } else if(button == GLFW_MOUSE_BUTTON_RIGHT) {
        if(action == GLFW_RELEASE) {
            showMenu = !showMenu;
        }
    }
}

void cursorpos_CB(GLFWwindow* window, double xpos, double ypos)
{
    if(ImGui::GetIO().WantCaptureMouse) return;
    if(isDragging) {
        double deltaX = xpos - lastX;
        double deltaY = ypos - lastY;
        lastX = xpos; lastY = ypos;
        int w, h;
        glfwGetFramebufferSize(window, &w, &h);
        panX += float(deltaX / w);
        panY += float(deltaY / h);
    }
}

std::uint32_t compileShaders()
{
    auto vertShader = glCreateShader(GL_VERTEX_SHADER);
    const char* vSrc = vertShader_src;
    int vLen = sizeof(vertShader_src);
    glShaderSource(vertShader, 1, &vSrc, &vLen);
    glCompileShader(vertShader);

    auto fragShader = glCreateShader(GL_FRAGMENT_SHADER);
    const char* fSrc = fragShader_src;
    int fLen = sizeof(fragShader_src);
    glShaderSource(fragShader, 1, &fSrc, &fLen);
    glCompileShader(fragShader);

    auto program = glCreateProgram();
    glAttachShader(program, vertShader);
    glAttachShader(program, fragShader);
    glLinkProgram(program);

    glDeleteShader(vertShader);
    glDeleteShader(fragShader);
    return program;
}

int main(void)
{

    if(!glfwInit())
        return 1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    auto* window = glfwCreateWindow(WIND_W, WIND_H, WIND_TITLE, nullptr, nullptr);
    if(!window)
        return 1;

    glfwMakeContextCurrent(window);
    gladLoadGL();

    glfwSwapInterval(1);

    glfwSetFramebufferSizeCallback(window, resize_CB);
    glfwSetScrollCallback(window, scroll_CB);
    glfwSetMouseButtonCallback(window, mousebutton_CB);
    glfwSetCursorPosCallback(window, cursorpos_CB);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.IniFilename = nullptr;
    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 430 core");

    {
        int w, h;
        glfwGetFramebufferSize(window, &w, &h);
        glViewport(0, 0, w, h);
    }

    auto program = compileShaders();

    constexpr float verts[] = {
        -1.0f,  1.0f, 0.0f, 0.0f,
        -1.0f, -1.0f, 0.0f, 1.0f,
         1.0f, -1.0f, 1.0f, 1.0f,
         1.0f,  1.0f, 1.0f, 0.0f
    };

    constexpr std::uint32_t indices[] = {
      0, 1, 2,
      0, 2, 3
    };

    std::uint32_t VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2*sizeof(float)));
    glEnableVertexAttribArray(1);

    std::uint32_t gridText;
    glGenTextures(1, &gridText);
    glBindTexture(GL_TEXTURE_2D, gridText);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    float borderColor[] = {0.05f, 0.05f, 0.05f, 1.0f};
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

    Menu menu(GRID_W, GRID_H);

    glUseProgram(program);

    int antPosLoc = glGetUniformLocation(program, "antPos");
    int gridSizeLoc = glGetUniformLocation(program, "gridSize");
    int zoomLoc = glGetUniformLocation(program, "zoom");
    int panLoc = glGetUniformLocation(program, "pan");

    Grid& grid = menu.GetGrid();

    while(!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        menu.Render(showMenu);

        if(!menu.values.paused) {
            for(int i = 0; i < menu.values.simSteps; i++)
                grid.simulate();
        }

        glClear(GL_COLOR_BUFFER_BIT);

        glUniform2f(antPosLoc, float(grid.ant.x), float(grid.ant.y));
        glUniform1f(zoomLoc, zoomLevel);
        glUniform2f(panLoc, panX, panY);
        glUniform2f(gridSizeLoc, float(grid.w), float(grid.h));

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, grid.w, grid.h, 0, GL_BGRA, GL_UNSIGNED_BYTE, grid.getData());
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glfwSwapBuffers(window);
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glDeleteProgram(program);
    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
