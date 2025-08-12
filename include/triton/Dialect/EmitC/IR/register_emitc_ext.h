#ifndef REGISTER_EMITC_EXT_H
#define REGISTER_EMITC_EXT_H

#include "mlir/IR/DialectRegistry.h"
#include "mlir/IR/MLIRContext.h"

namespace mlir {

void registerEmitCExt(DialectRegistry &registry);

} // namespace mlir

#endif // REGISTER_EMITC_EXT_H
