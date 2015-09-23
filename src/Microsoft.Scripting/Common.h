#pragma once
#include "pch.h"

using namespace Platform;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
namespace MD = Windows::Foundation::Metadata;

namespace Microsoft
{
    namespace Scripting
    {
        ref class ScriptSource;

        namespace JavaScript
        {
            /// <summary>
            /// The "type" of a value, aligned with the JavaScript Language definition of "type".
            /// </summary>
            [MD::WebHostHidden]
            public enum class JavaScriptValueType
            {
                Object = 0,
                Array = 1,
                Boolean = 2,
                Date = 3,
                Function = 4,
                Number = 5,
                String = 6,
                Undefined = 7,
                Symbol = 8,
            };

            ref class JavaScriptRuntimeSettings;
            ref class JavaScriptRuntime;
            ref class JavaScriptEngine;
            ref class JavaScriptConverter;
            ref class JavaScriptPrimitiveValue;
            ref class JavaScriptObject;
            ref class JavaScriptArray;
            ref class JavaScriptFunction;
            ref class JavaScriptSymbol;
            ref class JavaScriptEngineSymbolRegistry;

            /// <summary>
            /// Represents any JavaScript variable.  JavaScript primitives are only accessible via this interface.
            /// </summary>
            /// <remarks>This interface is not intended to be implemented by user code.  It is only provided because 
            ///    many types may polymorph over it.</remarks>
            [MD::WebHostHidden]
            public interface class IJavaScriptValue :
                public IStringable
            {
                /// <summary>
                /// Gets the kind of value this variable is.
                /// </summary>
                property JavaScriptValueType Type
                {
                    JavaScriptValueType get();
                }

                /// <summary>
                /// Gets whether this variable is truthy.
                /// </summary>
                property bool IsTruthy
                {
                    bool get();
                }

                /// <summary>
                /// Gets a reference to the engine that owns this value.
                /// </summary>
                property JavaScriptEngine^ Engine
                {
                    JavaScriptEngine^ get();
                }

                /// <summary>
                /// Gets the native handle (a <c>JsValueRef</c>).
                /// </summary>
                /// <remarks> You should not use this unless you absolutely know what you are doing. </remarks>
                property IntPtr Handle
                {
                    IntPtr get();
                }

                /// <summary>
                /// Tests whether two variables are equal, permitting coercion (equivalent to the JavaScript <c>==</c> operator). 
                /// The variables must be from the same engine.
                /// </summary>
                bool SimpleEquals(IJavaScriptValue^ other);
                /// <summary>
                /// Tests whether two variables are equal, excluding coercion (equivalent to the JavaScript <c>===</c> operator). 
                /// The variables must be from the same engine.
                /// </summary>
                bool StrictEquals(IJavaScriptValue^ other);
            };

