#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define N_DICE 5
#define TURNS 11
#define VALUES 6
#define MAX_POINTS 485

void rollDice(int originalArr[N_DICE], int diceCount);
void showRoll(int arr[N_DICE]);
void sortDescending(int arr[N_DICE]);
void initializeArrays(int arr[], int capacity);
void countFrequencies(int dice[], int frequencies[]);
void addScore(int scores[], int dice[], int frequencies[], int firstGenerala);
void showScore(int score[]);
int getFinalScore(int arr[]);
char *showPerformance(int score);
void showAuxArr(int arr[]);
void resetGame(int *t, int timeArr[], int playerCount, int scoreP1[],
               int scoreP2[], int *turnP1, int *turnP2);
void timeLogger(int player, time_t start, int times[]);

int main() {
  // Instantiate variables for dice, repeater, final reset and player count
  int dice[N_DICE], a, repeat, playerCount = 1;
  // Create seed for dice randomness
  srand(time(NULL));

  do {
    printf("Enter 1 to play solo or 2 to play with 2 players > ");
    scanf("%d", &playerCount);
  } while (playerCount >= 3 || playerCount <= 0);

  // Instantiate variables for player scores and turns
  int scoreP1[TURNS], scoreP2[TURNS], finalScoreP1, finalScoreP2;
  int turnP1 = 1, turnP2 = 1;
  char nameP1[21], nameP2[21];
  int firstGeneralaP1 = 0, firstGeneralaP2 = 0;

  // Total elapsed time in seconds for each player
  int times[playerCount];

  initializeArrays(scoreP1, TURNS);

  if (playerCount == 2) {
    initializeArrays(scoreP2, TURNS);
  }

  printf("Enter player 1 name, max 20 characters > ");
  scanf("%20s", nameP1);

  if (playerCount == 2) {
    printf("Enter player 2 name, max 20 characters > ");
    scanf("%20s", nameP2);
  }

  int activeTurn;
  for (activeTurn = 0; activeTurn < (TURNS * playerCount); activeTurn++) {
    int reset = 1;

    // Array to store frequencies of each number. Required for initialization
    int frequencies[VALUES];

    initializeArrays(frequencies, VALUES);
    int rollCount = 1;

    // Ternary operators that summarize a control structure to print name and
    // active turn
    printf("Player %s's Turn: %d of %d",
           playerCount == 1      ? nameP1
           : activeTurn % 2 == 0 ? nameP1
                                 : nameP2,
           playerCount == 1      ? activeTurn + 1
           : activeTurn % 2 == 0 ? turnP1
                                 : turnP2,
           TURNS);

    rollDice(dice, N_DICE);

    // Instantiate time_t start with long type, and save start time after
    // rolling dice
    time_t start = time(NULL);

    do {
      printf("If you want to repeat your roll (attempt 2), press 1, otherwise "
             "press 0 > ");
      scanf("%d", &repeat);
    } while (repeat < 0 || repeat > 1);

    while (repeat != 0 && rollCount < 3) {
      int diceToRoll = 0;

      printf("Enter the number of dice to roll again > ");
      scanf("%d", &diceToRoll);

      while (diceToRoll > 5 || diceToRoll < 1) {
        printf("Maximum number of dice to roll is 5, please choose 1 to 5 dice "
               "> ");
        scanf("%d", &diceToRoll);
      }
      rollDice(dice, diceToRoll);
      rollCount++;

      if (rollCount >= 3) {
        break;
      } else {
        printf("If you want to repeat your roll (attempt %d), press 1, "
               "otherwise press 0 > ",
               rollCount + 1);
        scanf("%d", &repeat);
      }
    }

    countFrequencies(dice, frequencies);

    if (playerCount == 2) {
      if (activeTurn % 2 == 0) {
        turnP1++;
        addScore(scoreP1, dice, frequencies, firstGeneralaP1);
        showScore(scoreP1);
        timeLogger(0, start, times);
      } else {
        turnP2++;
        addScore(scoreP2, dice, frequencies, firstGeneralaP2);
        showScore(scoreP2);
        timeLogger(1, start, times);
      }
    } else {
      turnP1++;
      addScore(scoreP1, dice, frequencies, firstGeneralaP1);
      showScore(scoreP1);
      timeLogger(0, start, times);
    }

    do {
      printf("If you want to reset the game press 0, otherwise press 1 > ");
      scanf("%d", &reset);
    } while (reset != 1 && reset != 0);

    if (reset == 0) {
      resetGame(&activeTurn, times, playerCount, scoreP1, scoreP2, &turnP1,
                &turnP2);
    }

    if (playerCount == 2) {
      if (activeTurn % 2 == 0) {
        printf("Total elapsed time for P%d is %d seconds\n", 1, times[0]);
      } else {
        printf("Total elapsed time for P%d is %d seconds\n", 2, times[1]);
      }
    } else {
      printf("Total elapsed time for P%d is %d seconds\n", 1, times[0]);
    }
  }

  finalScoreP1 = getFinalScore(scoreP1);

  if (playerCount == 2) {
    finalScoreP2 = getFinalScore(scoreP2);
    if (finalScoreP1 > finalScoreP2) {
      printf("Player %s has scored %d points, beating player %s who scored %d "
             "points.\n",
             nameP1, finalScoreP1, nameP2, finalScoreP2);
    } else if (finalScoreP1 < finalScoreP2) {
      printf("Player %s has scored %d points, beating player %s who scored %d "
             "points.\n",
             nameP2, finalScoreP2, nameP1, finalScoreP1);
    } else {
      printf("Both players have scored %d points. It's a tie!\n", finalScoreP1);
    }
    printf("Performance of %s (P1): %s\n", nameP1,
           showPerformance(finalScoreP1));
    printf("Performance of %s (P2): %s\n", nameP2,
           showPerformance(finalScoreP2));
  } else {
    printf("%s your final score was %d\n", nameP1, finalScoreP1);
    printf("Performance: %s\n", showPerformance(finalScoreP1));
  }
  printf("Press any key to close the game > ");
  scanf("%d", &a);
  return 0;
}

