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
            };

            ref class JavaScriptRuntimeSettings;
            ref class JavaScriptRuntime;
            ref class JavaScriptEngine;
            ref class JavaScriptConverter;
            ref class JavaScriptPrimitiveValue;
            ref class JavaScriptObject;
            ref class JavaScriptArray;
            ref class JavaScriptFunction;

            [MD::WebHostHidden]
            public interface class IJavaScriptValue :
                public IStringable
            {
                property JavaScriptValueType Type
                {
                    JavaScriptValueType get();
                }

                property bool IsTruthy
                {
                    bool get();
                }

                property JavaScriptEngine^ Engine
                {
                    JavaScriptEngine^ get();
                }

                property IntPtr Handle
                {
                    IntPtr get();
                }

                bool SimpleEquals(IJavaScriptValue^ other);
                bool StrictEquals(IJavaScriptValue^ other);
            };

            [MD::WebHostHidden]
            public interface class IJavaScriptObject :
                public IJavaScriptValue
            {
            public:
                property JavaScriptArray^ Keys
                {
                    JavaScriptArray^ get();
                }
                property bool IsExtensible
                {
                    bool get();
                }
                property IJavaScriptObject^ Prototype
                {
                    IJavaScriptObject^ get();
                    void set(IJavaScriptObject^ value);
                }
                property bool IsSealed
                {
                    bool get();
                }
                property bool IsFrozen
                {
                    bool get();
                }

                bool IsPrototypeOf(IJavaScriptObject^ other);
                bool PropertyIsEnumerable(String^ propertyName);

                IJavaScriptValue^ GetPropertyByName(String^ propertyName);
                void SetPropertyByName(String^ propertyName, IJavaScriptValue^ value);
                void DeletePropertyByName(String^ propertyName);
                IJavaScriptValue^ GetValueAtIndex(IJavaScriptValue^ index);
                void SetValueAtIndex(IJavaScriptValue^ index, IJavaScriptValue^ value);
                void DeleteValueAtIndex(IJavaScriptValue^ index);
                bool HasOwnProperty(String^ propertyName);
                bool HasProperty(String^ propertyName);
                JavaScriptObject^ GetOwnPropertyDescriptor(String^ propertyName);
                void DefineProperty(String^ propertyName, IJavaScriptObject^ descriptor);
                void DefineProperties(IJavaScriptObject^ propertiesContainer);
                JavaScriptArray^ GetOwnPropertyNames();
                void PreventExtensions();
                void Seal();
                void Freeze();
            };

            [MD::WebHostHidden]
            public delegate IJavaScriptValue^ JavaScriptCallableFunction(
                JavaScriptEngine^ callingEngine,
                bool asConstructor,
                IJavaScriptValue^ thisValue,
                IIterable<IJavaScriptValue^>^ arguments
                );

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
                virtual IJavaScriptValue^ GetValueAtIndex(IJavaScriptValue^ index); \
                virtual void SetValueAtIndex(IJavaScriptValue^ index, IJavaScriptValue^ value); \
                virtual void DeleteValueAtIndex(IJavaScriptValue^ index); \
                virtual bool HasOwnProperty(String^ propertyName); \
                virtual bool HasProperty(String^ propertyName); \
                virtual JavaScriptObject^ GetOwnPropertyDescriptor(String^ propertyName); \
                virtual void DefineProperty(String^ propertyName, IJavaScriptObject^ descriptor); \
                virtual void DefineProperties(IJavaScriptObject^ propertiesContainer); \
                virtual JavaScriptArray^ GetOwnPropertyNames(); \
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

