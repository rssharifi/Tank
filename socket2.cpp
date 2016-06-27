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
		TcpSocket socket_client;
		TcpSocket socket_server;
		TcpListener listener;
	public:
		sf::Event event;
		vector <char> event1;

		Connect(int port){
			socket_client.connect("192.168.1.52", 55001);
			listener.listen(port);		
		}
		void accept(){
			listener.accept(socket_server);	
		}
		void Recv(){
			while(true){
				char buffer[10];
				size_t received=0;
				socket_client.receive(buffer, sizeof(buffer), received);
				event1.push_back(buffer[0]);
			}
		}
		
};	
	
	
	
int main(){
		Connect game(55002);
		sf::RenderWindow window(sf::VideoMode(1200, 1000), "Tank");
		sf::Texture boll;
		boll.loadFromFile("bullet.png");
		sf::Sprite bollS(boll);
		bollS.setPosition(600,500);
		sf::Event event;
	while(window.isOpen()){
		window.clear();
		window.draw(bollS);
		window.display();
		game.accept();
		game.Recv();
		if(game.event1[0]==3)
			bollS.move(-5,0);
		Thread *thread=new Thread(& Connect::Recv,& game  );
		thread->launch();
		
	}
		system("pause");
	return 0;
}
