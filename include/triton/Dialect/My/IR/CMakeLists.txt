set(MLIR_BINARY_DIR ${CMAKE_BINARY_DIR})

set(LLVM_TARGET_DEFINITIONS MyDialect.td)
mlir_tablegen(MyDialect.h.inc -gen-dialect-decls)
mlir_tablegen(MyDialect.cpp.inc -gen-dialect-defs)
add_mlir_doc(MyDialect MyDialect dialects/ -gen-dialect-doc)

set(LLVM_TARGET_DEFINITIONS MyOps.td)
mlir_tablegen(MyOps.h.inc -gen-op-decls)
mlir_tablegen(MyOps.cpp.inc -gen-op-defs)
add_mlir_doc(MyOps MyOps dialects/ -gen-op-doc)


add_public_tablegen_target(MyTableGen)
