import vtk
import sys

from vtkmodules.vtkCommonCore import vtkObject
from DijkstraOnVTKImage import DijkstraOnVTKImage

file_name = sys.argv[1]
reader = vtk.vtkPNGReader()
# Check the image can be read
if not reader.CanReadFile(file_name):
    print('CanReadFile failed for ', file_name)

reader.SetFileName(file_name)
# reader.UpdateInformation()

# image_viewer = vtk.vtkImageViewer()
image_viewer = vtk.vtkImageViewer2()
image_viewer.SetInputConnection( reader.GetOutputPort( ) )
image_viewer.SetZSlice(0)
image_viewer.SetColorLevel(128)
image_viewer.SetColorWindow(256)
image_viewer.GetRenderWindow().SetSize(512,512)
image_viewer.GetRenderer( ).SetBackground(1,0,0)


iren = vtk.vtkRenderWindowInteractor()
image_viewer.SetupInteractor(iren)
image_viewer.GetRenderWindow().SetWindowName('Dijktra')

interactor_style = vtk.vtkInteractorStyleImage()
iren.SetInteractorStyle(interactor_style)

handle = vtk.vtkPointHandleRepresentation2D()
handle.GetProperty().SetColor(0,1,0)

rep = vtk.vtkSeedRepresentation()
rep.SetHandleRepresentation(handle)

seed_widget = vtk.vtkSeedWidget()
seed_widget.SetInteractor(iren)
seed_widget.SetRepresentation(rep)

# class vtkSeedImageCallback(vtk.vtkCallbackCommand):
#     def __init__(self):
#         super().__init__()

#     def Execute(self, caller: vtk.vtkObject, eid: int, callData) -> None:
#         if (eid == vtk.vtkCommand.PlacePointEvent):
#             print(self.SeedRepresentation.GetNumberOfSeeds())
#         if (event == vtkCommand::InteractionEvent)
#     {
#       if (calldata)    
#         # return super().Execute(caller, eid, callData)
#     def SetRepresentation(self,rep):
#         self.SeedRepresentation = rep
#     def SetWidget(self,widget):
#         self.SetWidget=widget
#   virtual void Execute(vtkObject*, unsigned long event, void* calldata)
#   {
#     if (event == vtkCommand::PlacePointEvent)
#     {
#       std::cout << "Placing point..." << std::endl;
#       std::cout << "There are now "
#                 << this->SeedRepresentation->GetNumberOfSeeds() << " seeds."
#                 << std::endl;
#       for (unsigned int seedId = 0; static_cast<int>(seedId) <
#            this->SeedRepresentation->GetNumberOfSeeds();
#            seedId++)
#       {
#         double pos[3];
#         this->SeedRepresentation->GetSeedDisplayPosition(seedId, pos);
#         std::cout << "Seed " << seedId << " : (" << pos[0] << " " << pos[1]
#                   << " " << pos[2] << ")" << std::endl;
#       }
#       return;
#     }
#     if (event == vtkCommand::InteractionEvent)
#     {
#       std::cout << "Interaction..." << std::endl;
#       if (calldata)
#       {
#         double pos[3];
#         this->SeedRepresentation->GetSeedDisplayPosition(0, pos);
#         std::cout << "Moved to (" << pos[0] << " " << pos[1] << " " << pos[2]
#                   << ")" << std::endl;
#       }
#       return;
#     }
#   }
# seedCallback = vtkSeedImageCallback()
# seedCallback.SetRepresentation(rep)
# seedCallback.SetWidget(seed_widget)
# seed_widget.AddObserver(vtk.vtkCommand.PlacePointEvent,print)
# seed_widget.AddObserver(vtk.vtkCommand.PlacePointEvent,seedCallback)
# seed_widget.AddObserver(vtk.vtkCommand.InteractionEvent, seedCallback)

seed_widget.On()
image_viewer.Render()
iren.Initialize()
iren.Start()

if rep.GetNumberOfSeeds() > 1:
    s = [0,0,0]
    e = [0,0,0]
    rep.GetSeedWorldPosition(0,s)
    rep.GetSeedWorldPosition(1,e)
    print(s,e)
    dmap = vtk.vtkImageEuclideanDistance()
    dmap.SetInputConnection( reader.GetOutputPort())
    path = DijkstraOnVTKImage(dmap.GetOutputPort(),s,e)
    print(path.GetNumberOfPoints())

    path_mapper = vtk.vtkPolyDataMapper()
    path_mapper.SetInputData(path.GetOutputPort())

    viewer = vtk.vtkImageViewer2()
    viewer.SetInputConnection( reader.GetOutputPort() )
