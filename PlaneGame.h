#pragma once

#include "SFML/Audio.hpp"
#include "SFML/Window.hpp"
#include "SFML/Graphics.hpp"
#include "SFML/System.hpp"
#include <iostream>
#include <vector>

using namespace sf;

class PlaneGame
{
	private:
		//Window properties
		RenderWindow* window;
		VideoMode videoMode;
		Event evt;
		bool isEnd;

		//Game properties:
		//Texturs:
		Texture planeTex;
		Texture shipTex;
		Texture barrelTex;

		//Text:
		Text hpText;
		Text fuelText;
		Text pointsText;
		Font font;

		//Player:
		int hp;
		double fuel;
		float maxFuel;
		bool isShot;
		int points;
		Sprite player;

		//Oil barrels:
		float barrelsDelay;
		float maxBarrelsDelay;
		int maxBarrels;
		Sprite barrel;
		std::vector<Sprite> barrels;

		//Ships:
		float shipsDelay;
		float maxShipsDelay;
		int maxShips;
		Sprite ship;
		std::vector<Sprite> ships;

		//Fuel and Hp bars:
		RectangleShape hpBar;
		RectangleShape fuelBar;

		//River:
		int maxRivs;
		RectangleShape river;
		std::vector<RectangleShape> rivs;

		//Bullets:
		float bulletDelay;
		float maxBulletDelay;
		int maxBullets;
		RectangleShape bullet;
		std::vector<RectangleShape> bullets;

		//Setting window properties:
		void setGame();

		//Loading textures:
		void loadTextures()
		{
			this->planeTex.loadFromFile("plane.png");
			this->planeTex.setSmooth(true);
			this->shipTex.loadFromFile("ship.png");
			this->barrelTex.loadFromFile("barrel.png");
		}

		//Setting text:
		void setText();

		//Rendering window:
		void renderWindow();

		//Setting player:
		void setPlayer();

		//Setting barrel:
		void setBarrel();

		//Setting bars:
		void setBars();

		//Setting river
		void setRiver();	  
		
		//Setting bullet:
		void setBullet();

		//Setting ship:
		void setShip();

		//Managing events:
		void events();

		//Plane controls:
		void planeMovement();

		//Returns player's position:
		Vector2f getPlayerPos() { return Vector2f(this->player.getPosition().x + 50.f, this->player.getPosition().y + 12.f); }

		//Barrels spawn:
		void spawnBarrels() { this->barrels.push_back(this->barrel); }

		//River spawn:
		void addRiver() { this->river.setPosition(rand() % 300, this->window->getSize().y); this->rivs.push_back(this->river); }

		//Ships spawn:
		void spawnShips() { this->ships.push_back(this->ship); }

		//Bullet spawn:
		void spawnBullet() { this->bullets.push_back(this->bullet); }

		//Checks if plane is above the river
		bool isOnRiver();

		//Update ======================================================================================================================
		//Collisions:
		void updateMovables();

		//Updating bars:
		void updateBars();

		//Updating text:
		void updateText();

		//Render ======================================================================================================================
		//Player:
		void renderPlayer(RenderTarget& window) { window.draw(this->player); }

		//Barrels:
		void renderBarrels(RenderTarget& window) { for (auto i : this->barrels) window.draw(i); }

		//Ships:
		void renderShips(RenderTarget& window) { for (auto i : this->ships) window.draw(i); }

		//Bars:
		void renderBars(RenderTarget& window) { window.draw(this->hpBar); window.draw(this->fuelBar); }

		//River:
		void renderRiver(RenderTarget& window) { for (auto i : this->rivs) window.draw(i); }

		//Bullets:
		void renderBullets(RenderTarget& window) { for (auto i : this->bullets) window.draw(i); }

		//Rendering text:
		void renderText(RenderTarget& window) { window.draw(this->hpText); window.draw(this->fuelText); window.draw(this->pointsText); }

	public:
		//Setting game:
		PlaneGame();

		//Checking game condition
		const bool ifWorks() const { return (this->window->isOpen() && !this->isEnd); }

		//Updating game:
		void update()
		{
			this->events();
			if (this->ifWorks())
			{
				this->planeMovement();
				this->fuel -= 0.025;
				this->updateMovables();
				this->updateText();
				this->updateBars();
			}

			if (this->hp <= 0 || this->fuel <= 0)
				this->isEnd = true;
		}

		//Everything:
		void render()
		{
			this->window->clear(Color::Color(34, 139, 34));
			this->renderRiver(*this->window);
			this->renderBarrels(*this->window);
			this->renderShips(*this->window);
			this->renderPlayer(*this->window);
			this->renderBars(*this->window);
			this->renderBullets(*this->window);
			this->renderText(*this->window);
			this->window->display();
		}

		//Deleting game window
		~PlaneGame()
		{
			delete this->window;
		}
};

