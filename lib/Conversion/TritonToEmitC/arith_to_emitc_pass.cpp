//===- ArithToEmitCPass.cpp - Arith to EmitC Pass ---------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file implements a pass to convert the Arith dialect to the EmitC
// dialect.
//
//===----------------------------------------------------------------------===//

#include "mlir/Conversion/ArithToEmitC/ArithToEmitCPass.h"

#include "mlir/Conversion/ArithToEmitC/ArithToEmitC.h"
#include "mlir/Dialect/Arith/IR/Arith.h"
#include "mlir/Dialect/EmitC/IR/EmitC.h"
#include "mlir/Pass/Pass.h"
#include "mlir/Transforms/DialectConversion.h"
#include "triton/Conversion/TritonToEmitC/Passes.h"
#include "triton/Dialect/EmitC/IR/Dialect.h.inc"

#define GET_OP_CLASSES
#include "triton/Dialect/EmitC/IR/Ops.h.inc"

namespace mlir {
  #define GEN_PASS_DEF_CONVERTARITHTOEMITC
  #include "mlir/Conversion/Passes.h.inc"
} // namespace mlir

// using namespace mlir;

namespace mlir::triton {

template <typename ArithOp, typename EmitCOp>
class ArithOpConversion final : public OpConversionPattern<ArithOp> {
public:
  using OpConversionPattern<ArithOp>::OpConversionPattern;

  LogicalResult
  matchAndRewrite(ArithOp arithOp, typename ArithOp::Adaptor adaptor,
                  ConversionPatternRewriter &rewriter) const override {

    rewriter.template replaceOpWithNewOp<EmitCOp>(arithOp, arithOp.getType(),
                                                  adaptor.getOperands());

    return success();
  }
};


void populate_arith_to_emitc_patterns(TypeConverter &typeConverter, RewritePatternSet &patterns) {
  MLIRContext *ctx = patterns.getContext();

  // clang-format off
  patterns.add<
    // 替换populateArithToEmitCPatterns中的IntegerOpConversion
    ArithOpConversion<arith::AddIOp, emitc::AddOp>,
    ArithOpConversion<arith::MulIOp, emitc::MulOp>,
    ArithOpConversion<arith::SubIOp, emitc::SubOp>,

    // 添加emit_ext中的op conversion
    ArithOpConversion<arith::MinSIOp, emitc::MinSIOp>
  >(typeConverter, ctx, /*benefit=*/2);
  // clang-format on
}


namespace {

class ConvertArithToEmitC : public impl::ConvertArithToEmitCBase<ConvertArithToEmitC> {
  void runOnOperation() {
    ConversionTarget target(getContext());

    target.addLegalDialect<emitc::EmitCDialect>();
    target.addLegalDialect<emitc_ext::EmitCExtDialect>();
    target.addIllegalDialect<arith::ArithDialect>();

    RewritePatternSet patterns(&getContext());

    TypeConverter typeConverter;
    typeConverter.addConversion([](Type type) { return type; });

    populateArithToEmitCPatterns(typeConverter, patterns);
    populate_arith_to_emitc_patterns(typeConverter, patterns);

    if (failed(applyPartialConversion(getOperation(), target, std::move(patterns)))) {
      signalPassFailure();
    }
  }
};

} // anonymous namespace

std::unique_ptr<mlir::Pass> createArithToEmitcConversionPass() {
  return std::make_unique<ConvertArithToEmitC>();
}

} // namespace


