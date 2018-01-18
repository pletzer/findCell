program findCell

! !USES:
  use ESMF
  !use ESMF_MeshMod

  implicit none

#ifndef ESMF_MPIUNI
  include "mpif.h"
#endif

  integer, parameter :: MAXNAMELEN = 64

  integer            :: rc, ind
  type(ESMF_VM)      :: vm
  integer            :: PetNo, PetCnt
  character(ESMF_MAXPATHLEN) :: meshfilename, pointfilename
  character(ESMF_MAXPATHLEN) :: cwd
  character(len=40)  :: method, flag, lineTypeStr
  character(len=ESMF_MAXPATHLEN) :: commandbuf1(2)
  character(len=256) :: argStr
  logical            :: terminateProg
  !real(ESMF_KIND_R8) :: starttime, endtime
  logical            :: checkFlag
  type(ESMF_LogKind_Flag) :: msgbuf(1)
  type(ESMF_LogKind_Flag) :: logflag
  character(len=ESMF_MAXPATHLEN)  :: argvalue
  integer            :: count, i

  type(ESMF_Mesh) :: mesh

  integer                         :: numNodes, numCells
  integer*8                       :: mreaderId
  integer, allocatable            :: nodeIds(:), elementIds(:), elementTypes(:), elementConn(:)
  real(ESMF_KIND_R8), allocatable :: nodeCoords(:)
  integer, allocatable            :: nodeOwners(:)

  type(ESMF_LocStream) :: points
  integer*8 :: preaderId
  integer :: numPoints
  type(ESMF_Field) :: meshField, pointField
  type(ESMF_ArraySpec) :: arrayspec
  real(ESMF_KIND_R8), allocatable :: pointXCoords(:), pointYCoords(:), pointZCoords(:)

  type(ESMF_RouteHandle) :: regridHandle

  
  terminateProg = .false.
  
  ! Check if --no_log is given, if so, call ESMF_Initialize() with ESMF_LOGKIND_NONE flag
#ifndef ESMF_MPIUNI
  call MPI_Init(rc)
  if (rc /= MPI_SUCCESS) then
      write(*,*) "ERROR: findCell initialization error."
      stop 1
  endif
  call MPI_Comm_rank(MPI_COMM_WORLD, PetNo, rc) 
  if (rc /= MPI_SUCCESS) then
      write(*,*) "ERROR: findCell initialization error."
      call MPI_Finalize(rc)
      stop 1
  endif
#else
  PetNo = 0
