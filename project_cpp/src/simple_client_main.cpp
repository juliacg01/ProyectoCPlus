#include "../include/ClientSocket.hpp"
#include "../include/SocketException.hpp"
#include "../include/Draw.hpp"
#include "../include/Utils.hpp"
#include "../include/Planes.hpp"
#include <iostream>
#include <string.h>
#include <vector>
#include <list>
#include <cstdlib>
#include <fstream>
#include <iomanip>
#include <ctime>
#include <sstream>

std::string date(){
	auto t = std::time(nullptr);
	auto tm = *std::localtime(&t);

	std::ostringstream oss;
	oss << std::put_time(&tm, "[%d/%m/%Y %H:%M:%S]: ");
	auto str = oss.str();

	return str;
}

int CheckPoint (int x){
	if(x>-250 && x<250){
		return 1;
	}
	else{
		return 0;
	}
}

int CheckOption (std::string str){
	for(char ch:str){
		if(std::isdigit(ch) == 0){ //isdigit comprueba si el char es un numero o no
			return 0; //es decir, no es un numero
		}
	}
	return 1;
}

void menu(){
	std::cout<<"------------------------------------------"<<std::endl;
	std::cout<<"                   MENU                   "<<std::endl;
	std::cout<<"------------------------------------------"<<std::endl;
	std::cout<< "Options: "<< std::endl;
	std::cout<<"1. Update planes from server"<< std::endl;
	std::cout<<"2. Show planes" << std::endl;
	std::cout<<"3. Check collisions" << std::endl;
	std::cout<<"4. Solve collisions" << std::endl;
	std::cout<<"5. Send planes to server"<<std::endl;
	std::cout<<"6. Update a plane to server"<<std::endl;
	std::cout<<"7. Add new plane to server"<<std::endl;
	std::cout<<"0. Exit"<<std::endl;
	std::cout<<std::endl;
	std::cout<<"Choose: ";
}

