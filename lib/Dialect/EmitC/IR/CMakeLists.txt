add_triton_library(EmitCIR
  Dialect.cpp
  Ops.cpp


  DEPENDS
  EmitCTableGen

  LINK_LIBS PUBLIC
  MLIRIR
)
