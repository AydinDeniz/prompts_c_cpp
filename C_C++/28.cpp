
#include <iostream>
#include <vector>

class VRObject {
public:
    float x, y, z;
    
    VRObject(float x, float y, float z) : x(x), y(y), z(z) {}
    
    void render() {
        std::cout << "Rendering object at (" << x << ", " << y << ", " << z << ")" << std::endl;
    }
};

class VREngine {
    std::vector<VRObject> objects;

public:
    void addObject(float x, float y, float z) {
        objects.emplace_back(x, y, z);
    }

    void renderScene() {
        std::cout << "Rendering VR Scene..." << std::endl;
        for (auto& obj : objects) {
            obj.render();
        }
    }
};

int main() {
    VREngine engine;
    engine.addObject(1.0f, 2.0f, -5.0f);
    engine.addObject(-3.0f, 1.5f, -7.0f);
    engine.renderScene();

    return 0;
}
