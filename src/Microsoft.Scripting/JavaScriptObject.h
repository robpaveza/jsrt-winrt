#pragma once
#include "Common.h"

using namespace Windows::Foundation;

namespace Microsoft
{
    namespace Scripting
    {
        namespace JavaScript
        {
            [MD::WebHostHidden]
            public ref class JavaScriptObject sealed:
                public IJavaScriptObject
            {
            private:
                JavaScriptPrimitiveValue^ primitive_;
                JsValueRef handle_;
                JavaScriptEngine^ engine_;

            internal:
                JavaScriptObject(JavaScriptPrimitiveValue^ containedPrimitive);
                property JavaScriptPrimitiveValue^ PrimitiveDelegate
                {
                    JavaScriptPrimitiveValue^ get() { return primitive_; }
                }

            public:
#pragma region IJavaScriptObject virtual properties
                property JavaScriptArray^ Keys
                {
                    virtual JavaScriptArray^ get();
                }
                property bool IsExtensible
                {
                    virtual bool get();
                }
                property IJavaScriptObject^ Prototype
                {
                    virtual IJavaScriptObject^ get();
                    virtual void set(IJavaScriptObject^ value);
                }
                property bool IsSealed
                {
                    virtual bool get();
                }
                property bool IsFrozen
                {
                    virtual bool get();
                }
                property Object^ ExternalData
                {
                    virtual Object^ get();
                    virtual void set(Object^ data);
                }

                virtual bool IsPrototypeOf(IJavaScriptObject^ other);
                virtual bool PropertyIsEnumerable(String^ propertyName);

                virtual IJavaScriptValue^ GetPropertyByName(String^ propertyName);
                virtual void SetPropertyByName(String^ propertyName, IJavaScriptValue^ value);
                virtual void DeletePropertyByName(String^ propertyName);
                virtual IJavaScriptValue^ GetPropertyBySymbol(JavaScriptSymbol^ symbol);
                virtual void SetPropertyBySymbol(JavaScriptSymbol^ symbol, IJavaScriptValue^ value);
                virtual void DeletePropertyBySymbol(JavaScriptSymbol^ symbol);
                virtual IJavaScriptValue^ GetValueAtIndex(IJavaScriptValue^ index);
                virtual void SetValueAtIndex(IJavaScriptValue^ index, IJavaScriptValue^ value);
                virtual void DeleteValueAtIndex(IJavaScriptValue^ index);
                virtual bool HasOwnProperty(String^ propertyName);
                virtual bool HasProperty(String^ propertyName);
                virtual JavaScriptObject^ GetOwnPropertyDescriptor(String^ propertyName);
                virtual void DefineProperty(String^ propertyName, IJavaScriptObject^ descriptor);
                virtual void DefineProperties(IJavaScriptObject^ propertiesContainer);
                virtual JavaScriptArray^ GetOwnPropertyNames();
                virtual JavaScriptArray^ GetOwnPropertySymbols();
                virtual void PreventExtensions();
                virtual void Seal();
                virtual void Freeze();
#pragma endregion IJavaScriptObject virtual properties

            DECLARE_JAVASCRIPT_PRIMITIVE_VALUE_VIRTUAL_MEMBERS()
            
            };
        };
    };
};
