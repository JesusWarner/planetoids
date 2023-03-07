#include <iostream>
#include <math.h>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

using namespace std;

//Simbolos o valores
int maxEnemigos = 10;
float pi = 3.14159;

//Cosas de la pantalla
int ancho = 600;
int largo = 500;

//Ajustes jugador
int r = 30;
float fuerza = 0;

class jugador {
public:

	sf::Text naveEspacial;
	sf::Font font;
	float angulo = 0;
	bool vivo = true;
	sf::Clock c1;
	sf::Clock c2;

	jugador()
	{
		if (!font.loadFromFile("\AGENCYB.ttf"))
		{
			cout << "No sirve";
		}
		naveEspacial.setFont(font);
		naveEspacial.setLetterSpacing(0);
		naveEspacial.setLineSpacing(0);
		naveEspacial.setOrigin(r/4, r/2);
		naveEspacial.setPosition(sf::Vector2f(ancho / 2, largo / 2));
		naveEspacial.setCharacterSize(r);
		naveEspacial.setString("A");

	}

	void MecanicasJugador()
	{
		angulo = (naveEspacial.getRotation() - 90) * pi / 180;

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
		{
			naveEspacial.rotate(2);
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
		{
			naveEspacial.rotate(-2);
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
		{
			if (c1.getElapsedTime().asMilliseconds() > 0.1 && fuerza <= 2)
			{
				fuerza += 0.1;
			}

			float posX = fuerza * cos(angulo);
			float posY = fuerza * sin(angulo);
			naveEspacial.move(sf::Vector2f(posX, posY));
			c1.restart();

		}

		if (!sf::Keyboard::isKeyPressed(sf::Keyboard::W) && c2.getElapsedTime().asMilliseconds() > 0.1 && fuerza >= 0)
		{
			fuerza -= 0.01;
			float posX = fuerza * cos(angulo);
			float posY = fuerza * sin(angulo);
			naveEspacial.move(sf::Vector2f(posX, posY));
			c2.restart();
		}

	}

	void bordesTeleport()
	{
		if (naveEspacial.getPosition().x < 0)
		{
			naveEspacial.move(ancho, 0);
		}

		if (naveEspacial.getPosition().x > ancho)
		{
			naveEspacial.move(-ancho, 0);

		}

		if (naveEspacial.getPosition().y < 0)
		{
			naveEspacial.move(0, largo);
		}

		if (naveEspacial.getPosition().y > largo)
		{
			naveEspacial.move(0, -largo);
		}
	}

	void draw(sf::RenderWindow &wind)
	{
		if (vivo == true)
		{
			MecanicasJugador();
			bordesTeleport();
		}
		wind.draw(naveEspacial);
	}
};

class balas {
public:

	sf::CircleShape shape;
	float anguloDisparo = 0;
	sf::Clock vida;

	balas(float rad = 5.f) 
	{
		shape.setRadius(rad);
		shape.setOrigin(rad / 2, rad / 2);
		shape.setFillColor(sf::Color::White);
	}
};

class planetas {
public:

	sf::CircleShape pShape;
	float anguloP = 0;
	int vida = 1;
	float fuerzaP = 0;

	planetas()
	{
		pShape.setOutlineThickness(-2);
		pShape.setFillColor(sf::Color::Black);
	}

};


int main()
{
	int yaPerdiste = 0;

	//Jugador
	jugador NaveEspacial;

	//Sonido
	sf::SoundBuffer buffer;
	if (!buffer.loadFromFile("\disparoSound.wav"))
	{
		cout << "No se reprodujo el sonudo" << endl;
	}
	sf::SoundBuffer buffer1;
	if (!buffer1.loadFromFile("\perdisteSound.wav"))
	{
		cout << "No se reprodujo el sonido 2" << endl;
	}
	sf::Sound disparoSonido;
	disparoSonido.setBuffer(buffer);
	sf::Sound perdisteSonido;
	perdisteSonido.setBuffer(buffer1);

	//Puntaje
	sf::Text t;
	sf::Font f;
	if (!f.loadFromFile("\AGENCYB.ttf"))
	{
		cout << "No sirve";
	}
	t.setFont(f);
	t.setCharacterSize(20);
	t.setFillColor(sf::Color::White);
	t.setPosition(sf::Vector2f(ancho - 40, 20));
	int puntos = 0;

	//Tiempo de disparo
	sf::Clock disparoTiempo;

	//Tiempo para enemigos
	sf::Clock esperarEnemigo;

	//Balas
	balas b;
	vector<balas> armas;

	//Planetas o Enemigos
	planetas p;
	vector<planetas> enemigos;

	sf::ContextSettings settings;
	settings.antialiasingLevel = 8;
	sf::RenderWindow window(sf::VideoMode(ancho, largo), "Planetoids", sf::Style::Default, settings);
	window.setFramerateLimit(60);

	while (window.isOpen())
	{

		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed) window.close();
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) window.close();
		}

