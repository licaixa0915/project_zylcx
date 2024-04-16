#ifndef MSCREENGRABTIP_H
#define MSCREENGRABTIP_H
#include <QWidget>
class KScreenGrabDialog;
class QLabel;
class KScreenGrabTip : public QWidget
{
    Q_OBJECT

public:
    KScreenGrabTip(KScreenGrabDialog* parent = nullptr);
    void reset(QWidget* btn);

protected:
    virtual bool eventFilter(QObject *, QEvent *) override;

private:
    virtual void paintEvent(QWidget*, QPaintEvent *);

private slots:
    void onChangeSkinMode();

public:
    void setText(const QString&);

private:
    KScreenGrabDialog* m_dlg;
    QWidget* m_main;
    QString m_text;
    QLabel* m_label;
    QLabel* m_title;
};

#endif // MSCREENGRABTIP_H
