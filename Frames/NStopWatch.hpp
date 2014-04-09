#ifndef __NSTOPWATCH_HPP__
#define __NSTOPWATCH_HPP__

#include <QtGlobal>

#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
    #include <QtWidgets>
#else
    #include <QtGui>
#endif

namespace Ui {
    class NStopWatch;
}

class NStopWatch : public QFrame
{
        Q_OBJECT

    public:

        explicit NStopWatch(QWidget* parent = 0);
        virtual ~NStopWatch(void);

        QTime elapsedTime(void) const;

        bool isActive(void) const;

    public slots:

        void start(void);
        void stop(void);
        void reset(void);

    private slots:

        void refreshWatch(void);

    signals:

        void started(void);
        void stopped(QTime const& time);
        void reseted(void);

    private:

        Ui::NStopWatch *ui;

        bool    _pause;
        QTime   _watchTime;
        QTime   _startingPauseTime;
        QTimer* _refreshTimer;
};

#endif /* __NSTOPWATCH_HPP__ */
