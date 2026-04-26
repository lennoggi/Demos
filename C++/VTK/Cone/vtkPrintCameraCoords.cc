#include "iostream"
#include "vtkRenderer.h"
#include "vtkCamera.h"
#include "vtkPrintCameraCoords.hh"

using namespace std;
using namespace Commands;

/* -----------------------------------------
 * vtkPrintCameraCoords class implementation
 * ----------------------------------------- */
Commands::vtkPrintCameraCoords* vtkPrintCameraCoords::New() {
    return new vtkPrintCameraCoords;
}

void Commands::vtkPrintCameraCoords::Execute(vtkObject *caller, unsigned long, void*) {
    auto renderer = reinterpret_cast<vtkRenderer*>(caller);
    cout << renderer->GetActiveCamera()->GetPosition()[0] << " "
         << renderer->GetActiveCamera()->GetPosition()[1] << " "
         << renderer->GetActiveCamera()->GetPosition()[2] << endl;
}
