#ifndef SCREENGRAB_HINT_H
#define SCREENGRAB_HINT_H
#include <QWidget>
#include <QDateTime>
#include <QGraphicsOpacityEffect>
#include <QTimer>
class KScreenGrabHint : public QWidget
{
	Q_OBJECT

public:
	KScreenGrabHint(QWidget *parent, int timeout = -1);
	~KScreenGrabHint();

	void display();
	void setText(const QString& text);

protected:
	void paintEvent(QPaintEvent* event);

private slots:
	void onTimer();

private:
	QDateTime m_startTime;
	QString m_strHintText;
	QGraphicsOpacityEffect* m_effect;
	QTimer* m_timer;
	int m_timeout;
};

class KScreenGrabDoneHint : public QWidget
{
    Q_OBJECT

public:
    KScreenGrabDoneHint(QWidget *parent, int timeout = -1);
    ~KScreenGrabDoneHint();

    void display();
    void setText(const QString& text);

protected:
    void paintEvent(QPaintEvent* event);

private slots:
    void onTimer();

private:
    QDateTime m_startTime;
    QString m_strHintText;
    //QGraphicsOpacityEffect* m_effect;
    QTimer* m_timer;
    int m_timeout;
    QIcon m_icon;
};

#endif // SCREENGRAB_HINT_H
