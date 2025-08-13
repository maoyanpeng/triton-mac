#ifndef TRITON_CONVERSION_PASSES
#define TRITON_CONVERSION_PASSES

include "mlir/Pass/PassBase.td"


//===----------------------------------------------------------------------===//
// ArithToEmitC
//===----------------------------------------------------------------------===//

def ConvertTritonToEmitC : Pass<"convert-triton-to-emitc"> {
  let summary = "Convert Triton dialect to EmitC dialect";
  let dependentDialects = ["mlir::emitc::EmitCDialect"];
}

#endif // TRITON_CONVERSION_PASSES