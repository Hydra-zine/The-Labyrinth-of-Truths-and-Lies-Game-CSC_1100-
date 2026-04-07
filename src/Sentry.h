#ifndef SENTRY_H
#define SENTRY_H

#include <string>
#include <vector>

class Sentry {
public:
    bool isTruthful;
    std::vector<std::string> questionSet;

    Sentry(std::vector<std::string> questions);

    bool getResponse(bool actualAnswer);

    std::string getRandomQuestion();
};

#endif
