#ifndef ERROR_H
#define ERROR_H

enum class Error{
    InvalidUSer,
    UserExists,
    NotTurn,
    NotDone, // A pplayer has still a turn pending
    NotFinished, //The result was not gotten
    InvalidGame
};

#endif // ERROR_H
