#include "movedialog.h"
#include "ui_movedialog.h"
#include "Subtitles/subtitleprocessor.h"
#include "Subtitles/subpicture.h"

#include <QIntValidator>
#include <QDoubleValidator>

MoveDialog::MoveDialog(QWidget *parent, SubtitleProcessor* subtitleProcessor) :
    QDialog(parent),
    ui(new Ui::MoveDialog),
    cropOfsY(subtitleProcessor->getCropOfsY()),
    offsetX(subtitleProcessor->getMoveOffsetX()),
    offsetY(subtitleProcessor->getMoveOffsetY()),
    moveModeX(subtitleProcessor->getMoveModeX()),
    moveModeY(subtitleProcessor->getMoveModeY())
{
    ui->setupUi(this);
    setWindowFlags(Qt::Dialog | Qt::CustomizeWindowHint | Qt::WindowTitleHint);

    okBackground = new QPalette(ui->aspectRatioLineEdit->palette());
    errorBackground = new QPalette();
    errorBackground->setColor(QPalette::Base, QColor(0xffe1acac));

    xButtonGroup = new QButtonGroup(this);
    yButtonGroup = new QButtonGroup(this);

    xButtonGroup->addButton(ui->keepXPositionRadioButton);
    xButtonGroup->addButton(ui->moveLeftRadioButton);
    xButtonGroup->addButton(ui->moveRightRadioButton);
    xButtonGroup->addButton(ui->moveToCenterRadioButton);

    yButtonGroup->addButton(ui->keepYPositionRadioButton);
    yButtonGroup->addButton(ui->moveInsideBoundsRadioButton);
    yButtonGroup->addButton(ui->moveOutsideBoundsRadioButton);

    this->subtitleProcessor = subtitleProcessor;

    switch((int)moveModeX)
    {
    case (int)MoveModeX::KEEP:
    {
        ui->keepXPositionRadioButton->setChecked(true);
    } break;
    case (int)MoveModeX::LEFT:
    {
        ui->moveLeftRadioButton->setChecked(true);
    } break;
    case (int)MoveModeX::RIGHT:
    {
        ui->moveRightRadioButton->setChecked(true);
    } break;
    case (int)MoveModeX::CENTER:
    {
        ui->moveToCenterRadioButton->setChecked(true);
    } break;
    }
    switch((int)moveModeY)
    {
    case (int)MoveModeY::KEEP:
    {
        ui->keepYPositionRadioButton->setChecked(true);
    } break;
    case (int)MoveModeY::INSIDE:
    {
        ui->moveInsideBoundsRadioButton->setChecked(true);
    } break;
    case (int)MoveModeY::OUTSIDE:
    {
        ui->moveOutsideBoundsRadioButton->setChecked(true);
    } break;
    }

    xOffsetValidator = new QIntValidator;
    ui->xOffsetLineEdit->setValidator(xOffsetValidator);
    yOffsetValiator = new QIntValidator;
    ui->yOffsetLineEdit->setValidator(yOffsetValiator);
    cropOffsetValidator = new QIntValidator;
    ui->cropOffsetYLineEdit->setValidator(cropOffsetValidator);
    aspectRatioValidator = new QDoubleValidator;
    ui->aspectRatioLineEdit->setValidator(aspectRatioValidator);

    ui->aspectRatioLineEdit->blockSignals(true);
    ui->xOffsetLineEdit->blockSignals(true);
    ui->yOffsetLineEdit->blockSignals(true);
    ui->cropOffsetYLineEdit->blockSignals(true);

    ui->aspectRatioLineEdit->setText(QString::number(screenRatioTrg, 'g', 6));
    ui->xOffsetLineEdit->setText(QString::number(offsetX));
    ui->yOffsetLineEdit->setText(QString::number(offsetY));
    ui->cropOffsetYLineEdit->setText(QString::number(cropOfsY));

    ui->aspectRatioLineEdit->blockSignals(false);
    ui->xOffsetLineEdit->blockSignals(false);
    ui->yOffsetLineEdit->blockSignals(false);
    ui->cropOffsetYLineEdit->blockSignals(false);
}

MoveDialog::~MoveDialog()
{
    delete ui;
}

void MoveDialog::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Return || event->key() == Qt::Key_Enter)
    {
        if (ui->xOffsetLineEdit->hasFocus())
        {
            on_xOffsetLineEdit_editingFinished();
        }
        if (ui->yOffsetLineEdit->hasFocus())
        {
            on_yOffsetLineEdit_editingFinished();
        }
        if (ui->aspectRatioLineEdit->hasFocus())
        {
            on_aspectRatioLineEdit_editingFinished();
        }
        if (ui->cropOffsetYLineEdit->hasFocus())
        {
            on_cropOffsetYLineEdit_editingFinished();
        }
    }
    QDialog::keyPressEvent(event);
}

