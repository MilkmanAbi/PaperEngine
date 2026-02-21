#pragma once

#include "Entity.h"
#include <memory>
#include <vector>
#include <string>
#include <algorithm>

namespace Paper {

/**
 * Scene - Container for entities and game logic
 */
class Scene {
public:
    Scene(const std::string& name = "Scene");
    virtual ~Scene();

    // Lifecycle
    virtual void onEnter() {}
    virtual void onExit() {}
    virtual void update(float deltaTime);
    virtual void render();

    // Entity management
    Entity* createEntity(const std::string& name = "Entity");
    void destroyEntity(Entity* entity);
    void clearEntities();

    const std::vector<std::unique_ptr<Entity>>& getEntities() const { return entities; }
    Entity* findEntityByName(const std::string& name) const;

    // Scene properties
    const std::string& getName() const { return name; }
    void setName(const std::string& newName) { name = newName; }

    // Background color for paper-like aesthetics
    Color backgroundColor = Color::paper();

protected:
    std::string name;
    std::vector<std::unique_ptr<Entity>> entities;
    std::vector<Entity*> entitiesToDestroy;

    void processPendingDestructions();
};

/**
 * SceneManager - Handles scene transitions and management
 */
class SceneManager {
public:
    SceneManager();
    ~SceneManager();

    void update(float deltaTime);
    void render();

    // Scene management
    void addScene(const std::string& name, std::unique_ptr<Scene> scene);
    void setActiveScene(const std::string& name);
    Scene* getActiveScene() const { return activeScene.get(); }
    Scene* getScene(const std::string& name) const;

    void removeScene(const std::string& name);
    void clearScenes();

private:
    std::unordered_map<std::string, std::unique_ptr<Scene>> scenes;
    std::unique_ptr<Scene> activeScene;
    std::string activeSceneName;
};

} // namespace Paper
