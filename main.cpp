#include "CroipeEngine/Croipe.cpp"

int main() {
    Croipe::RenderPlatform::CreateScreen("Test", { "Generic Loading Message", "Message 2" });

    Croipe::Math::Wireframe SpaceShip;
    SpaceShip.mesh.LoadFromObjectFile("res/spaceship.obj");
    SpaceShip.translation.z = 8.0f;
    SpaceShip.translation.y = 4.0f;

    Croipe::Math::Wireframe EnemyShip;
    EnemyShip.mesh.LoadFromObjectFile("res/spaceship.obj");
    EnemyShip.translation.z = 16.0f;
    EnemyShip.rotation.y = 34.5f;
    EnemyShip.rotation.x = 0.05f;

    while (true) {
        Croipe::RenderPlatform::ClearScreen();
        Croipe::TimeHandler::ClockStart();
        float speed = 100.0f * Croipe::TimeHandler::ElaspedTime();

        SpaceShip.rotation.z = 0.0f;

        if (Croipe::InputLayer::KeyPressed(VK_UP) || Croipe::InputLayer::KeyPressed(VK_DOWN) || Croipe::InputLayer::KeyPressed(VK_RIGHT) || Croipe::InputLayer::KeyPressed(VK_LEFT)) {
            Croipe::AudioLayer::PlaySoundFile("sound/beep.wav");
        }

        if (Croipe::InputLayer::KeyHeld(VK_UP)) {
            SpaceShip.translation.y -= speed;
        }

        if (Croipe::InputLayer::KeyHeld(VK_DOWN)) {
            SpaceShip.translation.y += speed;
        }

        if (Croipe::InputLayer::KeyHeld(VK_RIGHT)) {
            SpaceShip.rotation.z = 0.3f;
            SpaceShip.translation.x += speed;
        }

        if (Croipe::InputLayer::KeyHeld(VK_LEFT)) {
            SpaceShip.rotation.z = -0.3f;
            SpaceShip.translation.x -= speed;
        }


        Croipe::RenderPlatform::RenderWireframe(EnemyShip, 255, 0, 0);
        Croipe::RenderPlatform::RenderWireframe(SpaceShip, 0, 255, 0);
        Croipe::RenderPlatform::UpdateScreen();
        Croipe::TimeHandler::ClockEnd();
    }

    Croipe::RenderPlatform::Dispose();

    return 0;
}