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

#include "vtkPrintCameraCoords.hh"

using namespace Commands;


// ***** Parameters *****
constexpr inline double CONE_HEIGHT = 3.0;
constexpr inline double CONE_RADIUS = 1.0;
constexpr inline int    CONE_NEDGES = 20;

#define CONE_COLOR       "MistyRose"
#define BACKGROUND_COLOR "MidnightBlue"

#define WINDOW_WIDTH  1000
#define WINDOW_HEIGHT 1000
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

    // Create a renderer and assign the actor to it
    auto renderer = vtkRenderer::New();
    renderer->AddActor(actor);
    renderer->SetBackground(colors->GetColor3d(BACKGROUND_COLOR).GetData());

    // Create the render window and assign the renderer to it
    auto renderwin = vtkRenderWindow::New();
    renderwin->AddRenderer(renderer);
    renderwin->SetSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    renderwin->SetWindowName(WINDOW_TITLE);

    /* Add an observer (i.e., something that catches signals thrown by some VTK
     * entity) to the renderer                                                  */
    auto callback = Commands::vtkPrintCameraCoords::New();
    renderer->AddObserver(vtkCommand::StartEvent, callback);


    // Display the window and rotate the camera all around the scene
    for (int i = 0; i < NTURNS*360; ++i) {
        renderwin->Render();
        renderer->GetActiveCamera()->Azimuth(1.0);
        renderer->GetActiveCamera()->Elevation(1.0);
        renderer->GetActiveCamera()->OrthogonalizeViewUp();
    }


    // Clean up
    callback  -> Delete();
    renderwin -> Delete();
    renderer  -> Delete();
    colors    -> Delete();
    actor     -> Delete();
    mapper    -> Delete();
    cone      -> Delete();

    return 0;
}
