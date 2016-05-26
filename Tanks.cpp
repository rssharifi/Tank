#include <iostream>
#include <cmath>
#include <SFML/Graphics.hpp>
#include <ctime>

using namespace std;


class Vector
{
public:
	float x;
	float y;

	Vector()
	{
		x = 0;
		y = 0;
	}
};


class Tank
{
private:
	Vector *position;
	int direction;
	float radius;

public:
	Tank()
	{
		position = new Vector();
		direction = 0;
		radius = 5.0f;
	}

	void setPosition(float x, float y)
	{
		position->x = x;
		position->y = y;
	}

	Vector *getPosition()
	{
		return position;
	}

	void setDirection(int dir)
	{
		direction = dir;
	}

	int getDirection()
	{
		return direction;
	}

	float getRadius()
	{
		return radius;
	}
};


class Bomb
{
private:
	Vector *position;
        Vector *velocity; 
	float radius;
public:
	Bomb()
	{
		position = new Vector();
		velocity = new Vector();
		radius = 2.0f;
	}
        void setPosition(float x, float y)
	{
		position->x = x;
		position->y = y;
	}

	Vector *getPosition()
	{
		return position;
	}

	Vector *getVelocity()
	{
		return velocity;
	}

	float getRadius()
	{
		return radius;
	}
};


class World {
private:
	Tank *mLeftTank;
	Tank *mRightTank;

	int mScoreLeft;
	int mScoreRight;

	float mWorldLength;
        float mWorldWidth; 

        float mCycleTime; 

        Bomb *mLeftBomb;
	Bomb *mRightBomb;

public:
	World(){
		mScoreLeft = 0;
		mScoreRight = 0;

		mLeftTank = new Tank();
		mLeftTank->setPosition(5, 50);
		mLeftTank->setDirection(0);

		mRightTank = new Tank();
		mRightTank->setPosition(95,50);
		mRightTank->setDirection(180);

		/*mLeftBomb = NULL;
		mRightBomb = NULL;                
                */
		mWorldLength = 100.0f;
		mWorldWidth = 100.0f;

		mCycleTime = 20; // miliseconds
	}

        void setLeftBomb(Bomb *bomb){
                mLeftBomb=bomb;
        }

        void setRightBomb(Bomb *bomb){
                mRightBomb=bomb;
        }

        void setmScoreRight(int a)
        {
                mScoreRight=a;
        }

        void setmScoreLeft(int a)
        {
                mScoreLeft=a;
        }

        int getmScoreRight()
        {
                return mScoreRight;
        }
        int getmScoreLeft()
        {
                return mScoreLeft;
        }
	int getWorldLength()
	{
		return mWorldLength;
	}

	int getWorldWidth()
	{
		return mWorldWidth;
	}

	float getCycleTime()
	{
		return mCycleTime;
	}

	Tank *getLeftTank()
	{
		return mLeftTank;
	}

	Tank *getRightTank()
	{
		return mRightTank;
	}
        
	Bomb *getLeftBomb()
	{
		return mLeftBomb;
	}

	Bomb *getRightBomb()
	{
		return mRightBomb;
	}
};


class EllipseShape : public sf::Shape
{
public :

    explicit EllipseShape(const sf::Vector2f& radius = sf::Vector2f(0, 0)) :
    m_radius(radius)
    {
        update();
    }

    void setRadius(const sf::Vector2f& radius)
    {
        m_radius = radius;
        update();
    }

    const sf::Vector2f& getRadius() const
    {
        return m_radius;
    }

    virtual unsigned int getPointCount() const
    {
        return 30; // fixed, but could be an attribute of the class if needed
    }

    virtual sf::Vector2f getPoint(unsigned int index) const
    {
        static const float pi = 3.141592654f;

        float angle = index * 2 * pi / getPointCount() - pi / 2;
        float x = std::cos(angle) * m_radius.x;
        float y = std::sin(angle) * m_radius.y;

        return sf::Vector2f(m_radius.x + x, m_radius.y + y);
    }

private :

    sf::Vector2f m_radius;
};


class GUI{
private:
	int monitorLength;
	int monitorWidth;

	float kLength;
	float kWidth;

	sf::RenderWindow *window;
	sf::Event event;

public:
	GUI()
	{
		monitorLength = 1000;
		monitorWidth = 1000;

		window = new sf::RenderWindow(sf::VideoMode(monitorLength, monitorWidth), "Tanks");
	}

	void drawTank(Tank *tank, bool isLeft)
	{
		float radiusX = tank->getRadius() * kLength;
		float radiusY = tank->getRadius() * kWidth;

		EllipseShape shape(sf::Vector2f(radiusX, radiusY));
		if (isLeft)
			shape.setFillColor(sf::Color::Red);
		else
			shape.setFillColor(sf::Color::Green);
		shape.setPosition(tank->getPosition()->x * kLength - radiusX, tank->getPosition()->y * kWidth - radiusY);

		sf::RectangleShape line(sf::Vector2f(20 + tank->getRadius() * std::max(kLength, kWidth), 3));
		line.setPosition(tank->getPosition()->x * kLength, tank->getPosition()->y * kWidth);
		line.rotate(-tank->getDirection());

		window->draw(shape);
		window->draw(line);
	}

	void drawBomb(Bomb *bomb, bool isLeft)
	{
		if (!bomb)
		   return ;

		float radiusX = bomb->getRadius() * kLength;
		float radiusY = bomb->getRadius() * kWidth;
		EllipseShape shape(sf::Vector2f(radiusX, radiusY));
		if (isLeft)
			shape.setFillColor(sf::Color(100, 0, 0));
		else
			shape.setFillColor(sf::Color(0, 0, 100));
		shape.setPosition(bomb->getPosition()->x * kLength - radiusX, bomb->getPosition()->y * kWidth - radiusY);
		window->draw(shape);
	}