void MoveDialog::setIndex(int idx)
{
    index = idx;

    subtitleProcessor->convertSup(idx, idx + 1, subtitleProcessor->getNumberOfFrames());
    subPicture = new SubPicture(subtitleProcessor->getSubPictureTrg(idx));
    image = subtitleProcessor->getTrgImagePatched(subPicture);

    originalX = subPicture->getOfsX();
    originalY = subPicture->getOfsY();

    ui->infoLabel->setText(QString("Frame %1 of %2").arg(QString::number(idx + 1))
                           .arg(QString::number(subtitleProcessor->getNumberOfFrames())));
    move();
    ui->subtitleImage->setOffsets(subPicture->getOfsX(), subPicture->getOfsY());
    ui->subtitleImage->setDimension(subPicture->width, subPicture->height);
    ui->subtitleImage->setImage(image, subPicture->getImageWidth(), subPicture->getImageHeight());
    ui->subtitleImage->setScreenRatio(screenRatioTrg);
    ui->subtitleImage->setCropOfsY(cropOfsY);
    ui->subtitleImage->setExcluded(subPicture->exclude);
    ui->subtitleImage->update();
}

void MoveDialog::on_previousButton_clicked()
{
    if (index > 0)
    {
        setIndex(index - 1);
    }
}

void MoveDialog::on_nextButton_clicked()
{
    if (index < subtitleProcessor->getNumberOfFrames() - 1)
    {
        setIndex(index + 1);
    }
}

void MoveDialog::on_keepXPositionRadioButton_clicked()
{
    moveModeX = MoveModeX::KEEP;
    subPicture->setOfsX(originalX);
    setRatio(screenRatioTrg);
}

void MoveDialog::on_moveLeftRadioButton_clicked()
{
    moveModeX = MoveModeX::LEFT;
    setRatio(screenRatioTrg);
}

void MoveDialog::on_moveRightRadioButton_clicked()
{
    moveModeX = MoveModeX::RIGHT;
    setRatio(screenRatioTrg);
}

void MoveDialog::on_moveToCenterRadioButton_clicked()
{
    moveModeX = MoveModeX::CENTER;
    setRatio(screenRatioTrg);
}

void MoveDialog::on_keepYPositionRadioButton_clicked()
{
    moveModeY = MoveModeY::KEEP;
    subPicture->setOfsY(originalY);
    setRatio(screenRatioTrg);
}

void MoveDialog::on_moveInsideBoundsRadioButton_clicked()
{
    moveModeY = MoveModeY::INSIDE;
    setRatio(screenRatioTrg);
}

void MoveDialog::on_moveOutsideBoundsRadioButton_clicked()
{
    moveModeY = MoveModeY::OUTSIDE;
    setRatio(screenRatioTrg);
}

void MoveDialog::on_xOffsetLineEdit_editingFinished()
{
    int x = ui->xOffsetLineEdit->text().toInt();

    if (x < 0)
    {
        x = 0;
    }
    else if (x > subPicture->width / 3)
    {
        x = subPicture->width / 3;
    }

    if (x != offsetX)
    {
        offsetX = x;
        setRatio(screenRatioTrg);
    }
    ui->xOffsetLineEdit->setText(QString::number(offsetX));
}

void MoveDialog::on_xOffsetLineEdit_textChanged(const QString &arg1)
{
    int x = arg1.toInt();

    if (x < 0 || x > subPicture->width / 3)
    {
        ui->xOffsetLineEdit->setPalette(*errorBackground);
    }
    else
    {
        if (x != offsetX)
        {
            offsetX = x;
            setRatio(screenRatioTrg);
        }
        ui->xOffsetLineEdit->setPalette(*okBackground);
    }
}

void MoveDialog::on_yOffsetLineEdit_editingFinished()
{
    int y = ui->yOffsetLineEdit->text().toInt();

    if (y < 0)
    {
        y = 0;
    }
    else if (y > subPicture->height / 3)
    {
        y = subPicture->height / 3;
    }

    if (y != offsetY)
    {
        offsetY = y;
        setRatio(screenRatioTrg);
    }
    ui->yOffsetLineEdit->setText(QString::number(offsetY));
}

