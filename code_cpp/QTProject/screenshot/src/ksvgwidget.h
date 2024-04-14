#ifndef _K_SVG_WIDGET_
#define _K_SVG_WIDGET_

#include <QWidget>
#include <QtSvg/QSvgRenderer>

QT_BEGIN_NAMESPACE
class QWidget;
QT_END_NAMESPACE

class KSvgWidget : public QWidget
{
	Q_OBJECT
public:
	explicit KSvgWidget(QWidget* parent = nullptr);
    KSvgWidget(const QString& icon, QWidget* parent = nullptr);
	~KSvgWidget();

	QSvgRenderer* renderer() const;

	QSize sizeHint() const override;
public Q_SLOTS:
	void load(const QString& file);
	void load(const QByteArray& contents);
protected:
	void paintEvent(QPaintEvent* event) override;

private:
	QSvgRenderer* m_svgRender = nullptr;
};
#endif