#include "../include/ServerSocket.hpp"
#include "../include/SocketException.hpp"
#include "../include/Draw.hpp"
#include "../include/Utils.hpp"
#include "../include/Planes.hpp"
#include <string>
#include <iostream>
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

int main ( int argc, char *argv[] )
{
	srand(time(NULL)); 
	List list_server;
	std::cout << "Server running....\n";

	try
  	{
		// Create the socket
		ServerSocket server ( atoi(argv[1]) );
		int id, st_X, st_Y, end_X, end_Y, elements;
		int n_planes = atoi(argv[2]);

		for(int i=0; i<n_planes; i++){
			id = 100 + rand() % 300; //Numero aleatorio entre 100 y 400
			st_X = -250 + rand() %500; //Numero aleatorio entre -250 y 250
			st_Y = -250 + rand() %500;
			end_X = -250 + rand() %500;
			end_Y = -250 + rand() %500;

			list_server.AddPlane(id, st_X, st_Y, end_X, end_Y);
		}
		list_server.Show();
		list_server.CollisionDetection();
		std::cout<<std::endl;

		while ( true )
		{
			Utils utils;

			ServerSocket new_sock;
			std::cout << "Waiting client to connect..." << std::endl;
			server.accept ( new_sock );
			std::cout << "Conection accepted" << std::endl;
			std::cout<<std::endl;
			
			std::string data;
			std::string msg;
			const char delim = ',';
			const char delim2 = '/';
			std::vector<std::string> vdata;
			std::vector<std::string> vdata2;
			int exist = 0;

			std:: fstream file;
			//Para abrir el fichero se indica el nombre, y como se quiere abrir. 
			//En este caso, fstream::app escribe al final del archivo sin borrar nada 
			file.open(argv[3], std::fstream::app); 
			//Comprobamos que el fichero se abre correctamente
			if (!file){
				std::cout << "Error open archive "<< argv[3] <<std::endl;
				exit(EXIT_FAILURE);
			}		
			file<< date()<< "New Session Started " <<std::endl;

			try
			{
				while ( true )
				{
					// Read message from client
					new_sock >> data;

					// Tokenize the message
					utils.tokenize(data, delim, vdata);
					std::string nick = vdata[0];
					int option = std::stoi(vdata[1]);
					std::cout << "User: "<< nick << " - Option: "<< option<<std::endl;

					//Guardamos la informacion en el archivo log
					file << date() << "User: "<< nick << " - Option: "<< option<< std::endl;

					//Procedimeintos que el servidor realiza en distintas opciones
					if(option==1){
						std::string planes = list_server.msgList();
						int elements=list_server.CountPlanes();
						planes = std::to_string(elements) + "," + planes;
						new_sock << planes;
					}
					if(option==5){
						int elements = std::stoi(vdata[2]);
						//Eliminamos los tres primeros elementos que son el nombre del usuario, la opcion y la cantidad de aviones
						vdata.erase (vdata.begin(),vdata.begin()+2);
						
						//bucle para tokenizar cada avion para conseguir sus datos y añadirlos en la lista
						for(int i=1; i<=elements; i++){
							// Tokenize el mensaje con el delimitador de /
							utils.tokenize(vdata[i], delim2, vdata2);

							//Se guarda cada dato en una variable
							id = std::stoi(vdata2[0]);
							st_X = std::stoi(vdata2[1]);
							st_Y = std::stoi(vdata2[2]);
							end_X = std::stoi(vdata2[3]);
							end_Y = std::stoi(vdata2[4]);

							//Comprueba si el avion introduccido por el cliente existe o no
							exist = list_server.FindID(id);
							//Si no existe lo introducce directamente a la lista del servidor
							if(exist==0){
								list_server.AddPlane(id, st_X, st_Y, end_X, end_Y);
							}
							//Si ya existe, lo modifica
							else{
								list_server.UpdatePlane(id, st_X, st_Y, end_X, end_Y);
							}
							//Borramos el vector que contiene los datos del avion para poder volver a realizar el bucle
							vdata2.clear();
						}
						file<< date()<< "Update Server List"<<std::endl;
						std::cout<< "Update Server List... "<<std::endl;
						list_server.Show();
					}
					if(option==6){
						//Se guarda cada dato en una variable
						id = std::stoi(vdata[2]);
						st_X = std::stoi(vdata[3]);
						st_Y = std::stoi(vdata[4]);
						end_X = std::stoi(vdata[5]);
						end_Y = std::stoi(vdata[6]);

						//Se busca en la lista del servidor si existe un avion con la id introduccida
						exist = list_server.FindID(id);
						//Si existe se modifica los datos del avion
						if(exist==1){
							list_server.UpdatePlane(id, st_X, st_Y, end_X, end_Y);

							file<< date()<< "Update plane with ID: "<< id<<std::endl;
							list_server.Show();
							std::cout << std::endl;
						}
						else{
							list_server.AddPlane(id, st_X, st_Y, end_X, end_Y);

							file<< date()<< "Update plane with ID: "<< id<<std::endl;
							list_server.Show();
							std::cout << std::endl;
						}
						
					}
					if (option==7){
						//Se guarda cada dato en una variable
						id = std::stoi(vdata[2]);
						st_X = std::stoi(vdata[3]);
						st_Y = std::stoi(vdata[4]);
						end_X = std::stoi(vdata[5]);
						end_Y = std::stoi(vdata[6]);

						//Comprueba si el avion introduccido por el cliente existe o no
						exist = list_server.FindID(id);
						//Si no existe lo introducce directamente a la lista del servidor
						if(exist==0){
							list_server.AddPlane(id, st_X, st_Y, end_X, end_Y);
							file<< date()<< "Add new plane with ID: "<<id <<std::endl;
						}
						//Si ya existe lo modifica
						else{
							list_server.UpdatePlane(id, st_X, st_Y, end_X, end_Y);
							file<< date()<< "Update plane with ID: "<< id<<std::endl;
						}

						list_server.Show();
						std::cout << std::endl;
					}
					//Se borra el vector del tokenize cada vez que se realice una opcion para que la proxima informacion se guarde en esos espacios
					vdata.clear();	
				}
				//Ceramos el archivo de texto
				file.close();
			}
			catch ( SocketException& ) {}
			std::cout << "Conection closed " << std::endl;
		}
	}
	catch ( SocketException& e )
	{
		std::cout << "Exception was caught:" << e.description() << "\nExiting.\n";
	}

	return 0;
}