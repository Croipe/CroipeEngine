# CroipeEngine
A header only C++ 17 game engine, allowing you to draw 3D Wireframe graphics directly to the command prompt. Everything is built up from the ability to draw pixels and Windows libraries, and the line algorithm is implemented from scratch. The engine features the ability to lead .obj files, take input, and play audio (although it breaks when more than one sound is played due to the way the Windows library used works).

![Alt Spaceship In Command Prompt](Images/spaceship_example.png)
[Code](https://github.com/Croipe/CroipeExamples/tree/main/SpaceShipExample) \
[The spaceship comes from OneLoneCoder's Repository](https://github.com/OneLoneCoder/videos/blob/master/VideoShip.obj)

# Dependencies
Made entirely from the standard and Windows libraries.

# A Minimal Example
```cpp
#include "CroipeEngine/Croipe.cpp"

int main() {
  Croipe::RenderPlatform::CreateScreen("Title", { "Generic Loading Message", "Message 2" });
  
  while (true) {
    Croipe::RenderPlatform::ClearScreen();
    Croipe::TimeHandler::ClockStart();
    // Game Logic
    Croipe::RenderPlatform::UpdateScreen();
    Croipe::TimeHandler::ClockEnd();
  }
  
  Croipe::RenderPlatform::Dispose();
  return 0;
}
```
# Todo
- ~~Move examples to seperate repositories~~
