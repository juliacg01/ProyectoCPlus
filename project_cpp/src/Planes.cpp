#include "../include/Planes.hpp"
#include <iostream>
#include <list>

//Funcion para calcular el angulo que forma dos vectores
float Plane::CalculateAngle(){
    float ab = (getStX() * getEndX()) + (getStY() * getEndY());
    float a = sqrt(pow(getStX(), 2) + pow(getStY(), 2));
    float b = sqrt(pow(getEndX(), 2) + pow(getEndY(), 2));

    float angle = acos(ab / (a*b));
    float ang = angle *(180/3.141592); //Pasamos de radianes a grados

    return ang;
}

//Funcion que segun el angulo se imprime una flecha con la direccion del avion
void Plane::ShowDirection(float angle){
    if ( angle>157.5 || angle<-157.5) std::cout<<"←";
    else if ( angle>-157.5 && angle<-122.5) std::cout<<"↙";
    else if ( angle>-112.5 && angle<-67.5) std::cout<<"↓";
    else if ( angle>-67.5 && angle<-22.5) std::cout<<"↘";
    else if ( angle> -22.5 && angle< 22.5 ) std::cout<<"→";
    else if ( angle> 22.5 && angle<67.5 ) std::cout<<"↗";
    else if ( angle>67.5 && angle<112.5 ) std::cout<<"↑";
    else if ( angle> 112.5 && angle<157.5 ) std::cout<<"↖";
}

//Funcion para mostrar los datos del avion
void Plane::ShowPlane(){
    float angle = CalculateAngle();

	std::cout<< "Plane "<< ID <<": Start ";
	std::cout<<p_start;
    std::cout<<" - Goal ";
    std::cout<<p_end;
    std::cout<< " - Direction: ";
    ShowDirection(angle);
    std::cout<<" - Ang: "<< angle;
	std::cout<< std::endl;
}

//Sobrecaraga del elemento << para mostrar los datos del avion
std::ostream &operator << (std::ostream &mostrar, Plane &p){//sobrecarga del operador <<
    float angle = p.CalculateAngle();
 	mostrar << "Plane "<< p.getID() <<": Start ";
    mostrar << p.p_start <<" - Goal "; 
    mostrar << p.p_end << " - Direction: ";
    p.ShowDirection(angle);
    mostrar <<" - Ang: "<< angle << std::endl;

    return mostrar;
} 

//Funcion que recorre la lista de avion y los muestra 
void List::Show(){
	std::list<Plane>::iterator start = list.begin();
    std::list<Plane>::iterator stop = list.end();
    std::cout << std::endl;
    std::cout << std::endl;  
	std::cout << "--------------------------------------------"<< std::endl;    
    std::cout <<"                 PLANES:"					   <<std::endl;
	std::cout << "--------------------------------------------"<< std::endl;  
    while (start != stop){
        start->ShowPlane();
        ++start;
    }
    std::cout<<std::endl;
}

//Funcion que devuelve una string con el formato mensaje que contiene los datos de los aviones
std::string Plane::msgPlane(){
    int st_X = getStX();
    int st_Y = getStY();
    int end_X = getEndX();
    int end_Y = getEndY();

    std::string msg = std::to_string(ID) + "/" + std::to_string(st_X) + "/" + std::to_string(st_Y) + "/" + std::to_string(end_X) + "/" + std::to_string(end_Y) ;
    return msg;
}

//Funcion que devuelve una string con todos los aviones en formato mensaje
std::string List::msgList(){
    std::list<Plane>::iterator start = list.begin();
    std::list<Plane>::iterator stop = list.end();
    std::string msg;
    while(start!=stop){
        msg = msg + "," + start->msgPlane() ;
        ++start;
    }
    return msg;
}

//Funcion para ñadir un nuevo avion a la lista
void List::AddPlane(int id, int st_X, int st_Y, int end_X, int end_Y){
    Plane p(id, st_X, st_Y, end_X, end_Y);
    list.push_back(p);
}

//Funcion para contar los elementos/ aviones que hay en la lista guardados
int List::CountPlanes(){
    std::list<Plane>::iterator start = list.begin();
    std::list<Plane>::iterator stop = list.end();
    int count=0;

    while(start!=stop){
        ++count;
        ++start;
    }
    return count;
}

