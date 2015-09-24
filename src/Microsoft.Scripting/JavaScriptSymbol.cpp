#include "pch.h"
#include "JavaScriptSymbol.h"
#include "JavaScriptPrimitiveValue.h"
#include "JavaScriptEngine.h"
#include "JavaScriptConverter.h"
#include "JavaScriptObject.h"
#include "JavaScriptFunction.h"

using namespace Platform::Collections;
using namespace Microsoft::Scripting::JavaScript;

JavaScriptSymbol::JavaScriptSymbol(JavaScriptPrimitiveValue^ primitive, JavaScriptObject^ object) :
    primitive_(primitive),
    engine_(primitive->Engine),
    handle_(GetHandleFromVar(primitive))
{

}

String^ JavaScriptSymbol::Description::get()
{
    throw ref new NotImplementedException(L"Converting a symbol to a string in the host is not currently supported.  To get the Symbol's description, if there is one, request it via toString in script.");
}

DECLARE_JAVASCRIPT_PRIMITIVE_VALUE_IMPLEMENTATION(JavaScriptSymbol)
