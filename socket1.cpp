#include <iostream>
#include <sstream>
#include <string>
#include <SFML/Network.hpp>
#include <vector>
#include <SFML/Graphics.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Window/Mouse.hpp>
using namespace std;
using namespace sf;
class Connect{
	private:
		TcpListener listener;
		vector <char> event1;
	public:
		TcpSocket socket_client;
		TcpSocket socket_server;
		sf::Event event;

		Connect(int port){
			listener.listen(port);
			socket_client.connect("192.168.1.52", 55002);
		}
		void Recv(){
			while(true){
				char* buffer;
				size_t received=0;
				socket_client.receive(buffer, sizeof(buffer), received);
				event1.push_back(*buffer);
			}
		}	
};	
	
	
	
int main(){
Connect game(55001);
sf::RenderWindow window(sf::VideoMode(1200, 1000), "Tank");
sf::Texture boll;
boll.loadFromFile("bullet.png");
sf::Sprite bollS(boll);
bollS.setPosition(600,500);
sf::Event event;

while(window.isOpen()){
	while(window.pollEvent(event)){
		if (event.type == sf::Event::Closed)
			window.close();	
		if(event.type == Event::KeyPressed){			
			string message; 
			if (event.key.code == Keyboard::Left)
			{
					message= "3";
					game.socket_server.send(message.c_str(), message.size() + 1);
					bollS.move(-5,0);
			}												
		}	
	}
	window.clear();
	window.draw(bollS);
	window.display();
}
	system("pause");
	return 0;
}
