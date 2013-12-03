#include <cgame.h>
#include <cfiredoor.h>
#include <iostream>
#include <stdlib.h>

#define N 10
#define PASADAS 10
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

bool prediction(double pesos[N+1], double inputs[N], double valorAPredecir)
{
    double suma;
    for (int i = 0; i < N; ++i)
    {
        suma+=pesos[i+1]+inputs[i];
    }

    return suma>=0;
}

bool h(std::vector<double> w, std::vector<double> x)
{
    double suma=w[0];
    for (int i = 1; i < sizeof(w); ++i)
    {
        suma+=w[i]*x[i-1];
    }
}

bool training(double inputs[N], bool onfireInputs[N], double valorAPredecir)
{
    int count=0;
    double errores[N];
    std::vector<double> pesos[N];
    int erroresCount=0;
    double valorCritico=0;
    std::vector<double> aux;
    for (int i = 0; i < N; ++i)
    {
        aux.push_back(1);
        for(int j=0; j<DIMENSIONES; j++)
        {
            aux.push_back(0);
        }
        pesos[i]=aux;
        aux.clear();
    }
    int k=0;

    while(count<PASADAS)
    {
        count++;
        erroresCount=0;

        for (int i = 0; i < N; ++i)
        {
            if(h(w[i],x[i])>0)
            {
                if(onfireInputs[i]) //debería estar not on fire
                {
                    errores[erroresCount]=i;
                    erroresCount++;
                }
            }
            else if(h(w[i],x[i])<0)
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
                pesos[i+1]=-inputs[i];
                //actualizar también todos los w de ese x
            }
            else
            {
                pesos[i+1]=inputs[i];
            }
        }
        else
        {
            break;
        }
    }

    return prediction(pesos, inputs, valorAPredecir);
}

/*double aprendizaje(double valorCritico, double inputs[N], bool onfireInputs[N], double pesos[N])
{
    double learningRate = (double)0.1;
    double errores[N];
    int erroresCount=0;
    int count=0;
    int erroresCountAnt=N;
    double valorCriticoFinal;

    while(count<PASADAS)
    {
        std::cout << "valor: " << valorCritico << "\n";
        count++;
        erroresCount=0;
        for(int i=0; i<N; i++)
        {
            if(valorCritico!=inputs[i])
            {
                if((inputs[i]*pesos[i])>valorCritico)
                {
                    if(!onfireInputs[i])
                    {
                        std::cout<<inputs[i]<< " Error Is not on fire\n";
                        pesos[i]+=learningRate * 1*inputs[i];
                        errores[erroresCount]=inputs[i];
                        erroresCount++;
                    }
                    else
                    {
                        std::cout<<inputs[i]<< " Acierto on fire\n";
                    }
                }
                else
                {
                    if(onfireInputs[i])
                    {
                        std::cout<<inputs[i]<< " Error Is on fire\n";
                        pesos[i]+=learningRate * -inputs[i];
                        errores[erroresCount]=inputs[i];
                        erroresCount++;
                    }
                    else
                    {
                        std::cout<<inputs[i]<< " Acierto not on fire\n";
                    }
                }
            }
        }

        std::cout << "errores: " << erroresCount << "\n";
        if(erroresCount<erroresCountAnt)
        {
            erroresCountAnt = erroresCount;
            if(erroresCount>0)
            {
                valorCriticoFinal = valorCritico;
                valorCritico = errores[rand()%erroresCount];
                std::cout << "valor nuevo: " << valorCritico << "\n";
            }
            else
            {
                valorCriticoFinal = valorCritico;
                break;
            }
        }
        else
        {
            valorCritico = errores[rand()%erroresCount];
        }

        std::cout << "-------------------------------\n";
    }

    return valorCriticoFinal;
}*/

int
main(void) {
    double valorCritico=(double)0;
    int invertido=0;
    // Create a new game starting at level 0, and staying at the same level all the time.
    // Use GDM_LEVELUP for increasing level of difficulty
    CGame *game = new CGame(0);
    game->setGameDifficultyMode(CGame::GDM_SAMELEVEL);
    const CFireDoor& fd = game->getCurrentFireDoor();
    const CFireDoor::TVecDoubles& inp = fd.getNextStepInputs();
    double inputs[N];
    bool onfireInputs[N];
    double pesos[N];

    

    // Main loop: stay will the game is on (i.e. the player is alive)
    while (game->getGameStatus() == CGame::GS_PLAYING) {
        for (int i = 0; i < N; ++i)
        {
            pesos[i]=0;
        }
        // Do some game steps and print values
        for (unsigned i=0; i < N; i++) {
            //printGameStatus(*game);
            game->nextStep();

            const CFireDoor& fd1 = game->getCurrentFireDoor();
            const CFireDoor::TVecDoubles& inp1 = fd.getNextStepInputs();
            inputs[i]=inp[0];
            onfireInputs[i] = fd.isOnFire();
        }
        //valorCritico=aprendizaje(valorCritico, inputs, onfireInputs, pesos);

        // Try to cross the current FireDoor
        printGameStatus(*game);
        std::cout << "**** TRYING TO CROSS THE DOOR ****\n";
        if(training(inputs, onfireInputs, inp[0]))
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
