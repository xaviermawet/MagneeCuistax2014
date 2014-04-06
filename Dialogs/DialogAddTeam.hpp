#ifndef __DIALOGADDTEAM_HPP__
#define __DIALOGADDTEAM_HPP__

#include <QtGlobal>

#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
    #include <QtWidgets>
#else
    #include <QtGui>
#endif

namespace Ui {
class DialogAddTeam;
}

class DialogAddTeam : public QDialog
{
    Q_OBJECT

    public:

        /*!
         * \brief DialogAddTeam : default constructor
         * \param parent : pointer to a parent widget if exists
         */
        explicit DialogAddTeam(QWidget* parent = 0);

        /*!
         * \brief ~DialogAddTeam : virtual destructor
         */
        virtual ~DialogAddTeam(void);

    private:

        Ui::DialogAddTeam *ui;
};

#endif /* __DIALOGADDTEAM_HPP__ */
