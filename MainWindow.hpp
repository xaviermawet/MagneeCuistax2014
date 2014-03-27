#ifndef __MAINWINDOW_HPP__
#define __MAINWINDOW_HPP__

#include <QtGlobal>

#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
    #include <QtWidgets>
#else
    #include <QtGui>
#endif

#define QSETTINGS_MAINWINDOW_KEYWORD "MainWindow"

namespace Ui {
    class MainWindow;
}

/*!
 * \brief The MainWindow class
 */
class MainWindow : public QMainWindow
{
        Q_OBJECT

    public:

        /*!
         * \brief MainWindow : default constructor
         * \param parent : pointer to a parent MainWindow if exists
         */
        explicit MainWindow(QWidget* parent = 0);

        /*!
         * \brief ~MainWindow : virtual destructor
         */
        virtual ~MainWindow(void);

    protected:

        /*!
         * \brief centerOnScreen : center the MainWindow
         */
        void centerOnScreen(void);

        /*!
         * \brief readSettings : restore all previous MainWindow settings
         */
        void readSettings(void);

        /*!
         * \brief writeSettings : save all current MainWindow settings
         */
        void writeSettings(void) const;

        /*!
         * \brief readLayoutSettings : restore previous MainWindow layout settings
         * \param settingsGroup : name of the goup for layout settings
         */
        void readLayoutSettings(const QString& settingsGroup);

        /*!
         * \brief writeLayoutSettings : save current MainWindow layout settings
         * \param settingsGroup : name of the goup for layout settings
         */
        void writeLayoutSettings(const QString& settingsGroup) const;

        /*!
         * \brief closeEvent : overloaded method for writing settings
         */
        virtual void closeEvent(QCloseEvent* event);

    protected:

        Ui::MainWindow *ui;
};

#endif /* __MAINWINDOW_HPP__ */
