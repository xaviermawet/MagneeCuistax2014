#ifndef __LAP_HPP__
#define __LAP_HPP__

#include <QtGlobal>

#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
    #include <QtWidgets>
#else
    #include <QtGui>
#endif

class Lap
{
    public:

        explicit Lap(int lapNumber = 0, QTime const& lastTime = QTime(0, 0));

        // SETTER

        /*!
         * \brief setLapNumber : change the lap number
         * \param lapNumber : the new lap number
         */
        void setLapNumber(int lapNumber);

        /*!
         * \brief addLaps : increase the current lap number
         * \param numberOfLaps : the number of laps
         */
        void addLaps(int numberOfLaps);

        /*!
         * \brief setTime : change the elapsed time for the lap
         * \param elapsedTime : the new elapsed time
         */
        void setTime(QTime elapsedTime);

        // GETTER

        /*!
         * \brief lapNumber : get the current lap number
         * \return the lap number
         */
        int lapNumber(void) const;

        /*!
         * \brief elapsedTime : get the elapsed time for the current lap
         * \return the elapsed time
         */
        QTime elapsedTime(void) const;

    protected:

        int _lapNumber;
        QTime _elapsedTime;
};

#endif /* __LAP_HPP__ */
