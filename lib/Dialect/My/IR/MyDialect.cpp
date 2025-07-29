
#include "mlir/IR/BuiltinOps.h"
#include "mlir/IR/Dialect.h"
#include "triton/Dialect/My/IR/MyDialect.h.inc"
#include "triton/Dialect/My/IR/MyDialect.cpp.inc"

#define GET_OP_CLASSES
#include "triton/Dialect/My/IR/MyOps.h.inc"

namespace mlir {

namespace my {
  void MyDialect::initialize() {
    // registerTypes();
  
    addOperations<
  #define GET_OP_LIST
  #include "triton/Dialect/My/IR/MyOps.cpp.inc"
        >();
  
  }
  
}

} // namespace
