#include "mlir/IR/Builders.h"
#include "mlir/IR/BuiltinAttributes.h"
#include "mlir/IR/BuiltinTypes.h"
#include "mlir/IR/OperationSupport.h"
// #include "mlir/Interfaces/FunctionImplementation.h"
// #include "mlir/Interfaces/FunctionInterfaces.h"
#include "mlir/Support/LLVM.h"
#include "mlir/IR/BuiltinOps.h"
#include "mlir/IR/Dialect.h"



#define GET_OP_CLASSES
#include "triton/Dialect/My/IR/MyOps.h.inc"

#define GET_OP_CLASSES
#include "triton/Dialect/My/IR/MyOps.cpp.inc"

namespace mlir {
namespace my {
 
void MyOp::custom_func() {
//   auto inputType = getInput().getType().cast<mlir::TensorType>();
//   auto outputType = getOutput().getType().cast<mlir::TensorType>();
  
//   // 检查输入/输出类型匹配
//   if (inputType != outputType)
//     return op.emitError("Input/output types must match");
  
  return ;
}

// void My_AddFOp::build(OpBuilder &builder, OperationState &state,
//                       Value lhs, Value rhs) {
//   build(builder, state, lhs.getType(), lhs, rhs);
// }

} // namespace my
} // namespace mlir
