#include "ShapeOverlay.h"

#include <QPainter>
#include <QPainterPath>
#include <QPen>
#include <QPolygonF>

#include <cmath>



// Default step-cut profile (upper half, cm).
//   Steps at X = -5,-3,-1,1,3,5 with Y = 0,1,2,3
//   -> 10 cm wide, 6 cm tall (mirrored), top plateau 2 cm.
QVector<QPointF> ShapeOverlay::defaultStepProfileCm()
{
    return {
        { -5.0, 0.0 },
        { -5.0, 1.0 },
        { -3.0, 1.0 },
        { -3.0, 2.0 },
        { -1.0, 2.0 },
        { -1.0, 3.0 },
        {  1.0, 3.0 },
        {  1.0, 2.0 },
        {  3.0, 2.0 },
        {  3.0, 1.0 },
        {  5.0, 1.0 },
        {  5.0, 0.0 },
    };
}

ShapeOverlay::ShapeOverlay(QWidget *parent)
    : QWidget(parent)
    , m_profileCm(defaultStepProfileCm())
{
    setAttribute(Qt::WA_TransparentForMouseEvents);
    setAttribute(Qt::WA_TranslucentBackground);
    setAttribute(Qt::WA_NoSystemBackground);
    setFocusPolicy(Qt::NoFocus);
}

// ------------------------------------------------------------------
void ShapeOverlay::setShapeEnabled(bool enabled)
{
    if (m_enabled == enabled) return;
    m_enabled = enabled;
    update();
}

void ShapeOverlay::setProfilePointsCm(const QVector<QPointF>& pointsCm)
{
    m_profileCm.clear();
    m_profileCm.reserve(pointsCm.size());

    for (const QPointF& p : pointsCm)
    {
        QPointF q = p;
        if (m_snapUm)
        {
            q.setX(snapCm(p.x()));
            q.setY(snapCm(p.y()));
        }
        m_profileCm.append(q);
    }
    update();
}

void ShapeOverlay::resetToDefaultStepProfile()
{
    m_profileCm = defaultStepProfileCm();
    update();
}

void ShapeOverlay::setPixelsPerCm(double px)
{
    const double v = qBound(5.0, px, 300.0);
    if (qFuzzyCompare(v, m_pixelsPerCm)) return;
    m_pixelsPerCm = v;
    update();
}

void ShapeOverlay::setOriginAtCenter(bool center)
{
    if (m_originAtCenter == center) return;
    m_originAtCenter = center;
    update();
}

void ShapeOverlay::setShapeLineWidth(int px)
{
    const int v = qBound(1, px, 10);
    if (v == m_lineWidth) return;
    m_lineWidth = v;
    update();
}

void ShapeOverlay::setFillColor(const QColor& c)
{
    m_fill = c;
    update();
}

void ShapeOverlay::setOutlineColor(const QColor& c)
{
    m_outline = c;
    update();
}

void ShapeOverlay::setMicrometerSnapping(bool on)
{
    m_snapUm = on;
}

// ------------------------------------------------------------------
double ShapeOverlay::snapCm(double cm) const
{
    // 1 µm = 0.0001 cm
    constexpr double kUm = 0.0001;
    return std::round(cm / kUm) * kUm;
}


// ... (defaultStepProfileCm, ctor, all setters unchanged) ...

QVector<QPointF> ShapeOverlay::closedProfileCm() const
{
    QVector<QPointF> path;
    if (m_profileCm.size() < 2) return path;

    path.reserve(m_profileCm.size() * 2);

    // Upper half, left -> right
    for (const QPointF& p : m_profileCm)
        path.append(p);

    // FIX #7: mirror every point back, left-to-right reverse.
    //         Only SKIP the exact ENDPOINTS if they sit on Y=0 (they
    //         already appear in the upper list and their mirror is
    //         themselves). A middle point at Y=0 must be mirrored so
    //         the shape touches the axis at the right X.
    const int n = m_profileCm.size();
    for (int i = n - 1; i >= 0; --i)
    {
        const QPointF& p = m_profileCm[i];
        const bool isEndpoint = (i == 0 || i == n - 1);
        if (isEndpoint && qAbs(p.y()) < 1e-12)
            continue;                   // endpoint on X axis -> already there
        path.append(QPointF(p.x(), -p.y()));
    }
    return path;
}

// ... (infoString, paintEvent unchanged) ...

// ------------------------------------------------------------------
QString ShapeOverlay::infoString() const
{
    if (m_profileCm.size() < 2)
        return QStringLiteral("Shape: --");

    double minX = m_profileCm.first().x();
    double maxX = minX;
    double maxY = 0.0;

    for (const QPointF& p : m_profileCm)
    {
        minX = qMin(minX, p.x());
        maxX = qMax(maxX, p.x());
        maxY = qMax(maxY, qAbs(p.y()));
    }

    const double w   = maxX - minX;
    const double h   = 2.0 * maxY;            // mirrored total height
    const double wUm = w * 10000.0;
    const double hUm = h * 10000.0;

    return QStringLiteral("Step shape: %1 × %2 cm  (%3 × %4 µm)")
        .arg(w,   0, 'f', 4)
        .arg(h,   0, 'f', 4)
        .arg(wUm, 0, 'f', 1)
        .arg(hUm, 0, 'f', 1);
}

// ------------------------------------------------------------------
void ShapeOverlay::paintEvent(QPaintEvent *)
{
    if (!m_enabled || m_profileCm.size() < 2)
        return;

    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing, true);

    const QPointF origin = m_originAtCenter
                               ? QPointF(width()  * 0.5,
                                         height() * 0.5)
                               : QPointF(0.0, 0.0);

    // cm -> widget coordinates (Y inverted: +cm_y = upward on screen)
    QPolygonF poly;
    const QVector<QPointF> closed = closedProfileCm();
    poly.reserve(closed.size());

    for (const QPointF& pt : closed)
    {
        const double wx = origin.x() + pt.x() * m_pixelsPerCm;
        const double wy = origin.y() - pt.y() * m_pixelsPerCm;
        poly.append(QPointF(wx, wy));
    }

    QPainterPath path;
    path.addPolygon(poly);
    path.closeSubpath();

    p.fillPath(path, m_fill);

    QPen pen(m_outline);
    pen.setWidth(m_lineWidth);
    pen.setJoinStyle(Qt::MiterJoin);
    p.setPen(pen);
    p.setBrush(Qt::NoBrush);
    p.drawPath(path);

    // Tiny origin marker (yellow)
    p.setPen(Qt::NoPen);
    p.setBrush(QColor(255, 255, 0, 220));
    p.drawEllipse(origin, 3.5, 3.5);
}