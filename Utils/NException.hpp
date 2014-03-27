#ifndef __NEXCEPTION_HPP__
#define __NEXCEPTION_HPP__

#include <QString>

/*!
 * \brief The NException class is a basic class for exceptions management
 *        Named QException before Qt5, but in framework 5.1 Qt introduces
 *        a new class named QException. For Qt4 compatibility, I don't use
 *        this new class
 */
class NException
{
    public:

        /*!
         * \brief NException : default constructor
         * \param message : error message
         * \param code : error code
         */
        explicit NException(QString const& message, int code = -1);

        /*!
         * \brief NException : copy constructor
         * \param copy : NException object to copy
         */
        NException(NException const& copy);

        /*!
         * \brief assignation operator overload
         * \param copy : NException object to copy
         */
        virtual NException& operator=(NException const& copy);

        /*!
         * \brief ~NException : virtual destructor
         */
        virtual ~NException(void) throw();



        /*!
         * \brief what : getter for error message
         * \return : error message
         */
        virtual QString what(void) const throw();

        /*!
         * \brief code : getter for error code
         * \return : error code
         */
        virtual int code(void) const throw();

    protected:

        QString _message;
        int     _code;
};

#endif /* __NEXCEPTION_HPP__ */
