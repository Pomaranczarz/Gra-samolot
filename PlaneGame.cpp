#include "PlaneGame.h"

void PlaneGame::setGame()
{
	this->isEnd = false;
	this->hp = 100;
	this->maxFuel = 100.f;
	this->fuel = this->maxFuel;
	this->maxBarrelsDelay = 750.f;
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

void PlaneGame::setText()
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

void PlaneGame::renderWindow()
{
	this->window = nullptr;
	this->videoMode.height = 600;
	this->videoMode.width = 800;
	this->window = new RenderWindow(this->videoMode, "PlaneGame", Style::Default);
	this->window->setFramerateLimit(60);
}

void PlaneGame::setPlayer()
{
	this->player.setTexture(this->planeTex);
	this->player.setPosition(this->window->getSize().x / 2, 0.f);
}

void PlaneGame::setBarrel()
{
	this->barrel.setTexture(this->barrelTex);
	this->barrel.setScale(0.5f, 0.5f);
	this->barrel.setPosition(20.f, 20.f);
}

void PlaneGame::setBars()
{
	this->hpBar.setSize(Vector2f(this->hp, 30.f));
	this->fuelBar.setSize(Vector2f(this->hp, 30.f));

	this->hpBar.setFillColor(Color::Red);
	this->fuelBar.setFillColor(Color::Green);

	this->hpBar.setPosition(10.f, 10.f);
	this->fuelBar.setPosition(120.f, 10.f);
}

void PlaneGame::setRiver()
{
	this->river.setFillColor(Color::Color(100, 149, 237));
	this->river.setSize(Vector2f(this->window->getSize().x - 300.f, this->window->getSize().y));
	this->river.setPosition(170.f, 0.f);
}

void PlaneGame::setBullet()
{
	this->bullet.setFillColor(Color::Black);
	this->bullet.setSize(Vector2f(4.f, 8.f));
	this->bullet.setPosition(0.f, 0.f);
}

void PlaneGame::setShip()
{
	this->ship.setTexture(this->shipTex);
	this->ship.setScale(0.5f, 0.5f);
	this->ship.setPosition(2.f, 20.f);
}

PlaneGame::PlaneGame()
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

void PlaneGame::events()
{
	while (this->window->pollEvent(this->evt))
	{
		if (this->evt.type == Event::Closed)
			this->window->close();
		if (Keyboard::isKeyPressed(Keyboard::Escape))
			this->window->close();
	}
}

void PlaneGame::planeMovement()
{
	if (Keyboard::isKeyPressed(Keyboard::A) && this->player.getPosition().x > 0)
		this->player.move(-5.f, 0.f);
	if (Keyboard::isKeyPressed(Keyboard::D) && this->player.getPosition().x + 100.f < this->window->getSize().x)
		this->player.move(5.f, 0.f);
	if (Keyboard::isKeyPressed(Keyboard::W) && this->player.getPosition().y > 0)
		this->player.move(0.f, -5.f);
	if (Keyboard::isKeyPressed(Keyboard::S) && this->player.getPosition().y + 24.f < this->window->getSize().y)
		this->player.move(0.f, 5.f);
}

bool PlaneGame::isOnRiver()
{
	for (size_t i = 0; i < this->rivs.size(); i++)
	{
		if (this->rivs[i].getGlobalBounds().intersects(this->player.getGlobalBounds()))
			return true;
	}
	return false;
}

void PlaneGame::updateMovables()
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
		{
			if (this->player.getPosition().x + this->player.getGlobalBounds().width / 2 < this->rivs[i].getPosition().x ||
				this->player.getPosition().x + this->player.getGlobalBounds().width / 2 > this->rivs[i].getPosition().x + this->rivs[i].getSize().x)
				this->hp -= 1;
		}

		if (this->rivs[i].getPosition().y + this->rivs[i].getSize().y < 0)
			this->rivs.erase(this->rivs.begin() + i);
	}

	if (!this->isOnRiver())
		this->hp -= 1;

	//Respawning movables in the river area:
	for (size_t i = 0; i < this->rivs.size(); i++)
	{
		this->barrel.setPosition(rand() % int(this->rivs[i].getSize().x - 100.f) + this->rivs[i].getPosition().x,
			this->window->getSize().y + this->barrel.getGlobalBounds().height);

		this->ship.setPosition(rand() % int(this->rivs[i].getSize().x - this->ship.getGlobalBounds().width) + this->rivs[i].getPosition().x,
			this->window->getSize().y - this->ship.getGlobalBounds().height);
	}

	//Checking bullets' collisions:
	for (size_t i = 0; i < this->bullets.size(); i++)
	{
		this->bullets[i].move(0.f, 5.f);

		if (this->bullets[i].getPosition().y > this->window->getSize().y)
			this->bullets.erase(this->bullets.begin() + i);

		if (!this->bullets.empty() && !this->ships.empty())
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

void PlaneGame::updateBars()
{
	this->hpBar.setSize(Vector2f(this->hp, this->hpBar.getSize().y));
	this->fuelBar.setSize(Vector2f(this->fuel, this->fuelBar.getSize().y));
}

void PlaneGame::updateText()
{
	this->hpText.setString("HP: " + std::to_string(this->hp));
	this->fuelText.setString("Fuel: " + std::to_string(int(std::ceil(this->fuel))));
	this->pointsText.setString("Points: " + std::to_string(this->points));
}

