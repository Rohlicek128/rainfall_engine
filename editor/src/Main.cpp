#include <iostream>

#include "Editor.h"
#include "EditorApplication.h"


int main()
{
    std::cout << "[EDITOR] START\n";

    editor::Editor editor;
    editor::EditorApplication app;
    editor.run(app);

    std::cout << "[EDITOR] END\n";

    return 0;
}
