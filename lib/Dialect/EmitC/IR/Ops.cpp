#include "mlir/Dialect/EmitC/IR/EmitC.h"
#include "mlir/Dialect/EmitC/IR/EmitCTraits.h"
#include "mlir/IR/Builders.h"
#include "mlir/IR/BuiltinAttributes.h"
#include "mlir/IR/BuiltinTypes.h"
#include "mlir/IR/OperationSupport.h"
#include "mlir/Support/LLVM.h"
#include "mlir/IR/BuiltinOps.h"
#include "mlir/IR/Dialect.h"



#define GET_OP_CLASSES
#include "triton/Dialect/EmitC/IR/Ops.h.inc"

#define GET_OP_CLASSES
#include "triton/Dialect/EmitC/IR/Ops.cpp.inc"

namespace mlir {
namespace emitc {


} // namespace my
} // namespace mlir