            /// <summary>
            /// Represents a JavaScript variable which is an Object or higher in the type hierarchy.
            /// </summary>
            [MD::WebHostHidden]
            public interface class IJavaScriptObject :
                public IJavaScriptValue
            {
            public:
                /// <summary>
                /// Gets an array of strings representing the names of own properties (equivalent to calling <c>Object.keys()</c> 
                /// passing in this object as the target).
                /// </summary>
                property JavaScriptArray^ Keys
                {
                    JavaScriptArray^ get();
                }
                /// <summary>
                /// Gets whether additional properties may be added to this object.
                /// </summary>
                property bool IsExtensible
                {
                    bool get();
                }
                /// <summary>
                /// Gets or sets the object which is this object's prototype (equivalent to accessing <c>__proto__</c>).
                /// </summary>
                property IJavaScriptObject^ Prototype
                {
                    IJavaScriptObject^ get();
                    void set(IJavaScriptObject^ value);
                }
                /// <summary>
                /// Gets whether this object is sealed (from calling <c>Object.seal()</c>).
                /// </summary>
                property bool IsSealed
                {
                    bool get();
                }

                /// <summary>
                /// Gets whether this object is frozen (from calling <c>Object.freeze()</c>).
                /// </summary>
                property bool IsFrozen
                {
                    bool get();
                }

                /// <summary>
                /// Gets whether this object is the prototype of another object.
                /// </summary>
                bool IsPrototypeOf(IJavaScriptObject^ other);
                /// <summary>
                /// Gets whether a particular named property is enumerable on this object.
                /// </summary>
                bool PropertyIsEnumerable(String^ propertyName);

                /// <summary>
                /// Gets a property by name.
                /// </summary>
                IJavaScriptValue^ GetPropertyByName(String^ propertyName);
                /// <summary>
                /// Sets a property by name.
                /// </summary>
                void SetPropertyByName(String^ propertyName, IJavaScriptValue^ value);
                /// <summary>
                /// Deletes a property by name.
                /// </summary>
                void DeletePropertyByName(String^ propertyName);
                /// <summary>
                /// Gets a property by symbol.
                /// </summary>
                IJavaScriptValue^ GetPropertyBySymbol(JavaScriptSymbol^ symbol);
                /// <summary>
                /// Sets a property by symbol.
                /// </summary>
                void SetPropertyBySymbol(JavaScriptSymbol^ symbol, IJavaScriptValue^ value);
                /// <summary>
                /// Deletes a property by symbol.
                /// </summary>
                void DeletePropertyBySymbol(JavaScriptSymbol^ symbol);
                /// <summary>
                /// Gets a value at an index.
                /// </summary>
                IJavaScriptValue^ GetValueAtIndex(IJavaScriptValue^ index);
                /// <summary>
                /// Sets a value at an index.
                /// </summary>
                void SetValueAtIndex(IJavaScriptValue^ index, IJavaScriptValue^ value);
                /// <summary>
                /// Deletes a value at an index.
                /// </summary>
                void DeleteValueAtIndex(IJavaScriptValue^ index);
                /// <summary>
                /// Determines whether an object has an own property (not inherited from its prototype chain).
                /// </summary>
                bool HasOwnProperty(String^ propertyName);
                /// <summary>
                /// Determines whether an object has a property, inherited or not.
                /// </summary>
                bool HasProperty(String^ propertyName);
                /// <summary>
                /// Gets an own-property's descriptor, if present.
                /// </summary>
                JavaScriptObject^ GetOwnPropertyDescriptor(String^ propertyName);
                /// <summary>
                /// Defines a property (equivalent to <c>Object.defineProperty</c>).
                /// </summary>
                void DefineProperty(String^ propertyName, IJavaScriptObject^ descriptor);
                /// <summary>
                /// Defines multiple properties (equivalent to <c>Object.defineProperties</c>).
                /// </summary>
                void DefineProperties(IJavaScriptObject^ propertiesContainer);
                /// <summary>
                /// Gets an array of own-property names.
                /// </summary>
                JavaScriptArray^ GetOwnPropertyNames();
                /// <summary>
                /// Gets an array of own-property Symbols.
                /// </summary>
                JavaScriptArray^ GetOwnPropertySymbols();
                /// <summary>
                /// Prevents the addition of properties to the object.
                /// </summary>
                void PreventExtensions();
                /// <summary>
                /// Seals the object (such as <c>Object.seal</c>).
                /// </summary>
                void Seal();
                /// <summary>
                /// Freezes the object (such as <c>Object.freeze</c>).
                /// </summary>
                void Freeze();
            };

            /// <summary>
            /// A host-provided native function which may be called from a script engine.
            /// </summary>
            /// <param name="callingEngine">The engine which is calling to the host.</param>
            /// <param name="asConstructor">Whether the function is being called as a constructor (such as <c>new Foo()</c>).</param>
            /// <param name="thisValue">An object context being passed as <c>this</c>, if any.</param>
            /// <param name="arguments">Arguments being passed to the function.</param>
            [MD::WebHostHidden]
            public delegate IJavaScriptValue^ JavaScriptCallableFunction(
                JavaScriptEngine^ callingEngine,
                bool asConstructor,
                IJavaScriptValue^ thisValue,
                IIterable<IJavaScriptValue^>^ arguments
                );

            /// <summary>
            /// A host-provided native function called when an external object is finalized.
            /// </summary>
            /// <param name="arguments">An object passed to <c>CreateExternalObject</c>.</param>
            [MD::WebHostHidden]
            public delegate void JavaScriptExternalObjectFinalizeCallback(
                Object^ additionalData);

#ifndef USE_EDGEMODE_JSRT
            [MD::WebHostHidden]
            public enum class JavaScriptRuntimeVersion
            {
                InternetExplorer10 = 0,
                InternetExplorer11 = 1,
            };
#endif // !USE_EDGEMODE_JSRT

        };
    };
};

extern String^ NoMutateJsRuntimeSettings;
extern String^ EngineDisposed;

#define CheckForFailure(CHECKER, EXPR) \
    jsErr = (EXPR); \
    if (JsNoError != jsErr) \
        (CHECKER)->ProcessError(jsErr);

#define EngCheckForFailure(EXPR) CheckForFailure(this, EXPR)
#define ObjCheckForFailure(EXPR) CheckForFailure(engine_, EXPR)

