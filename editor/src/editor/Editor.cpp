#include "Editor.h"

#include "EditorEngine.h"
#include <engine/core/Engine.h>


namespace editor
{
    void Run()
    {
        engine::Run<EditorEngine>();
    }
}
