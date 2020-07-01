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
			this->maxBulletDelay = 30.f;
			this->bulletDelay = this->maxBulletDelay;
			this->maxBullets = 30;
			this->isShot = false;
			this->points = 0;
		}

		//Loading textures:
		void loadTextures()
		{
			this->planeTex.loadFromFile("plane.png");
			this->shipTex.loadFromFile("ship.png");
			this->barrelTex.loadFromFile("barrel.png");
		}

		//Setting text:
		void setText()
		{
			this->font.loadFromFile("arial.ttf");

			this->hpText.setFont(this->font);
			this->hpText.setCharacterSize(15);
			this->hpText.setString("Sample hp text");
			this->hpText.setPosition(10.f, 10.f);

			this->fuelText.setFont(this->font);
			this->fuelText.setCharacterSize(15);
			this->fuelText.setString("Sample fuel text");
			this->fuelText.setPosition(120.f, 10.f);

			this->pointsText.setFont(this->font);
			this->pointsText.setCharacterSize(15);
			this->pointsText.setString("Sample points text");
			this->pointsText.setPosition(230.f, 10.f);
		}

		//Rendering window:
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

		//Setting river
		void setRiver()
		{
			this->river.setFillColor(Color::Color(100, 149, 237));
			this->river.setSize(Vector2f(this->window->getSize().x - 300.f, this->window->getSize().y));
			this->river.setPosition(170.f, 0.f);
		}		  

		void setBullet()
		{
			this->bullet.setFillColor(Color::Black);
			this->bullet.setSize(Vector2f(4.f, 8.f));
			this->bullet.setPosition(0.f, 0.f);
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
			this->setText();
			this->setPlayer();
			this->setShip();
			this->setBarrel();
			this->setBars();
			this->setBullet();
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
			if (Keyboard::isKeyPressed(Keyboard::D) && this->player.getPosition().x + 100.f < this->window->getSize().x)
				this->player.move(5.f, 0.f);
			if (Keyboard::isKeyPressed(Keyboard::W) &&  this->player.getPosition().y > 0)
				this->player.move(0.f, -5.f);
			if (Keyboard::isKeyPressed(Keyboard::S) && this->player.getPosition().y + 24.f < this->window->getSize().y)
				this->player.move(0.f, 5.f);
		}

		//Returns player's position:
		Vector2f getPlayerPos()
		{
			return Vector2f(this->player.getPosition().x + 50.f, this->player.getPosition().y + 12.f);
		}

		//Barrels spawn:
		void spawnBarrels()
		{
			this->barrels.push_back(this->barrel);
		}

		//River spawn:
		void addRiver()
		{
			this->river.setPosition(rand()%300, this->window->getSize().y);
			this->rivs.push_back(this->river);
		}

		//Ships spawn:
		void spawnShips()
		{
			this->ships.push_back(this->ship);
		}

		void spawnBullet()
		{
			this->bullets.push_back(this->bullet);
		}

		//Collisions:
		void updateMovables()
		{
			//Spawning ships:
			if (this->ships.size() < this->maxShips)
			{
				if (this->shipsDelay >= this->maxShipsDelay)
				{
					this->spawnShips();
					this->shipsDelay = 0.f;
				}
				this->shipsDelay++;
			}

			//Spawning barrels:
			if (this->barrels.size() < this->maxBarrels)
			{
				if (this->barrelsDelay >= this->maxBarrelsDelay)
				{
					this->spawnBarrels();
					this->barrelsDelay = 0.f;
				}
				this->barrelsDelay++;
			}

			//Spawning river:
			if (this->rivs.size() < this->maxRivs)
			{
				this->addRiver();
			}

			if (this->bulletDelay < this->maxBulletDelay)
				this->bulletDelay++;

			if (this->bulletDelay >= this->maxBulletDelay && Keyboard::isKeyPressed(Keyboard::Space))
			{
				this->bullet.setPosition(this->getPlayerPos());
				this->bulletDelay = 0.f;
				this->spawnBullet();
				this->isShot = false;
			}

			//Checking collisions:
			//Player - ships:
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

			//Player - barrels:
			for (size_t i = 0; i < this->barrels.size(); i++)
			{
				this->barrels[i].move(0.f, -2.f);
				if (this->barrels[i].getPosition().y < 0)
					this->barrels.erase(this->barrels.begin() + i);
				else if (this->barrels[i].getGlobalBounds().intersects(this->player.getGlobalBounds()))
				{
					this->barrels.erase(this->barrels.begin() + i);
					if (this->fuel + 20 <= this->maxFuel)
						this->fuel += 20;
					else
						this->fuel = this->maxFuel;
				}
			}
			
			//Player - river:
			for (size_t i = 0; i < this->rivs.size(); i++)
			{
				this->rivs[i].move(0.f, -2.f);
				if (this->rivs[i].getGlobalBounds().intersects(this->player.getGlobalBounds()))
					if (this->player.getPosition().x  + 50.f < this->rivs[i].getPosition().x ||
						this->player.getPosition().x > this->rivs[i].getPosition().x + this->rivs[i].getSize().x - 50.f)
						this->hp -= 1;

				if (this->rivs[i].getPosition().y + this->rivs[i].getSize().y < 0)
					this->rivs.erase(this->rivs.begin() + i);
			}

			//Respawning movables in the river area:
			for (size_t i = 0; i < this->rivs.size(); i++)
			{
				this->barrel.setPosition(rand() % int (this->rivs[i].getSize().x - 100.f) + this->rivs[i].getPosition().x,
					this->window->getSize().y);

				this->ship.setPosition(rand() % int(this->rivs[i].getSize().x - 100.f) + this->rivs[i].getPosition().x,
					this->window->getSize().y);
			}

			for (size_t i = 0; i < this->bullets.size(); i++)
			{
				this->bullets[i].move(0.f, 5.f);

				if (this->bullets[i].getPosition().y > this->window->getSize().y)
					this->bullets.erase(this->bullets.begin() + i);
				
				if(!this->bullets.empty() && !this->ships.empty())
					for (size_t j = 0; j < this->ships.size(); j++)
						if (this->bullets[i].getGlobalBounds().intersects(this->ships[j].getGlobalBounds()))
						{
							this->points++;
							this->bullets.erase(this->bullets.begin() + i);
							this->ships.erase(this->ships.begin() + j);
							break;
						}
			}
		}

		//Updating bars:
		void updateBars()
		{
			this->hpBar.setSize(Vector2f(this->hp, this->hpBar.getSize().y));
			this->fuelBar.setSize(Vector2f(this->fuel, this->fuelBar.getSize().y));
		}

		void updateText()
		{
			this->hpText.setString("HP: " + std::to_string(this->hp));
			this->fuelText.setString("Fuel: " + std::to_string(int(std::ceil(this->fuel))));
			this->pointsText.setString("Points: " + std::to_string(this->points));
		}

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

		//Bars:
		void renderBars(RenderTarget& window)
		{
			window.draw(this->hpBar);
			window.draw(this->fuelBar);
		}

		//River:
		void renderRiver(RenderTarget& window)
		{
			for (auto i : this->rivs)
				window.draw(i);
		}

		//Bullets:
		void renderBullets(RenderTarget& window)
		{
			for (auto i : this->bullets)
				window.draw(i);
		}

		void renderText(RenderTarget& window)
		{
			window.draw(this->hpText);
			window.draw(this->fuelText);
			window.draw(this->pointsText);
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

		~PlaneGame()
		{
			delete this->window;
		}
};

