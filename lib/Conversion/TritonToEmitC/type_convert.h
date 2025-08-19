#ifndef TRITON_TO_EMITC_TYPECONVERTER_H
#define TRITON_TO_EMITC_TYPECONVERTER_H

#include "mlir/Conversion/LLVMCommon/TypeConverter.h"

namespace mlir::triton {

class TritonToEmitCTypeConverter : public mlir::TypeConverter {
public:
  using TypeConverter::convertType;
  TritonToEmitCTypeConverter();
};

} // namespace mlir::triton

#endif  // TRITON_TO_EMITC_TYPECONVERTER_H
