#include <QtSvg/QSvgRenderer>
#include <QWidget>
#include <QStyleOption>
#include <QPainter>
#include "ksvgwidget.h"

//////////////////////////////////////////////////////////////////////////
KSvgWidget::KSvgWidget(QWidget* parent /*= nullptr*/)
	: QWidget(parent)
	, m_svgRender(new QSvgRenderer(this))
{
	QObject::connect(m_svgRender, SIGNAL(repaintNeeded()),
		this, SLOT(update()));
}

KSvgWidget::KSvgWidget(const QString& icon, QWidget* parent /*= nullptr*/)
	: QWidget(parent)
	, m_svgRender(new QSvgRenderer(this))
{
	QObject::connect(m_svgRender, SIGNAL(repaintNeeded()),
		this, SLOT(update()));
    m_svgRender->load(icon);
}

KSvgWidget::~KSvgWidget()
{

}

QSvgRenderer* KSvgWidget::renderer() const
{
	return m_svgRender;
}

QSize KSvgWidget::sizeHint() const
{
	if (m_svgRender->isValid())
		return m_svgRender->defaultSize();
	else
		return QSize(128, 64);
}

void KSvgWidget::load(const QByteArray& contents)
{
	m_svgRender->load(contents);
}


void KSvgWidget::load(const QString& file)
{
	m_svgRender->load(file);
}

void KSvgWidget::paintEvent(QPaintEvent* event)
{
	QStyleOption opt;
    opt.initFrom(this);
	QPainter p(this);
	style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
	m_svgRender->render(&p);
}