#define CheckForFailure1(CHECKER, EXPR) \
    JsErrorCode jsErr = (EXPR); \
    if (JsNoError != jsErr) \
        (CHECKER)->ProcessError(jsErr);

#define EngCheckForFailure1(EXPR) CheckForFailure1(this, EXPR)
#define ObjCheckForFailure1(EXPR) CheckForFailure1(engine_, EXPR)


#pragma region Delegation shorthand for IJavaScriptValue
// Note: This depends on having a reference to the primitive as an object-level
// field called `primitive_`.
#define DECLARE_JAVASCRIPT_PRIMITIVE_VALUE_VIRTUAL_MEMBERS() \
            public: \
                virtual String^ ToString() override; \
                property JavaScriptValueType Type \
                { \
                    virtual JavaScriptValueType get(); \
                } \
\
                property bool IsTruthy \
                { \
                    virtual bool get(); \
                } \
                property JavaScriptEngine^ Engine \
                { \
                    virtual JavaScriptEngine^ get() { return engine_; } \
                } \
                virtual bool SimpleEquals(IJavaScriptValue^ other); \
                virtual bool StrictEquals(IJavaScriptValue^ other); \
                property IntPtr Handle \
                { \
                    virtual IntPtr get() { return (IntPtr)(void*)handle_; } \
                } \

#define DECLARE_JAVASCRIPT_PRIMITIVE_VALUE_IMPLEMENTATION(TYPENAME) \
JavaScriptValueType TYPENAME::Type::get() \
{ \
    return primitive_->Type; \
} \
\
bool TYPENAME::IsTruthy::get() \
{ \
    return primitive_->IsTruthy; \
} \
\
String^ TYPENAME::ToString() \
{ \
    return primitive_->ToString(); \
} \
\
bool TYPENAME::SimpleEquals(IJavaScriptValue^ other) \
{ \
    return primitive_->SimpleEquals(other); \
} \
\
bool TYPENAME::StrictEquals(IJavaScriptValue^ other) \
{ \
    return primitive_->StrictEquals(other); \
} \
\

#pragma endregion 

#pragma region Delegation shorthand for IJavaScriptObject
// Note: Expects an object-level JavaScriptObject^ implementation named `object_`.
#define DECLARE_JAVASCRIPT_OBJECT_VIRTUAL_MEMBERS() \
            public: \
                property JavaScriptArray^ Keys \
                { \
                    virtual JavaScriptArray^ get(); \
                } \
                property bool IsExtensible \
                { \
                    virtual bool get(); \
                } \
                property IJavaScriptObject^ Prototype \
                { \
                    virtual IJavaScriptObject^ get(); \
                    virtual void set(IJavaScriptObject^ value); \
                } \
                property bool IsSealed \
                { \
                    virtual bool get(); \
                } \
                property bool IsFrozen \
                { \
                    virtual bool get(); \
                } \
\
                virtual bool IsPrototypeOf(IJavaScriptObject^ other); \
                virtual bool PropertyIsEnumerable(String^ propertyName); \
\
                virtual IJavaScriptValue^ GetPropertyByName(String^ propertyName); \
                virtual void SetPropertyByName(String^ propertyName, IJavaScriptValue^ value); \
                virtual void DeletePropertyByName(String^ propertyName); \
                virtual IJavaScriptValue^ GetPropertyBySymbol(JavaScriptSymbol^ symbol); \
                virtual void SetPropertyBySymbol(JavaScriptSymbol^ symbol, IJavaScriptValue^ value); \
                virtual void DeletePropertyBySymbol(JavaScriptSymbol^ symbol); \
                virtual IJavaScriptValue^ GetValueAtIndex(IJavaScriptValue^ index); \
                virtual void SetValueAtIndex(IJavaScriptValue^ index, IJavaScriptValue^ value); \
                virtual void DeleteValueAtIndex(IJavaScriptValue^ index); \
                virtual bool HasOwnProperty(String^ propertyName); \
                virtual bool HasProperty(String^ propertyName); \
                virtual JavaScriptObject^ GetOwnPropertyDescriptor(String^ propertyName); \
                virtual void DefineProperty(String^ propertyName, IJavaScriptObject^ descriptor); \
                virtual void DefineProperties(IJavaScriptObject^ propertiesContainer); \
                virtual JavaScriptArray^ GetOwnPropertyNames(); \
                virtual JavaScriptArray^ GetOwnPropertySymbols(); \
                virtual void PreventExtensions(); \
                virtual void Seal(); \
                virtual void Freeze(); \
                DECLARE_JAVASCRIPT_PRIMITIVE_VALUE_VIRTUAL_MEMBERS() \


