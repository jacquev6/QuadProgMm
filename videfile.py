from ViDE.Project.Description import *

qp = CppDynamicLibrary(
    name = "qp",
    sources = AllCppIn( "src/QP" ),
    headers = AllHppIn( "src/QP" ),
    stripHeaders = lambda h: h[4:]
)

CppExecutable(
    name = "test_qp",
    sources = [ "src/test_qp.cpp" ],
    localLibraries = [ qp ],
    externalLibraries = []
)

CppExecutable(
    name = "spring_chain_example",
    sources = [ "src/spring_chain_example.cpp" ],
    localLibraries = [ qp ],
    externalLibraries = []
)