int main ( int argc, char *argv[] )
{
	srand(time(NULL)); 
	Utils utils;
	List list_client;

	try
	{
		std::string ip = argv[1];
		int puerto = atoi(argv[2]);
		std::string nick = argv[3];
		std::string msg;
		int id, st_X, st_Y, end_X, end_Y;
		std::vector<std::string> vdata;
		std::vector<std::string> vdata2;
		const char delim = ',';
		const char delim2 = '/';
		int exist = 1;
		std::string reply;

		ClientSocket client_socket (ip, puerto);

		std::fstream file;
		//Para abrir el fichero se indica el nombre, y como se quiere abrir. 
		//En este caso, fstream::app escribe al final del archivo sin borrar nada 
		file.open(argv[4], std::fstream::app);
		//Comprobamos que el fichero se abre correctamente
		if (!file){
			std::cout << "Error open archive "<< argv[4] <<std::endl;
			exit(EXIT_FAILURE);
		}		
		file<< date()<< "New Session Started " <<std::endl;

		while( true ){
			std::string option_str;
			int option;
			menu();
			std::cin>>option_str;
			//Comprobamos que la opcion introducida no es una cadena de caracteres
			if (CheckOption(option_str) == 0) {
				std::cout<<"Incorrect option, try again..."<<std::endl;
				file<< date()<<"Incorrect option"<<std::endl;
				continue;
			}
			//Al comprobar que es un numero, se pasa la string a entero sin que de error
			option=std::stoi(option_str);
			file << date() << "User: "<< nick << " - Option: "<< option<< std::endl;

			//Si se introduce el valor 0 se cierra la conexion
			if(option==0){
				file << date() << "Conection closed" <<std::endl;
				break;
			} 
			//Si se introduce un valor distinto entre 0 y 7 salta error
			if(option<0 || option>7){
				std::cout<<"Incorrect option, try again..."<<std::endl;
				file<< date()<<"Incorrect option"<<std::endl;
			}
			//Procedimientos que se realizan en cada opcion que puede elegir
			switch(option){
				case 1:
					try
					{
						// Send message to server
						msg = nick + "," + std::to_string(option);
						client_socket << msg;

						// Read message from server
						std::cout<<"Updating from the server..."<<std::endl;
						client_socket >> reply;

						// Tokenize the message
						utils.tokenize(reply, delim, vdata);

						int elements = std::stoi(vdata[0]);

						for(int i=1; i<=elements; i++){
							// Tokenize el mensaje con el delimitador /
							utils.tokenize(vdata[i], delim2, vdata2);
							//Se guarda cada dato en una variable
							id = std::stoi(vdata2[0]);
							st_X = std::stoi(vdata2[1]);
							st_Y = std::stoi(vdata2[2]);
							end_X = std::stoi(vdata2[3]);
							end_Y = std::stoi(vdata2[4]);

							//Comprobamos si existe el avion en la lista
							exist = list_client.FindID(id);
							//Si existe se modifica con los nuevos valores
							if(exist==1){
								list_client.UpdatePlane(id, st_X, st_Y, end_X, end_Y);
							}
							//Si no existe, se añade a la lista
							else{
								list_client.AddPlane(id, st_X, st_Y, end_X, end_Y);
							}

							vdata2.clear();
						}
						std::cout<< "Update Client List... "<< std::endl;
						std::cout<<std::endl;
						file<< date()<< "Update Client List"<<std::endl;
					}
					catch ( SocketException& ) {} 
					break;
				case 2:
					try
					{
						// Send message to server
						msg = nick + "," + std::to_string(option);
						client_socket << msg;

						list_client.Show();
						list_client.DrawRadar();
					}
					catch ( SocketException& ) {} 
					break;
				case 3:
					try
					{
						// Send message to server
						msg = nick + "," + std::to_string(option);
						client_socket << msg;
						
						int elements = list_client.CountPlanes();
						if(elements>1){
							list_client.CollisionDetection();
							std::cout << std::endl;
						}
						//Si en la lista hay 1 o ningun avion no pueden existir colisiones
						else{
							std::cout<<"There are not enough planes to have collisions..."<<std::endl;
						}
						
					}
					catch ( SocketException& ) {} 
					break;
				case 4:
					try
					{
						msg = nick + "," + std::to_string(option);
						client_socket << msg;
						
						list_client.SolvingCollisions();
						//list_client.CollisionDetection();
					}
					catch ( SocketException& ) {} 
					break;
				case 5:
					try
					{
						std::string planes = list_client.msgList();
						int elements=list_client.CountPlanes();
						msg = nick + "," + std::to_string(option) + "," + std::to_string(elements) + "," + planes;
						client_socket << msg;
						std::cout<<"Update Server List... "<<std::endl;
						std::cout<<std::endl;
					}
					catch ( SocketException& ) {} 
					break;
				case 6:
					try
					{
						list_client.Show();
						//Comprobamos si existe o no un avion con esta ID 
						do{
							std::cout<<"Introduce ID: ";
							std::cin>>id;
							exist = list_client.FindID(id);
							if(exist == 0) std::cout<< "Not exist plane with ID: "<< id <<std::endl;
						}while(exist==0);

						do{
							std::cout<<"Introduce new start point x: ";
							std::cin>>st_X;
							exist = CheckPoint(st_X);
							if(exist==0) std::cout<<"Incorrect Point, the interval is [-250, 250]. Try again...."<<std::endl;
						}while(exist==0);
						do{
							std::cout<<"Introduce  new start point y: ";
							std::cin>>st_Y;
							exist = CheckPoint(st_Y);
							if(exist==0) std::cout<<"Incorrect Point, the interval is [-250, 250]. Try again...."<<std::endl;
						}while(exist==0);
						do{
							std::cout<<"Introduce new end point x: ";
							std::cin>>end_X;
							exist = CheckPoint(end_X);
							if(exist==0) std::cout<<"Incorrect Point, the interval is [-250, 250]. Try again...."<<std::endl;
						}while(exist==0);
						do{
							std::cout<<"Introduce new end spoint y: ";
							std::cin>>end_Y;
							exist = CheckPoint(end_Y);
							if(exist==0) std::cout<<"Incorrect Point, the interval is [-250, 250]. Try again...."<<std::endl;
						}while(exist==0);

						list_client.UpdatePlane(id, st_X, st_Y, end_X, end_Y);
						std::cout<<"Update plane..."<<std::endl;
						std::cout<<std::endl;
						file<< date()<< "Update plane with ID: "<< id<<std::endl;

						// Send message to server
						msg = nick + "," + std::to_string(option) + ","+ std::to_string(id) + "," + std::to_string(st_X)+ "," + std::to_string(st_Y)+ "," + std::to_string(end_X)+ "," + std::to_string(end_Y);
						client_socket << msg;
					}
					catch ( SocketException& ) {} 
					break;
				case 7:
					try
					{
						list_client.Show();

						std::cout<<"Introduce ID: ";
						std::cin>>id;

						//Comprobamos si existe el avion en la lista, ya que si existe no se añade
						exist = list_client.FindID(id);
						if(exist==1){
							std::cout<<"Plane exist in the client list..."<<std::endl;
						}
						else{
							
							do{
								std::cout<<"Introduce new start point x: ";
								std::cin>>st_X;
								exist = CheckPoint(st_X);
								if(exist==0) std::cout<<"Incorrect Point, the interval is [-250, 250]. Try again...."<<std::endl;
							}while(exist==0);
							do{
								std::cout<<"Introduce  new start point y: ";
								std::cin>>st_Y;
								exist = CheckPoint(st_Y);
								if(exist==0) std::cout<<"Incorrect Point, the interval is [-250, 250]. Try again...."<<std::endl;
							}while(exist==0);
							do{
								std::cout<<"Introduce new end point x: ";
								std::cin>>end_X;
								exist = CheckPoint(end_X);
								if(exist==0) std::cout<<"Incorrect Point, the interval is [-250, 250]. Try again...."<<std::endl;
							}while(exist==0);
							do{
								std::cout<<"Introduce new end spoint y: ";
								std::cin>>end_Y;
								exist = CheckPoint(end_Y);
								if(exist==0) std::cout<<"Incorrect Point, the interval is [-250, 250]. Try again...."<<std::endl;
							}while(exist==0);
						
							//Se añade el nuevo avion a la lista de cliente
							list_client.AddPlane(id, st_X, st_Y, end_X, end_Y);
							list_client.DrawRadar();
							file<< date()<< "Add new plane with ID: "<<id <<std::endl;

							// Y se le envia al servidor para que la incluya en su lista de aviones
							msg = nick + "," + std::to_string(option) + "," + std::to_string(id) + "," + std::to_string(st_X)+ "," + std::to_string(st_Y)+ "," + std::to_string(end_X)+ "," + std::to_string(end_Y);
							client_socket << msg;
						}
					}
					catch ( SocketException& ) {} 
					break;
			}
		}  
		//Ceramos el archivo de texto
		file.close(); 
	}
	catch ( SocketException& e )
	{
		std::cout << "Exception was caught:" << e.description() << "\n";
	}

	return 0;
}