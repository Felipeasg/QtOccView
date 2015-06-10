#include "occview.h"

#include <QStyleFactory>

#include <Graphic3d_GraphicDriver.hxx>
#include <OpenGl_GraphicDriver.hxx>

#include <X11/Xutil.h>
#include <Xw_Window.hxx>

#include <Aspect_DisplayConnection.hxx>

#include <AIS_Shape.hxx>

OccView::OccView(Handle_AIS_InteractiveContext theContext, QWidget *parent)
    : QWidget(parent),
      myContext(theContext),
      mXmin(0),
      mXmax(0),
      mYmin(0),
      mYmax(0),
      mDegenerateModeIsOn(Standard_True),
      mCurrentMode(CurAction3d_DynamicRotation),
      mRectBand(NULL)
{

//    myView = theContext->CurrentViewer()->CreateView();

//    myContext = new AIS_InteractiveContext(myViewer);

    if ( myView.IsNull() )
      myView = theContext->CurrentViewer()->CreateView();

  #if defined(_WIN32) || defined(__WIN32__)
    Aspect_Handle aWindowHandle = (Aspect_Handle )winId();
    Handle(WNT_Window) hWnd = new WNT_Window (aWindowHandle);
  #elif defined(__APPLE__) && !defined(MACOSX_USE_GLX)
    NSView* aViewHandle = (NSView* )winId();
    Handle(Cocoa_Window) hWnd = new Cocoa_Window (aViewHandle);
  #else
    Window aWindowHandle = (Window )winId(); //Include X11/Xutil.h
    Handle(Aspect_DisplayConnection) aDispConnection = myContext->CurrentViewer()->Driver()->GetDisplayConnection(); //Include Graphic3d_GraphicDriver.hxx
    Handle(Xw_Window) hWnd = new Xw_Window (aDispConnection, aWindowHandle); //Include Xw_Window.hxx
  #endif // WNT

    myView->SetWindow (hWnd);
    if ( !hWnd->IsMapped() )
    {
      hWnd->Map();
    }
    myView->SetBackgroundColor (Quantity_NOC_BLACK);
    myView->MustBeResized();
    //Eixo x, y, z
    myView->TriedronDisplay(Aspect_TOTP_LEFT_LOWER, Quantity_NOC_GOLD, 0.08, V3d_ZBUFFER);


//    if (myIsRaytracing)
    //      myView->ChangeRenderingParams().Method = Graphic3d_RM_RAYTRACING;


    Standard_Boolean aSizeDependant = Standard_True;

    mLayer = new Visual3d_Layer (myView->Viewer()->Viewer(), Aspect_TOL_UNDERLAY, aSizeDependant);

    myView->FitAll();
    myView->ZFitAll();
    myView->Redraw();

    uptdateGradientBackground(mLayer, Quantity_NOC_BLUE4, Quantity_NOC_GRAY65);

    this->setMouseTracking( true );

}

QSize OccView::sizeHint() const
{
    return QSize(200,100);
}

void OccView::pan()
{
    mCurrentMode = CurAction3d_DynamicPanning;
}

void OccView::fitAll()
{
    myView->FitAll();
    myView->ZFitAll();
    myView->Redraw();
}

void OccView::reset()
{
    myView->Reset();
}

void OccView::zoom()
{
    mCurrentMode = CurAction3d_DynamicZooming;
}

void OccView::rotate()
{
    mCurrentMode = CurAction3d_DynamicRotation;
}

void OccView::mousePressEvent(QMouseEvent *e)
{
    if (e->button() == Qt::LeftButton)
    {
        onLButtonDown((e->buttons() | e->modifiers()), e->pos());
    }
    else if (e->button() == Qt::MidButton)
    {
        onMButtonDown((e->buttons() | e->modifiers()), e->pos());
    }
    else if (e->button() == Qt::RightButton)
    {
        onRButtonDown((e->buttons() | e->modifiers()), e->pos());
    }
}

