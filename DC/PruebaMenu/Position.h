#pragma once
class Position
{
private:
	float x, y, z;
public:
	Position(float, float, float);
	Position(const Position&);
	Position(void);
	~Position(void);

	/*getters y setters*/
	float getX() const;
	void setX(float);

	float getY() const;
	void setY(float);

	float getZ() const;
	void setZ(float);

	/*sobrecarga operadores */
	//Position operator=(const Position&);
};

