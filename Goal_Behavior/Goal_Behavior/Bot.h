#pragma once

class Bot
{
private: 
		int salud;
		int posx;
		int posy;
		bool arma;
		Goal_Think*  mente;
		
public:
	Bot(int,int,int,bool);
	void setSalud(int);
	void setPosition(int,int);
	void setArma(bool);
	int getSalud();
	int getPosx();
	int getPosy();
	bool getArma();
	~Bot(void);
};

