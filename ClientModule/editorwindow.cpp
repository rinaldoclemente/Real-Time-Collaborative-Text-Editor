#include "editorwindow.h"
#include "ui_editorwindow.h"
#include <QInputDialog>
#include <QLineEdit>
#include <QColorDialog>     //FOR OPEN COLOR PALETTE
#include <QFileDialog>      //FOR OPEN SAVE WITH NAME LOCALLY
#include <QTextStream>      //FOR SAVE THE FILE LOCALLY AND PDF CONVERSION
#include <QMessageBox>
#include <QPrinter>         //FOR PRINTING THE PDF

//CONSTRUCTOR
EditorWindow::EditorWindow(QString text, QWidget *parent): QMainWindow(parent), ui(new Ui::EditorWindow), textname(text){
    ui->setupUi(this);
    ui->DocName->setText(text);
}

//DESTRUCTOR
EditorWindow::~EditorWindow(){
    delete ui;
}

//BUTTON FOR CHANGE TYPE OF TEXT
void EditorWindow::on_buttonGrassetto_clicked(){
    if(ui->buttonGrassetto->isChecked()){
        ui->RealTextEdit->setFontWeight(QFont::Bold);
    }else{
         ui->RealTextEdit->setFontWeight(QFont::Light);
         ui->buttonGrassetto->setCheckable(true);
    }
}

void EditorWindow::on_buttonCorsivo_clicked(){
    if(ui->buttonCorsivo->isChecked()){
        ui->RealTextEdit->setFontItalic(true);
    }else{
         ui->RealTextEdit->setFontItalic(false);
         ui->buttonCorsivo->setCheckable(true);
    }
}

void EditorWindow::on_buttonSottolineato_clicked(){
    if(ui->buttonSottolineato->isChecked()){
        ui->RealTextEdit->setFontUnderline(true);
    }else{
         ui->RealTextEdit->setFontUnderline(false);
         ui->buttonSottolineato->setCheckable(true);
    }
}

//BUTTON FOR CHANGE COLOUR OF TEXT
void EditorWindow::on_buttonEvidenziato_clicked(){
    QColor backColour = QColorDialog::getColor();
    ui->RealTextEdit->setTextBackgroundColor(backColour);
}

void EditorWindow::on_buttonColor_clicked(){
    QColor txtColour = QColorDialog::getColor();
    ui->RealTextEdit->setTextColor(txtColour);
}


//BUTTON FOR ALIGN THE TEXT
void EditorWindow::on_buttonAlignDX_clicked(){
    ui->RealTextEdit->setAlignment(Qt::AlignRight);
}

void EditorWindow::on_buttonAlignCX_clicked(){
    ui->RealTextEdit->setAlignment(Qt::AlignCenter);
}

void EditorWindow::on_buttonAlignSX_clicked(){
    ui->RealTextEdit->setAlignment(Qt::AlignLeft);
}

//TODO JUSTIFY


//BUTTON FOR UNDO AND REDO
void EditorWindow::on_buttonUndo_clicked(){
    ui->RealTextEdit->undo();
}

void EditorWindow::on_buttonRedo_clicked(){
    ui->RealTextEdit->redo();
}


//BUTTON FOR CUT-COPY-PASTE
void EditorWindow::on_buttonTaglia_clicked(){
    ui->RealTextEdit->cut();
}

void EditorWindow::on_buttonIncolla_clicked(){
    ui->RealTextEdit->paste();
}

void EditorWindow::on_buttonCopia_clicked(){
    ui->RealTextEdit->copy();
}


