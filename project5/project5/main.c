//
//  main.c
//  yahtzee
//
//  Created by mac on 12/11/19.
//  Copyright © 2019 mac. All rights reserved.
//

#include <time.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>


int randome()
{
//    void *num;
//    int dev_dice;
//
//    dev_dice = open("/dev/dice", O_RDONLY);
//    read(dev_dice, num, 1);
//    close(dev_dice);
    
    int rand_int = rand()%(6 -1 +1)+1;
    return rand_int;
}

int calculate_upper(int choice, int *dices, int *scoreBoard){
    int points = 0;
    int i;
    if (scoreBoard[choice - 1] >= 0) {
        return 0;
    }
    
    for (i = 0; i < 5; i++) {
        if (dices[i] == choice) {
            points += choice;
        }
    }
    scoreBoard[choice - 1] = points;
    return 1;
}

char check_of_a_kind(int number, int *dices, int *scoreBoard) {
    int index = 0;
    int points = 0;
    int count = 0;
    int i, j;
    
    if (number == 3) {
        index = 6;
    } else if (number == 4){
        index = 7;
    } else {
        index = 11;
    }
    if (scoreBoard[index] >= 0) {
        return 0;
    }
    
    for (i = 1; i <= 6; i++) {
        for (j = 0; j < 5; j++) {
            if (dices[j] == i) {
                count++;
            }
        }
        if (count >= number) {
            for (j = 0; j < 5; j++) {
                points += dices[j];
            }
            if (number == 5){
                points = 50;
            }
            scoreBoard[index] = points;
            return 1;
        }
        count = 0;
    }

    scoreBoard[index] = 0;
    return 1;
}


int comparator(const void *a, const void *b) {
    return (*(int *)a - *(int *)b);            // Returns -1, 0, or 1
}

int check_full_house(int *dices, int *scoreBoard) {
    int points = 0;
    int i = 0;
    int count1 = 0;
    int count2 = 0;
    
    if (scoreBoard[8] >= 0) {
        return 0;
    }
    
    qsort(dices, 5, sizeof(int), comparator);
    
    for (i = 0; i < 3; i++){
        if (dices[0] == dices[i]) {
            count1++;
        }
    }
    for (i = 4; i > 2; i--){
        if (dices[4] == dices[i]) {
            count2++;
        }
    }
    
    if (count1 + count2 == 5) {
        points = 25;
    }
    
    scoreBoard[8] = points;
    return 1;
}


char check_straight(int choice, int *dices, int *scoreBoard) {
    int points = 0;
    int i, j;
    int straight = 1;
    int repeat = 0;
    int index;
    int length;
    
    if (choice == 4){
        index = 9;
        length = 3;
    } else {
        index = 10;
        length = 4;
    }
    
    if (scoreBoard[index] >= 0) {
        return 0;
    }
    
    qsort(dices, 5, sizeof(int), comparator);
    
    for (i = 0; i < 2; i++) {
        straight = 1;
        repeat = 0;
        
        for (j = 0; j < length; j++) {
            if ((dices[j] - dices[j + 1]) != -1) {
                if (dices[j] == dices[j + 1] && !repeat) {
                    repeat = 1;
                    continue;
                } else {
                    straight = 0;
                    break;
                }
            }
        }
    }
    
    if (straight) {
        if (choice == 4){
            points = 30;
        } else {
            points = 40;
        }
        
    }
    scoreBoard[index] = points;        // Saves the points in the player's scorecard
    return 1;                // Returns successful
}

void print_scores(int rounds, int *scoreBoard){
    int i;
    int total = 0;
    int bonus = 0;
    
    for (i = 0; i < 13; i++) {        // Calculates the player's total score
        if (scoreBoard[i] > 0) {
            total += scoreBoard[i];
        }
    }
    if(scoreBoard[0]+scoreBoard[1]+scoreBoard[2]+scoreBoard[3]+scoreBoard[4]+scoreBoard[5] >= 63) {
        bonus += 35;
    } else {
        bonus = 0;
    }
    
    printf("\nYour score so far is: %d\n\n", total);
    // Ones and Twos
    if (scoreBoard[0] >= 0){
        printf("Ones: %d \t\t\t\t ", scoreBoard[0]);
    } else {
        printf("Ones:   \t\t\t\t ");
    }
    if (scoreBoard[1] >= 0){
        printf("Twos: %d\n",scoreBoard[1]);
    } else {
        printf("Twos:  \n");
    }
    
    // Threes and Fours
    if (scoreBoard[2] >= 0){
        printf("Threes: %d \t\t\t\t ", scoreBoard[2]);
    } else {
        printf("Threes:   \t\t\t\t ");
    }
    if (scoreBoard[3] >= 0){
        printf("Fours: %d\n",scoreBoard[3]);
    } else {
        printf("Fours:  \n");
    }
    
    // Fives and Sixs
    if (scoreBoard[4] >= 0){
        printf("Fives: %d \t\t\t\t ", scoreBoard[4]);
    } else {
        printf("Fives:   \t\t\t\t ");
    }
    if (scoreBoard[5] >= 0){
        printf("Sixes: %d\n",scoreBoard[5]);
    } else {
        printf("Sixes:  \n");
    }
    
    // Bonus
    if (bonus >= 0){
        printf("Upper Section Bons: %d\n", bonus);
    } else {
        printf("Upper Section Bons: 0\n");
    }

    // Three of a Kind and Four of a Kind
    if (scoreBoard[6] >= 0){
        printf("Three of a Kind: %d \t\t ", scoreBoard[6]);
    } else {
        printf("Three of a Kind:  \t\t ");
    }
    if (scoreBoard[7] >= 0){
        printf("Four of a Kind: %d\n",scoreBoard[7]);
    } else {
        printf("Four of a Kind:  \n");
    }
    
    // Small Straight and Large Straight
    if (scoreBoard[8] >= 0){
        printf("Small Straight: %d \t\t ", scoreBoard[8]);
    } else {
        printf("Small Straight:   \t\t ");
    }
    if (scoreBoard[9] >= 0){
        printf("Large Straight: %d\n",scoreBoard[9]);
    } else {
        printf("Large Straight:  \n");
    }
    
    // Full Hose and Yahtzee
    if (scoreBoard[10] >= 0){
        printf("Full Hose: %d \t\t\t ", scoreBoard[10]);
    } else {
        printf("Full Hose:   \t\t\t ");
    }
    if (scoreBoard[11] >= 0){
        printf("Yahtzee: %d\n",scoreBoard[11]);
    } else {
        printf("Yahtzee:  \n");
    }
    
    // Chance
    if (scoreBoard[12] >= 0){
        printf("Chance: %d\n", scoreBoard[12]);
    } else {
        printf("Chance:  \n");
    }

    printf("\n********** round %d end**********\n\n", rounds);
}