// Functions

// Dice rolling (for all cases)
void rollDice(int arr[N_DICE], int diceCount) {
  if (diceCount == N_DICE) {
    printf("\nRolling!\n");
    int a = 0;
    for (int i = 0; i < N_DICE; i++) {
      a = (rand() % 6) + 1;
      arr[i] = a;
    }
    sortDescending(arr);
    showRoll(arr);
    return;
  }

  int selectorArr[diceCount];

  for (int i = 0; i < diceCount; i++) {
    int num;
    printf("Select the position of element %d of %d that you chose > ", i + 1,
           diceCount);
    scanf("%d", &num);
    while (num > 5 || num < 1) {
      printf("Invalid position. Dice positions are from 1 to 5. Enter a valid "
             "position > ");
      scanf("%d", &num);
    }

    selectorArr[i] = num - 1;
  }

  for (int i = 0; i < diceCount; i++) {
    for (int j = 0; j < N_DICE; j++) {
      if (selectorArr[i] == j) {
        int a = (rand() % 6) + 1;
        arr[j] = a;
      }
    }
  }
  printf("Rolling!\n");
  showRoll(arr);
  return;
}

// Sorts dice in descending order for easy recognition. Also runs internally in
// rollDice()
void sortDescending(int arr[N_DICE]) {
  int aux = 0;
  for (int i = 0; i < N_DICE; i++) {
    for (int k = 0; k < N_DICE - 1; k++) {
      if (arr[k + 1] > arr[k]) {
        aux = arr[k];
        arr[k] = arr[k + 1];
        arr[k + 1] = aux;
      }
    }
  }
}

// Shows dice results, runs internally in rollDice()
void showRoll(int arr[N_DICE]) {
  for (int i = 0; i < N_DICE; i++) {
    printf("|%d| ", arr[i]);
  }
  printf("\n");
}

