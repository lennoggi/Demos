/* =========================================================
 * This tutorial is inspired by the one at
 * https://examples.vtk.org/site/Cxx/Tutorial/Tutorial_Step1
 * ========================================================= */

#include <vtkConeSource.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkProperty.h>
#include <vtkNamedColors.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkCamera.h>


#define NTURNS 10

#define WINDOW_WIDTH  1000
#define WINDOW_HEIGHT 1000

#define WINDOW_TITLE "VTK cone example"


//int main() {
int main(int argc, char **argv) {
    // Create a cone object
    auto cone = vtkConeSource::New();
    cone->SetHeight(3.0);
    cone->SetRadius(1.0);
    cone->SetResolution(10);

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
    actor->GetProperty()->SetColor(colors->GetColor3d("MistyRose").GetData());

    // Create a renderer and assign the actor to it
    auto renderer = vtkRenderer::New();
    renderer->AddActor(actor);
    renderer->SetBackground(colors->GetColor3d("MidnightBlue").GetData());

    // Create the render window and assign the renderer to it
    auto renderwin = vtkRenderWindow::New();
    renderwin->AddRenderer(renderer);
    renderwin->SetSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    renderwin->SetWindowName(WINDOW_TITLE);

    // Display the window and rotate the camera all around the scene
    for (int i = 0; i < NTURNS*360; ++i) {
        renderwin->Render();
        renderer->GetActiveCamera()->Azimuth(1);
    }

    // Clean up
//    renderwin -> Delete();
//    renderer  -> Delete();
//    colors    -> Delete();
//    actor     -> Delete();
//    mapper    -> Delete();
//    cone      -> Delete();

    return EXIT_SUCCESS;
}
