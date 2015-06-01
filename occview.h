#ifndef OCCVIEW_H
#define OCCVIEW_H

#include <QObject>
#include <QWidget>

#include <AIS_InteractiveContext.hxx>
#include <V3d_View.hxx>

#include <BRep_Tool.hxx>

#include <BRepAlgoAPI_Fuse.hxx>

#include <BRepBuilderAPI_MakeEdge.hxx>
#include <BRepBuilderAPI_MakeFace.hxx>
#include <BRepBuilderAPI_MakeWire.hxx>
#include <BRepBuilderAPI_Transform.hxx>

#include <BRepFilletAPI_MakeFillet.hxx>

#include <BRepLib.hxx>

#include <BRepOffsetAPI_MakeThickSolid.hxx>
#include <BRepOffsetAPI_ThruSections.hxx>

#include <BRepPrimAPI_MakeCylinder.hxx>
#include <BRepPrimAPI_MakePrism.hxx>

#include <GC_MakeArcOfCircle.hxx>
#include <GC_MakeSegment.hxx>

#include <GCE2d_MakeSegment.hxx>

#include <gp.hxx>
#include <gp_Ax1.hxx>
#include <gp_Ax2.hxx>
#include <gp_Ax2d.hxx>
#include <gp_Dir.hxx>
#include <gp_Dir2d.hxx>
#include <gp_Pnt.hxx>
#include <gp_Pnt2d.hxx>
#include <gp_Trsf.hxx>
#include <gp_Vec.hxx>

#include <Geom_CylindricalSurface.hxx>
#include <Geom_Plane.hxx>
#include <Geom_Surface.hxx>
#include <Geom_TrimmedCurve.hxx>

#include <Geom2d_Ellipse.hxx>
#include <Geom2d_TrimmedCurve.hxx>

#include <TopExp_Explorer.hxx>

#include <TopoDS.hxx>
#include <TopoDS_Edge.hxx>
#include <TopoDS_Face.hxx>
#include <TopoDS_Wire.hxx>
#include <TopoDS_Shape.hxx>
#include <TopoDS_Compound.hxx>

#include <TopTools_ListOfShape.hxx>

class OccView : public QWidget
{
    Q_OBJECT
public:
    explicit OccView(QWidget *parent = 0);

    QSize sizeHint() const;
signals:

public slots:

protected:
    virtual void                  paintEvent( QPaintEvent* );
    virtual void                  resizeEvent( QResizeEvent* );
private:

    Handle(V3d_Viewer) Viewer (const Standard_ExtString theName,
                                               const Standard_CString theDomain,
                                               const Standard_Real theViewSize,
                                               const V3d_TypeOfOrientation theViewProj,
                                               const Standard_Boolean theComputedMode,
                                               const Standard_Boolean theDefaultComputedMode );

    TopoDS_Shape
    MakeBottle(const Standard_Real myWidth, const Standard_Real myHeight,
               const Standard_Real myThickness);

    Handle(V3d_Viewer)             myViewer;
    Handle(V3d_View)                myView;
    Handle(AIS_InteractiveContext)  myContext;
};

#endif // OCCVIEW_H
