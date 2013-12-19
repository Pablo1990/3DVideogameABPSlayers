#include <cgame.h>
#include <cfiredoor.h>
#include <iostream>
#include <stdlib.h>
#include <SFML/Graphics.hpp>

#define N 40               // Número de muestras que se toman
#define PLA_ITS 1000        // Número de iteraciones para el perceptron
#define DIMENSIONS 2

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
    sf::RenderWindow window(sf::VideoMode(1000, N*10), "Traza");
    window.clear();

    for (int i = 0; i < N; ++i) {
        sf::CircleShape shape(2.f);
        std::cout << "puntos: " << inputs[i][0] << inputs[i][1] << "\n";

        if (onfireInputs[i])
            shape.setFillColor(sf::Color::Red);
        else
            shape.setFillColor(sf::Color::Blue);
        
        if (DIMENSIONS==1)
            shape.setPosition(inputs[i][0] + 100, i * 3);
        else if (DIMENSIONS==2)
            shape.setPosition(inputs[i][0]/100 + 500, inputs[i][1]);
            
        window.draw(shape);
    }

    sf::RectangleShape rectangle;
    rectangle.setSize(sf::Vector2f(2, 200));
    rectangle.setOutlineColor(sf::Color::Green);

    double point1 = (-1 * weights[0]) / weights[2];

    double point2 = (-1 * weights[2] * point1 - weights[0]) / weights[1];

    rectangle.setPosition(point1/100+500, point2);
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
    double sum = weights[0];
    
    for (int i = 1; i < DIMENSIONS+1; ++i)
        sum += weights[i] * x[i-1];

    return sum >= 0;
}

void updateWeights(double weights[DIMENSIONS+1], double inputs[DIMENSIONS], double correct) {
    weights[0] += correct;
    
    for (int i = 1; i < DIMENSIONS+1; ++i)
        weights[i] += correct*inputs[i-1];
}

bool training(double inputs[N][DIMENSIONS], bool onfireInputs[N], double currentInput[DIMENSIONS]) {
    double errors[N];
    double weights[DIMENSIONS+1];
    double bestWeights[DIMENSIONS+1];
    int errorsCount = 0;
    int randomInput;
    int minErrorsCount = N+1;
    
    weights[0] = 1;     // Se inicializa con peso=1 porque es el threshold
    for (int i = 1; i <= DIMENSIONS; i++)
        weights[i] = 0;
    
    for (int i=0; i < PLA_ITS; i++) {
        errorsCount = 0;

        for (int j = 0; j < N; j++) {            
            if (h(weights, inputs[j]) == onfireInputs[j]) {  // Se comprueba si hay error
                errors[errorsCount] = j;
                errorsCount++;
            }
        }

        if (errorsCount > 0) {  // Se actualiza un peso en caso de haber error
            if (errorsCount < minErrorsCount) {  // Se guardan los mejores pesos
                minErrorsCount = errorsCount;
                std::copy(&weights[0], &weights[DIMENSIONS+1], bestWeights);
            }

            randomInput = errors[rand()%errorsCount];
            updateWeights(weights, inputs[randomInput], (onfireInputs[randomInput]? -1 : 1));
        }
        else
            return h(weights, currentInput);

        pintarActual(inputs, onfireInputs, weights);
    }

    return h(bestWeights, currentInput);
}

int main(void) {
    // Create a new game starting at level 0, and staying at the same level all the time.
    // Use GDM_LEVELUP for increasing level of difficulty
    CGame *game = new CGame(DIMENSIONS-1);
    game->setGameDifficultyMode(CGame::GDM_SAMELEVEL);
    double inputs[N][DIMENSIONS];
    bool onfireInputs[N];
    double currentInput[DIMENSIONS];

    // Main loop: stay will the game is on (i.e. the player is alive)
    while (game->getGameStatus() == CGame::GS_PLAYING) {
    // Do some game steps and print values
        for (unsigned i=0; i < N; i++) {
            //printGameStatus(*game);
            const CFireDoor::TVecDoubles& inp = 
                    game->getCurrentFireDoor().getNextStepInputs();

            for (int j=0; j<DIMENSIONS; ++j)
                inputs[i][j] = inp[j];   

            game->nextStep();
            onfireInputs[i] = game->getCurrentFireDoor().isOnFire();
        }
        // Try to cross the current FireDoor
        printGameStatus(*game);
        std::cout << "**** TRYING TO CROSS THE DOOR ****\n";

        const CFireDoor::TVecDoubles& inp =
                game->getCurrentFireDoor().getNextStepInputs();
        
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