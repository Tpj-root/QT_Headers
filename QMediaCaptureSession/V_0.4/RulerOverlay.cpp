// ============================================================
// RulerOverlay.cpp
// ============================================================
#include "RulerOverlay.h"

#include <QPainter>
#include <QPen>
#include <QFont>
#include <QRectF>

#include <cmath>

RulerOverlay::RulerOverlay(QWidget *parent)
    : QWidget(parent)
{
    // Let the camera picture show through and never eat mouse events.
    setAttribute(Qt::WA_TransparentForMouseEvents);
    setAttribute(Qt::WA_TranslucentBackground);
    setAttribute(Qt::WA_NoSystemBackground);
    setFocusPolicy(Qt::NoFocus);
}

// ------------------------------------------------------------------
// Setters
// ------------------------------------------------------------------
void RulerOverlay::setGridEnabled(bool enabled)
{
    if (m_gridEnabled == enabled) return;
    m_gridEnabled = enabled;
    update();
}

void RulerOverlay::setRulersEnabled(bool enabled)
{
    if (m_rulersEnabled == enabled) return;
    m_rulersEnabled = enabled;
    update();
}

void RulerOverlay::setPixelsPerCm(double px)
{
    const double v = qBound(5.0, px, 300.0);
    if (qFuzzyCompare(v, m_pixelsPerCm)) return;
    m_pixelsPerCm = v;
    emit pixelsPerCmChanged(m_pixelsPerCm);   // NEW
    update();
}
void RulerOverlay::setXSpacingCm(double cm)
{
    const double v = qBound(0.1, cm, 20.0);
    if (qFuzzyCompare(v, m_xSpacingCm)) return;
    m_xSpacingCm = v;
    update();
}

void RulerOverlay::setYSpacingCm(double cm)
{
    const double v = qBound(0.1, cm, 20.0);
    if (qFuzzyCompare(v, m_ySpacingCm)) return;
    m_ySpacingCm = v;
    update();
}

void RulerOverlay::setGridLineWidth(int px)
{
    const int v = qBound(1, px, 10);
    if (v == m_gridLineWidth) return;
    m_gridLineWidth = v;
    update();
}

void RulerOverlay::setGridColor(const QColor &c)
{
    m_gridColor = c;
    update();
}

void RulerOverlay::setAxisColor(const QColor &c)
{
    m_axisColor = c;
    update();
}

void RulerOverlay::setOriginAtCenter(bool center)
{
    if (m_originAtCenter == center) return;
    m_originAtCenter = center;
    update();
}

// ------------------------------------------------------------------
// Painting
// ------------------------------------------------------------------
void RulerOverlay::paintEvent(QPaintEvent *)
{
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing, true);
    p.setRenderHint(QPainter::TextAntialiasing, true);

    const QPointF origin = m_originAtCenter
                               ? QPointF(width()  * 0.5,
                                         height() * 0.5)
                               : QPointF(0.0, 0.0);

    if (m_gridEnabled)
        drawGrid(p, origin);

    if (m_rulersEnabled)
        drawRulers(p, origin);
}

void RulerOverlay::drawGrid(QPainter &p, const QPointF &origin)
{
    const double stepX = m_pixelsPerCm * m_xSpacingCm;
    const double stepY = m_pixelsPerCm * m_ySpacingCm;

    // Safety: never draw a grid denser than 2 px (would freeze / be useless)
    if (stepX < 2.0 || stepY < 2.0)
        return;

    QPen pen(m_gridColor);
    pen.setWidth(m_gridLineWidth);
    p.setPen(pen);

    // Vertical lines (spacing = X step)
    for (double x = origin.x(); x <= width(); x += stepX)
        p.drawLine(QPointF(x, 0.0), QPointF(x, height()));

    for (double x = origin.x() - stepX; x >= 0.0; x -= stepX)
        p.drawLine(QPointF(x, 0.0), QPointF(x, height()));

    // Horizontal lines (spacing = Y step)
    for (double y = origin.y(); y <= height(); y += stepY)
        p.drawLine(QPointF(0.0, y), QPointF(width(), y));

    for (double y = origin.y() - stepY; y >= 0.0; y -= stepY)
        p.drawLine(QPointF(0.0, y), QPointF(width(), y));
}

