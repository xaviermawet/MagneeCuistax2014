#ifndef __DATAVIEWER_HPP__
#define __DATAVIEWER_HPP__

#include <QtGlobal>

#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
    #include <QtWidgets>
#else
    #include <QtGui>
#endif

// Personal Widgets
#include "Widgets/NStopWatch.hpp"

namespace Ui {
    class DataViewer;
}

class DataViewer : public QWidget
{
        Q_OBJECT

    public:

        /*!
         * \brief DataViewer : Default constructor
         * \param parent : Pointer to a parent widget if exists
         * \param flag : sets the window flags
         */
        explicit DataViewer(QWidget* parent = 0,
                            Qt::WindowFlags flag = Qt::Window);

        void setTableViewModel(QAbstractItemModel* model);
        void setRaceTitle(QString const& title);

        /*!
         * \brief ~DataViewer : Virtual destructor
         */
        virtual ~DataViewer(void);

    signals:

        void closed(void);

    public slots:

        void startStopWatch(void);
        void stopStopWatch(void);
        void resetStopWatch(void);
        void setFontSize(int fontSize);

    protected:

        void createHeader(void);

        /*!
         * \brief closeEvent : overloaded method for writing settings
         */
        virtual void closeEvent(QCloseEvent* event);

    protected:

        // Widgets
        Ui::DataViewer* ui;
        NStopWatch*     _stopWatch;
};

#endif /* __DATAVIEWER_HPP__ */
