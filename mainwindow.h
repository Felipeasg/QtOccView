#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>

#include "occview.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:
    // initialize the OpenCASCADE modeler.
    void InitializeModeler(void);

protected:
    //! create all the actions.
    void createActions(void);

    //! create all the menus.
    void createMenus(void);

    //! create the toolbar.
    void createToolBars(void);
private slots:

    //! show about box.
    void about(void);

    //! make box test.
    void makeBox(void);

    //! make cone test.
    void makeCone(void);

    //! make sphere test.
    void makeSphere(void);

    //! make cylinder test.
    void makeCylinder(void);

    //! make torus test.
    void makeTorus(void);

    //! fillet test.
    void makeFillet(void);

    //! chamfer test.
    void makeChamfer(void);

    //! test extrude algorithm.
    void makeExtrude(void);

    //! test revol algorithm.
    void makeRevol(void);

    //! test loft algorithm.
    void makeLoft(void);

    //! test boolean operation cut.
    void testCut(void);

    //! test boolean operation fuse.
    void testFuse(void);

    //! test boolean operation common.
    void testCommon(void);

    //! timer Redraw
    void timerRedraw(void);

private:
    Ui::MainWindow *ui;

    OccView* occView;

    //! the occ viewer.
    Handle_V3d_Viewer mViewer;

    //! the interactive context.
    Handle_AIS_InteractiveContext mContext;

    //! the exit action.
    QAction* mExitAction;

    //! the actions for the view: pan, reset, fitall.
    QAction* mViewZoomAction;
    QAction* mViewPanAction;
    QAction* mViewRotateAction;
    QAction* mViewResetAction;
    QAction* mViewFitallAction;

    //! the actions to test the OpenCASCADE modeling algorithms.
    QAction* mMakeBoxAction;
    QAction* mMakeConeAction;
    QAction* mMakeSphereAction;
    QAction* mMakeCylinderAction;
    QAction* mMakeTorusAction;

    //! make a fillet box.
    QAction* mFilletAction;
    QAction* mChamferAction;
    QAction* mExtrudeAction;
    QAction* mRevolveAction;
    QAction* mLoftAction;

    //! boolean operations.
    QAction* mCutAction;
    QAction* mFuseAction;
    QAction* mCommonAction;

    //! show the about info action.
    QAction* mAboutAction;

    //! the menus of the application.
    QMenu* mFileMenu;
    QMenu* mViewMenu;
    QMenu* mPrimitiveMenu;
    QMenu* mModelingMenu;
    QMenu* mHelpMenu;

    //! the toolbars of the application.
    QToolBar* mViewToolBar;
    QToolBar* mNavigateToolBar;
    QToolBar* mPrimitiveToolBar;
    QToolBar* mModelingToolBar;
    QToolBar* mHelpToolBar;
};

#endif // MAINWINDOW_H
