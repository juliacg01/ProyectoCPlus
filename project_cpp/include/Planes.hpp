#ifndef Plane_class
#define Plane_class

#include "Point.hpp"
#include "Utils.hpp"
#include "Draw.hpp"
#include <iostream>
#include <list>
#include <cmath>
#include <math.h>
#include <algorithm>

class Plane{
    public:
        Plane(int id, int st_x, int st_y, int end_x, int end_y){
			ID = id;
            p_start.setXY(st_x, st_y);
            p_end.setXY(end_x, end_y);
			}
        ~Plane(){};
        int getID(){return ID;}
        int getStX(){return p_start.getX();}
        int getStY(){return p_start.getY();}
        int getEndX(){return p_end.getX();}
        int getEndY(){return p_end.getY();}
        void setID(int id){ID = id;}
        void SetPoint(int st_X, int st_Y, int end_X, int end_Y){
            p_start.setXY(st_X, st_Y);
            p_end.setXY(end_X, end_Y);
        }
		void ShowPlane();
        friend std::ostream& operator << (std::ostream &mostrar, Plane &p); //sobrecarga del operador <<
        float CalculateAngle();
        void ShowDirection(float angle);
        std::string msgPlane();
        //Sobrecarga para poder buscar los aviones por su ID
        int operator==(const Plane& right) {
            return this -> ID == right.ID;
        }
    private: 
        int ID;
        Point p_start;
        Point p_end;
};

class List{
    private:
        std::list<Plane> list;
    public:
        List(){};
        ~List(){};
        void AddPlane(int id, int st_X, int st_Y, int end_X, int end_Y);
        void UpdatePlane(int id, int st_X, int st_Y, int end_X, int end_Y);
        int CountPlanes();
        void Show();
        std::string msgList();
        int FindID(int id);
        void DrawRadar();
        void CollisionDetection();
        void SolvingCollisions();
};

#endif