void OccView::mouseReleaseEvent(QMouseEvent *e)
{
    if (e->button() == Qt::LeftButton)
    {
        onLButtonUp(e->buttons() | e->modifiers(), e->pos());
    }
    else if (e->button() == Qt::MidButton)
    {
        onMButtonUp(e->buttons() | e->modifiers(), e->pos());
    }
    else if (e->button() == Qt::RightButton)
    {
        onRButtonUp(e->buttons() | e->modifiers(), e->pos());
    }
}

void OccView::mouseMoveEvent(QMouseEvent *e)
{
    onMouseMove(e->buttons(), e->pos());
}

void OccView::wheelEvent(QWheelEvent *e)
{
    onMouseWheel(e->buttons(), e->delta(), e->pos());
}

void OccView::onLButtonDown(const int theFlags, const QPoint thePoint)
{
    Q_UNUSED(theFlags);

    // Save the current mouse coordinate in min.
    mXmin = thePoint.x();
    mYmin = thePoint.y();
    mXmax = thePoint.x();
    mYmax = thePoint.y();
}

void OccView::onMButtonDown(const int theFlags, const QPoint thePoint)
{
    //Q_UNUSED(theFlags);
    Q_UNUSED(theFlags)

    // Save the current mouse coordinate in min.
    mXmin = thePoint.x();
    mYmin = thePoint.y();
    mXmax = thePoint.x();
    mYmax = thePoint.y();

    if (mCurrentMode == CurAction3d_DynamicRotation)
    {
        myView->StartRotation(thePoint.x(), thePoint.y());
    }
}

void OccView::onRButtonDown(const int theFlags, const QPoint thePoint)
{
    Q_UNUSED(theFlags);
    Q_UNUSED(thePoint);
}

void OccView::onMouseWheel(const int theFlags, const int theDelta, const QPoint thePoint)
{
    Q_UNUSED(theFlags);

    Standard_Integer aFactor = 16;

    Standard_Integer aX = thePoint.x();
    Standard_Integer aY = thePoint.y();

    if (theDelta > 0)
    {
        aX += aFactor;
        aY += aFactor;
    }
    else
    {
        aX -= aFactor;
        aY -= aFactor;
    }

    myView->Zoom(thePoint.x(), thePoint.y(), aX, aY);
}

void OccView::onLButtonUp(const int theFlags, const QPoint thePoint)
{
    // Hide the QRubberBand
    if (mRectBand)
    {
        mRectBand->hide();
    }

    // Ctrl for multi selection.
    if (thePoint.x() == mXmin && thePoint.y() == mYmin)
    {
        if (theFlags & Qt::ControlModifier)
        {
            multiInputEvent(thePoint.x(), thePoint.y());
        }
        else
        {
            inputEvent(thePoint.x(), thePoint.y());
        }
    }
}

void OccView::onMButtonUp(const int theFlags, const QPoint thePoint)
{
    Q_UNUSED(theFlags);

    if (thePoint.x() == mXmin && thePoint.y() == mYmin)
    {
        panByMiddleButton(thePoint);
    }
}

void OccView::onRButtonUp(const int theFlags, const QPoint thePoint)
{
    Q_UNUSED(theFlags);

    popup(thePoint.x(), thePoint.y());
}

void OccView::onMouseMove(const int theFlags, const QPoint thePoint)
{
    // Draw the rubber band.
    if (theFlags & Qt::LeftButton)
    {
        drawRubberBand(mXmin, mYmin, thePoint.x(), thePoint.y());

        dragEvent(thePoint.x(), thePoint.y());
    }

    // Ctrl for multi selection.
    if (theFlags & Qt::ControlModifier)
    {
        multiMoveEvent(thePoint.x(), thePoint.y());
    }
    else
    {
        moveEvent(thePoint.x(), thePoint.y());
    }

    // Middle button.
    if (theFlags & Qt::MidButton)
    {
        switch (mCurrentMode)
        {
        case CurAction3d_DynamicRotation:
            myView->Rotation(thePoint.x(), thePoint.y());
            break;

        case CurAction3d_DynamicZooming:
            myView->Zoom(mXmin, mYmin, thePoint.x(), thePoint.y());
            break;

        case CurAction3d_DynamicPanning:
            myView->Pan(thePoint.x() - mXmax, mYmax - thePoint.y());
            mXmax = thePoint.x();
            mYmax = thePoint.y();
            break;
        }
    }
}

