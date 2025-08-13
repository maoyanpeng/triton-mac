#ifndef TRITON_TO_EMITC_PASSES_H
#define TRITON_TO_EMITC_PASSES_H

#include "mlir/Pass/Pass.h"

namespace mlir {
namespace triton {

std::unique_ptr<Pass> createArithToEmitcConversionPass();
std::unique_ptr<Pass> createTritonToEmitcConversionPass();

} // namespace triton
} // namespace mlir

#endif // TRITON_TO_EMITC_PASSES_H
