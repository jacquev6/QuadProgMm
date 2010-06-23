from ViDE.Project.Description import *

DynamicLibrary(
    name = "qp",
    sources = AllCppIn( "src/QP" ),
    publicHeaders = AllHppIn( "src/QP" ),
    stripFromHeaders = "src/",
    localLibraries = [],
    externalLibraries = []
)

Executable(
    name = "test_qp",
    sources = [ "src/test_qp.cpp" ],
    localLibraries = [ "qp" ],
    externalLibraries = []
)
