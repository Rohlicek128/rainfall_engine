#pragma once

#include "EditorApplication.h"
#include <string>


namespace editor
{
    class EditorApplication;

    class Editor
    {
        std::string working_dir_;

    public:
        Editor();

        void run(EditorApplication& app);
    };
}
