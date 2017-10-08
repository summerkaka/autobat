#ifndef LINEEDITOR_H
#define LINEEDITOR_H

#include <QWidget>
#include <QLineEdit>
#include <QEvent>

class LineEditor : public QWidget
{
    Q_OBJECT
public:
    explicit LineEditor(QWidget *parent = nullptr);
    bool is_clicked = false;
    QLineEdit *line_edit;

signals:

public slots:

private:
    bool eventFilter(QObject *object, QEvent* event);

};

#endif // LINEEDITOR_H
