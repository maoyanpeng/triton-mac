#include "mlir/Dialect/EmitC/IR/EmitC.h"
#include "mlir/Pass/Pass.h"
#include "mlir/Transforms/DialectConversion.h"
// #include "mlir/Dialect/Arith/IR/Arith.h"
// #include "mlir/Dialect/ControlFlow/IR/ControlFlow.h"
// #include "mlir/Dialect/Func/IR/FuncOps.h"
// #include "mlir/Dialect/Math/IR/Math.h"
// #include "mlir/Dialect/SCF/IR/SCF.h"
#include "mlir/Dialect/Tensor/IR/Tensor.h"
#include "mlir/IR/BuiltinOps.h"
#include "mlir/IR/Dialect.h"
#include "mlir/Interfaces/ControlFlowInterfaces.h"
#include "mlir/Interfaces/FunctionInterfaces.h"
#include "mlir/Interfaces/SideEffectInterfaces.h"
#include "triton/Dialect/Triton/IR/OpsEnums.h.inc"
#include "triton/Dialect/Triton/IR/Traits.h"
#include "triton/Dialect/Triton/IR/Types.h"

#include "triton/Conversion/TritonToEmitC/Passes.h"
#include "triton/Dialect/EmitC/IR/Dialect.h.inc"
#include "triton/Dialect/Triton/IR/Dialect.h.inc"

#define GET_OP_CLASSES
#include "triton/Dialect/EmitC/IR/Ops.h.inc"

#define GET_OP_CLASSES
#include "triton/Dialect/Triton/IR/Ops.h.inc"

#define GEN_PASS_CLASSES
#include "triton/Conversion/TritonToEmitC/Passes.h.inc"

namespace mlir::triton {

template <typename TritonOp, typename EmitCOp>
class TritonOpConversion final : public OpConversionPattern<TritonOp> {
public:
  using OpConversionPattern<TritonOp>::OpConversionPattern;

  LogicalResult matchAndRewrite(TritonOp arithOp, typename TritonOp::Adaptor adaptor,
                  ConversionPatternRewriter &rewriter) const override {
    rewriter.template replaceOpWithNewOp<EmitCOp>(arithOp, arithOp.getType(),
                                                  adaptor.getOperands());

    return success();
  }
};

void populate_triton_to_emitc_patterns(TypeConverter &typeConverter, RewritePatternSet &patterns) {
  MLIRContext *ctx = patterns.getContext();

  // clang-format off
  patterns.add<
    // TritonOpConversion<triton::FuncOp, emitc::FuncOp>,
    TritonOpConversion<triton::GetProgramIdOp, emitc::GetProgramIdOp>
  >(typeConverter, ctx, /*benefit=*/1);
  // clang-format on
}

namespace {

class ConvertTritonToEmitC : public ConvertTritonToEmitCBase<ConvertTritonToEmitC> {
  void runOnOperation() {
    ConversionTarget target(getContext());

    target.addLegalDialect<emitc::EmitCDialect>();
    target.addLegalDialect<emitc_ext::EmitCExtDialect>();
    // target.addIllegalDialect<triton::TritonDialect>();
    target.addIllegalOp<triton::GetProgramIdOp>();

    RewritePatternSet patterns(&getContext());

    TypeConverter typeConverter;
    typeConverter.addConversion([](Type type) { return type; });

    populate_triton_to_emitc_patterns(typeConverter, patterns);

    if (failed(applyPartialConversion(getOperation(), target, std::move(patterns)))) {
      signalPassFailure();
    }
  }
};

} // anonymous namespace

std::unique_ptr<mlir::Pass> createTritonToEmitcConversionPass() {
  return std::make_unique<ConvertTritonToEmitC>();
}

} // namespace mlir::triton
