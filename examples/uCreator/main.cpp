// ImGui - standalone example application for Glfw + OpenGL 3, using programmable pipeline

#include "IMGUI/imgui.h"
#include "imgui_impl_glfw_gl3.h"
#include <stdio.h>
#include "glew/GL/glew.h"
#include "glfw/include/GLFW/glfw3.h"
#include "ImWindow/ImWindow.h"
#include "uWindows.h"
#include "include/bx/fpumath.h"

#include "Scene3D/src/Scene3D.h"
#ifdef _DEBUG
#pragma comment(lib, "uEngine_d.lib")
#else
#pragma comment(lib, "uEngine.lib")
#endif

static void error_callback(int error, const char* description)
{
    fprintf(stderr, "Error %d: %s\n", error, description);
}

using namespace ImWindow;

static void ShowExampleMenuFile()
{
    ImGui::MenuItem("(dummy menu)", NULL, false, false);
    if (ImGui::MenuItem("New")) {}
    if (ImGui::MenuItem("Open", "Ctrl+O")) {}
    if (ImGui::BeginMenu("Open Recent"))
    {
        ImGui::MenuItem("fish_hat.c");
        ImGui::MenuItem("fish_hat.inl");
        ImGui::MenuItem("fish_hat.h");
        if (ImGui::BeginMenu("More.."))
        {
            ImGui::MenuItem("Hello");
            ImGui::MenuItem("Sailor");
            if (ImGui::BeginMenu("Recurse.."))
            {
                ShowExampleMenuFile();
                ImGui::EndMenu();
            }
            ImGui::EndMenu();
        }
        ImGui::EndMenu();
    }
    if (ImGui::MenuItem("Save", "Ctrl+S")) {}
    if (ImGui::MenuItem("Save As..")) {}
    ImGui::Separator();
    if (ImGui::BeginMenu("Options"))
    {
        static bool enabled = true;
        ImGui::MenuItem("Enabled", "", &enabled);
        ImGui::BeginChild("child", ImVec2(0, 60), true);
        for (int i = 0; i < 10; i++)
            ImGui::Text("Scrolling Text %d", i);
        ImGui::EndChild();
        static float f = 0.5f;
        static int n = 0;
        ImGui::SliderFloat("Value", &f, 0.0f, 1.0f);
        ImGui::InputFloat("Input", &f, 0.1f);
        ImGui::Combo("Combo", &n, "Yes\0No\0Maybe\0\0");
        ImGui::EndMenu();
    }
    if (ImGui::BeginMenu("Colors"))
    {
        for (int i = 0; i < ImGuiCol_COUNT; i++)
            ImGui::MenuItem(ImGui::GetStyleColName((ImGuiCol)i));
        ImGui::EndMenu();
    }
    if (ImGui::BeginMenu("Disabled", false)) // Disabled
    {
        IM_ASSERT(0);
    }
    if (ImGui::MenuItem("Checked", NULL, true)) {}
    if (ImGui::MenuItem("Quit", "Alt+F4")) {}
}

