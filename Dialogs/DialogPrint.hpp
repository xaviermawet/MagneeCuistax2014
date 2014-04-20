#ifndef __DIALOGPRINT_HPP__
#define __DIALOGPRINT_HPP__

#include <QtGlobal>

#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
    #include <QtWidgets>
    #include <QPrinter>
#else
    #include <QtGui>
#endif

namespace Ui {
    class DialogPrint;
}

class DialogPrint : public QDialog
{
        Q_OBJECT

    public:
        explicit DialogPrint(QWidget* parent = 0);
        virtual ~DialogPrint(void);

        // Getters
        QPrinter::Orientation orientation(void) const;
        QString title(void) const;
        QString outputFilePath(void) const;

        // Setters
        void setOrientation(QPrinter::Orientation orientation);
        void setTitle(QString const& title);
        void setOutputFilePath(QString const& outputFilePath);

    private slots:

        // Autoconnect
        void on_pushButtonOpenSaveFile_clicked(void);

    private:
        Ui::DialogPrint *ui;
};

#endif /* __DIALOGPRINT_HPP__ */
