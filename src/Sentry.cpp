#include "Sentry.h"
#include <cstdlib>


//not setting up srand((unsigned int)time(NULL)); here since in a class it would be called multiple times

Sentry::Sentry(std::vector<std::string> questions) {
    questionSet = questions;
    //sentries and their integers. The reason I am using integers is because
    //I will have the sentry integer land on either 0 or 1 to determine if it lies or not.
    //A 0 will mean its a lie, a 1 means it is a truth. 
    int flip = rand() % 2;
    isTruthful = (flip == 1);
}

bool Sentry::getResponse(bool actualAnswer) {
  if(isTruthful == actualAnswer) return true;
  return false;
}

std::string Sentry::getRandomQuestion() {
    int i = rand() % questionSet.size();
    return questionSet[i];
}