#endif

  !------------------------------------------------------------------------
  ! Initialize ESMF
  !
  call ESMF_Initialize (defaultCalKind=ESMF_CALKIND_GREGORIAN, &
         defaultlogfilename="findCell.Log", &
         logkindflag=logflag, rc=rc)
  if (rc /= ESMF_SUCCESS) call ErrorMsgAndAbort(-1)
  
  !------------------------------------------------------------------------
  ! get global vm information
  !
  call ESMF_VMGetGlobal(vm, rc=rc)
  if (rc /= ESMF_SUCCESS) call ErrorMsgAndAbort(-1)

  ! set up local pet info
  call ESMF_VMGet(vm, localPet=PetNo, petCount=PetCnt, rc=rc)
  if (rc /= ESMF_SUCCESS) call ErrorMsgAndAbort(PetNo)

  !------------------------------------------------------------------------
  ! Parse keyword based arguments at Pet 0
  !   then broadcast the results to the rest of the Pets
  !
  if (PetNo == 0) then

    call ESMF_UtilGetArgIndex('-m', argindex=ind, rc=rc)
    if (ind <= 0) then
      print *, "ERROR: must provide VTK mesh file using -m vtk_file option"
      call PrintUsage()
      terminateProg=.true.
    else
      call ESMF_UtilGetArg(ind + 1, argvalue=meshfilename)
    endif

    
    call ESMF_UtilGetArgIndex('-p', argindex=ind, rc=rc)
    if (ind <= 0) then
      print *, "ERROR: must provide point file using -p point_file option"
      call PrintUsage()
      terminateProg=.true.
    else
      call ESMF_UtilGetArg(ind + 1, argvalue=pointfilename)
    endif

    ! Group the command line arguments and broadcast to other PETs
    commandbuf1(1) = meshfilename
    commandbuf1(2) = pointfilename

  endif

  ! Broadcast the command line arguments to all the PETs
  call ESMF_VMBroadcast(vm, commandbuf1, len(commandbuf1)*size(commandbuf1), 0, rc=rc)
  if (rc /= ESMF_SUCCESS) call ErrorMsgAndAbort(PetNo)
  meshfilename = commandbuf1(1)
  pointfilename = commandbuf1(2)
  meshfilename = trim(adjustl(meshfilename))
  pointfilename = trim(adjustl(pointfilename))


  ! Read the mesh file

  mesh = ESMF_MeshCreate(parametricDim=3, spatialDim=3, coordSys=ESMF_COORDSYS_CART, rc=rc)
  if (rc /= ESMF_SUCCESS) call ErrorMsgAndAbort(PetNo)

  write(*,*) "[", PetNo, "] mesh file: ", meshfilename
  call vtk_reader_new(mreaderId)
  call vtk_reader_setfilename(mreaderId, meshfilename, len_trim(meshfilename))
  call vtk_reader_getnumberofpoints(mreaderId, numNodes)
  call vtk_reader_getnumberofcells(mreaderId, numCells)

  allocate(nodeIds(numNodes), nodeCoords(3*numNodes), nodeOwners(numNodes), stat=rc)

  nodeOwners(:) = PetNo
  call vtk_reader_fillvertices(mreaderId, nodeIds(1), nodeCoords(1))
  call ESMF_MeshAddNodes(mesh, nodeIds, nodeCoords, nodeOwners, rc=rc)
  if (rc /= ESMF_SUCCESS) call ErrorMsgAndAbort(PetNo)

  ! assuming hexahedral cells
  allocate(elementIds(numCells), elementTypes(numCells), elementConn(8*numCells))
  elementTypes(:) = ESMF_MESHELEMTYPE_HEX
  call vtk_reader_fillconnectivity(mreaderId, elementIds, elementConn)
  call ESMF_MeshAddElements(mesh, elementIds, elementTypes, elementConn, rc=rc)
  if (rc /= ESMF_SUCCESS) call ErrorMsgAndAbort(PetNo)

  ! Read the point file

  write(*,*) "[", PetNo, "] point file: ", pointfilename
  call vtk_reader_new(preaderId)
  call vtk_reader_setfilename(preaderId, pointfilename, len_trim(pointfilename))
  call vtk_reader_getnumberofpoints(preaderId, numPoints)

  points = ESMF_LocStreamCreate(name="points", localCount=numPoints, &
                              & coordSys=ESMF_COORDSYS_CART, rc=rc)
  if (rc /= ESMF_SUCCESS) call ErrorMsgAndAbort(PetNo)

  allocate(pointXCoords(numPoints), &
         & pointYCoords(numPoints), &
         & pointZCoords(numPoints), stat=rc)

  call vtk_reader_fillxyzvertices(preaderId, pointXCoords(1), pointYCoords(1), pointZCoords(1))
  call ESMF_LocStreamAddKey(points, keyName="ESMF:X", farray=pointXCoords, &
      &                     datacopyflag=ESMF_DATACOPY_REFERENCE, &
      &                     rc=rc)
  if (rc /= ESMF_SUCCESS) call ErrorMsgAndAbort(PetNo)

  call ESMF_LocStreamAddKey(points, keyName="ESMF:Y", farray=pointYCoords, &
      &                     datacopyflag=ESMF_DATACOPY_REFERENCE, &
      &                     rc=rc)
  if (rc /= ESMF_SUCCESS) call ErrorMsgAndAbort(PetNo)

  call ESMF_LocStreamAddKey(points, keyName="ESMF:Z", farray=pointZCoords, &
      &                     datacopyflag=ESMF_DATACOPY_REFERENCE, &
      &                     rc=rc)
  if (rc /= ESMF_SUCCESS) call ErrorMsgAndAbort(PetNo)

  ! Attach fields to the Mesh and LocStream (needed for regridding)
  call ESMF_ArraySpecSet(arrayspec, 1, typekind=ESMF_TYPEKIND_R4, rc=rc)
  if (rc /= ESMF_SUCCESS) call ErrorMsgAndAbort(PetNo)
  meshField = ESMF_FieldCreate(mesh, arrayspec, rc=rc)
  if (rc /= ESMF_SUCCESS) call ErrorMsgAndAbort(PetNo)
  pointField = ESMF_FieldCreate(points, typekind=ESMF_TYPEKIND_R4, rc=rc) ! CAN I REDUCE THE MEMORY FOOTPRINT HERE?
  if (rc /= ESMF_SUCCESS) call ErrorMsgAndAbort(PetNo)

  ! Compute the interpolation weights
  call ESMF_FieldRegridStore(meshField, pointField, routeHandle=regridHandle, &
                           & regridMethod=ESMF_REGRIDMETHOD_BILINEAR, &
                           & unmappedaction=ESMF_UNMAPPEDACTION_IGNORE, rc=rc)
  if (rc /= ESMF_SUCCESS) call ErrorMsgAndAbort(PetNo)

  ! Extract the cell information from the regridHandle
  ! TO DO 


  ! Output success
  if (PetNo==0) then
    write(*,*) "Successful execution."
    !write(*,*) "Completed weight generation in ", (endtime-starttime)*1000, "msecs"
    write(*,*) 
  endif

  ! clean up
  call ESMF_FieldRegridRelease(regridHandle, rc=rc)
  call ESMF_FieldDestroy(meshField, rc=rc)
  call ESMF_FieldDestroy(pointField, rc=rc)
  call vtk_reader_del(preaderId)
  call ESMF_LocStreamDestroy(points, rc=rc)
  call vtk_reader_del(mreaderId)
  call ESMF_MeshDestroy(mesh, rc=rc)
  if (rc /= ESMF_SUCCESS) call ErrorMsgAndAbort(PetNo)

1111  continue
  call ESMF_Finalize()

contains

  subroutine ErrorMsgAndAbort(localPet)
    integer ::  localPet
  
    if (localPet >= 0) then
      write(*,*) "ERROR: Problem on processor ",localPet,". Please see the PET*.findCell.Log files for a traceback."
    else
      write(*,*) "ERROR: Please see the PET*.RegridWeightGen.Log files for a traceback."
    endif
  
    call ESMF_Finalize(endflag=ESMF_END_ABORT)
  
  end subroutine ErrorMsgAndAbort

  subroutine PrintUsage()
    print *, "Usage: findCell -m vtk_file -p point_file" 
    print *, ""
  end subroutine PrintUsage

end program findCell
