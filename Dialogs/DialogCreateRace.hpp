#ifndef __DIALOGCREATERACE_HPP__
#define __DIALOGCREATERACE_HPP__

#include <QtGlobal>

#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
    #include <QtWidgets>
#else
    #include <QtGui>
#endif

namespace Ui {
    class DialogCreateRace;
}

class DialogCreateRace : public QDialog
{
        Q_OBJECT

    public:

        /*!
         * \brief DialogCreateRace : default constructor
         * \param parent : pointer to a parent widget if exists
         */
        explicit DialogCreateRace(QWidget* parent = 0);

        /*!
         * \brief ~DialogCreateRace : virtual destructor
         */
        virtual ~DialogCreateRace(void);

    private:

        Ui::DialogCreateRace* ui;
};

#endif /* __DIALOGCREATERACE_HPP__ */
