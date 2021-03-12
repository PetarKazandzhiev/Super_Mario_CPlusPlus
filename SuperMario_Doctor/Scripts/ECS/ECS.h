#pragma once
#include <iostream>
#include <vector>
#include <memory>
#include <algorithm>
#include <bitset>
#include <array>


class Component;
class Entity;

using ComponentID = std::size_t;

inline ComponentID getComponentTypeID()
{
	static ComponentID lastID = 0;
	return lastID++;
}

template <typename T> 
inline ComponentID getComponentTypeID() noexcept
{
	static ComponentID typeID = getComponentTypeID();
	return typeID;
}

constexpr std::size_t maxComponents = 32;

using ComponentBitSet = std::bitset<maxComponents>;
using ComponentArray = std::array<Component*, maxComponents>;

class Component
{
private:

public:
	Entity* entity;

	virtual void init() {}
	virtual void update() {}
	virtual void draw() {}

	virtual ~Component() {}
};

class Entity
{
private:
	bool active = true;
	std::vector<std::unique_ptr<Component> > components;

	ComponentArray componentArray;
	ComponentBitSet componentBitSet;

public:

	int speed = 3;
	int wSize = 32;
	int hSize = 32;
	int jumpSpeed = 18;
	int zIndex = 0;

	Entity() {}

	Entity(int inputWSize, int inputHSize)
	{
		wSize = inputWSize;
		hSize = inputHSize;
	}

	Entity(int inputSpeed, int inputJump, int inputWSize, int inputHSize)
	{
		speed = inputSpeed;
		wSize = inputWSize;
		hSize = inputHSize;
		jumpSpeed = inputJump;
	}
;
	void update()
	{
		for (auto& component : components)
		{
			component->update();
		}
	}

	void draw() 
	{
		for (auto& component : components)
		{
			component->draw();
		}
	}

	bool isActive() const{ return active; }
	void destroy() { active = false; }

	template <typename T> 
	bool hasComponent() const
	{
		return componentBitSet[getComponentTypeID<T>()];
	}

	template <typename T, typename... TArgs> 
	T& addComponent( TArgs&&... mArgs)
	{
		T* c(new T(std::forward<TArgs>(mArgs)...));
		c->entity = this;
		std::unique_ptr<Component> uPtr{ c };
		components.emplace_back(std::move(uPtr));

		componentArray[getComponentTypeID<T>()] = c;
		componentBitSet[getComponentTypeID<T>()] = true;

		c->init();
		return *c;
	}

	template <typename T>
	T& getComponent() const
	{
		auto ptr(componentArray[getComponentTypeID<T>()]);
		return *static_cast<T*>(ptr);		
	}
};

class Manager
{
private:
	std::vector<std::unique_ptr<Entity>> entities;

	Entity& createEntity(Entity* e)
	{
		std::unique_ptr<Entity> uPtr{ e };
		entities.emplace_back(std::move(uPtr));
		std::sort(entities.begin(), entities.end(), [](std::unique_ptr<Entity> &a, std::unique_ptr<Entity> &b) {
			return a->zIndex > b->zIndex;
		});
		return *e;
	}
public:
	void update()
	{
		for (auto& entity : entities)
		{
			entity->update();
		}
	}

	void draw()
	{
		for (auto& entity : entities)
		{
			entity->draw();
		}
	}

	void refresh()
	{
		//remove the not active entities
		entities.erase(std::remove_if(std::begin(entities), std::end(entities), [](const std::unique_ptr<Entity>& mEntity)
			{
				return !mEntity->isActive();
			}),
			std::end(entities));
	}

	Entity& addEntity(int inputWSize = 32, int inputHSize = 32)
	{
		Entity* e = new Entity(inputWSize, inputHSize);
		return createEntity(e);
	}
	Entity& addMovingEntity(int inputSpeed, int inputJump = 18, int inputWSize = 32, int inputHSize = 32)
	{
		Entity* e = new Entity(inputSpeed, inputJump, inputWSize, inputHSize);
		return createEntity(e);
	}
};