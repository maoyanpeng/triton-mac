add_triton_library(MyIR
  MyDialect.cpp
  MyOps.cpp


  DEPENDS
  MyTableGen

  LINK_LIBS PUBLIC
  MLIRIR
)
