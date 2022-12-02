#include <SFML/Graphics.hpp>
#include <vector>

const int GAME_WIDTH = 1920;
const int GAME_HEIGHT = 1080;

class Body
{
public:
	sf::CircleShape shape;
	sf::Vector2f vec;

	Body(sf::Vector2f initPos, int initMass)
	{
		shape.setPosition(initPos);
		shape.setRadius(initMass);
		shape.setFillColor(sf::Color::Black);
	}

	void update(std::vector<Body> others)
	{
		for (int i = 0; i < others.size(); i++) {
			float otherX = others.at(i).shape.getPosition().x;
			float otherY = others.at(i).shape.getPosition().y;
			float thisX = shape.getPosition().x;
			float thisY = shape.getPosition().y;
			if (otherX != thisX && otherY != thisY) {
				float distanceX = otherX - thisX;
				float distanceY = otherY - thisY;
				float distance = sqrt(distanceX * distanceX + distanceY * distanceY);
				float directionX = distanceX / distance;
				float directionY = distanceY / distance;
				float force = 0.001 * shape.getRadius() * others.at(i).shape.getRadius() / distance * distance;
				directionX *= force;
				directionY *= force;
				vec.x += directionX;
				vec.y += directionY;
			}
		}
		shape.move(vec);
	}
};

int random(int max)
{
	return std::rand() % max;
}

int main()
{
	std::srand(static_cast<unsigned int>(std::time(NULL)));

	sf::RenderWindow window(sf::VideoMode(GAME_WIDTH, GAME_HEIGHT, 32), "SFML",
		sf::Style::Titlebar | sf::Style::Close | sf::Style::Fullscreen);
	window.setVerticalSyncEnabled(true);

	std::vector<Body> bodies;
	for (int i = 0; i < 10; i++) {
		for (int j = 0; j < 10; j++) {
			Body b(sf::Vector2f(600 + i * 70, 250 + j * 70), 3);
			bodies.push_back(b);
		}
	}

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				window.close();
				break;
			}

			if (event.type == sf::Event::MouseButtonPressed) {
				if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Right)) {
					Body b(sf::Vector2f(sf::Mouse::getPosition().x, sf::Mouse::getPosition().y), 50);
					bodies.push_back(b);
				} else {
					Body b(sf::Vector2f(sf::Mouse::getPosition().x, sf::Mouse::getPosition().y), 10);
					bodies.push_back(b);
				}
			}
		}

		window.clear(sf::Color::White);

		for (int i = 0; i < bodies.size(); i++) {
			bodies.at(i).update(bodies);
			window.draw(bodies.at(i).shape);
		}

		window.display();
	}

	return EXIT_SUCCESS;
}