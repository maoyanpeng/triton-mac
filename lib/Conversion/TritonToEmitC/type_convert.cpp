#include "type_convert.h"

#include "mlir/Dialect/EmitC/IR/EmitC.h"
#include "triton/Dialect/Triton/IR/Types.h"

namespace mlir::triton {

TritonToEmitCTypeConverter::TritonToEmitCTypeConverter() {
  // 添加默认转换：保留标准类型（如整数、浮点数）
  addConversion([](Type type) -> std::optional<Type> {
      return type;
  });

  // 转换 Triton 指针类型 -> EmitC 指针类型
  addConversion([](triton::PointerType type) -> Type {
      return emitc::PointerType::get(type.getPointeeType());
  });

  // 转换 Triton Tensor 类型（可能需要展平或特殊处理）
  addConversion([](RankedTensorType tensorType) -> Type {
      return emitc::ArrayType::get(tensorType.getShape(), tensorType.getElementType());
  });

  addSourceMaterialization([&](OpBuilder &builder, Type resultType,
                               ValueRange inputs,
                               Location loc) -> std::optional<Value> {
      if (inputs.size() != 1) return std::nullopt;
      return builder.create<UnrealizedConversionCastOp>(loc, resultType, inputs).getResult(0);
  });
  addTargetMaterialization([&](OpBuilder &builder, Type resultType,
                               ValueRange inputs,
                               Location loc) -> std::optional<Value> {
      if (inputs.size() != 1) return std::nullopt;
      return builder.create<UnrealizedConversionCastOp>(loc, resultType, inputs).getResult(0);
  });
}


} // namespace mlir::triton
