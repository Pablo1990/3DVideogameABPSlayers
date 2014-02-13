#include "Position.h"

using namespace std;

Position::Position(){
	this->x = 0;
	this->y = 0;
	this->z = 0;
}

Position::Position(const Position& p){
	this->x = p.getX();
	this->y = p.getY();
	this->z = p.getZ();
}

Position::Position(float x, float y, float z)
{
	this->x = x;
	this->y = y;
	this->z = z;
}


Position::~Position(void)
{
}

float Position::getX() const{
	return x;
}

void Position::setX(float x){
	this->x = x;
}

float Position::getY() const {
	return y;
}

void Position::setY(float y){
	this->y=y;
}

float Position::getZ() const{
	return z;
}

void Position::setZ(float z){
	this->z = z;
}

std::ostream& operator<<(std::ostream &strm, const Position &p) {
	return strm << "Position(" << p.getX << ", " << p.getY() << ", " << p.getZ() << ")";
}


