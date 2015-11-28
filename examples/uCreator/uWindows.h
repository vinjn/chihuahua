#pragma once

#include "ImWindow/ImwWindow.h"

using namespace ImWindow;

struct EmptyWindow : public ImwWindow
{
    EmptyWindow()
    {
        SetClosable(false);
    }

    virtual void OnGui()
    {
        ImGui::Text("Placeholder");

        if (ImGui::Button("Click"))
        {
        }
    }
};

struct HierarchyWindow : public EmptyWindow
{
    HierarchyWindow()
    {
        SetTitle("Hierarchy");
    }

    void OnGui()
    {

    }
};

struct SceneWindow : public EmptyWindow
{
    SceneWindow()
    {
        SetTitle("Scene");
    }

    void OnGui()
    {

    }
};

struct GameWindow : public EmptyWindow
{
    GameWindow()
    {
        SetTitle("Game");
    }

    void OnGui()
    {

    }
};

struct ProjectWindow : public EmptyWindow
{
    ProjectWindow()
    {
        SetTitle("Project");
    }

    void OnGui()
    {

    }
};

#define IM_ARRAYSIZE(_ARR)  ((int)(sizeof(_ARR)/sizeof(*_ARR)))

struct ConsoleWindow : public EmptyWindow
{
    ImGuiTextBuffer     Buf;
    ImGuiTextFilter     Filter;
    ImVector<int>       LineOffsets;        // Index to lines offset
    bool                ScrollToBottom;

    void    Clear()     { Buf.clear(); LineOffsets.clear(); }

    void    AddLog(const char* fmt, ...) IM_PRINTFARGS(2)
    {
        int old_size = Buf.size();
        va_list args;
        va_start(args, fmt);
        Buf.appendv(fmt, args);
        va_end(args);
        for (int new_size = Buf.size(); old_size < new_size; old_size++)
            if (Buf[old_size] == '\n')
                LineOffsets.push_back(old_size);
        ScrollToBottom = true;
    }

    void    Draw()
    {
        if (ImGui::Button("Clear")) Clear();
        ImGui::SameLine();
        bool copy = ImGui::Button("Copy");
        ImGui::SameLine();
        Filter.Draw("Filter", -100.0f);
        ImGui::Separator();
        ImGui::BeginChild("scrolling", ImVec2(0, 0), false, ImGuiWindowFlags_HorizontalScrollbar);
        if (copy) ImGui::LogToClipboard();

        if (Filter.IsActive())
        {
            const char* buf_begin = Buf.begin();
            const char* line = buf_begin;
            for (int line_no = 0; line != NULL; line_no++)
            {
                const char* line_end = (line_no < LineOffsets.Size) ? buf_begin + LineOffsets[line_no] : NULL;
                if (Filter.PassFilter(line, line_end))
                    ImGui::TextUnformatted(line, line_end);
                line = line_end && line_end[1] ? line_end + 1 : NULL;
            }
        }
        else
        {
            ImGui::TextUnformatted(Buf.begin());
        }

        if (ScrollToBottom)
            ImGui::SetScrollHere(1.0f);
        ScrollToBottom = false;
        ImGui::EndChild();
    }

    ConsoleWindow()
    {
        SetTitle("Console");
    }

    void OnGui()
    {
        static float last_time = -1.0f;
        float time = ImGui::GetTime();
        if (time - last_time >= 0.3f)
        {
            const char* random_words[] = { "system", "info", "warning", "error", "fatal", "notice", "log" };
            AddLog("[%s] Hello, time is %.1f, rand() %d\n", random_words[rand() % IM_ARRAYSIZE(random_words)], time, (int)rand());
            last_time = time;
        }

        Draw();
    }
};

struct InspectorWindow : public EmptyWindow
{
    InspectorWindow()
    {
        SetTitle("Inspector");
    }

    void OnGui()
    {

    }
};

