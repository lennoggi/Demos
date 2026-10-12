#ifndef VTKPRINT_CAMERA_COORDS
#define VTKPRINT_CAMERA_COORDS

#include <vtkObject.h>
#include <vtkCommand.h>

namespace Commands {

class vtkPrintCameraCoords : public vtkCommand {
    public:
        vtkPrintCameraCoords() = default;
        static vtkPrintCameraCoords* New();
        void Execute(vtkObject *caller, unsigned long, void*) override;
};

}

#endif  // VTK_PRINT_CAMERA_COORDS
