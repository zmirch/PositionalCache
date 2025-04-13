#include "World.h"
#include <iostream>
#include <cstdlib> // For rand()
#include <ctime>   // For time()
#include <algorithm> // For std::min
#include <optional>

#include <raylib.h>
#include <variant>

#include "../Framework/Bounds.h"
#include "Button.h"

namespace FrameworkUser
{

int WIDTH = 1280, HEIGHT = 800, CIRCLERADIUS = 4;

World world(Point2D(WIDTH, HEIGHT));

std::vector<EntityHandle<WorldEntity>> selectedEntities;
Point2D selectionPointA;
Point2D selectionPointB;
Rectangle selectionRectangle;

Button blueButton = Button(Vector2{ static_cast<float>(WIDTH) - 180, static_cast<float>(HEIGHT) - 60 }, 20, 20, BLUE);
Button greenButton = Button(Vector2{ static_cast<float>(WIDTH) - 150, static_cast<float>(HEIGHT) - 60 }, 20, 20, GREEN);
Button redButton = Button(Vector2{ static_cast<float>(WIDTH) - 120, static_cast<float>(HEIGHT) - 60 }, 20, 20, RED);
Button add10EntitiesButton = Button(Vector2{ static_cast<float>(40), static_cast<float>(70) }, 20, 20, LIGHTGRAY);
Button add100EntitiesButton = Button(Vector2{ static_cast<float>(80), static_cast<float>(70) }, 20, 20, LIGHTGRAY);
Button add1000EntitiesButton = Button(Vector2{ static_cast<float>(120), static_cast<float>(70) }, 20, 20, LIGHTGRAY);
Button add10KEntitiesButton = Button(Vector2{ static_cast<float>(160), static_cast<float>(70) }, 20, 20, LIGHTGRAY);
Button worldVectorButton = Button(Vector2{ static_cast<float>(WIDTH) - 180, static_cast<float>(30) }, 20, 20, LIGHTGRAY);
Button worldBasicButton = Button(Vector2{ static_cast<float>(WIDTH) - 120, static_cast<float>(30) }, 20, 20, LIGHTGRAY);
const char* currentAlgorithmText = nullptr;

void ClearEntities()
{
	selectedEntities.clear();
	world.clear();
}

void ColorSelection(EntityColor color)
{
	for (auto& entity : selectedEntities)
	{
		if (entity->hasEntity())
		{
			entity->getEntity().setColor(color);
		}
	}
}

void squareSelection(PositionalCache::Bounds boundingBox) {
	selectedEntities.clear();
	world.selectArea(boundingBox, [&](EntityView<WorldEntity>& safeView)
	{
		selectedEntities.push_back(safeView.getHandle());
	});
}

void Update()
{
	if (world.isTesting)
		return;

	if (IsKeyPressed(KEY_C))
	{
		ClearEntities();
		selectedEntities.clear();
	}

	if (IsMouseButtonPressed(0))
	{
		Vector2 mousePosition = GetMousePosition();

		selectionPointA.setX(mousePosition.x);
		selectionPointA.setY(mousePosition.y);
		selectionRectangle.x = mousePosition.x;
		selectionRectangle.y = mousePosition.y;
		selectionRectangle.width = 0;
		selectionRectangle.height = 0;
	}

	if (IsMouseButtonReleased(0))
	{
		Vector2 mousePosition = GetMousePosition();

		if (blueButton.isPressed(mousePosition, true))
		{
			ColorSelection(EntityColor::Blue);
		}
		else if (greenButton.isPressed(mousePosition, true))
		{
			ColorSelection(EntityColor::Green);
		}
		else if (redButton.isPressed(mousePosition, true))
		{
			ColorSelection(EntityColor::Red);
		}
		else if (add10EntitiesButton.isPressed(mousePosition, true))
		{
			world.addNEntities(10);
		}
		else if (add100EntitiesButton.isPressed(mousePosition, true))
		{
			world.addNEntities(100);
		}
		else if (add1000EntitiesButton.isPressed(mousePosition, true))
		{
			world.addNEntities(1000);
		}
		else if (add10KEntitiesButton.isPressed(mousePosition, true))
		{
			world.addNEntities(10000);
		}
		else if (worldVectorButton.isPressed(mousePosition, true))
		{
			if (world.getCurrentCacheType() != CacheType::Vector)
			{
				ClearEntities();
				world.setCacheType(CacheType::Vector);
				currentAlgorithmText = "Vector";
			}
		}
		else if (worldBasicButton.isPressed(mousePosition, true))
		{
			if (world.getCurrentCacheType() != CacheType::Basic)
			{
				ClearEntities();
				world.setCacheType(CacheType::Basic);
				currentAlgorithmText = "Map";
			}
		}
		else
		{
			// Update selection end point
			selectionPointB.setX(mousePosition.x);
			selectionPointB.setY(mousePosition.y);

			// Determine top-left corner of drawn Rectangle
			selectionRectangle.x = std::min(selectionPointA.getX(), selectionPointB.getX());
			selectionRectangle.y = std::min(selectionPointA.getY(), selectionPointB.getY());

			// Determine width and height of drawn Rectangle
			selectionRectangle.width = abs(selectionPointA.getX() - selectionPointB.getX());
			selectionRectangle.height = abs(selectionPointA.getY() - selectionPointB.getY());
			squareSelection(PositionalCache::Bounds(selectionPointA, selectionPointB));
		}
	}
}

void DrawEntity(const EntityView<WorldEntity>& safeView)
{
	Point2D entityPosition = safeView.getEntity().getPosition();
	EntityColor entityColor = safeView.getEntity().getEntity().getColor(); // TODO second getEntity rename
	Color drawColor = WHITE;
	switch (entityColor) {
	case EntityColor::Red:
		drawColor = RED;
		break;
	case EntityColor::Blue:
		drawColor = BLUE;
		break;
	case EntityColor::Green:
		drawColor = GREEN;
		break;
	}
	DrawCircle(entityPosition.getX(), entityPosition.getY(), CIRCLERADIUS, drawColor);
}

void Draw()
{
	BeginDrawing();
	ClearBackground(BLACK);

	for (auto& entity : selectedEntities)
	{
		if(entity->hasEntity())
		{
			DrawCircle(entity->getPosition().getX(), entity->getPosition().getY(), CIRCLERADIUS + 2, WHITE);
		}
	}

	world.getAllEntities([&](EntityView<WorldEntity>& safeView)
	{
		DrawEntity(safeView);
	});

	DrawRectangleLinesEx(selectionRectangle, 1, LIGHTGRAY);

	DrawRectangle(WIDTH - 200, HEIGHT - 100, 180, 70, GRAY);
	DrawText("Color", WIDTH - 190, HEIGHT - 100, 30, BLACK);
	DrawText("Press [C] to clear", 170, 30, 20, DARKGRAY);
	DrawText("Add Entities", 30, 30, 20, WHITE);
	DrawText("10", 40, 93, 17, WHITE);
	DrawText("100", 80, 93, 17, WHITE);
	DrawText("1000", 120, 93, 17, WHITE);
	DrawText("10k", 160, 93, 17, WHITE);
	DrawText("Vector", static_cast<float>(WIDTH) - 200, 53, 17, WHITE);
	DrawText("Map", static_cast<float>(WIDTH) - 120, 53, 17, WHITE);
	DrawText("Cache Algorithms", static_cast<float>(WIDTH) - 200, 5, 17, WHITE);
	DrawText(TextFormat("Current: %s", currentAlgorithmText), WIDTH - 200, 70, 17, YELLOW);

	blueButton.Draw();
	greenButton.Draw();
	redButton.Draw();
	add10EntitiesButton.Draw();
	add100EntitiesButton.Draw();
	add1000EntitiesButton.Draw();
	add10KEntitiesButton.Draw();
	worldVectorButton.Draw();
	worldBasicButton.Draw();

	EndDrawing();
}
}

using namespace FrameworkUser;

int main()
{
	InitWindow(WIDTH, HEIGHT, "Positional Cache Demo");
	SetTargetFPS(60);

	std::srand(static_cast<unsigned>(std::time(0)));
	currentAlgorithmText = (world.getCurrentCacheType() == FrameworkUser::CacheType::Vector) ? "Vector" : "Map";

	world.startRandomMovements();

	while (!WindowShouldClose())
	{
		Update();
		Draw();
	}

	CloseWindow();

	return 0;
}