/* ==========================================
 * This tutorial is inspired by the ones at
 * https://examples.vtk.org/site/Cxx/Tutorial
 * ========================================== */
#include <vtkConeSource.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkProperty.h>
#include <vtkNamedColors.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkCamera.h>
#include "vtkRenderWindowInteractor.h"
//#include "vtkInteractorStyleTrackballCamera.h"
#include "vtkInteractorStyleJoystickCamera.h"

#include "vtkPrintCameraCoords.hh"

using namespace Commands;


// ***** Parameters *****
constexpr inline double CONE_HEIGHT = 3.0;
constexpr inline double CONE_RADIUS = 1.0;
constexpr inline int    CONE_NEDGES = 20;

#define CONE_COLOR       "MistyRose"
#define BACKGROUND_COLOR "MidnightBlue"

#define WINDOW_WIDTH  1000
#define WINDOW_HEIGHT 500
#define WINDOW_TITLE "VTK cone example"

constexpr inline int NTURNS = 10;


//int main() {
int main(int argc, char **argv) {
    // Create a cone object
    auto cone = vtkConeSource::New();
    cone->SetHeight(CONE_HEIGHT);
    cone->SetRadius(CONE_RADIUS);
    cone->SetResolution(CONE_NEDGES);

    /* Specify a mapper, i.e., an interface between the object and the graphics
     * primitives (e.g. lines, surfaces, volumes, light rays, ...), and set the
     * mapper's input to the cone object's output                               */
    auto mapper = vtkPolyDataMapper::New();
    mapper->SetInputConnection(cone->GetOutputPort());

    /* Create an actor from the mapper to orchestrate rendering of the graphics
     * primitives                                                                */
    auto actor = vtkActor::New();
    actor->SetMapper(mapper);

    // Edit the cone's color via the actor
    auto colors = vtkNamedColors::New();
    actor->GetProperty()->SetColor(colors->GetColor3d(CONE_COLOR).GetData());


    /* Create two separate renderers and assign the actors and a viewport to
     * each of them                                                             */
    auto renderer1 = vtkRenderer::New();
    renderer1->AddActor(actor);
    renderer1->SetBackground(colors->GetColor3d(BACKGROUND_COLOR).GetData());
    renderer1->SetViewport(0.0, 0.0, 0.5, 1.0);  // xmin, xmax, ymin, ymax

    auto renderer2 = vtkRenderer::New();
    renderer2->AddActor(actor);
    renderer2->SetBackground(colors->GetColor3d(BACKGROUND_COLOR).GetData());
    renderer2->SetViewport(0.5, 0.0, 1.0, 1.0);  // xmin, xmax, ymin, ymax


    // Create the render window and assign the renderer to it
    auto renderwin = vtkRenderWindow::New();
    renderwin->AddRenderer(renderer1);
    renderwin->AddRenderer(renderer2);
    renderwin->SetSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    renderwin->SetWindowName(WINDOW_TITLE);

    /* Add an observer to one of the renderers that watches for the
     * vtkCommand::StartEvent and calls a custom callback function which prints
     * the camera coordinates                                                   */
    //auto callback = Commands::vtkPrintCameraCoords::New();
    //renderer1->AddObserver(vtkCommand::StartEvent, callback);


    // Display the window and rotate the camera all around the scene
    //for (int i = 0; i < NTURNS*360; ++i) {
    //    renderwin->Render();
    //    renderer1->GetActiveCamera()->Azimuth(1.0);
    //    renderer1->GetActiveCamera()->Elevation(1.0);
    //    renderer1->GetActiveCamera()->OrthogonalizeViewUp();
    //    renderer2->GetActiveCamera()->Azimuth(-1.0);
    //    renderer2->GetActiveCamera()->Elevation(-1.0);
    //    renderer2->GetActiveCamera()->OrthogonalizeViewUp();
    //}

    /* Watch for events in the vtkRenderWindow and enable an interaction with it
     * with a custom style. Press "e" to exit the event loop (started by
     * interactor->Start()).                                                    */
    auto interactor = vtkRenderWindowInteractor::New();
    interactor->SetRenderWindow(renderwin);

    //auto istyle = vtkInteractorStyleTrackballCamera::New();
    auto istyle = vtkInteractorStyleJoystickCamera::New();

    interactor->Initialize();
    interactor->Start();


    // Clean up
    istyle     -> Delete();
    //interactor -> Delete();
    //callback  -> Delete();
    renderwin  -> Delete();
    renderer1  -> Delete();
    renderer2  -> Delete();
    colors     -> Delete();
    actor      -> Delete();
    mapper     -> Delete();
    cone       -> Delete();

    return 0;
}
