#ifndef OCCVIEW_H
#define OCCVIEW_H

#include <QObject>
#include <QWidget>
#include <QRubberBand>
#include <QMenu>
#include <QMouseEvent>

#include <AIS_InteractiveContext.hxx>
#include <V3d_View.hxx>

#include <Visual3d_Layer.hxx>

#if defined(_WIN32) || defined(__WIN32__)
#include <WNT_Window.hxx>
#include <Aspect_Handle.hxx>
#endif

// the key for multi selection :
#define MULTISELECTIONKEY Qt::ShiftModifier

// the key for shortcut ( use to activate dynamic rotation, panning )
#define CASCADESHORTCUTKEY Qt::ControlModifier

class OccView : public QWidget
{
    Q_OBJECT
public:
    //! mouse actions.
    enum CurrentAction3d
    {
        CurAction3d_Nothing,
        CurAction3d_DynamicZooming,
        CurAction3d_WindowZooming,
        CurAction3d_DynamicPanning,
        CurAction3d_GlobalPanning,
        CurAction3d_DynamicRotation
    };

public:
    explicit OccView(Handle_AIS_InteractiveContext theContext, QWidget *parent = 0);

    QSize sizeHint() const;
signals:
    void selectionChanged(void);
public slots:

    //! operations for the view.
    void pan(void);
    void fitAll(void);
    void reset(void);
    void zoom(void);
    void rotate(void);

protected:
    // Paint events.
    virtual void                  paintEvent( QPaintEvent* );
    virtual void                  resizeEvent( QResizeEvent* );

    // Mouse events.
    virtual void mousePressEvent(QMouseEvent* e);
    virtual void mouseReleaseEvent(QMouseEvent* e);
    virtual void mouseMoveEvent(QMouseEvent * e);
    virtual void wheelEvent(QWheelEvent * e);

    // Button events.
    virtual void onLButtonDown(const int theFlags, const QPoint thePoint);
    virtual void onMButtonDown(const int theFlags, const QPoint thePoint);
    virtual void onRButtonDown(const int theFlags, const QPoint thePoint);
    virtual void onMouseWheel(const int theFlags, const int theDelta, const QPoint thePoint);
    virtual void onLButtonUp(const int theFlags, const QPoint thePoint);
    virtual void onMButtonUp(const int theFlags, const QPoint thePoint);
    virtual void onRButtonUp(const int theFlags, const QPoint thePoint);
    virtual void onMouseMove(const int theFlags, const QPoint thePoint);

    // Popup menu.
    virtual void addItemInPopup(QMenu* theMenu);

protected:

    void popup(const int x, const int y);
    void dragEvent(const int x, const int y);
    void inputEvent(const int x, const int y);
    void moveEvent(const int x, const int y);
    void multiMoveEvent(const int x, const int y);
    void multiDragEvent(const int x, const int y);
    void multiInputEvent(const int x, const int y);
    void drawRubberBand(const int minX, const int minY, const int maxX, const int maxY);
    void panByMiddleButton(const QPoint& thePoint);
    void uptdateGradientBackground(const Handle_Visual3d_Layer &theLayer, const  Quantity_Color& theTopColor, const Quantity_Color& theBottomColor);

private:


    //! the occ viewer.
    Handle_V3d_View myView;
    Handle_V3d_Viewer myViewer;

    //! the occ context.
    Handle_AIS_InteractiveContext myContext;

    //! the mouse current mode.
    CurrentAction3d mCurrentMode;

    //! save the mouse position.
    Standard_Integer mXmin;
    Standard_Integer mYmin;
    Standard_Integer mXmax;
    Standard_Integer mYmax;

    //! rubber rectangle for the mouse selection.
    QRubberBand* mRectBand;

    //! save the degenerate mode state.
    Standard_Boolean mDegenerateModeIsOn;

    Handle_Visual3d_Layer mLayer;
    Quantity_Color mTopColor;
    Quantity_Color mBottomColor;
};

#endif // OCCVIEW_H
