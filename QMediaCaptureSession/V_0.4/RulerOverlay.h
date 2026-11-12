// ============================================================
// RulerOverlay.h
// ============================================================
#pragma once

#include <QWidget>
#include <QColor>

/**
 * Transparent overlay that draws a measurement grid + X/Y rulers.
 *
 * Origin (0,0) is the CENTER of the widget by default, so the grid
 * is centered on the camera image.
 */
class RulerOverlay : public QWidget
{
    Q_OBJECT

public:
    explicit RulerOverlay(QWidget *parent = nullptr);

    // ---- visibility ----
    void setGridEnabled(bool enabled);
    bool isGridEnabled() const { return m_gridEnabled; }

    void setRulersEnabled(bool enabled);
    bool isRulersEnabled() const { return m_rulersEnabled; }

    // ---- calibration ----
    void   setPixelsPerCm(double px);
    double pixelsPerCm() const { return m_pixelsPerCm; }

    // ---- grid size (spacing, in cm) ----
    void   setXSpacingCm(double cm);          // spacing of VERTICAL lines
    double xSpacingCm() const { return m_xSpacingCm; }

    void   setYSpacingCm(double cm);          // spacing of HORIZONTAL lines
    double ySpacingCm() const { return m_ySpacingCm; }

    // ---- appearance ----
    void setGridLineWidth(int px);
    int  gridLineWidth() const { return m_gridLineWidth; }

    void   setGridColor(const QColor &c);
    QColor gridColor() const { return m_gridColor; }

    void   setAxisColor(const QColor &c);
    QColor axisColor() const { return m_axisColor; }

    void setOriginAtCenter(bool center);
    bool originAtCenter() const { return m_originAtCenter; }

    QSize sizeHint() const override { return QSize(800, 600); }

signals:
    void pixelsPerCmChanged(double px);

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    void drawGrid(QPainter &p, const QPointF &origin);
    void drawRulers(QPainter &p, const QPointF &origin);

    bool m_gridEnabled    = true;
    bool m_rulersEnabled  = true;
    bool m_originAtCenter = true;

    double m_pixelsPerCm = 38.0;
    double m_xSpacingCm  = 1.0;
    double m_ySpacingCm  = 1.0;

    int m_gridLineWidth = 1;

    // QColor m_gridColor {  0, 255, 120, 110 };
    // QColor m_gridColor { 255, 0, 0, 110 };  // Red with transparency
    QColor m_gridColor { 255, 0, 0, 255 }; // For solid red:
    QColor m_axisColor { 255, 255, 255, 220 };
    QColor m_textColor {  0, 255, 120, 235 };
};