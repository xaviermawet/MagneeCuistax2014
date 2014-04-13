#ifndef __DIALOGSETTINGS_HPP__
#define __DIALOGSETTINGS_HPP__

#include <QtGlobal>

#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
    #include <QtWidgets>
#else
    #include <QtGui>
#endif

namespace Ui {
    class DialogSettings;
}

class DialogSettings : public QDialog
{
        Q_OBJECT

    public:

        /*!
         * \brief DialogSettings : Default constructor
         * \param parent : point to a parent widget if exists
         */
        explicit DialogSettings(QWidget* parent = 0);

        /*!
         * \brief ~DialogSettings : Virtual destructor
         */
        virtual ~DialogSettings(void);

    protected slots:

        void applyChanges(QAbstractButton* buttonApply);

    protected:

        Ui::DialogSettings* ui;
};

#endif /* __DIALOGSETTINGS_HPP__ */
