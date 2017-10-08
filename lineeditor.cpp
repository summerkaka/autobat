#include "lineeditor.h"

LineEditor::LineEditor(QWidget *parent) :
    QWidget(parent)
{
    line_edit = new QLineEdit(this);
    this->installEventFilter(this);
}

bool LineEditor::eventFilter(QObject *object, QEvent *event)
{
    if(object == line_edit && event->type() == QEvent::MouseButtonPress) {
        is_clicked = true;
        printf("is clicked");
    }

    return false;
}