	void show(World *world)
	{
		window->clear();

		kLength = ((float) monitorLength) / (world->getWorldLength());
		kWidth  = ((float) monitorWidth) / (world->getWorldWidth());

		drawTank(world->getLeftTank(), true);
		drawTank(world->getRightTank(), false);

		drawBomb(world->getLeftBomb(), true);
		drawBomb(world->getRightBomb(), false);

		window->display();
	}

	bool pollEvent()
	{
		if (window->pollEvent(event))
			return true;
	}

	sf::Event::EventType getEventType()
	{
		return event.type;
	}

	void close()
	{
		window->close();
	}
};


class Manager
{
private:
	World *world;
	GUI *gui;

public:
	Manager()
	{
		world = new World();
		gui = new GUI();
	}

	void run()
	{
                bool running =  true;
		while (running)
		{
			while (gui->pollEvent())
			{
				if (gui->getEventType() == sf::Event::Closed)
					running = false;
			}

			handleGame();
			gui->show(world);
			sf::sleep(sf::milliseconds(world->getCycleTime()));
		}
		gui->close();
	}

	void handleGame()
	{
		Vector *pos1 = world->getLeftTank()->getPosition();
                Vector *pos2 = world->getRightTank()->getPosition();
                int direction1 = world->getLeftTank()->getDirection();
                int direction2 = world->getRightTank()->getDirection();
                float radius1 = world->getLeftTank()->getRadius();
                //float radius2 = world->getLeftBomb()->getRadius();
                int score1=world->getmScoreLeft();
                int score2=world->getmScoreRight();

                if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left)){
                     pos2->x-=0.5;
                }
                if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right)){
                  if(pos2->x >=95)
                      pos2->x=95;
                   else 
                      pos2->x+=0.5;
                }
                if(sf::Keyboard::isKeyPressed(sf::Keyboard::A)){
                   if(pos1->x <=5)
                      pos1->x=5;
                   else 
                      pos1->x-=0.5;
                }
                if(sf::Keyboard::isKeyPressed(sf::Keyboard::D)){
                  pos1->x+=0.5;
                }
                if(sf::Keyboard::isKeyPressed(sf::Keyboard::Up)){
                  direction2--;
                }
                if(sf::Keyboard::isKeyPressed(sf::Keyboard::Down)){
                  direction2++;
                }
                if(sf::Keyboard::isKeyPressed(sf::Keyboard::W)){ 
                  direction1++;
                }
                if(sf::Keyboard::isKeyPressed(sf::Keyboard::S)){
                  direction1--;
                }

                if(sf::Keyboard::isKeyPressed(sf::Keyboard::RShift)){
                      Bomb *a=new Bomb();
                      world->setRightBomb(a);
                      world->getRightBomb()->setPosition(pos2->x,pos2->y);     
                }

                if(world->getRightBomb() != NULL){
                      float radius2 = world->getRightBomb()->getRadius();
                      Vector *pos4 = world->getRightBomb()->getPosition();
                      if(direction2 % 360 >90 && direction2 % 360 <270){
                          pos4->x -= 0.2;
                          pos4->y += 0.2 * tan (direction2); 
                      }
                      if(direction2 % 360 <90 && direction2 % 360 > 270){
                          pos4->x -= -0.2;
                          pos4->y += -0.2 * tan (direction2);
                      }
                      if(direction2 % 360 ==90){
                          pos4->y += -0.2  ;
                      }
                      if(direction2 % 360 ==270){
                          pos4->y += 0.2 ;
                      }
                      
                      if(sqrt((pos1->x - pos4->x)*(pos1->x - pos4->x) + (pos1->y - pos4->y)*(pos1->y - pos4->y)) <= radius1 +radius2){
                          score2+=1;
                          world->setmScoreRight(score2);
                          cout<< score2;
                      }
                     
                }

                 /*if(sf::Keyboard::isKeyPressed(sf::Keyboard::LShift)){
                      world->setLeftBomb(new Bomb());
                      world->getLeftBomb()->setPosition(pos1->x,pos1->y);                      
                }

                if(world->getLeftBomb() != NULL){
                      Vector *pos3 = world->getLeftBomb()->getPosition();
                      if(direction1 % 360 >90 && direction1 % 360 <270){
                          pos3->x -= 0.2;
                          pos3->y += 0.2 * tan (direction1); 
                      }
                      if(direction1 % 360 <90 && direction1 % 360 > 270){
                          pos3->x -= -0.2;
                          pos3->y += -0.2 * tan (direction1);
                      }
                      if(direction1 % 360 ==90){
                          pos3->y += -0.2  ;
                      }
                      if(direction1 % 360 ==270){
                          pos3->y += 0.2 ;
                      }
                      if(sqrt((pos2->x - pos3->x)*(pos2->x - pos3->x) + (pos2->y - pos3->y)*(pos2->y - pos3->y)) <= radius1 +radius2){
                          score1+=1;
                          world->setmScoreLeft(score1);
                          cout<< score1;
                      }   
                     
                }*/
              
		if(pos2->x - pos1->x <= 2*radius1)
                {
			pos1->x = 5;
                        pos2->x=95;
                }
            
                world->getLeftTank()->setPosition(pos1->x, pos1->y);
                world->getRightTank()->setPosition(pos2->x, pos2->y);
                world->getLeftTank()->setDirection(direction1);
                world->getRightTank()->setDirection(direction2);
              	
	}
};


int main()
{cout<<"a";
	Manager manager = Manager();
	manager.run();
	return 0;
}
