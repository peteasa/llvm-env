/************************************************************
 *
 * main.cxx
 *
 * An application to test llvm tools
 *
 * Copyright (c) 2017 Peter Saunderson <peteasa@gmail.com>
 *
 * This program is free software: you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 3 of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this program. If not, see <http://www.gnu.org/licenses/>.
 *
 *************************************************************/

#include <iostream>
#include <vector>

#include "clang/Frontend/CompilerInstance.h"

#include "clang/Basic/TargetOptions.h"
#include "clang/Basic/TargetInfo.h"

#include "clang/Lex/Preprocessor.h"
#include "clang/Lex/HeaderSearch.h"

#include "clang/CodeGen/CodeGenAction.h"

#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/AssemblyAnnotationWriter.h"

int main(int argc, char* argv[])
{
    if (argc < 2)
	{
        std::cout << "Error:  filename expected as a parameter" << std::endl;
		return 0;
	}

    std::vector<const char*> args;
    args.push_back(argv[1]);
    args.push_back("-O0");

    // TODO replace this with compInvocation.setLangDefaults
    args.push_back("-std=c++11");

    clang::CompilerInstance compInst;
    compInst.createDiagnostics();

    //
    // Now create Header Search Options
    //
    std::shared_ptr<clang::HeaderSearchOptions> pHeaderSearchOptions =
        compInst.getHeaderSearchOptsPtr();

    //
    // Now add machine specific header search paths
    //
    pHeaderSearchOptions->AddPath(
        "/usr/include",
        clang::frontend::Angled,
        false,
        false);
    pHeaderSearchOptions->AddPath(
        "/usr/include/c++/5.4.0",
        clang::frontend::Angled,
        false,
        false);
    pHeaderSearchOptions->AddPath(
        "/usr/include/c++/5.4.0/backward",
        clang::frontend::Angled,
        false,
        false);
    pHeaderSearchOptions->AddPath(
        "/usr/include/x86_64-linux-gnu",
        clang::frontend::Angled,
        false,
        false);
    pHeaderSearchOptions->AddPath(
        "/usr/include/x86_64-linux-gnu/c++/5.4.0",
        clang::frontend::Angled,
        false,
        false);
    pHeaderSearchOptions->AddPath(
        "/usr/lib/gcc/x86_64-linux-gnu/5.4.0/include",
        clang::frontend::Angled,
        false,
        false);
    pHeaderSearchOptions->AddPath(
        "/usr/lib/gcc/x86_64-linux-gnu/5.4.0/include-fixed",
        clang::frontend::Angled,
        false,
        false);

    clang::CompilerInvocation &compInvocation = compInst.getInvocation();

    clang::TargetOptions &targetOptions = compInvocation.getTargetOpts();
    targetOptions.Triple = llvm::sys::getDefaultTargetTriple();
    llvm::Triple llvmTriple(compInvocation.getTargetOpts().Triple);

    std::shared_ptr<clang::TargetOptions> pTargetOptions =
        std::make_shared<clang::TargetOptions>(targetOptions);
    clang::TargetInfo *pTargetInfo =
        clang::TargetInfo::CreateTargetInfo(
            compInst.getDiagnostics(),
            pTargetOptions);
    compInst.setTarget(pTargetInfo);

    /* TODO get this to work */
    clang::LangOptions langOpts;
    langOpts.GNUMode = 1;
    langOpts.CXXExceptions = 1;
    langOpts.RTTI = 1;
    langOpts.Bool = 1;
    langOpts.CPlusPlus11 = 1;
    clang::PreprocessorOptions &PreprocOpts = compInst.getPreprocessorOpts();

    compInvocation.setLangDefaults(
        langOpts,
        clang::IK_CXX,
        llvmTriple,
        PreprocOpts,
        clang::LangStandard::lang_cxx11);

    clang::CompilerInvocation::CreateFromArgs(
        compInvocation,
        &args[0], &args[0] + args.size(),
        compInst.getDiagnostics());

    // Compile
    llvm::LLVMContext *llvmContext = new llvm::LLVMContext;
    //clang::EmitLLVMAction action(llvmContext);
    clang::EmitLLVMOnlyAction action(llvmContext);
    if (compInst.ExecuteAction(action))
    {
        // Retrieve module
        std::unique_ptr<llvm::Module> module = action.takeModule();
        if (module)
        {
            //module->dump();

            // Dump IR
            std::error_code err;
            llvm::raw_fd_ostream ir("llvm-ir-output.ll", err, llvm::sys::fs::F_None);
            llvm::AssemblyAnnotationWriter asmWriter;
            module->print(ir, &asmWriter);
            ir.close();
        }
    }

    return 0;
}
