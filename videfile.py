from ViDE.Project.Description import *

DynamicLibrary(
    name = "qp",
    sources = AllCppIn( "src/QP" ),
    headers = AllHppIn( "src/QP" ),
    # stripFromHeaders = "src/",
    # localLibraries = [],
    # externalLibraries = []
)

# Executable(
    # name = "test_qp",
    # sources = [ "src/test_qp.cpp" ],
    # localLibraries = [ "qp" ],
    # externalLibraries = []
# )

# Executable(
    # name = "spring_chain_example",
    # sources = [ "src/spring_chain_example.cpp" ],
    # localLibraries = [ "qp" ],
    # externalLibraries = []
# )