// Adds scores to each player in their respective array
void addScore(int scores[], int dice[], int frequencies[], int firstGenerala) {
  // This array is used to track which scores have been recorded and which
  // haven't (independent of value) It's checked by referencing through compared
  // indices
  int auxArr[TURNS];
  initializeArrays(auxArr, TURNS);
  printf("Scoring Options:\n");

  for (int i = 0; i < VALUES; i++) {
    if (scores[i] == 0) {
      printf("%d) Score %d on %d\n", i + 1, (frequencies[i] * (i + 1)), i + 1);
      auxArr[i] = frequencies[i] * (i + 1);
      if (frequencies[i] == 0) {
        auxArr[i] = -1;
      }
    }
  }

  // Straight
  if (scores[6] == 0) {
    int straight = 0;
    int straightCount = 0;
    for (int i = 0; i < VALUES - 1; i++) {
      if (frequencies[i] == 1) {
        straightCount++;
      }
    }
    if (straightCount == 5) {
      straight = 1;
    } else {
      straightCount = 0;
      for (int i = 1; i < VALUES; i++) {
        if (frequencies[i] == 1) {
          straightCount++;
        }
      }
    }
    if (straightCount == 5) {
      straight = 1;
    }

    if (straight) {
      printf("7) Score Straight\n");
      auxArr[6] = 35;
    } else {
      printf("7) Cross out Straight\n");
      auxArr[6] = -1;
    }
  }

  // Full House
  if (scores[7] == 0) {

    int threeOfAKind = 0, pair = 0;
    for (int i = 0; i < VALUES; i++) {
      if (frequencies[i] == 3) {
        threeOfAKind = 1;
      }
      if (frequencies[i] == 2) {
        pair = 1;
      }
    }

    if (threeOfAKind && pair) {
      printf("8) Score Full House\n");
      auxArr[7] = 50;
    } else {
      printf("8) Cross out Full House\n");
      auxArr[7] = -1;
    }
  }

  // Four of a Kind
  if (scores[8] == 0) {
    int fourOfAKind = 0;
    for (int i = 0; i < VALUES; i++) {
      if (frequencies[i] == 4) {
        fourOfAKind = 1;
      }
    }

    if (fourOfAKind) {
      printf("9) Score Four of a Kind\n");
      auxArr[8] = 75;
    } else {
      printf("9) Cross out Four of a Kind\n");
      auxArr[8] = -1;
    }
  }

  // Generala (Five of a Kind)
  if (scores[9] == 0) {
    int generala = 0;
    for (int i = 0; i < VALUES; i++) {
      if (frequencies[i] == 5) {
        generala = 1;
        firstGenerala = 1;
      }
    }

    if (generala && !firstGenerala) {
      printf("10) Score Generala\n");
      auxArr[9] = 100;
    } else {
      printf("10) Cross out Generala\n");
      auxArr[9] = -1;
    }
  }

  // Double Generala
  if (scores[10] == 0) {

    int generala = 0;
    for (int i = 0; i < VALUES; i++) {
      if (frequencies[i] == 5) {
        generala = 1;
      }
    }

    if (firstGenerala == 1 && generala == 1) {
      printf("11) Score Double Generala\n");
      auxArr[10] = 120;
    } else {
      printf("11) Cross out Double Generala\n");
      auxArr[10] = -1;
    }
  }

  // Optional: Run to show the auxiliary array used for calculation
  // showAuxArr(auxArr);

  int selector = 0;
  printf("\nChoose an option > ");
  scanf("%d", &selector);

  // The value in auxArr[selector - 1] will be 0 if the score was already
  // recorded
  while ((selector > 12 || selector < 0) || auxArr[selector - 1] == 0) {
    printf("Choose a number within the given options range > ");
    scanf("%d", &selector);
  }
  scores[selector - 1] = auxArr[selector - 1];
}

// Counts how many times each die value appeared in the turn, used for detecting
// scoring combinations
void countFrequencies(int dice[], int frequencies[]) {
  for (int i = 0; i < N_DICE; i++) {
    frequencies[dice[i] - 1]++;
  }
}

// Resets score arrays to 0 when restarting or starting the game for the first
// time
void initializeArrays(int arr[], int capacity) {
  for (int i = 0; i < capacity; i++) {
    arr[i] = 0;
  }
}

// Shows each player's scores
void showScore(int score[]) {
  printf("\n----------------\n");
  printf("Scores are:\n");
  for (int i = 0; i < TURNS; i++) {
    if (score[i] == -1) {
      printf("%d) \n", i + 1);
    } else {
      printf("%d) %d\n", i + 1, score[i]);
    }
  }
  printf("\n----------------\n");
}

// Sums the array values to get final score
int getFinalScore(int arr[]) {
  int score = 0;
  for (int i = 0; i < TURNS; i++) {
    if (arr[i] != -1) {
      score += arr[i];
    }
  }
  return score;
}

// Determines performance based on final score. Returns a char* pointer since it
// stores a string.
char *showPerformance(int score) {
  if (score < 0 || score > MAX_POINTS) {
    return "Score out of range";
  }

  if (score >= 0 && score <= 121) {
    return "Poor";
  } else if (score >= 122 && score <= 242) {
    return "Average";
  } else if (score >= 243 && score <= 363) {
    return "Good";
  } else {
    return "Excellent";
  }
}

// Resets the game if the player chooses to
void resetGame(int *t, int timeArr[], int playerCount, int scoreP1[],
               int scoreP2[], int *turnP1, int *turnP2) {
  // Set to -1 because the loop increments activeTurn variable, so it ends up at
  // 0
  *t = -1;
  initializeArrays(scoreP1, TURNS);
  if (playerCount == 2) {
    *turnP1 = 1;
    *turnP2 = 1;
    initializeArrays(scoreP2, TURNS);
  }
  for (int i = 0; i < playerCount; i++) {
    timeArr[i] = 0;
  }
}

// Shows play time for active turn and total accumulated time
void timeLogger(int player, time_t start, int times[]) {
  // Instantiate the final execution moment
  time_t end = time(NULL);
  // Using difftime we calculate the time difference between end and start
  int elapsedTimeSeconds = difftime(end, start);
  // Add to each player's total time
  times[player] += elapsedTimeSeconds;
}

// Optional function to show auxiliary array used for scoring (step by step
// calculation)
void showAuxArr(int arr[]) {
  for (int i = 0; i < TURNS; i++) {
    printf("\nAux array values at this moment %d) %d\n", i + 1, arr[i]);
  }
}
