#pragma once
#include "Common.h"

using namespace Windows::Foundation;

namespace Microsoft
{
    namespace Scripting
    {
        namespace JavaScript
        {
            /// <summary>
            /// Represents a JavaScript Object.
            /// </summary>
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
                /// <summary>
                /// Gets an array of the object's own-property names.
                /// </summary>
                property JavaScriptArray^ Keys
                {
                    virtual JavaScriptArray^ get();
                }
                /// <summary>
                /// Gets whether properties can be added to the object.
                /// </summary>
                property bool IsExtensible
                {
                    virtual bool get();
                }
                /// <summary>
                /// Gets or sets an object's prototype.
                /// </summary>
                property IJavaScriptObject^ Prototype
                {
                    virtual IJavaScriptObject^ get();
                    virtual void set(IJavaScriptObject^ value);
                }
                /// <summary>
                /// Gets whether the object has been sealed.
                /// </summary>
                property bool IsSealed
                {
                    virtual bool get();
                }
                /// <summary>
                /// Gets whether the object has been frozen.
                /// </summary>
                property bool IsFrozen
                {
                    virtual bool get();
                }

                /// <summary>
                /// Gets whether the object is a prototype of another.
                /// </summary>
                virtual bool IsPrototypeOf(IJavaScriptObject^ other);
                /// <summary>
                /// Gets whether the specified property is enumerable on the object.
                /// </summary>
                virtual bool PropertyIsEnumerable(String^ propertyName);

                /// <summary>
                /// Gets a property by its name.
                /// </summary>
                virtual IJavaScriptValue^ GetPropertyByName(String^ propertyName);
                /// <summary>
                /// Sets a property by name.
                /// </summary>
                virtual void SetPropertyByName(String^ propertyName, IJavaScriptValue^ value);
                /// <summary>
                /// Deletes a property by name.
                /// </summary>
                virtual void DeletePropertyByName(String^ propertyName);
#ifdef USE_EDGEMODE_JSRT
                /// <summary>
                /// Gets a property by symbol.
                /// </summary>
                virtual IJavaScriptValue^ GetPropertyBySymbol(JavaScriptSymbol^ symbol);
                /// <summary>
                /// Sets a property by symbol.
                /// </summary>
                virtual void SetPropertyBySymbol(JavaScriptSymbol^ symbol, IJavaScriptValue^ value);
                /// <summary>
                /// Deletes a property by symbol.
                /// </summary>
                virtual void DeletePropertyBySymbol(JavaScriptSymbol^ symbol);
#endif // USE_EDGEMODE_JSRT
                /// <summary>
                /// Gets a value at the specified index.
                /// </summary>
                virtual IJavaScriptValue^ GetValueAtIndex(IJavaScriptValue^ index);
                /// <summary>
                /// Sets a value at the specified index.
                /// </summary>
                virtual void SetValueAtIndex(IJavaScriptValue^ index, IJavaScriptValue^ value);
                /// <summary>
                /// Deletes a value at the specified index.
                /// </summary>
                virtual void DeleteValueAtIndex(IJavaScriptValue^ index);
                /// <summary>
                /// Gets whether the specified name is an own-property of the object.
                /// </summary>
                virtual bool HasOwnProperty(String^ propertyName);
                /// <summary>
                /// Gets whether the object has a property with the specified name.
                /// </summary>
                virtual bool HasProperty(String^ propertyName);
                /// <summary>
                /// Gets the property descriptor object if the object has an own-property of that name.
                /// </summary>
                virtual JavaScriptObject^ GetOwnPropertyDescriptor(String^ propertyName);
                /// <summary>
                /// Defines a property using the specified object as a descriptor.
                /// </summary>
                virtual void DefineProperty(String^ propertyName, IJavaScriptObject^ descriptor);
                /// <summary>
                /// Defines many properties, using the specified object as a key-value container of property names to descriptors.
                /// </summary>
                virtual void DefineProperties(IJavaScriptObject^ propertiesContainer);
                /// <summary>
                /// Gets an array of own-property names.
                /// </summary>
                virtual JavaScriptArray^ GetOwnPropertyNames();
                /// <summary>
                /// Gets an array of own-property symbols.
                /// </summary>
                virtual JavaScriptArray^ GetOwnPropertySymbols();
                /// <summary>
                /// Prevents additional properties from being added.
                /// </summary>
                virtual void PreventExtensions();
                /// <summary>
                /// Seals the object.
                /// </summary>
                virtual void Seal();
                /// <summary>
                /// Freezes the object.
                /// </summary>
                virtual void Freeze();
#pragma endregion IJavaScriptObject virtual properties

                DECLARE_JAVASCRIPT_PRIMITIVE_VALUE_VIRTUAL_MEMBERS(JavaScriptObject)
            
            };
        };
    };
};
