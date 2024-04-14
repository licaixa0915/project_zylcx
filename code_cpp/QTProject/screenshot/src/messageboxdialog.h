#ifndef __MESSAGEBOXDIALOG_H__
#define __MESSAGEBOXDIALOG_H__
#include <QWidget>
#include <QDialog>
QT_BEGIN_NAMESPACE
class QLabel;
QT_END_NAMESPACE
class KBoxTitleBar : public QWidget
{
	Q_OBJECT
public:
	explicit KBoxTitleBar(QWidget* parent);
	~KBoxTitleBar();
	void init();

	void setTitleBarColor(const QColor&);
signals:
	void maximize();
	void minimize();
	void close();


protected slots:
	void onSkinChanged();
protected:
	bool event(QEvent* e) override;
	void paintEvent(QPaintEvent* e) override;
private:
	QColor m_titleColor;
};

class KMessageBoxDialog : public QDialog
{
	typedef QDialog Base;
	Q_OBJECT

public:
	explicit KMessageBoxDialog(QWidget *parent = 0);
	~KMessageBoxDialog();

	void setContent(const QString& text);
protected:
	virtual bool event(QEvent*) override;
	virtual bool eventFilter(QObject *, QEvent *) override;
	virtual void paintEvent(QPaintEvent *) override;
	virtual void keyPressEvent(QKeyEvent *) override;

private slots:
	void onSkinChanged();

private:
	QLabel* m_labelText;
	QPushButton* m_btnOk;
};

#endif //__MESSAGEBOXDIALOG_H__