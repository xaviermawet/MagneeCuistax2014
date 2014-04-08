#ifndef __DIALOGCREATERACE_HPP__
#define __DIALOGCREATERACE_HPP__

#include <QtGlobal>

#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
    #include <QtWidgets>
#else
    #include <QtGui>
#endif

#include "Utils/NException.hpp"
#include "DataBase/DataBaseManager.hpp"

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

    protected:

        /*!
         * \brief updateSaveButtonState : Hide or shows the save button if
         * the name, the place and the distance are all valid
         */
        void updateSaveButtonState(void);

    private slots:

        // Autoconnect
        void on_lineEditName_textEdited(QString const& raceName);
        void on_lineEditPlace_textEdited(QString const& racePlace);
        void on_doubleSpinBoxDistance_valueChanged(double distance);

    private:

        Ui::DialogCreateRace* ui;

        QRegExp _regex;
        bool    _validName;
        bool    _validPlace;
        bool    _validDistance;
};

#endif /* __DIALOGCREATERACE_HPP__ */