void OccView::addItemInPopup(QMenu *theMenu)
{
    Q_UNUSED(theMenu);
}

void OccView::popup(const int x, const int y)
{
    Q_UNUSED(x);
    Q_UNUSED(y);
}

void OccView::dragEvent(const int x, const int y)
{
    myContext->Select( mXmin, mYmin, x, y, myView );

    emit selectionChanged();
}

void OccView::inputEvent(const int x, const int y)
{
    Q_UNUSED(x);
    Q_UNUSED(y);

    myContext->Select();

    emit selectionChanged();
}

void OccView::moveEvent(const int x, const int y)
{
    myContext->MoveTo(x, y, myView);
}

void OccView::multiMoveEvent(const int x, const int y)
{
    myContext->MoveTo(x, y, myView);
}

void OccView::multiDragEvent(const int x, const int y)
{
    myContext->ShiftSelect( mXmin, mYmin, x, y, myView );

    emit selectionChanged();
}

void OccView::multiInputEvent(const int x, const int y)
{
    Q_UNUSED(x);
    Q_UNUSED(y);

    myContext->ShiftSelect();

    emit selectionChanged();
}

void OccView::drawRubberBand(const int minX, const int minY, const int maxX, const int maxY)
{
    QRect aRect;

    // Set the rectangle correctly.
    (minX < maxX) ? (aRect.setX(minX)) : (aRect.setX(maxX));
    (minY < maxY) ? (aRect.setY(minY)) : (aRect.setY(maxY));

    aRect.setWidth(abs(maxX - minX));
    aRect.setHeight(abs(maxY - minY));

    if (!mRectBand)
    {
        mRectBand = new QRubberBand(QRubberBand::Rectangle, this);

        // setStyle is important, set to windows style will just draw
        // rectangle frame, otherwise will draw a solid rectangle.
        mRectBand->setStyle(QStyleFactory::create("windows"));
    }

    mRectBand->setGeometry(aRect);
    mRectBand->show();
}

void OccView::panByMiddleButton(const QPoint &thePoint)
{
    Standard_Integer aCenterX = 0;
    Standard_Integer aCenterY = 0;

    QSize aSize = size();

    aCenterX = aSize.width() / 2;
    aCenterY = aSize.height() / 2;

    myView->Pan(aCenterX - thePoint.x(), thePoint.y() - aCenterY);
}

void OccView::uptdateGradientBackground(const Handle_Visual3d_Layer &theLayer, const Quantity_Color &theTopColor, const Quantity_Color &theBottomColor)
{
    int aWidth = this->width();
    int aHeigth = this->height();

    theLayer->Clear();
    theLayer->Begin();
    theLayer->SetViewport(aWidth, aHeigth);

    theLayer->BeginPolygon();
    theLayer->SetColor (theTopColor);
    theLayer->AddVertex (-1,1);
    theLayer->AddVertex (1,1);

    theLayer->SetColor (theBottomColor);
    theLayer->AddVertex (1,-1);
    theLayer->AddVertex (-1,-1);
    theLayer->ClosePrimitive();
    theLayer->End();
}

void OccView::paintEvent(QPaintEvent *)
{
    myView->Redraw();
}

void OccView::resizeEvent(QResizeEvent *)
{
    if( !myView.IsNull() )
    {
      myView->MustBeResized();
    }
}