static void ShowExampleAppMainMenuBar()
{
    if (ImGui::BeginMainMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            ShowExampleMenuFile();
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Edit"))
        {
            if (ImGui::MenuItem("Undo", "CTRL+Z")) {}
            if (ImGui::MenuItem("Redo", "CTRL+Y", false, false)) {}  // Disabled item
            ImGui::Separator();
            if (ImGui::MenuItem("Cut", "CTRL+X")) {}
            if (ImGui::MenuItem("Copy", "CTRL+C")) {}
            if (ImGui::MenuItem("Paste", "CTRL+V")) {}
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }
}

int main(int, char**)
{
    // Setup window
    glfwSetErrorCallback(error_callback);
    if (!glfwInit())
        exit(1);
    //glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    //glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    //glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#if __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
    auto vidMode = glfwGetVideoMode(glfwGetPrimaryMonitor());
    auto displayWidth = vidMode->width - 80;
    auto displayHeight = vidMode->height - 80;

    GLFWwindow* window = glfwCreateWindow(displayWidth, displayHeight, "uCreator", NULL, NULL);
    glfwMakeContextCurrent(window);
    glewInit();

    glfwGetFramebufferSize(window, &displayWidth, &displayHeight);

    Scene_initializeRenderer(displayWidth, displayHeight);

    // Setup ImGui binding
    ImGui_ImplGlfwGL3_Init(window, true);

    ImwWindowManager winMgr;
    winMgr.Init();

    auto hierachyWindow = new HierarchyWindow();
    auto sceneWindow = new SceneWindow();
    auto gameWindow = new GameWindow();
    auto projectWindow = new ProjectWindow();
    auto consoleWindow = new ConsoleWindow();
    auto inspectorWindow = new InspectorWindow();

    // right
    winMgr.Dock(inspectorWindow);

    // bottom
    winMgr.DockWith(projectWindow, inspectorWindow, E_DOCK_ORIENTATION_LEFT);
    winMgr.DockWith(consoleWindow, projectWindow, E_DOCK_ORIENTATION_CENTER);

    // top-right
    winMgr.DockWith(sceneWindow, projectWindow, E_DOCK_ORIENTATION_TOP);
    winMgr.DockWith(gameWindow, sceneWindow, E_DOCK_ORIENTATION_CENTER);

    // top-left
    winMgr.DockWith(hierachyWindow, sceneWindow, E_DOCK_ORIENTATION_LEFT);

    // Load Fonts
    // (see extra_fonts/README.txt for more details)
    //ImGuiIO& io = ImGui::GetIO();
    //io.Fonts->AddFontDefault();
    //io.Fonts->AddFontFromFileTTF("../../extra_fonts/Cousine-Regular.ttf", 15.0f);
    //io.Fonts->AddFontFromFileTTF("../../extra_fonts/DroidSans.ttf", 16.0f);
    //io.Fonts->AddFontFromFileTTF("../../extra_fonts/ProggyClean.ttf", 13.0f);
    //io.Fonts->AddFontFromFileTTF("../../extra_fonts/ProggyTiny.ttf", 10.0f);
    //io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 18.0f, NULL, io.Fonts->GetGlyphRangesJapanese());

    // Merge glyphs from multiple fonts into one (e.g. combine default font with another with Chinese glyphs, or add icons)
    //static const ImWchar icons_ranges[] = { 0xf000, 0xf3ff, 0 }; // will not be copied by AddFont* so keep in scope.
    //ImFontConfig icons_config; icons_config.MergeMode = true; icons_config.PixelSnapH = true;
    //io.Fonts->AddFontFromFileTTF("../../extra_fonts/DroidSans.ttf", 18.0f);
    //io.Fonts->AddFontFromFileTTF("../../extra_fonts/fontawesome-webfont.ttf", 18.0f, &icons_config, icons_ranges);

    long nodePtr = Scene_addMeshNode("c:\\Users\\vincentz\\Documents\\VrViewer\\maya-dio\\Dio.obj");
    MeshNode_setAnimationByIndex(nodePtr, 0);
    Node_setMaterialType(nodePtr, Transparent);

    // Main loop
    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();
        ImGui_ImplGlfwGL3_NewFrame();

        ShowExampleAppMainMenuBar();

        float time = glfwGetTime();

        Scene_clear(100, 100, 100, 255);

        winMgr.Run();

        // sceneWindow
        {
            auto sceneWindowPos = sceneWindow->GetLastPosition();
            auto sceneWindowSize = sceneWindow->GetLastSize();
            glViewport(sceneWindowPos.x, displayHeight - 1 - sceneWindowPos.y - sceneWindowSize.y, sceneWindowSize.x, sceneWindowSize.y);

            float scale[16];
            float rot[16];
            float trans[16];
            float tmp0[16];
            float mtx[16];

            float k = 0.1f;
            bx::mtxScale(scale, k, k, k);
            bx::mtxRotateY(rot, time*0.1f);
            bx::mtxTranslate(trans, 0, 0, 30);
            //bx::mtxMul(tmp0, scale, rot);
            bx::mtxMul(mtx, rot, trans);

            float proj[16];
            bx::mtxProj(proj, 60.0f, sceneWindowSize.x / sceneWindowSize.y, 0.1f, 1000.0f, true);

            Node_setModelMatrix(nodePtr, mtx);
            Camera_setProjectionMatrix(proj);
            Scene_render();
        }

        glfwSwapBuffers(window);
    }

    // Cleanup
    Scene_destroy();
    ImGui_ImplGlfwGL3_Shutdown();
    glfwTerminate();

    return 0;
}
