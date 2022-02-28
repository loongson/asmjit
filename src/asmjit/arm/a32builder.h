// This file is part of AsmJit project <https://asmjit.com>
//
// See asmjit.h or LICENSE.md for license and copyright information
// SPDX-License-Identifier: Zlib

#ifndef ASMJIT_ARM_A32BUILDER_H_INCLUDED
#define ASMJIT_ARM_A32BUILDER_H_INCLUDED

#include "../core/api-config.h"
#ifndef ASMJIT_NO_BUILDER

#include "../core/builder.h"
#include "../arm/a32emitter.h"

ASMJIT_BEGIN_SUB_NAMESPACE(a32)

//! \addtogroup asmjit_a32
//! \{

//! AArch64 builder implementation.
class ASMJIT_VIRTAPI Builder
  : public BaseBuilder,
    public EmitterExplicitT<Builder> {
public:
  ASMJIT_NONCOPYABLE(Builder)
  typedef BaseBuilder Base;

  //! \name Construction & Destruction
  //! \{

  ASMJIT_API explicit Builder(CodeHolder* code = nullptr) noexcept;
  ASMJIT_API virtual ~Builder() noexcept;

  //! \}

  //! \name Finalize
  //! \{

  ASMJIT_API Error finalize() override;

  //! \}
};

//! \}

ASMJIT_END_SUB_NAMESPACE

#endif // !ASMJIT_NO_BUILDER
#endif // ASMJIT_ARM_A32BUILDER_H_INCLUDED