void MoveDialog::on_yOffsetLineEdit_textChanged(const QString &arg1)
{
    int y = arg1.toInt();

    if (y < 0 || y > subPicture->height / 3)
    {
        ui->yOffsetLineEdit->setPalette(*errorBackground);
    }
    else
    {
        if (y != offsetY)
        {
            offsetY = y;
            setRatio(screenRatioTrg);
        }
        ui->yOffsetLineEdit->setPalette(*okBackground);
    }
}

void MoveDialog::on_aspectRatioLineEdit_editingFinished()
{
    double ratio = ui->aspectRatioLineEdit->text().toDouble();

    if (ratio > 4.0)
    {
        ratio = 4.0;
    }
    else if (ratio < screenRatio)
    {
        ratio = screenRatio;
    }
    if (ratio != screenRatioTrg)
    {
        screenRatioTrg = ratio;
        setRatio(screenRatioTrg);
    }
    ui->aspectRatioLineEdit->setText(QString::number(screenRatioTrg, 'g', 6));
}

void MoveDialog::on_aspectRatioLineEdit_textChanged(const QString &arg1)
{
    double ratio = arg1.toDouble();

    if (ratio < screenRatio || ratio > 4.0)
    {
        ui->aspectRatioLineEdit->setPalette(*errorBackground);
    }
    else
    {
        if (QString::number(ratio, 'g', 6) == QString::number(screenRatioTrg, 'g', 6))
        {
            screenRatioTrg = ratio;
            setRatio(screenRatioTrg);
        }
        ui->aspectRatioLineEdit->setPalette(*okBackground);
    }
}

void MoveDialog::on_aspectRatio1Button_clicked()
{
    setRatio(21.0 / 9);
}

void MoveDialog::on_aspectRatio2Button_clicked()
{
    setRatio(2.35);
}

void MoveDialog::on_aspectRatio3Button_clicked()
{
    setRatio(2.4);
}

void MoveDialog::on_cropOffsetYLineEdit_editingFinished()
{
    int y = ui->cropOffsetYLineEdit->text().toInt();

    if (y < 0)
    {
        y = 0;
    }
    else if (y > subPicture->height / 3)
    {
        y = subPicture->height / 3;
    }

    if (y != cropOfsY)
    {
        cropOfsY = y;
        ui->subtitleImage->setCropOfsY(cropOfsY);
        setRatio(screenRatioTrg);
    }
    ui->cropOffsetYLineEdit->setText(QString::number(cropOfsY));
}

void MoveDialog::on_cropOffsetYLineEdit_textChanged(const QString &arg1)
{
    int y = arg1.toInt();

    if (y < 0 || y > subPicture->height / 3)
    {
        ui->cropOffsetYLineEdit->setPalette(*errorBackground);
    }
    else
    {
        if (y != cropOfsY)
        {
            cropOfsY = y;
            ui->subtitleImage->setCropOfsY(cropOfsY);
            setRatio(screenRatioTrg);
        }
        ui->cropOffsetYLineEdit->setPalette(*okBackground);
    }
}

void MoveDialog::on_cropBarsButton_clicked()
{
    cropOfsY = (int)((subPicture->height * cineBarFactor) + .5);
    ui->subtitleImage->setCropOfsY(cropOfsY);
    setRatio(screenRatioTrg);
    ui->cropOffsetYLineEdit->setText(QString::number(cropOfsY));
}

void MoveDialog::on_cancelButton_clicked()
{
    reject();
}

void MoveDialog::on_moveAllButton_clicked()
{
    subtitleProcessor->setCropOfsY(cropOfsY);
    subtitleProcessor->setMoveModeX(moveModeX);
    subtitleProcessor->setMoveModeY(moveModeY);
    subtitleProcessor->setMoveOffsetX(offsetX);
    subtitleProcessor->setMoveOffsetY(offsetY);
    subtitleProcessor->setCineBarFactor(cineBarFactor);

    accept();
}

void MoveDialog::setRatio(double ratio)
{
    if (QString::number(screenRatioTrg, 'g', 6) == QString::number(ratio, 'g', 6))
    {
        ui->aspectRatioLineEdit->setText(QString::number(ratio, 'g', 6));
    }
    screenRatioTrg = ratio;
    cineBarFactor = (1.0 - (screenRatio / screenRatioTrg)) / 2.0;
    move();
    ui->subtitleImage->setScreenRatio(screenRatioTrg);
    ui->subtitleImage->setOffsets(subPicture->getOfsX(), subPicture->getOfsY());
    ui->subtitleImage->update();
}

void MoveDialog::move()
{
    subtitleProcessor->moveToBounds(subPicture, index + 1, cineBarFactor, offsetX, offsetY, moveModeX, moveModeY, cropOfsY);
}
