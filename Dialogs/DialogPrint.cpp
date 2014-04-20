#include "DialogPrint.hpp"
#include "ui_DialogPrint.h"

DialogPrint::DialogPrint(QWidget* parent) :
    QDialog(parent), ui(new Ui::DialogPrint)
{
    this->ui->setupUi(this);
}

DialogPrint::~DialogPrint()
{
    delete this->ui;
}

QPrinter::Orientation DialogPrint::orientation(void) const
{
    if (this->ui->radioButtonLandscape->isChecked())
        return QPrinter::Landscape;
    return QPrinter::Portrait;
}

QString DialogPrint::title(void) const
{
    return this->ui->lineEditTitle->text();
}

QString DialogPrint::outputFilePath(void) const
{
    return this->ui->lineEditOutPutFile->text();
}

void DialogPrint::setOrientation(QPrinter::Orientation orientation)
{
    if (orientation == QPrinter::Landscape)
        this->ui->radioButtonLandscape->setChecked(true);
    this->ui->radioButtonPortrait->setChecked(true);
}

void DialogPrint::setTitle(const QString &title)
{
    this->ui->lineEditTitle->setText(title);
}

void DialogPrint::setOutputFilePath(const QString &outputFilePath)
{
    this->ui->lineEditOutPutFile->setText(outputFilePath);
}

void DialogPrint::on_pushButtonOpenSaveFile_clicked(void)
{
    QString outputFilePath = QFileDialog::getSaveFileName(
                this, tr("Select file destination"), QDir::homePath(),
                tr("Pdf File(*.pdf)"));
    if (!outputFilePath.isEmpty())
        this->setOutputFilePath(outputFilePath);
}