		//Enemigos

		if (enemigos.size() < maxEnemigos && esperarEnemigo.getElapsedTime().asSeconds() > 1)
		{
			int randomNumber = rand() % (largo + 40) + (-20);
			int randomAngulo = rand() % 270 + 180;
			float anguloEnemigo = randomAngulo * pi / 360;
			int randomFuerza = rand() % 2 + 1;
			int randomRadio = rand() % 40 + 10;
			int randomColor = rand() % 5 + 0;

			switch (randomColor)
			{
			case 0:
				if (randomColor == 0) p.pShape.setOutlineColor(sf::Color::White);
			case 1:
				if (randomColor == 1) p.pShape.setOutlineColor(sf::Color::Red);
			case 2:
				if (randomColor == 2) p.pShape.setOutlineColor(sf::Color::Blue);
			case 3:
				if (randomColor == 3) p.pShape.setOutlineColor(sf::Color::Cyan);
			case 4:
				if (randomColor == 4) p.pShape.setOutlineColor(sf::Color::Yellow);
				break;
			}
			

			p.pShape.setPosition(sf::Vector2f(ancho + 30, randomNumber));
			p.pShape.setRadius(randomRadio);
			p.pShape.setOrigin(randomRadio, randomRadio);
			p.anguloP = anguloEnemigo;
			p.fuerzaP = randomFuerza;
			p.vida = randomRadio;
			enemigos.push_back(p);
			esperarEnemigo.restart();

		}

		//cout << enemigos.size() << endl;

		for (int i = 0; i < enemigos.size(); i++)
		{
			if (NaveEspacial.vivo == true)
			{
				float posX = enemigos[i].fuerzaP * cos(enemigos[i].anguloP);
				float posY = enemigos[i].fuerzaP * sin(enemigos[i].anguloP);
				enemigos[i].pShape.move(posX, posY);
			}

			//eliminar enemigos sin estan afuera de zona o los toco una bala

			if (enemigos[i].pShape.getPosition().x < -50 || enemigos[i].pShape.getPosition().x >(ancho + 30)
				|| enemigos[i].pShape.getPosition().y < -40 || enemigos[i].pShape.getPosition().y >(largo + 30))
			{
				enemigos.erase(enemigos.begin() + i);
			}
		}

		//Armas

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && disparoTiempo.getElapsedTime().asSeconds() > 0.3 && NaveEspacial.vivo == true)
		{
			disparoSonido.play();
			b.shape.setPosition(NaveEspacial.naveEspacial.getPosition());
			b.anguloDisparo = NaveEspacial.angulo;
			b.vida.restart();
			armas.push_back(b);
			disparoTiempo.restart();
		}

		for (int i = 0; i < armas.size(); i++)
		{
			float x = 3 * cos(armas[i].anguloDisparo);
			float y = 3 * sin(armas[i].anguloDisparo);

			armas[i].shape.move(x, y);

			//eliminar balas

			if (armas[i].vida.getElapsedTime().asSeconds() > 6)
			{
				armas.erase(armas.begin() + i);
			}

			else {

				for (size_t k = 0; k < enemigos.size(); k++)
				{
					if (armas[i].shape.getGlobalBounds().intersects(enemigos[k].pShape.getGlobalBounds()))
					{
						armas.erase(armas.begin() + i);

						enemigos[k].vida -= 8;

						if (enemigos[k].vida <= 0)
						{
							puntos += 1;
							cout << puntos << endl;
							enemigos.erase(enemigos.begin() + k);
						}

						break;
					}
				}
			}
		}

		//Muerte personaje

		for (int i = 0; i < enemigos.size(); i++)
		{
			sf::Vector2f distanciaxy = NaveEspacial.naveEspacial.getPosition() - enemigos[i].pShape.getPosition();
			float distancia = sqrt(distanciaxy.x * distanciaxy.x + distanciaxy.y * distanciaxy.y);

			if (distancia < enemigos[i].pShape.getRadius())
			{
				NaveEspacial.naveEspacial.setFillColor(sf::Color::Red);
				NaveEspacial.vivo = false;
			}
		}

		//Dibujar en la pantalla
		string points = to_string(puntos);
		t.setString(points);

		if (NaveEspacial.vivo == false && yaPerdiste == 0)
		{
			perdisteSonido.play();
			yaPerdiste = 1;
		}

		window.clear();

		for (int i = 0; i < armas.size(); i++)
		{
			window.draw(armas[i].shape);
		}

		for (int i = 0; i < enemigos.size(); i++)
		{
			window.draw(enemigos[i].pShape);
		}

		NaveEspacial.draw(window);
		window.draw(t);
		window.display();

	}

	return 0;
}