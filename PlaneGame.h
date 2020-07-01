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

		//Player:
		int hp;
		double fuel;
		float maxFuel;
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

		//Sand:
		int maxRivs;
		RectangleShape river;
		std::vector<RectangleShape> rivs;

		//Setting window properties:
		void setGame()
		{
			this->isEnd = false;
			this->hp = 100;
			this->maxFuel = 100.f;
			this->fuel = this->maxFuel;
			this->maxBarrelsDelay = 500.f;
			this->barrelsDelay = this->maxBarrelsDelay;
			this->maxBarrels = 3;
			this->maxShipsDelay = 50.f;
			this->shipsDelay = this->maxShipsDelay;
			this->maxShips = 12;
			this->maxRivs = 2;
		}

		void loadTextures()
		{
			this->planeTex.loadFromFile("plane.png");
			this->shipTex.loadFromFile("ship.png");
			this->barrelTex.loadFromFile("barrel.png");
		}

		void renderWindow()
		{
			this->window = nullptr;
			this->videoMode.height = 600;
			this->videoMode.width = 800;
			this->window = new RenderWindow(this->videoMode, "PlaneGame", Style::Default);
			this->window->setFramerateLimit(60);
		}

		//Setting player:
		void setPlayer()
		{
			this->player.setTexture(this->planeTex);
			this->player.setPosition(this->window->getSize().x / 2, 0.f);
		}

		//Setting barrel:
		void setBarrel()
		{
			this->barrel.setTexture(this->barrelTex);
			this->barrel.setScale(0.5f, 0.5f);
			this->barrel.setPosition(20.f, 20.f);
		}

		//Setting bars:
		void setBars()
		{
			this->hpBar.setSize(Vector2f(this->hp, 30.f));
			this->fuelBar.setSize(Vector2f(this->hp, 30.f));

			this->hpBar.setFillColor(Color::Red);
			this->fuelBar.setFillColor(Color::Green);

			this->hpBar.setPosition(10.f, 10.f);
			this->fuelBar.setPosition(120.f, 10.f);
		}

		void setRiver()
		{
			this->river.setFillColor(Color::Blue);
			this->river.setSize(Vector2f(this->window->getSize().x - 300.f, this->window->getSize().y));
			this->river.setPosition(170.f, 0.f);
		}		  

		//Setting ship:
		void setShip()
		{
			this->ship.setTexture(this->shipTex);
			this->ship.setScale(0.5f, 0.5f);
			this->ship.setPosition(2.f, 20.f);
		}

	public:
		//Setting game:
		PlaneGame()
		{
			this->setGame();
			this->loadTextures();
			this->renderWindow();
			this->setPlayer();
			this->setShip();
			this->setBarrel();
			this->setBars();
			this->setRiver();
			this->rivs.push_back(this->river);
		}

		//Checking game condition
		const bool ifWorks() const
		{
			return (this->window->isOpen() && !this->isEnd);
		}

		//Managing events:
		void events()
		{
			while (this->window->pollEvent(this->evt))
			{
				if (this->evt.type == Event::Closed)
					this->window->close();
				if (Keyboard::isKeyPressed(Keyboard::Escape))
					this->window->close();
			}
		}

		//Plane controls:
		void planeMovement()
		{
			if (Keyboard::isKeyPressed(Keyboard::A) && this->player.getPosition().x > 0)
				this->player.move(-5.f, 0.f);
			if (Keyboard::isKeyPressed(Keyboard::D) && this->player.getPosition().x < this->window->getSize().x)
				this->player.move(5.f, 0.f);
			if (Keyboard::isKeyPressed(Keyboard::W) &&  this->player.getPosition().y > 0)
				this->player.move(0.f, -5.f);
			if (Keyboard::isKeyPressed(Keyboard::S) && this->player.getPosition().y < this->window->getSize().y)
				this->player.move(0.f, 5.f);
		}

		//Barrels spawn:
		void spawnBarrels()
		{
			this->barrel.setTexture(this->barrelTex);
			this->barrels.push_back(this->barrel);
		}

		void addRiver()
		{
			this->river.setPosition(rand()%100 + 170.f, this->window->getSize().y);
			this->rivs.push_back(this->river);
		}

		//Ships spawn:
		void spawnShips()
		{
			this->ships.push_back(this->ship);
		}

		//Collisions:
		void updateMovables()
		{
			if (this->ships.size() < this->maxShips)
			{
				if (this->shipsDelay >= this->maxShipsDelay)
				{
					this->spawnShips();
					this->shipsDelay = 0.f;
				}
				this->shipsDelay++;
			}

			if (this->barrels.size() < this->maxBarrels)
			{
				if (this->barrelsDelay >= this->maxBarrelsDelay)
				{
					this->spawnBarrels();
					this->barrelsDelay = 0.f;
				}
				this->barrelsDelay++;
			}

			if (this->rivs.size() < this->maxRivs)
			{
				this->addRiver();
			}

			for (size_t i = 0; i < this->ships.size(); i++)
			{
				this->ships[i].move(0.f, -2.f);
				if (this->ships[i].getPosition().y < 0)
					this->ships.erase(this->ships.begin() + i);
				else if (this->ships[i].getGlobalBounds().intersects(this->player.getGlobalBounds()))
				{
					this->hp -= 10;
					this->ships.erase(this->ships.begin() + i);
				}
			}

			for (size_t i = 0; i < this->barrels.size(); i++)
			{
				this->barrels[i].move(0.f, -2.f);
				if (this->barrels[i].getPosition().y < 0)
					this->barrels.erase(this->barrels.begin() + i);
				else if (this->barrels[i].getGlobalBounds().intersects(this->player.getGlobalBounds()))
				{
					this->barrels.erase(this->barrels.begin() + i);
					if (this->fuel + 10 <= this->maxFuel)
						this->fuel += 20;
					else
						this->fuel = this->maxFuel;
				}
			}
			
			for (size_t i = 0; i < this->rivs.size(); i++)
			{
				this->rivs[i].move(0.f, -2.f);
				if (this->player.getPosition().x < this->rivs[i].getPosition().x ||
					this->player.getPosition().x > this->rivs[i].getPosition().x + this->rivs[i].getSize().x)
					this->hp -= 1;
				if (this->rivs[i].getPosition().y + this->rivs[i].getSize().y < 0)
					this->rivs.erase(this->rivs.begin() + i);
			}

			for (size_t i = 0; i < this->rivs.size(); i++)
			{
				this->barrel.setPosition(rand() % int (this->rivs[i].getSize().x - 100.f) + this->rivs[i].getPosition().x,
					this->window->getSize().y);
				this->ship.setPosition(rand() % int(this->rivs[i].getSize().x - 100.f) + this->rivs[i].getPosition().x,
					this->window->getSize().y);
			}
		}

		//Updating bars:
		void updateBars()
		{
			this->hpBar.setSize(Vector2f(this->hp, this->hpBar.getSize().y));
			this->fuelBar.setSize(Vector2f(this->fuel, this->fuelBar.getSize().y));
		}

		//Updating game:
		void update()
		{
			this->events();
			if (this->ifWorks())
			{
				this->fuel -= 0.025;
				this->updateMovables();
				this->planeMovement();
				this->updateBars();
			}

			if (this->hp <= 0 || this->fuel <= 0)
				this->isEnd = true;
		}

		//Rendering game:
		//Player:
		void renderPlayer(RenderTarget& window)
		{
			window.draw(this->player);
		}

		//Barrels:
		void renderBarrels(RenderTarget& window)
		{
			for (size_t i = 0; i < this->barrels.size(); i++)
				window.draw(this->barrels[i]);
		}

		//Ships:
		void renderShips(RenderTarget& window)
		{
			for (size_t i = 0; i < this->ships.size(); i++)
				window.draw(this->ships[i]);
		}

		void renderBars(RenderTarget& window)
		{
			window.draw(this->hpBar);
			window.draw(this->fuelBar);
		}

		void renderRiver(RenderTarget& window)
		{
			for (auto i : this->rivs)
				window.draw(i);
		}

		//Everything:
		void render()
		{
			this->window->clear(Color::Yellow);
			this->renderRiver(*this->window);
			this->renderBarrels(*this->window);
			this->renderShips(*this->window);
			this->renderPlayer(*this->window);
			this->renderBars(*this->window);
			this->window->display();
		}

		~PlaneGame()
		{
			delete this->window;
		}
};

