#include "Hazel.h"

#include "Hazel/Core/EntryPoint.h"

#include "EditorLayer.h"

namespace hazel
{

  class HazelEditor : public Application
  {
  public:
    HazelEditor()
    : Application("hazel-editor")
    {
      pushLayer(new EditorLayer());
    }

    ~HazelEditor() override
    {
    }
  };

  Application *createApplication()
  {
    return new HazelEditor();
  }

}