#define DECLARE_JAVASCRIPT_OBJECT_IMPLEMENTATION(TYPENAME) \
DECLARE_JAVASCRIPT_PRIMITIVE_VALUE_IMPLEMENTATION( TYPENAME ) \
\
JavaScriptArray^ TYPENAME::Keys::get() \
{ \
    return object_->Keys; \
} \
\
bool TYPENAME::IsExtensible::get() \
{ \
    return object_->IsExtensible; \
} \
\
IJavaScriptObject^ TYPENAME::Prototype::get() \
{ \
    return object_->Prototype; \
} \
\
void TYPENAME::Prototype::set(IJavaScriptObject^ newPrototype) \
{ \
    object_->Prototype = newPrototype; \
} \
\
bool TYPENAME::IsSealed::get() \
{ \
    return object_->IsSealed; \
} \
\
bool TYPENAME::IsFrozen::get() \
{ \
    return object_->IsFrozen; \
} \
\
bool TYPENAME::IsPrototypeOf(IJavaScriptObject^ other) \
{ \
    return object_->IsPrototypeOf(other); \
} \
\
bool TYPENAME::PropertyIsEnumerable(String^ propertyName) \
{ \
    return object_->PropertyIsEnumerable(propertyName); \
} \
\
IJavaScriptValue^ TYPENAME::GetPropertyByName(String^ propertyName) \
{ \
    return object_->GetPropertyByName(propertyName); \
} \
\
void TYPENAME::SetPropertyByName(String^ propertyName, IJavaScriptValue^ value) \
{ \
    object_->SetPropertyByName(propertyName, value); \
} \
\
void TYPENAME::DeletePropertyByName(String^ propertyName) \
{ \
    object_->DeletePropertyByName(propertyName); \
} \
\
IJavaScriptValue^ TYPENAME::GetPropertyBySymbol(JavaScriptSymbol^ symbol) \
{ \
    return object_->GetPropertyBySymbol(symbol); \
} \
void TYPENAME::SetPropertyBySymbol(JavaScriptSymbol^ symbol, IJavaScriptValue^ value) \
{ \
    object_->SetPropertyBySymbol(symbol, value); \
} \
void TYPENAME::DeletePropertyBySymbol(JavaScriptSymbol^ symbol) \
{ \
    object_->DeletePropertyBySymbol(symbol); \
} \
IJavaScriptValue^ TYPENAME::GetValueAtIndex(IJavaScriptValue^ index) \
{ \
    return object_->GetValueAtIndex(index); \
} \
\
void TYPENAME::SetValueAtIndex(IJavaScriptValue^ index, IJavaScriptValue^ value) \
{ \
    object_->SetValueAtIndex(index, value); \
} \
\
void TYPENAME::DeleteValueAtIndex(IJavaScriptValue^ index) \
{ \
    object_->DeleteValueAtIndex(index); \
} \
\
bool TYPENAME::HasOwnProperty(String^ propertyName) \
{ \
    return object_->HasOwnProperty(propertyName); \
} \
\
bool TYPENAME::HasProperty(String^ propertyName) \
{ \
    return object_->HasProperty(propertyName); \
} \
\
JavaScriptObject^ TYPENAME::GetOwnPropertyDescriptor(String^ propertyName) \
{ \
    return object_->GetOwnPropertyDescriptor(propertyName); \
} \
\
void TYPENAME::DefineProperty(String^ propertyName, IJavaScriptObject^ descriptor) \
{ \
    object_->DefineProperty(propertyName, descriptor); \
} \
\
void TYPENAME::DefineProperties(IJavaScriptObject^ propertiesContainer) \
{ \
    object_->DefineProperties(propertiesContainer); \
} \
\
JavaScriptArray^ TYPENAME::GetOwnPropertyNames() \
{ \
    return object_->GetOwnPropertyNames(); \
} \
JavaScriptArray^ TYPENAME::GetOwnPropertySymbols() \
{ \
    return object_->GetOwnPropertySymbols(); \
} \
\
void TYPENAME::PreventExtensions() \
{ \
    object_->PreventExtensions(); \
} \
\
void TYPENAME::Seal() \
{ \
    object_->Seal(); \
} \
\
void TYPENAME::Freeze() \
{ \
    object_->Freeze(); \
} \

#pragma endregion

#define GetHandleFromVar(JS_VAR) (JsValueRef)(void*)((JS_VAR)->Handle)

