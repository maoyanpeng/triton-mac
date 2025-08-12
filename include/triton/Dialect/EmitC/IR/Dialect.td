#ifndef EMITCEXT_DIALECT_IR
#define EMITCEXT_DIALECT_IR

include "mlir/IR/OpBase.td"

//===----------------------------------------------------------------------===//
// EmitCExt dialect definition
//===----------------------------------------------------------------------===//

def EmitCExt_Dialect : Dialect {
  let name = "emitc_ext";
  let cppNamespace = "::mlir::emitc_ext";
  let summary = "Dialect to generate C/C++ from MLIR.";

  // let hasConstantMaterializer = 1;
  // let useDefaultTypePrinterParser = 1;
  // let useDefaultAttributePrinterParser = 1;
}

#endif // EMITCEXT_DIALECT_IR
