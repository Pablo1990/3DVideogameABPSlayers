#include <cgame.h>
#include <cfiredoor.h>
#include <iostream>
#include <stdlib.h>
#include <SFML/Graphics.hpp>

#define N 10
#define PASADAS 100
#define DIMENSIONES 1

using namespace FireDoorEscaper;

// Print Game Status Values
void
printGameStatus(const CGame& g) {
    const CFireDoor& fd = g.getCurrentFireDoor();
    const CFireDoor::TVecDoubles& inp = fd.getNextStepInputs();
    std::cout   << "Lvl: " << g.getLevel()
                << " | Steps: " << g.getSteps()
                << " | Score: " << g.getScore()
                << " | Door: " << g.getDoorsPassed() + 1
                << " | DSteps: " << fd.getSteps()
                << " | OnFire?: " << ((fd.isOnFire()) ? "yes" : "no")
                << std::endl
                << "=========================================================================\n"
                << "Next Inputs: [ ";
    for (unsigned i=0; i < inp.size(); i++)
        std::cout << inp[i] << " ";
    std::cout << "] \n";
}

void pintarActual(double inputs[N][DIMENSIONES], bool onfireInputs[N], double pesos[DIMENSIONES+1]){

    sf::RenderWindow window(sf::VideoMode(300, N*10), "Traza");
    window.clear();

    for (int i = 0; i < N; ++i)
    {
        sf::CircleShape shape(2.f);
        if(onfireInputs[i])
        {
            shape.setFillColor(sf::Color::Red);
        }
        else
        {
            shape.setFillColor(sf::Color::Blue);
        }
        shape.setPosition(inputs[i][0]+100, i*3);
        window.draw(shape);
    }

    sf::RectangleShape rectangle;
    rectangle.setSize(sf::Vector2f(2, 200));
    rectangle.setOutlineColor(sf::Color::Green);
    rectangle.setPosition(pesos[1], pesos[0]);
    window.draw(rectangle);
    std::cout<<"pesos "<< pesos[0]<<", " <<pesos[1]<<"\n"; 

    window.display();
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }
    }
}

bool h(double pesos[DIMENSIONES+1], double x[DIMENSIONES])
{
    double suma=pesos[0];
    for (int i = 1; i < DIMENSIONES+1; ++i)
    {
        suma+=pesos[i]*x[i-1];
    }
    return suma>=0;
}

void actualizarPesos(double pesos[DIMENSIONES+1], double inputs[DIMENSIONES], double correct)
{
    pesos[0]+=correct;
    for (int i = 1; i < DIMENSIONES+1; ++i)
    {
        pesos[i]+=correct*inputs[i-1];
    }
}

bool training(double inputs[N][DIMENSIONES], bool onfireInputs[N], double valorAPredecir[DIMENSIONES])
{
    int count=0;
    double errores[N];
    double pesos[DIMENSIONES+1];
    int erroresCount=0;
    double valorCritico=0;
        pesos[0]=1;
        for (int i = 1; i <= DIMENSIONES; ++i)
        {
            pesos[i] = 0;
        }
    int k=0;

    while(count<PASADAS)
    {
        count++;
        erroresCount=0;

        for (int i = 0; i < N; ++i)
        {
            if(h(pesos,inputs[i]))
            {
                if(onfireInputs[i]) //debería estar not on fire
                {
                    errores[erroresCount]=i;
                    erroresCount++;
                }
            }
            else
            {
                if(!onfireInputs[i]) //debería estar on fire
                {
                    errores[erroresCount]=i;
                    erroresCount++;
                }
            }
        }

        //cogemos otro valor para ver que pasa
        if(erroresCount>0)
        {
            int i = errores[rand()%erroresCount];
            if(!onfireInputs[i])
            {
                actualizarPesos(pesos, inputs[i], 1);
            }
            else
            {
                actualizarPesos(pesos, inputs[i], -1);
            }
        }
        else
        {
            break;
        }


        pintarActual(inputs, onfireInputs, pesos);
    }

    return h(pesos, valorAPredecir);
}

int
main(void) {
    // Create a new game starting at level 0, and staying at the same level all the time.
    // Use GDM_LEVELUP for increasing level of difficulty
    CGame *game = new CGame(0);
    game->setGameDifficultyMode(CGame::GDM_SAMELEVEL);
    double inputs[N][DIMENSIONES];
    bool onfireInputs[N];

    

    // Main loop: stay will the game is on (i.e. the player is alive)
    while (game->getGameStatus() == CGame::GS_PLAYING) {
    // Do some game steps and print values
        for (unsigned i=0; i < N; i++) {
            //printGameStatus(*game);
            game->nextStep();

            const CFireDoor& fd1 = game->getCurrentFireDoor();
            const CFireDoor::TVecDoubles& inp1 = fd1.getNextStepInputs();
            for(int j=0; j<DIMENSIONES; j++)
            {
                inputs[i][j]=inp1[j];   
            }
            onfireInputs[i] = fd1.isOnFire();
        }
        // Try to cross the current FireDoor
        printGameStatus(*game);
        std::cout << "**** TRYING TO CROSS THE DOOR ****\n";

        const CFireDoor& fd = game->getCurrentFireDoor();
        const CFireDoor::TVecDoubles& inp = fd.getNextStepInputs();
        double valorAPredecir[DIMENSIONES];
        for (int j = 0; j < DIMENSIONES; ++j)
        {
            valorAPredecir[j] = inp[j];
        }
        if(training(inputs, onfireInputs, valorAPredecir))
        {
            game->crossFireDoor();
        }        
        if (game->getGameStatus() != CGame::GS_PLAYING)
            std::cout << "!!!!!!!!!!! PLAYER GOT BURNED OUT !!!!!!!!!!!!!!\n";
        else
            std::cout << "****** DOOR PASSED *****\n";
    }

    // Game Over
    delete game;
    return 0;
}
