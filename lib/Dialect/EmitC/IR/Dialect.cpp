// #include "mlir/Dialect/EmitC/IR/EmitC.h"
#include "mlir/Dialect/EmitC/IR/EmitCTraits.h"
#include "mlir/IR/BuiltinOps.h"
#include "mlir/IR/Dialect.h"

#define GET_TYPEDEF_CLASSES
#include "mlir/Dialect/EmitC/IR/EmitCTypes.h.inc"

#include "triton/Dialect/EmitC/IR/Dialect.h.inc"
#include "triton/Dialect/EmitC/IR/Dialect.cpp.inc"

#include "triton/Dialect/EmitC/IR/register_emitc_ext.h"

#define GET_OP_CLASSES
#include "triton/Dialect/EmitC/IR/Ops.h.inc"


namespace mlir {
namespace emitc_ext {


void EmitCExtDialect::initialize() {

  // 注入自定义操作
  addOperations<
#define GET_OP_LIST
#include "triton/Dialect/EmitC/IR/Ops.cpp.inc"
  >();
}

} // namespace emitc_ext

// 注册扩展 Dialect
void registerEmitCExt(DialectRegistry &registry) {
  registry.insert<emitc_ext::EmitCExtDialect>();
}

} // namespace mlir