//Funcion booleana que busca el avion por su id, e informa si existe o no
int List::FindID(int id){
    Plane p(id,0,0,0,0);
    //Se crea un interador que recorra la lista y se busca si esta este avión en la lista
    std::list<Plane>::iterator it;
    it = std::find(list.begin(), list.end(), p);
    
    if (it != list.end()){
        return 1;
    }
    else{
        return 0;
    }
}

//Funcion que modifica los datos del avion con la ID que introduzca
void List::UpdatePlane(int id, int st_X, int st_Y, int end_X, int end_Y){
    Plane p(id,0,0,0,0);
    //Se crea un interador que recorra la lista y se busca si esta este avión en la lista
    std::list<Plane>::iterator it;
    it = std::find(list.begin(), list.end(), p);
    
    //Si esta el avion en la lista se elimina
    if (it != list.end()){
        it->SetPoint(st_X, st_Y, end_X, end_Y);
    }
}

//Funcion que recorra toda la lista comprobando si se colisionan entre ellos
void List::CollisionDetection(){
    std::list<Plane>::iterator start = list.begin();
    std::list<Plane>::iterator aux = list.begin();
    std::list<Plane>::iterator stop = list.end();
    Utils utils;

    std::cout<<"--------------------------------------------"<< std::endl;    
    std::cout <<"            COLLISION DETECTION:"					   <<std::endl;
	std::cout << "--------------------------------------------"<< std::endl;  

    while(start!=stop){
        ++aux;
        while(aux!=stop){
            Point s1(start->getStX(), start->getStY());
            Point e1(start->getEndX(), start->getEndY());
            int id1 = start->getID();

            Point s2(aux->getStX(), aux->getStY());
            Point e2(aux->getEndX(), aux->getEndY());
            int id2 = aux->getID();

            if (utils.doIntersect(s1, e1, s2, e2)){
                std::cout << "A"<< id1 << " vs A"<< id2<< " - Collisions:  Yes" << std::endl;
            }
            else{
                std::cout << "A"<< id1 << " vs A"<< id2<< " - Collisions:  No" << std::endl;
            }
            ++aux;
        }  
        ++start;
        aux = start;
    }
}

//Funcion para resolver las colisiones
void List::SolvingCollisions(){
    std::list<Plane>::iterator start = list.begin();
    std::list<Plane>::iterator aux = list.begin();
    std::list<Plane>::iterator stop = list.end();
    Utils utils;

    std::cout<<"--------------------------------------------"<< std::endl;    
    std::cout <<"Solving Collisions..." <<std::endl;

    while(start!=stop){
        ++aux;
        while(aux!=stop){
            Point s1(start->getStX(), start->getStY());
            Point e1(start->getEndX(), start->getEndY());
            int id1 = start->getID();

            Point s2(aux->getStX(), aux->getStY());
            Point e2(aux->getEndX(), aux->getEndY());
            int id2 = aux->getID();

            while (utils.doIntersect(s1, e1, s2, e2)){

                start->SetPoint(-250 + rand() %500, -250 + rand() %500, -250 + rand() %500, -250 + rand() %500);
                Point s1(start->getStX(), start->getStY());
                Point e1(start->getEndX(), start->getEndY());

                std::cout<<"A"<< id1 << " vs A"<< id2 << std::endl;
                start->ShowPlane();
                if(utils.doIntersect(s1, e1, s2, e2)) break;
            }
            ++aux;
        }  
        ++start;
        aux = start;
    }
    
    std::cout<<"All collisions have resolved"<<std::endl;
}

//Funcion que dibuja los aviones en el radar
void List::DrawRadar(){
    std::list<Plane>::iterator start = list.begin();
    std::list<Plane>::iterator stop = list.end();
    Draw dr(500);

    while(start!=stop){
        // Draw arrows, these represent the aiplanes and their direction
		dr.drawLine(start->getStX(), start->getStY(), start->getEndX(), start->getEndY(), "A" + std::to_string(start->getID()));
        ++start;
    }
    dr.show("Map", 1000);
}
