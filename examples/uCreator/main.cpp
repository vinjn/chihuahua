// ImGui - standalone example application for Glfw + OpenGL 3, using programmable pipeline

#include "../../source/IMGUI/imgui.h"
#include "imgui_impl_glfw_gl3.h"
#include <stdio.h>
#include "../../source/glew/GL/glew.h"
#include <GLFW/glfw3.h>

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
    GLFWwindow* window = glfwCreateWindow(1024, 768, "uCreator", NULL, NULL);
    glfwMakeContextCurrent(window);
    glewInit();

    Scene_initializeRenderer(1024, 768);

    // Setup ImGui binding
    ImGui_ImplGlfwGL3_Init(window, true);

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

    bool show_test_window = true;
    bool show_another_window = false;
    ImVec4 clear_color = ImColor(114, 144, 154);

    long nodePtr = Scene_addMeshNode("c:\\Users\\vincentz\\Documents\\VrViewer\\Cerberus_by_Andrew_Maximov\\Cerberus_LP.FBX");
    MeshNode_setAnimationByIndex(nodePtr, 0);
    float k = 10;
    Node_setScale(nodePtr, k, k, k);

    // Main loop
    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();
        ImGui_ImplGlfwGL3_NewFrame();

        // 1. Show a simple window
        // Tip: if we don't call ImGui::Begin()/ImGui::End() the widgets appears in a window automatically called "Debug"
        {
            static float f = 0.0f;
            ImGui::Text("Hello, world!");
            ImGui::SliderFloat("float", &f, 0.0f, 1.0f);
            ImGui::ColorEdit3("clear color", (float*)&clear_color);
            if (ImGui::Button("Test Window")) show_test_window ^= 1;
            if (ImGui::Button("Another Window")) show_another_window ^= 1;
            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
        }

        // 2. Show another simple window, this time using an explicit Begin/End pair
        if (show_another_window)
        {
            ImGui::SetNextWindowSize(ImVec2(200, 100), ImGuiSetCond_FirstUseEver);
            ImGui::Begin("Another Window", &show_another_window);
            ImGui::Text("Hello");
            ImGui::End();
        }

        // 3. Show the ImGui test window. Most of the sample code is in ImGui::ShowTestWindow()
        if (show_test_window)
        {
            ImGui::SetNextWindowPos(ImVec2(650, 20), ImGuiSetCond_FirstUseEver);
            ImGui::ShowTestWindow(&show_test_window);
        }

        // Rendering
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        //glViewport(0, 0, display_w, display_h);
        //glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
        //glClear(GL_COLOR_BUFFER_BIT);

        Scene_clear(clear_color.x * 255, clear_color.y * 255, clear_color.z * 255, clear_color.w * 255);
        {
            // from Metaio SDK
            float modelMatrix[] =
            {
                0.99942285, 0.020722449, -0.026918545, 0.0,
                -0.02240616, 0.9977092, -0.06383123, 0.0,
                0.025534146, 0.06439753, 0.9975976, 0.0,
                -38.078552, -193.14294, -1045.8368, 1.0
            };
            float proj[] =
            {
                3.4011114, 0.0, 0.0, 0.0,
                0.0, 1.9131252, 0.0, 0.0,
                -9.2589855E-4, 5.208254E-4, -1.0033389, -1.0,
                0.0, 0.0, -100.16695, 0.0
            };

            Node_setModelMatrix(nodePtr, modelMatrix);
            Camera_setProjectionMatrix(proj);
            Scene_render();
        }

        ImGui::Render();
        glfwSwapBuffers(window);
    }

    // Cleanup
    Scene_destroy();
    ImGui_ImplGlfwGL3_Shutdown();
    glfwTerminate();

    return 0;
}
