#ifndef MY_DIALECT
#define MY_DIALECT

// 包含 MLIR 基础定义
include "mlir/IR/OpBase.td"

// 定义 Dialect
def My_Dialect : Dialect {
  let name = "my";
  let summary = "My custom dialect";
  let cppNamespace = "::mlir::my";


}

#endif // MY_DIALECT