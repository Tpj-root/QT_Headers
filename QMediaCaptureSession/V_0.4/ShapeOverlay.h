#pragma once

#include <QWidget>
#include <QVector>
#include <QPointF>
#include <QColor>
#include <QString>

/**
 * Closed measurement shape aligned with the RulerOverlay grid.
 *
 * Coordinate model
 *   - Points are in CENTIMETERS, relative to the origin
 *     (widget center by default; +X right, +Y up).
 *   - Only the UPPER half (Y >= 0) is stored. The paint routine
 *     mirrors it across the X axis, giving a closed, symmetric
 *     contour whose "fixed line" is the X axis.
 *
 * Precision
 *   - Coordinates are `double` in cm -> sub-nanometer resolution.
 *   - Optional 1 µm snapping on setProfilePointsCm (10 000 µm/cm).
 *   - infoString() reports width/height in both cm (4 dp = 1 µm)
 *     and µm (1 dp = 0.1 µm).
 *
 * Extensibility
 *   - setProfilePointsCm() accepts any polyline (first & last points
 *     should sit on Y = 0 so the mirror joins cleanly).
 */
class ShapeOverlay : public QWidget
{
    Q_OBJECT

public:
    explicit ShapeOverlay(QWidget *parent = nullptr);

    // ---- enable / disable ----
    void setShapeEnabled(bool enabled);
    bool isShapeEnabled() const { return m_enabled; }

    // ---- geometry (upper half only, cm, Y >= 0) ----
    void setProfilePointsCm(const QVector<QPointF>& pointsCm);
    QVector<QPointF> profilePointsCm() const { return m_profileCm; }

    void resetToDefaultStepProfile();

    // ---- sync with ruler ----
    void   setPixelsPerCm(double px);
    double pixelsPerCm() const { return m_pixelsPerCm; }

    void setOriginAtCenter(bool center);
    bool originAtCenter() const { return m_originAtCenter; }

    // ---- appearance ----
    void setShapeLineWidth(int px);
    int  shapeLineWidth() const { return m_lineWidth; }

    void   setFillColor(const QColor& c);
    QColor fillColor() const { return m_fill; }

    void   setOutlineColor(const QColor& c);
    QColor outlineColor() const { return m_outline; }

    // ---- precision ----
    void setMicrometerSnapping(bool on);
    bool micrometerSnapping() const { return m_snapUm; }

    // ---- info ----
    QString infoString() const;
    QSize   sizeHint() const override { return QSize(800, 600); }

    static QVector<QPointF> defaultStepProfileCm();

protected:
    void paintEvent(QPaintEvent* event) override;

private:
    QVector<QPointF> closedProfileCm() const;   // upper + mirrored lower
    double snapCm(double cm) const;

    QVector<QPointF> m_profileCm;
    double m_pixelsPerCm = 38.0;
    int    m_lineWidth   = 2;
    QColor m_fill        { 100, 200, 255,  60 };
    QColor m_outline     {   0, 220, 255, 240 };
    bool   m_enabled     = true;
    bool   m_originAtCenter = true;
    bool   m_snapUm      = true;
};