int main()
{
    int i, j;
    int scoreBoard[13];
    for (i = 0; i < 13; i++) {
        scoreBoard[i] = -1;
    }

    int dices[5];
    char input[50];
    int total = 0;
    int rounds = 0;

    srand(time(NULL));
    while (rounds != 13)
    {
        printf("********** round %d **********\n\n", rounds);
        printf("Your roll: \n\n");

        for(i = 0; i <= 4; i++) {
            dices[i] = randome();
        }
        for(i = 0; i <= 4; i++){
            printf("%d ", dices[i]);
        }

        int num_of_reroll = 0;
        for (i = 0; i < 2; i++) {
            char input[50];
            printf("\n\nWhich dice to reroll? ");
            fgets(input, 50, stdin);
            char *token = strtok(input, " ");
            while (token != NULL) {
                int index = atoi(token) - 1;
                if (index == -1) {
                    break;
                }
                if (index > 4) {
                    break;
                }
                num_of_reroll += 1;
                dices[index] = randome();
                token = strtok(NULL, " ");
            }
            if (num_of_reroll == 0) {
                break;
            }
            if (i == 0) {
                printf("\nYour second roll:");
            }else{
                printf("\nYour third roll:");
            }
            printf("\n\n");
            for( j=0; j<=4; j++ ){
                printf("%d ", dices[j]);
            }
        }

        // Upper or lower section selection
        printf("\n\nPlace dice into:");
        printf("\n1) Upper Section");
        printf("\n2) Lower Section");
        printf("\n\nSelection? ");
        int selection;
        fgets(input, 50, stdin);
        selection = atoi(input);

        // Upper section
        if(selection == 1){
            printf("\nPlace dice into:");
            printf("\n1) Ones");
            printf("\n2) Twos");
            printf("\n3) Threes");
            printf("\n4) Fours");
            printf("\n5) Fives");
            printf("\n6) Sixes");
            printf("\n\nSelection? ");
            
            int choice;
            fgets(input, 50, stdin);
            choice = atoi(input);
            calculate_upper(choice, dices, scoreBoard);

            
        }

        // Lower section
        if(selection == 2){
            printf("\nPlace dice into:");
            printf("\n1) Three of a Kind");
            printf("\n2) Four of a Kind");
            printf("\n3) Full Hose");
            printf("\n4) Small Straight");
            printf("\n5) large Straight");
            printf("\n6) Yahtzee");
            printf("\n7) Chance");
            printf("\n\nSelection? ");

            int choice;
            fgets(input, 50, stdin);
            choice = atoi(input);
            if(choice == 1){
                check_of_a_kind(3, dices, scoreBoard);
            }
            if(choice == 2){
                check_of_a_kind(4, dices, scoreBoard);
            }
            if(choice == 3){
                check_full_house(dices, scoreBoard);
            }
            if(choice == 4){
                check_straight(4, dices, scoreBoard);
            }
            if(choice == 5){
                check_straight(5, dices, scoreBoard);
            }
            if(choice == 6){
                check_of_a_kind(5, dices, scoreBoard);
            }
            if(choice == 7){
                if (scoreBoard[12] >= 0) {
                    break;
                } else {
                    scoreBoard[12] = 0;
                    for (i = 0; i < 5; i++) {
                        scoreBoard[12] += dices[i];
                    }
                }
            }
        }

        // Get total score every round
        for (i = 0; i < 13; i++) {        // Calculates the player's total score
            if (scoreBoard[i] > 0) {
                total += scoreBoard[i];
            }
        }

        print_scores(rounds, scoreBoard);

        rounds++;
    }
}
