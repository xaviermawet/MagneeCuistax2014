#ifndef __DIALOGSETTINGS_HPP__
#define __DIALOGSETTINGS_HPP__

#include <QtGlobal>

#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
    #include <QtWidgets>
#else
    #include <QtGui>
#endif

#include "Common.hpp"

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

        /* Setters */
        void setNumberOfLaps(int numberOfLaps = 10);
        void setReloadPreviousLaps(bool reload = true);
        void setBackUpAndRestoreApplicationState(bool backUpAndRestore = true);
        void setOptionalFields(QList<RankingOptionalField> const& optionalFields);
        void setDataViewerGroupBoxVisible(bool visible);

        /* Getters */
        int  numberOfLaps(void) const;
        bool isReloadPreviousLapsChecked(void) const;
        bool isBackUpAndRestoreApplicationStateChecked(void) const;
        QList<RankingOptionalField> optionalFieldsSelected(void) const;

    signals:

        void fontSizeChange(int fontSize);

    protected slots:

        // Autoconnect
        void on_listWidgetRankingColumns_currentRowChanged(int currentRow);
        void on_listWidgetRankingColumns_itemActivated(QListWidgetItem* item);
        void on_pushButtonUP_clicked(void);
        void on_pushButtonDown_clicked(void);
        void on_spinBoxFontSize_valueChanged(int fontSize);

    protected:

        int itemIndexFor(RankingOptionalField const& field);

    protected:

        Ui::DialogSettings* ui;
};

#endif /* __DIALOGSETTINGS_HPP__ */
