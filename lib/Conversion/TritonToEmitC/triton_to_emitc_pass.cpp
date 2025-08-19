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

#include "type_convert.h"

#define GET_OP_CLASSES
#include "triton/Dialect/EmitC/IR/Ops.h.inc"

#define GET_OP_CLASSES
#include "triton/Dialect/Triton/IR/Ops.h.inc"

#define GEN_PASS_CLASSES
#include "triton/Conversion/TritonToEmitC/Passes.h.inc"

namespace mlir::triton {

static void addNamedAttrs(Operation *op, DictionaryAttr dictAttrs) {
  for (const NamedAttribute attr : dictAttrs.getValue())
    if (!op->hasAttr(attr.getName()))
      op->setAttr(attr.getName(), attr.getValue());
}

class CallOpConversion final : public OpConversionPattern<triton::CallOp> {
public:
  using OpConversionPattern<triton::CallOp>::OpConversionPattern;

  LogicalResult
  matchAndRewrite(triton::CallOp op, OpAdaptor adaptor,
                  ConversionPatternRewriter &rewriter) const override {
    // Multiple results func was not converted to `emitc.func`.
    if (op.getNumResults() > 1) {
      return rewriter.notifyMatchFailure(op, "only functions with zero or one result can be converted");
    }

    auto newOp = rewriter.replaceOpWithNewOp<emitc::CallOp>(
        op, op.getCallee(), op.getResultTypes(), adaptor.getOperands());
    addNamedAttrs(newOp, adaptor.getAttributes());
    return success();
  }
};

class FuncOpConversion final : public OpConversionPattern<triton::FuncOp> {
public:
  using OpConversionPattern<triton::FuncOp>::OpConversionPattern;

  LogicalResult
  matchAndRewrite(triton::FuncOp op, OpAdaptor adaptor,
                  ConversionPatternRewriter &rewriter) const override {
    // op->getParentOfType<ModuleOp>().dump();
    // 1. 转换函数签名类型
    auto typeConverter = getTypeConverter();
    mlir::TypeConverter::SignatureConversion signatureConverter(
        op.getFunctionType().getNumInputs());
    for (const auto &arg : llvm::enumerate(op.getArguments())) {
      mlir::Type convertedType = typeConverter->convertType(arg.value().getType());
      signatureConverter.addInputs(arg.index(), convertedType);
    }
    // 转换返回类型
    mlir::SmallVector<mlir::Type> convertedResultTypes;
    if (failed(typeConverter->convertTypes(op.getResultTypes(), convertedResultTypes))) {
      return failure();
    }

    // 2. 创建新的 emitc.func 操作
    auto newFuncType = mlir::FunctionType::get(
        op.getContext(),
        signatureConverter.getConvertedTypes(),
        convertedResultTypes
    );
    auto newFunc = rewriter.create<emitc::FuncOp>(
        op.getLoc(),
        op.getName(),
        newFuncType
    );
    for (const auto &namedAttr : op->getAttrs()) {
      if (namedAttr.getName() != op.getFunctionTypeAttrName() &&
          namedAttr.getName() != SymbolTable::getSymbolAttrName())
        newFunc->setAttr(namedAttr.getName(), namedAttr.getValue());
    }

    // 3. 迁移函数体并重写参数类型
    rewriter.inlineRegionBefore(op.getBody(), newFunc.getBody(), newFunc.end());
    if (failed(rewriter.convertRegionTypes(&newFunc.getBody(), *typeConverter, &signatureConverter))) {
      return failure();
    }

    // 4. 删除原 Triton func.func
    rewriter.eraseOp(op);

    return success();
  }
};

class ReturnOpConversion final : public OpConversionPattern<triton::ReturnOp> {
public:
  using OpConversionPattern<triton::ReturnOp>::OpConversionPattern;

  LogicalResult
  matchAndRewrite(triton::ReturnOp op, OpAdaptor adaptor,
                  ConversionPatternRewriter &rewriter) const override {
    if (op.getNumOperands() > 1) {
      return rewriter.notifyMatchFailure(op, "only zero or one operand is supported");
    }

    rewriter.replaceOpWithNewOp<emitc::ReturnOp>(
        op,
        op.getNumOperands() ? adaptor.getOperands()[0] : nullptr);
    return success();
  }
};

template <typename TritonOp, typename EmitCOp>
class GenericOpPattern final : public OpConversionPattern<TritonOp> {
public:
  using OpConversionPattern<TritonOp>::OpConversionPattern;

  LogicalResult matchAndRewrite(TritonOp op, typename TritonOp::Adaptor adaptor,
                  ConversionPatternRewriter &rewriter) const override {
    SmallVector<Type> retTypes;
    if (failed(this->getTypeConverter()->convertTypes(op->getResultTypes(), retTypes))) {
      return failure();
    }          
    rewriter.template replaceOpWithNewOp<EmitCOp>(op, retTypes, adaptor.getOperands(), op->getAttrs());

    return success();
  }
};

void populate_triton_to_emitc_patterns(TypeConverter &typeConverter, RewritePatternSet &patterns) {
  MLIRContext *ctx = patterns.getContext();
  // clang-format off
  patterns.add<
    CallOpConversion,
    FuncOpConversion,
    ReturnOpConversion,
    GenericOpPattern<triton::LoadexOp, emitc::LoadexOp>,
    GenericOpPattern<triton::StoreexOp, emitc::StoreexOp>,
    GenericOpPattern<triton::GetProgramIdOp, emitc::GetProgramIdOp>,
    GenericOpPattern<triton::AddPtrOp, emitc::AddOp>
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
    target.addIllegalOp<
      triton::CallOp,
      triton::FuncOp,
      triton::ReturnOp,
      triton::LoadexOp,
      triton::StoreexOp,
      triton::GetProgramIdOp,
      triton::AddPtrOp
    >();

    RewritePatternSet patterns(&getContext());
    TritonToEmitCTypeConverter typeConverter;

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
