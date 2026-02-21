#pragma once

#include "Vector2.h"
#include "Math.h"
#include <memory>
#include <vector>
#include <unordered_map>
#include <typeindex>
#include <string>

namespace Paper {

// Forward declarations
class Entity;
class Scene;

/**
 * Component - Base class for all components
 */
class Component {
public:
    virtual ~Component() = default;
    
    Entity* getEntity() const { return entity; }
    bool isEnabled() const { return enabled; }
    void setEnabled(bool value) { enabled = value; }

    virtual void onAttach() {}
    virtual void onDetach() {}
    virtual void update(float deltaTime) {}

protected:
    friend class Entity;
    Entity* entity = nullptr;
    bool enabled = true;
};

/**
 * Transform - Position, rotation, scale component (every entity has one)
 */
class Transform : public Component {
public:
    Vector2 position{0, 0};
    float rotation = 0.0f; // In degrees
    Vector2 scale{1, 1};
    int layer = 0; // Z-order for paper layering

    Transform() = default;
    Transform(const Vector2& pos) : position(pos) {}
    Transform(float x, float y) : position(x, y) {}

    Vector2 forward() const {
        float rad = rotation * 3.14159f / 180.0f;
        return Vector2(std::cos(rad), std::sin(rad));
    }

    Vector2 right() const {
        float rad = (rotation + 90) * 3.14159f / 180.0f;
        return Vector2(std::cos(rad), std::sin(rad));
    }
};

/**
 * Entity - Game object that holds components
 */
class Entity {
public:
    Entity(Scene* scene, const std::string& name = "Entity");
    ~Entity();

    // Component management
    template<typename T, typename... Args>
    T* addComponent(Args&&... args) {
        static_assert(std::is_base_of<Component, T>::value, "T must derive from Component");
        
        auto component = std::make_unique<T>(std::forward<Args>(args)...);
        T* ptr = component.get();
        ptr->entity = this;
        
        components[std::type_index(typeid(T))] = std::move(component);
        ptr->onAttach();
        return ptr;
    }

    template<typename T>
    T* getComponent() {
        auto it = components.find(std::type_index(typeid(T)));
        if (it != components.end()) {
            return static_cast<T*>(it->second.get());
        }
        return nullptr;
    }

    template<typename T>
    bool hasComponent() const {
        return components.find(std::type_index(typeid(T))) != components.end();
    }

    template<typename T>
    void removeComponent() {
        auto it = components.find(std::type_index(typeid(T)));
        if (it != components.end()) {
            it->second->onDetach();
            components.erase(it);
        }
    }

    // Entity properties
    Transform* transform() { return &m_transform; }
    const std::string& getName() const { return name; }
    void setName(const std::string& newName) { name = newName; }
    
    bool isActive() const { return active; }
    void setActive(bool value) { active = value; }

    Scene* getScene() const { return scene; }

    void update(float deltaTime);
    void destroy();

private:
    Scene* scene;
    std::string name;
    bool active = true;
    bool destroyed = false;
    
    Transform m_transform;
    std::unordered_map<std::type_index, std::unique_ptr<Component>> components;

    friend class Scene;
};

} // namespace Paper
