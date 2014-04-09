#include "Lap.hpp"

Lap::Lap(int lapNumber, const QTime& lastTime) :
    _lapNumber(lapNumber), _elapsedTime(lastTime)
{
    // Nothing to do here
}

void Lap::setLapNumber(int lapNumber)
{
    this->_lapNumber = lapNumber;
}

void Lap::addLaps(int numberOfLaps)
{
    this->_lapNumber += numberOfLaps;
}

void Lap::setTime(QTime elapsedTime)
{
    this->_elapsedTime = elapsedTime;
}

int Lap::lapNumber(void) const
{
    return this->_lapNumber;
}

QTime Lap::elapsedTime(void) const
{
    return this->_elapsedTime;
}