void RulerOverlay::drawRulers(QPainter &p, const QPointF &origin)
{
    const double pxPerCm = m_pixelsPerCm;
    if (pxPerCm < 5.0)
        return;

    const double pxPerMm = pxPerCm / 10.0;

    QPen axisPen(m_axisColor);
    axisPen.setWidth(qMax(1, m_gridLineWidth + 1));

    // ---- main axes through the origin ----
    p.setPen(axisPen);
    p.drawLine(QPointF(0.0, origin.y()), QPointF(width(), origin.y()));   // X axis
    p.drawLine(QPointF(origin.x(), 0.0), QPointF(origin.x(), height()));  // Y axis

    QFont font = p.font();
    font.setBold(true);
    font.setPointSize(8);
    p.setFont(font);

    const int  labelEvery = qMax(1, static_cast<int>(std::ceil(48.0 / pxPerCm)));
    const bool showMinor  = (pxPerMm >= 2.5);

    // ----------------------------------------------------------
    // X AXIS : ticks + labels (right = positive, left = negative)
    // ----------------------------------------------------------
    for (int i = 0; ; ++i)
    {
        const double xr = origin.x() + i * pxPerMm;
        const double xl = origin.x() - i * pxPerMm;

        const bool inR = (xr <= width() + 0.5);
        const bool inL = (i > 0) && (xl >= -0.5);
        if (!inR && !inL)
            break;

        const bool isCm   = (i % 10 == 0);
        const bool isHalf = (i % 5  == 0);
        if (!showMinor && !isCm)
            continue;

        const double len = isCm ? 12.0 : (isHalf ? 8.0 : 4.0);

        auto drawTick = [&](double x, int cmValue)
        {
            p.setPen(axisPen);
            p.drawLine(QPointF(x, origin.y()),
                       QPointF(x, origin.y() + len));

            if (isCm && (qAbs(cmValue) % labelEvery == 0))
            {
                p.setPen(m_textColor);
                p.drawText(QRectF(x - 30.0, origin.y() + 15.0, 60.0, 16.0),
                           Qt::AlignHCenter | Qt::AlignTop,
                           QString::number(cmValue));
            }
        };

        if (inR) drawTick(xr,  i / 10);
        if (inL) drawTick(xl, -(i / 10));
    }

    // ----------------------------------------------------------
    // Y AXIS : ticks + labels (up = positive, down = negative)
    // ----------------------------------------------------------
    for (int i = 0; ; ++i)
    {
        const double yDown = origin.y() + i * pxPerMm;
        const double yUp   = origin.y() - i * pxPerMm;

        const bool inDown = (yDown <= height() + 0.5);
        const bool inUp   = (i > 0) && (yUp >= -0.5);
        if (!inDown && !inUp)
            break;

        const bool isCm   = (i % 10 == 0);
        const bool isHalf = (i % 5  == 0);
        if (!showMinor && !isCm)
            continue;

        const double len = isCm ? 12.0 : (isHalf ? 8.0 : 4.0);

        auto drawTick = [&](double y, int cmValue)
        {
            p.setPen(axisPen);
            p.drawLine(QPointF(origin.x() - len, y),
                       QPointF(origin.x(), y));

            if (isCm && cmValue != 0 && (qAbs(cmValue) % labelEvery == 0))
            {
                p.setPen(m_textColor);
                p.drawText(QRectF(origin.x() - 54.0, y - 8.0, 40.0, 16.0),
                           Qt::AlignRight | Qt::AlignVCenter,
                           QString::number(cmValue));
            }
        };

        if (inDown) drawTick(yDown, -(i / 10));
        if (inUp)   drawTick(yUp,    (i / 10));
    }

    // ----------------------------------------------------------
    // Axis titles + scale read-out
    // ----------------------------------------------------------
    font.setPointSize(9);
    p.setFont(font);
    p.setPen(m_textColor);

    p.drawText(QRectF(width() - 80.0, origin.y() + 15.0, 72.0, 16.0),
               Qt::AlignRight | Qt::AlignTop,
               QStringLiteral("X (cm)"));

    p.save();
    p.translate(origin.x() + 18.0, height() - 14.0);
    p.rotate(-90.0);
    p.drawText(QPointF(0.0, 0.0), QStringLiteral("Y (cm)"));
    p.restore();

    p.setPen(QColor(255, 255, 0, 220));
    p.drawText(QRectF(8.0, 6.0, 240.0, 16.0),
               Qt::AlignLeft | Qt::AlignTop,
               QStringLiteral("Scale: %1 px/cm").arg(m_pixelsPerCm, 0, 'f', 1));
}