#include <cgame.h>
#include <cfiredoor.h>
#include <iostream>
#include <stdlib.h>
#include <SFML/Graphics.hpp>

#define N 30               // Número de muestras que se toman
#define PLA_ITS 100        // Número de iteraciones para el perceptron
#define DIMENSIONS 1

using namespace FireDoorEscaper;

// Print Game Status Values
void printGameStatus(const CGame& g) {
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

void pintarActual(double inputs[N][DIMENSIONS], bool onfireInputs[N], double weights[DIMENSIONS+1]) {
    sf::RenderWindow window(sf::VideoMode(300, N*10), "Traza");
    window.clear();

    for (int i = 0; i < N; ++i) {
        sf::CircleShape shape(2.f);
        
        if (onfireInputs[i])
            shape.setFillColor(sf::Color::Red);
        else
            shape.setFillColor(sf::Color::Blue);
        
        shape.setPosition(inputs[i][0]+100, i*3);
        window.draw(shape);
    }

    sf::RectangleShape rectangle;
    rectangle.setSize(sf::Vector2f(2, 200));
    rectangle.setOutlineColor(sf::Color::Green);
    rectangle.setPosition(weights[1], weights[0]);
    window.draw(rectangle);
    std::cout << "weights " << weights[0] << ", " <<weights[1] << "\n"; 

    window.display();
    
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }
    }
}

bool h(double weights[DIMENSIONS+1], double x[DIMENSIONS]) {
    double suma = weights[0];
    
    for (int i = 1; i < DIMENSIONS+1; ++i)
        suma += weights[i]*x[i-1];

    return suma>=0;
}

void updateWeights(double weights[DIMENSIONS+1], double inputs[DIMENSIONS], double correct) {
    weights[0] += correct;
    
    for (int i = 1; i < DIMENSIONS+1; ++i)
        weights[i] += correct*inputs[i-1];
}

bool training(double inputs[N][DIMENSIONS], bool onfireInputs[N], double currentInput[DIMENSIONS]) {
    int count = 0;
    double errors[N];
    double weights[DIMENSIONS+1];
    int errorsCount = 0;
    
    weights[0] = 1;     // Se inicializa con peso=1 porque es el threshold
    for (int i = 1; i <= DIMENSIONS; i++)
        weights[i] = 0;
    
    while (count < PLA_ITS) {
        count++;
        errorsCount = 0;

        for (int i = 0; i < N; i++) {
            if (h(weights,inputs[i])) {
                if (onfireInputs[i]) { //debería estar not on fire
                    errors[errorsCount] = i;
                    errorsCount++;
                }
            }
            else if ( ! onfireInputs[i]) { //debería estar on fire
                    errors[errorsCount] = i;
                    errorsCount++;
            }
        }

        //cogemos otro valor para ver que pasa
        if (errorsCount > 0) {
            int i = errors[rand()%errorsCount];
            
            if ( ! onfireInputs[i])
                updateWeights(weights, inputs[i], 1);
            else
                updateWeights(weights, inputs[i], -1);
        }
        else
            break;

        pintarActual(inputs, onfireInputs, weights);
    }

    return h(weights, currentInput);
}

int main(void) {
    // Create a new game starting at level 0, and staying at the same level all the time.
    // Use GDM_LEVELUP for increasing level of difficulty
    CGame *game = new CGame(0);
    game->setGameDifficultyMode(CGame::GDM_SAMELEVEL);
    double inputs[N][DIMENSIONS];
    bool onfireInputs[N];
    double currentInput[DIMENSIONS];
    const CFireDoor::TVecDoubles& inp;

    // Main loop: stay will the game is on (i.e. the player is alive)
    while (game->getGameStatus() == CGame::GS_PLAYING) {
    // Do some game steps and print values
        for (unsigned i=0; i < N; i++) {
            //printGameStatus(*game);
            inp = game->getCurrentFireDoor().getNextStepInputs();

            for (int j=0; j<DIMENSIONS; j++)
                inputs[i][j] = inp[j];   

            game->nextStep();
            onfireInputs[i] = game->getCurrentFireDoor().isOnFire();
        }
        // Try to cross the current FireDoor
        printGameStatus(*game);
        std::cout << "**** TRYING TO CROSS THE DOOR ****\n";

        inp = game->getCurrentFireDoor().getNextStepInputs();
        
        for (int j = 0; j < DIMENSIONS; ++j)
            currentInput[j] = inp[j];

        if (training(inputs, onfireInputs, currentInput))
            game->crossFireDoor();

        if (game->getGameStatus() != CGame::GS_PLAYING)
            std::cout << "!!!!!!!!!!! PLAYER GOT BURNED OUT !!!!!!!!!!!!!!\n";
        else
            std::cout << "****** DOOR PASSED *****\n";
    }

    // Game Over
    delete game;
    return 0;
}
