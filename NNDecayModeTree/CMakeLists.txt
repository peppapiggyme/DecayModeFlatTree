# Declare the package name:
atlas_subdir( NNDecayModeTree )

find_package( Eigen )
find_package( ROOT COMPONENTS Core TMVA )

# For evaluating RNNs
find_package( lwtnn REQUIRED )

atlas_depends_on_subdirs(
	PUBlIC
	Control/AthToolSupport/AsgTools
	Event/xAOD/xAODTau
	Event/xAOD/xAODTruth
	Event/xAOD/xAODTracking
	Event/xAOD/xAODEventInfo
    Event/XAOD/xAODPFlow
	Reconstruction/tauRecTools
	PhysicsAnalysis/D3PDTools/EventLoop
	Tools/PathResolver
	PRIVATE
	External/GaudiInterface
)

atlas_add_root_dictionary( NNDecayModeTreeLib NNDecayModeTreeLibCintDict
  ROOT_HEADERS
  NNDecayModeTree/NNDecayModeTree.h
  Root/LinkDef.h
  EXTERNAL_PACKAGES ROOT )

atlas_add_library( NNDecayModeTreeLib NNDecayModeTree/*.h Root/*.cxx ${NNDecayModeTreeLibCintDict}
		   PUBLIC_HEADERS NNDecayModeTree
		   INCLUDE_DIRS ${Boost_INCLUDE_DIRS} ${EIGEN_INCLUDE_DIRS}
		   PRIVATE_INCLUDE_DIRS ${ROOT_INCLUDE_DIRS} ${LWTNN_INCLUDE_DIRS}
		   LINK_LIBRARIES ${Boost_LIBRARIES} ${EIGEN_LIBRARIES} tauRecToolsLib ${LWTNN_LIBRARIES} 
		   PRIVATE_LINK_LIBRARIES ${ROOT_LIBRARIES} CxxUtils MVAUtils EventLoop )

atlas_install_scripts(share/*.py)