//FUNCTION FOR EXPORT TEXT INTO PDF
void EditorWindow::on_actionSave_triggered(){
    QString pathname;
    QString fileName = QFileDialog::getSaveFileName((QWidget* )0, "Esporta come PDF", QString(), "*.pdf");
    if (QFileInfo(fileName).suffix().isEmpty()) { fileName.append(".pdf"); }
    //QString fileName = QFileDialog::getSaveFileName(this,"Salva il file in locale");
       QFile File (fileName);
       pathname = fileName;
       if(!File.open(QFile::WriteOnly | QFile::Text)){
          //Return if the user cancels or does something unexpected!
          //I Don't like it, I suggest to change it with a try-catch statement
           return;
        }
        else{
          //Read the file
          QTextStream writeData(&File);
          QString fileText = ui->RealTextEdit->toHtml(); //HTML NO PLAINTEXT (DEVO PROVARE IO CON PLAINTEXT MA NON TOCCATE!!!!!!!)

          QTextDocument doc;
          doc.setHtml(fileText);
          QPrinter file;
          file.setOutputFormat(QPrinter::PdfFormat);
          file.setOutputFileName(textname+".pdf"); // better to use full path //GIOVANNI MODIFICO IO
          doc.print(&file); //REFERENCE DO NOT TOUCH IT FUNZIONA!

          writeData << &file; //like CIN, but in a stream of text
          File.flush();
          File.close();
        }
}

//EXIT BUTTON
void EditorWindow::on_buttonExit_clicked(){
    QApplication::exit();   //I've used exit() instead quit() or close() for this reason --> https://ux.stackexchange.com/questions/50893/do-we-exit-quit-or-close-an-application
}

//ACTION TO QUIT THE PROGRAM BY MENù
void EditorWindow::on_actionChiudi_triggered(){
    QApplication::quit();   //I've used quit() instead exit() or close() for this reason --> https://ux.stackexchange.com/questions/50893/do-we-exit-quit-or-close-an-application

}


//RENAME BUTTON v1 - "DEPRECATED FUNCTION --> SEE RENAME BUTTON v2"
void EditorWindow::on_pushButton_3_clicked(){
    bool ok;
        QString newText = QInputDialog::getText(this, tr("Titolo documento"),
                                             tr("Inserisci un nome per il documento:"), QLineEdit::Normal,
                                             textname, &ok);
        if (ok && !newText.isEmpty() && newText.size()<=15){
            //TODO controllo file database (nome e utente)
            //TODO Inserire il file nel database
            //TODO aprire il file nell'editor

            EditorWindow *ew = new EditorWindow(newText);
            ew->show();
            delete this;
        }
        else if (ok && !newText.isEmpty() && newText.size()>15){
            QMessageBox messageBox;
            messageBox.critical(nullptr,"Errore","Inserire un nome minore di 15 caratteri!");
            messageBox.setFixedSize(600,400);
            on_pushButton_3_clicked();
        }
        else if (ok && newText.isEmpty()){
            QMessageBox messageBox;
            messageBox.critical(nullptr,"Errore","Inserire un nome!");
            messageBox.setFixedSize(600,400);
            on_pushButton_3_clicked();
        }
        //AT THE END
}

//RENAME BUTTON v2 - TODO APPLY CONTROL LIKE RENAME BUTTON v1
void EditorWindow::on_renameButton_clicked(){
    QString newText = QInputDialog::getText(this, tr("Titolo documento"),
                                         tr("Inserisci un nome per il documento:"), QLineEdit::Normal,
                                         textname);
    ui->DocName->setText(newText);
}


//BUTTON FONT UP --> THIS FUNCTION HAS TO BE MODIFIED
void EditorWindow::on_buttonFontUp_clicked(){
    QTextCursor cursor = ui->RealTextEdit->textCursor();
    qreal a = ui->RealTextEdit->fontPointSize(); //getFontSize
    if (a<100){
        a++;
        ui->RealTextEdit->setFontPointSize(a);
    }
    ui->RealTextEdit->setTextCursor(cursor);
}

//BUTTON FONT DOWN --> THIS FUNCTION HAS TO BE MODIFIED
void EditorWindow::on_buttonFontDown_clicked(){
    QTextCursor cursor = ui->RealTextEdit->textCursor();
    qreal a = ui->RealTextEdit->fontPointSize(); //getFontSize
    if (a>0){
        a--;
        ui->RealTextEdit->setFontPointSize(a);
    }
    ui->RealTextEdit->setTextCursor(cursor);
}

